#pragma once
#include <string>
#include <vector>
#include <unordered_set>

namespace turing {

inline constexpr char BLANK = '_';

// 状态转移函数
struct Transition {
    std::string current_state;// 当前状态
    std::vector<char> read;// 读取的符号
    std::vector<char> write;// 写入的符号
    std::vector<char> move_direction;// 移动方向
    std::string next_state;// 下一个状态
};

struct Program {
    std::unordered_set<std::string> states; // 状态集合
    std::unordered_set<char> Sigma; // 符号集合
    std::unordered_set<char> Gamma; // 纸带符号集合
    std::string q0; // 初始状态
    std::unordered_set<std::string> q_finals; // 接受状态集合
    int num_tapes = 0; // 纸带数量
    std::vector<Transition> transitions; // 状态转移函数集合,使用vector存储保证多个函数满足时具有一定的优先性
};

Program parse_program(const std::string& program_str);// 程序字符串解析为Program结构体

}// namespace turing