#include "simulator.hpp"
#include <algorithm>
#include <utility>

Simulator::Simulator(turing::Program program, std::string input)
    : program_(std::move(program)), input_(std::move(input)), current_state_(program_.q0) {
    init_tapes();
    // 检查输入是否有效
    for (std::size_t i = 0; i < input_.size(); ++i) {
        if (program_.Sigma.find(input_[i]) == program_.Sigma.end()) {
            first_invalid_index_ = i;
            invalid_symbol_ = input_[i];
            break;
        }
    }
}

bool Simulator::input_valid() const {
    return first_invalid_index_ == std::string::npos;
}

std::size_t Simulator::first_invalid_index() const {
    return first_invalid_index_;
}

char Simulator::first_invalid_symbol() const {
    return invalid_symbol_;
}

bool Simulator::step_once() {
    if (halted_) {
        return false;// 图灵机已停止，无法继续执行
    }
    const turing::Transition* transition = find_transition();
    if (transition) {
        apply_transition(*transition);
        ++step_count_;
        return true; // 成功执行一次状态转移
    }
    halted_ = true;
    return false; // 没有找到匹配的状态转移，停止执行
}

const std::string& Simulator::current_state() const {
    return current_state_;
}

long long Simulator::step_count() const {
    return step_count_;
}

const std::vector<Tape>& Simulator::tapes() const {
    return tapes_;
}

bool Simulator::halted() const {
    return halted_;
}

const std::string& Simulator::input() const {
    return input_;
}

const turing::Transition* Simulator::find_transition() const {
    for (const auto& transition : program_.transitions) {
        if ( transition.current_state != current_state_ || std::find(transition.read.begin(), transition.read.end(), '*') != transition.read.end()) {
            continue; // 当前状态不匹配，或者读取符号包含通配符*，跳过
        }
        bool match = true;
        for ( std::size_t i = 0 ; i < tapes_.size() ; i++ ) {
            if ( tapes_[i].read() != transition.read[i] ) {
                match = false;
                break;
            }
        }
        if (match) {
            return &transition; // 找到匹配的状态转移
        }
    }
    for ( const auto& transition : program_.transitions ) {
        if ( transition.current_state != current_state_ || std::find(transition.read.begin(), transition.read.end(), '*') == transition.read.end() ) {
            continue; // 当前状态不匹配，或者读取符号不包含通配符*，跳过
        }
        bool match = true;
        for ( std::size_t i = 0 ; i < tapes_.size() ; i++ ) {
            char current_symbol = tapes_[i].read();
            if ( transition.read[i] == '*' ) {
                if ( current_symbol == turing::BLANK ) {
                    match = false; // 当前符号是空白符号，通配符*不能匹配空白符号
                    break;
                }
            } else {
                if (  current_symbol != transition.read[i] ) {
                    match = false; // 当前符号不匹配
                    break;
                }
            }
        }
        if (match) {
            return &transition; // 找到匹配的状态转移
        }
    }
    return nullptr; // 没有找到匹配的状态转移
}

void Simulator::apply_transition(const turing::Transition& transition) {
    for ( std::size_t i = 0 ; i < tapes_.size() ; i++ ) {
        char old_symbol = tapes_[i].read();
        char new_symbol = (transition.write[i] == '*' ? old_symbol : transition.write[i]);
        tapes_[i].write(new_symbol);
        tapes_[i].move(transition.move_direction[i]);
    }
    current_state_ = transition.next_state;
}

void Simulator::init_tapes() {
    tapes_.resize(program_.num_tapes);
    if (!input_.empty()) {
        tapes_[0].init_tape(input_); // 将输入字符串初始化到第一个纸带上
    }
}