#include "../include/debugger.h"
#include <string>
#include <vector>
#include <iostream>
#include <sys/ptrace.h>
#include <wait.h>
#include <registers.h>
#include <iomanip>
#include <fstream>
#include "linenoise.h"

std::string to_string(symbol_type st) {
    switch (st) {
        case symbol_type::notype:
            return "notype";
        case symbol_type::object:
            return "object";
        case symbol_type::func:
            return "func";
        case symbol_type::section:
            return "section";
        case symbol_type::file:
            return "file";
    }
}

symbol_type to_symbol_type(elf::stt sym) {
    switch (sym) {
        case elf::stt::notype:
            return symbol_type::notype;
        case elf::stt::object:
            return symbol_type::object;
        case elf::stt::func:
            return symbol_type::func;
        case elf::stt::section:
            return symbol_type::section;
        case elf::stt::file:
            return symbol_type::file;
        default:
            return symbol_type::notype;
    }
}


void debugger::run() {
    int wait_status;
    auto options = 0;

    waitpid(m_pid, &wait_status, options);

    char *line = nullptr;
    while ((line = linenoise("minidbg> ")) != nullptr) {
        handle_command(line);
        linenoiseHistoryAdd(line);
        linenoiseFree(line);
    }
}

void debugger::handle_command(const std::string &line) {
    auto args = split(line, ' ');
    auto command = args[0];

    if (is_prefix(command, "cont")) {
        continue_execution();
    } else if (is_prefix(command, "break")) {
        if (args[1][0] == '0' && args[1][1] == 'x') {
            std::string addr{args[1], 2};
            set_breakpoint_at_address(std::stol(addr, 0, 16));
        } else if (args[1].find(':') != std::string::npos) {
            auto file_and_line = split(args[1], ':');
            set_breakpoint_at_source_line(file_and_line[0], std::stoi(file_and_line[1]));
        } else {
            set_breakpoint_at_function(args[1]);
        }
    } else if (is_prefix(command, "step")) {
        step_in();
    } else if (is_prefix(command, "next")) {
        step_over();
    } else if (is_prefix(command, "finish")) {
        step_out();
    } else if (is_prefix(command, "register")) {
        if (is_prefix(args[1], "dump")) {
            dump_registers();
        }
    } else if (is_prefix(args[1], "read")) {
        std::cout << get_register_value(m_pid, get_register_from_name(args[2])) << std::endl;
    } else if (is_prefix(args[1], "write")) {
        std::string val{args[3], 2}; //assume 0xVAL
        set_register_value(m_pid, get_register_from_name(args[2]), std::stol(val, 0, 16));
    } else if (is_prefix(command, "memory")) {
        std::string addr{args[2], 2}; //assume 0xADDRESS

        if (is_prefix(args[1], "read")) {
            std::cout << std::hex << read_memory(std::stol(addr, 0, 16)) << std::endl;
        }
        if (is_prefix(args[1], "write")) {
            std::string val{args[3], 2}; //assume 0xVAL
            write_memory(std::stol(addr, 0, 16), std::stol(val, 0, 16));
        }
    } else if (is_prefix(command, "symbol")) {
        auto syms = lookup_symbol(args[1]);
        for (auto &&s : syms) {
            std::cout << s.name << ' ' << to_string(s.type) << " 0x" << std::hex << s.addr << std::endl;
        }
    } else if (is_prefix(command, "stepi")) {
        single_step_instruction_with_breakpoint_check();
        auto line_entry = get_line_entry_from_pc(get_pc());
        print_source(line_entry->file->path, line_entry->line);
    } else {
        std::cerr << "Unknown command\n";
    }
}

std::vector<std::string> debugger::split(const std::string &s, char delimiter) {
    std::vector<std::string> out{};
    std::stringstream ss{s};
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        out.push_back(item);
    }

    return out;
}

bool debugger::is_prefix(const std::string &s, const std::string &of) {
    if (s.size() > of.size())
        return false;

    return std::equal(s.begin(), s.end(), of.begin());
}

void debugger::continue_execution() {
    step_over_breakpoint();
    ptrace(PTRACE_CONT, m_pid, nullptr, nullptr);
    wait_for_signal();
}

