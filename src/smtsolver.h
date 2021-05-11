#ifndef SMTSOLVER_H
#define SMTSOLVER_H

#include <string>
#include <vector>
#include <utility>
#include <Godot.hpp>
#include <Sprite.hpp>

namespace godot {

class SMTSolver : public Object {
    GODOT_CLASS(SMTSolver, Object)

private:
    std::vector<std::string> clauses;
    std::vector<std::pair<std::string,std::string>> constants;

    enum class Result {SAT, UNSAT};
    Result call_z3(const std::string& input);

public:
    static void _register_methods();

    SMTSolver();
    ~SMTSolver();

    void _init(); // our initializer called by Godot
    void _process(float delta);

    void add_clause(godot::String);
    void add_constant(godot::String, godot::String);
    godot::String is_sat(void);
};

}

#endif
