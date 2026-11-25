#include "printer.hpp"

#include "simulator.hpp"
#include "tape.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace {

// 一条纸带在 verbose 输出中的可见窗口。
struct TapeView {
    long long lo = 0;
    long long hi = 0;
    std::vector<std::string> tokens;
    std::vector<std::size_t> widths;
};

std::string section_line(const std::string& name) {
    return std::string(20, '=') + " " + name + " " + std::string(20, '=');
}

// 负索引按题目要求省略负号，例如 -3 显示为 "3"。
std::string index_token(long long index) {
    if (index < 0) {
        index = -index;
    }
    return std::to_string(index);
}

TapeView make_view(const Tape& tape) {
    TapeView view;
    const long long head = tape.get_head_position();

    if (!tape.has_non_blank()) {
        view.lo = head;
        view.hi = head;
    } else {
        view.lo = std::min(tape.min_non_blank_position(), head);
        view.hi = std::max(tape.max_non_blank_position(), head);
    }

    for (long long i = view.lo; i <= view.hi; ++i) {
        view.tokens.push_back(index_token(i));
        view.widths.push_back(view.tokens.back().size());
    }
    return view;
}

std::string build_index_line(const TapeView& view) {
    std::string line;
    for (std::size_t i = 0; i < view.tokens.size(); ++i) {
        if (i != 0) {
            line += ' ';
        }
        line += view.tokens[i];
    }
    return line;
}

// Tape 行的每个符号与对应 Index token 的第一个字符左对齐。
std::string build_tape_line(const Tape& tape, const TapeView& view) {
    std::string line;
    for (std::size_t i = 0; i < view.tokens.size(); ++i) {
        line += tape.at(view.lo + static_cast<long long>(i));
        if (i + 1 < view.tokens.size()) {
            // 补足 token 宽度，再加一个分隔空格。
            line += std::string(view.widths[i] - 1, ' ');
            line += ' ';
        }
    }
    return line;
}

std::string build_head_line(const Tape& tape, const TapeView& view) {
    const long long head = tape.get_head_position();
    std::size_t indent = 0;

    for (std::size_t i = 0; i < view.tokens.size(); ++i) {
        const long long position = view.lo + static_cast<long long>(i);
        if (position == head) {
            break;
        }
        indent += view.widths[i] + 1;
    }

    return std::string(indent, ' ') + "^";
}

std::size_t label_width(std::size_t tape_count) {
    std::size_t width = std::max(std::string("Step").size(), std::string("State").size());

    for (std::size_t i = 0; i < tape_count; ++i) {
        const std::string suffix = std::to_string(i);
        width = std::max(width, std::string("Index").size() + suffix.size());
        width = std::max(width, std::string("Tape").size() + suffix.size());
        width = std::max(width, std::string("Head").size() + suffix.size());
    }
    return width;
}

void print_field(const std::string& label, std::size_t width, const std::string& value) {
    std::cout << label;
    if (label.size() < width) {
        std::cout << std::string(width - label.size(), ' ');
    }
    std::cout << " : " << value << '\n';
}

} // namespace

namespace out {

void print_illegal_normal() {
    std::cout << "illegal_input\n";
}

void print_plain_result(const std::string& content) {
    std::cout << content << '\n';
}

void print_input_error(const std::string& input, std::size_t pos, char bad) {
    std::cout << "Input: " << input << '\n';
    std::cout << section_line("ERR") << '\n';
    std::cout << "error: '" << bad
              << "' was not declared in the set of input symbols\n";
    std::cout << "Input: " << input << '\n';
    std::cout << std::string(7 + pos, ' ') << "^\n";
    std::cout << section_line("END") << '\n';
}

void print_run_header(const std::string& input) {
    std::cout << "Input: " << input << '\n';
    std::cout << section_line("RUN") << '\n';
}

void print_snapshot(const Simulator& sim) {
    const auto& tapes = sim.tapes();
    const std::size_t width = label_width(tapes.size());

    print_field("Step", width, std::to_string(sim.step_count()));
    print_field("State", width, sim.current_state());

    for (std::size_t i = 0; i < tapes.size(); ++i) {
        const Tape& tape = tapes[i];
        const TapeView view = make_view(tape);
        const std::string suffix = std::to_string(i);

        print_field("Index" + suffix, width, build_index_line(view));
        print_field("Tape" + suffix, width, build_tape_line(tape, view));
        print_field("Head" + suffix, width, build_head_line(tape, view));
    }

    std::cout << std::string(45, '-') << '\n';
}

void print_result(const std::string& content) {
    std::cout << "Result: " << content << '\n';
    std::cout << section_line("END") << '\n';
}

} // namespace out