void debugger::set_breakpoint_at_address(std::intptr_t addr) {
    std::cout << "Set breakpoint at address 0x" << std::hex << addr << std::endl;
    breakpoint bp{m_pid, addr};
    bp.enable();
    m_breakpoints.insert(std::make_pair(addr, bp));
}

void debugger::dump_registers() {
    for (const auto &rd:g_registers_descriptors) {
        std::cout
                << rd.name
                << "0x"
                << std::setfill('0')
                << std::setw(16)
                << std::hex
                << get_register_value(m_pid, rd.r)
                << std::endl;
    }
}

uint64_t debugger::read_memory(uint64_t address) {
    return ptrace(PTRACE_PEEKDATA, m_pid, address, nullptr);
}

void debugger::write_memory(uint64_t address, uint64_t value) {
    ptrace(PTRACE_POKEDATA, m_pid, address, value);
}

uint64_t debugger::get_pc() {
    return get_register_value(m_pid, reg::rip);
}

void debugger::set_pc(uint64_t pc) {
    set_register_value(m_pid, reg::rip, pc);
}

void debugger::step_over_breakpoint() {
    if (m_breakpoints.count(get_pc())) {
        auto &bp = m_breakpoints[get_pc()];
        if (bp.is_enabled()) {
            bp.disable();
            ptrace(PTRACE_SINGLESTEP, m_pid, nullptr, nullptr);
            wait_for_signal();
            bp.enable();
        }
    }
}

// encapsulate waitpid syscall
void debugger::wait_for_signal() {
    int wait_status;
    auto options = 0;
    waitpid(m_pid, &wait_status, options);

    auto siginfo = get_signal_info();
    switch (siginfo.si_signo) {
        case SIGTRAP:
            handle_sigtrap(siginfo);
            break;
        case SIGSEGV:
            std::cout << "Segfault, noooo. Reason: " << siginfo.si_code << std::endl;
            break;
        default:
            std::cout << "Got signal " << strsignal(siginfo.si_signo) << std::endl;
    }

}

// debugging information entry (DIE)
dwarf::die debugger::get_function_from_pc(uint64_t pc) {
    for (auto &cu: m_dwarf.compilation_units()) {
        if (die_pc_range(cu.root()).contains(pc)) {
            for (const auto &die: cu.root()) {
                if (die.tag == dwarf::DW_TAG::subprogram) {
                    if (die_pc_range(die).contains(pc)) {
                        return die;
                    }
                }
            }
        }
    }
    throw std::out_of_range{"cannot find function"};
}

// simply find the correct compilation unit, then ask the line table to get us
// the relevant entry
dwarf::line_table::iterator debugger::get_line_entry_from_pc(uint64_t pc) {
    for (auto &cu: m_dwarf.compilation_units()) {
        if (die_pc_range(cu.root()).contains(pc)) {
            auto &lt = cu.get_line_table();
            auto it = lt.find_address(pc);
            if (it == lt.end()) {
                throw std::out_of_range{"cannot find line entry"};
            } else {
                return it;
            }
        }
    }
    throw std::out_of_range{"cannot find line entry"};
}

void debugger::print_source(const std::string &file_name, unsigned line, unsigned n_lines_context) {
    std::ifstream file{file_name};

    auto start_line = line <= n_lines_context ? 1 : line - n_lines_context;
    auto end_line = line + n_lines_context + (line < n_lines_context ? n_lines_context - line : 0);

    char c{};

    auto current_line = 1u;

    //skip line until start line
    while (current_line != start_line && file.get(c)) {
        if (c == '\n')
            ++current_line;
    }

    //output cursor if we're at the current line
    std::cout << (current_line == line ? "> " : "  ");

    //write lines up until end_line
    while (current_line <= end_line && file.get(c)) {
        std::cout << c;
        if (c == '\n') {
            ++current_line;
            //output cursor if we're at the current line
            std::cout << (current_line == line ? "> " : "  ");
        }
    }

    //write newline and make sure that the stream is flushed properly
    std::cout << std::endl;
}

// to be able to tell what signal was sent to the process, but also we want to know
// how in was produced;
siginfo_t debugger::get_signal_info() {
    siginfo_t info;
    ptrace(PTRACE_GETSIGINFO, m_pid, nullptr, &info);
    return info;
}

