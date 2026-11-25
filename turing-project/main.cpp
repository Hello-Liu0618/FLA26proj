#include "printer.hpp"
#include "simulator.hpp"
#include "tm.hpp"

#include <exception>
#include <string>
#include <utility>

int main(int argc, char* argv[]) {
    bool verbose = false;
    int arg = 1;

    if (arg < argc) {
        const std::string flag = argv[arg];
        if (flag == "-v" || flag == "--verbose") {
            verbose = true;
            ++arg;
        }
    }

    if (argc - arg != 2) {
        return 1;
    }

    const std::string tm_path = argv[arg];
    const std::string input = argv[arg + 1];

    try {
        turing::Program program = turing::parse_program(tm_path);
        Simulator sim(std::move(program), input);

        if (!sim.input_valid()) {
            if (verbose) {
                out::print_input_error(sim.input(), sim.first_invalid_index(),
                                       sim.first_invalid_symbol());
            } else {
                out::print_illegal_normal();
            }
            return 0;
        }

        if (verbose) {
            out::print_run_header(sim.input());
            out::print_snapshot(sim);
        }

        while (sim.step_once()) {
            if (verbose) {
                out::print_snapshot(sim);
            }
        }

        const std::string result =
            sim.tapes().empty() ? std::string() : sim.tapes()[0].to_string();

        if (verbose) {
            out::print_result(result);
        } else {
            out::print_plain_result(result);
        }
        return 0;
    } catch (const std::exception&) {
        // 文件无法打开或程序文件格式错误：静默失败，不污染 stdout。
        return 1;
    }
}
