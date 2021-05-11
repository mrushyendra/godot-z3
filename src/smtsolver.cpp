#include "smtsolver.h"
#include <array>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "unistd.h"

using namespace godot;

void SMTSolver::_register_methods() {
    register_method("_process", &SMTSolver::_process);
    register_method("add_clause", &SMTSolver::add_clause);
    register_method("add_constant", &SMTSolver::add_constant);
    register_method("is_sat", &SMTSolver::is_sat);
}

SMTSolver::SMTSolver() {
}

SMTSolver::~SMTSolver() {
}

void SMTSolver::_init() {
}

void SMTSolver::_process(float delta) {
}

void SMTSolver::add_clause(godot::String clause) {
    std::wstring ws = clause.unicode_str();
    std::string cpp_clause(ws.begin(), ws.end()); // Not the most efficient, but works
    clauses.push_back(cpp_clause);
}
void SMTSolver::add_constant(godot::String nm, godot::String typ) {
    std::wstring ws_nm = nm.unicode_str();
    std::wstring ws_typ = typ.unicode_str();
    std::string cpp_nm(ws_nm.begin(), ws_nm.end());
    std::string cpp_typ(ws_typ.begin(), ws_typ.end());
    constants.push_back({cpp_nm, cpp_typ});
}

godot::String SMTSolver::is_sat(void) {
    const char* z3Cmd = "z3 -in";
    std::stringstream ss;
    for (const std::pair<std::string, std::string>& p : constants) {
        ss << "(declare-const " << p.first << " " << p.second << ")" << std::endl;
    }
    for (const std::string& clause : clauses) {
        ss << "(assert (" << clause << "))" << std::endl;
    }
    ss << "(check-sat)" << std::endl;
    ss << "(get-model)" << std::endl;
    std::string data(ss.str());

    if(call_z3(data) == Result::SAT) {
        std::cout << "Sat ! " << std::endl;
        return godot::String("Sat");
    } else {
        std::cout << "Unsat !" << std::endl;
        return godot::String("Unsat");
    }
}

SMTSolver::Result SMTSolver::call_z3(const std::string& input) {
    const char* z3Cmd = "z3 -in";

    // Open a child process running Z3.
    // Inspired by: https://github.com/vi/syscall_limiter/blob/master/writelimiter/popen2.c
    int stdin_fd[2];
    int stdout_fd[2];
    if(pipe(stdin_fd) || pipe(stdout_fd)) {
        std::cout << "pipe(s) failed" << std::endl;
    }
    pid_t p = fork();
    if(p < 0) return Result::UNSAT; /* Fork failed */
    if(p == 0) { /* child */
        close(stdin_fd[1]);
        dup2(stdin_fd[0], 0);
        close(stdout_fd[0]);
        dup2(stdout_fd[1], 1);
        execl("/bin/sh", "sh", "-c", z3Cmd, NULL);
        perror("execl"); exit(1);
    }
    close(stdin_fd[0]);
    close(stdout_fd[1]);

    // Write to the child process
    std::array<char, 10> output;
    int z3_in_fd = stdin_fd[1];
    int z3_out_fd = stdout_fd[0];
    write(z3_in_fd,input.c_str(),input.size());
    read(z3_out_fd,output.data(),10);

    // Parse output of z3
    if (output[0] == 's' && output[1] == 'a' && output[2] == 't') {
        return Result::SAT;
    } else {
        return Result::UNSAT;
    }
}
