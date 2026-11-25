#include "tape.hpp"
#include "tm.hpp"
#include <stdexcept>

char Tape::read() const {
    return tape_.count(head_position_) ? tape_.at(head_position_) : turing::BLANK;// 如果当前位置没有符号，则返回空白符号
}

void Tape::write(char symbol) {
    if (symbol == turing::BLANK) {// 如果写入的是空白符号，则删除当前位置的符号
        tape_.erase(head_position_);
    } else {
        tape_[head_position_] = symbol;// 否则直接写入符号
    }
}

void Tape::move(char direction) {
    switch (direction) {
        case 'l':
            head_position_--;
            break;
        case 'r':
            head_position_++;
            break;
        case '*':
            // stay in place
            break;
        default:
            throw std::invalid_argument("Invalid direction: " + std::string(1, direction));
    }
}

long long Tape::get_head_position() const {// 获取纸带头位置
    return head_position_;
}

long long Tape::min_non_blank_position() const {// 获取最小非空白符号位置
    if (tape_.empty()) {
        return 0;
    }
    long long min_pos = tape_.begin()->first;
    for (const auto& pair : tape_) {
        if (pair.second != turing::BLANK && pair.first < min_pos) {
            min_pos = pair.first;
        }
    }
    return min_pos;
}

long long Tape::max_non_blank_position() const {// 获取最大非空白符号位置
    if (tape_.empty()) {
        return 0;
    }
    long long max_pos = tape_.begin()->first;
    for (const auto& pair : tape_) {
        if (pair.second != turing::BLANK && pair.first > max_pos) {
            max_pos = pair.first;
        }
    }
    return max_pos;
}

bool Tape::has_non_blank() const {// 检查纸带是否有非空白符号(即检查纸带是否非空)
    return !tape_.empty();
}

char Tape::at(long long position) const {// 获取指定位置的纸带符号
    return tape_.count(position) ? tape_.at(position) : turing::BLANK;
}

std::string Tape::to_string() const {// 将纸带内容转换为字符串表示
    if (!has_non_blank()) {
        return ""; // 空纸带的有效内容为空，不应输出一个 '_'
    }
    std::string result = "";
    for (long long i = min_non_blank_position(); i <= max_non_blank_position(); ++i) {
        result += at(i);
    }
    return result;
}

void Tape::init_tape(const std::string& input) {// 初始化纸带内容
    tape_.clear();
    head_position_ = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] != turing::BLANK) {
            tape_[i] = input[i];
        }
    }
}