// man sigaction
void debugger::handle_sigtrap(siginfo_t info) {
    switch (info.si_code) {
        //one of these will be set if a breakpoint was hit
        case SI_KERNEL:
        case TRAP_BRKPT: {
            set_pc(get_pc() - 1); //put the pc back where is should be
            std::cout << "Hit breakpoint at address 0x" << std::hex << get_pc() << std::endl;
            auto line_entry = get_line_entry_from_pc(get_pc());
            print_source(line_entry->file->path, line_entry->line);
            return;;
        }
        case TRAP_TRACE:
            return;
        default:
            std::cout << "Unknown SIGTRAP code " << info.si_code << std::endl;
            return;
    }
}

void debugger::single_step_instruction() {
    ptrace(PTRACE_SINGLESTEP, m_pid, nullptr, nullptr);
    wait_for_signal();
}

void debugger::single_step_instruction_with_breakpoint_check() {
    // check to see if we need to disable and enable a breakpoint
    if (m_breakpoints.count(get_pc())) {
        step_over_breakpoint();
    } else {
        single_step_instruction();
    }

}

void debugger::step_out() {
    auto frame_pointer = get_register_value(m_pid, reg::rbp);
    auto return_address = read_memory(frame_pointer + 8);

    bool should_remove_breakpoint = false;
    if (!m_breakpoints.count(return_address)) {
        set_breakpoint_at_address(return_address);
        should_remove_breakpoint = true;
    }

    continue_execution();

    if (should_remove_breakpoint) {
        remove_breakpoint(return_address);
    }
}

void debugger::remove_breakpoint(std::intptr_t addr) {
    if (m_breakpoints.at(addr).is_enabled()) {
        m_breakpoints.at(addr).disable();
    }
    m_breakpoints.erase(addr);
}

void debugger::step_in() {
    auto line = get_line_entry_from_pc(get_pc())->line;

    while ((get_line_entry_from_pc(get_pc()))->line == line) {
        single_step_instruction_with_breakpoint_check();
    }

    auto line_entry = get_line_entry_from_pc(get_pc());
    print_source(line_entry->file->path, line_entry->line);
}

void debugger::step_over() {
    auto func = get_function_from_pc(get_pc());
    auto func_entry = at_low_pc(func);
    auto func_end = at_high_pc(func);

    auto line = get_line_entry_from_pc(func_entry);
    auto start_line = get_line_entry_from_pc(get_pc());

    std::vector<std::intptr_t> to_delete{};

    while (line->address < func_end) {
        if (line->address != start_line->address && !m_breakpoints.count(line->address)) {
            set_breakpoint_at_address(line->address);
            to_delete.push_back(line->address);
        }
        ++line;
    }

    auto frame_pointer = get_register_value(m_pid, reg::rbp);
    auto return_address = read_memory(frame_pointer + 8);

    if (!m_breakpoints.count(return_address)) {
        set_breakpoint_at_address(return_address);
        to_delete.push_back(return_address);
    }

    continue_execution();

    for (auto addr: to_delete) {
        remove_breakpoint(addr);
    }
}

void debugger::set_breakpoint_at_function(const std::string &name) {
    for (const auto &cu : m_dwarf.compilation_units()) {
        for (const auto &die: cu.root()) {
            if (die.has(dwarf::DW_AT::name) && at_name(die) == name) {
                auto low_pc = at_low_pc(die);
                auto entry = get_line_entry_from_pc(low_pc);
                ++entry; //skip prologue
                set_breakpoint_at_address(entry->address);
            }
        }
    }
}

void debugger::set_breakpoint_at_source_line(const std::string &file, unsigned line) {
    for (const auto &cu: m_dwarf.compilation_units()) {
        const auto &lt = cu.get_line_table();

        for (const auto &entry: lt) {
            if (entry.is_stmt && entry.line == line) {
                set_breakpoint_at_address(entry.address);
                return;
            }
        }
    }
}

std::vector<symbol> debugger::lookup_symbol(const std::string &name) {
    std::vector<symbol> syms;

    for (auto &sec:m_elf.sections()) {
        if (sec.get_hdr().type != elf::sht::symtab && sec.get_hdr().type != elf::sht::dynsym) {
            continue;
        }
        for (const auto &sym: sec.as_symtab()) {
            if (sym.get_name() == name) {
                auto &d = sym.get_data();
                syms.push_back((symbol{to_symbol_type(d.type()), sym.get_name(), d.value}));
            }
        }
    }
    return syms;
}








































