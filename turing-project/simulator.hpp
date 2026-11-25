#pragma once

#include "tape.hpp"
#include "tm.hpp"
#include <string>
#include <vector>

class Simulator {
public:

    Simulator(turing::Program program, std::string input); // 构造函数，初始化模拟器

    bool input_valid() const; // 检查输入是否有效
    std::size_t first_invalid_index() const; // 获取第一个无效输入的索引
    char first_invalid_symbol() const; // 获取第一个无效输入的符号

    bool step_once(); // 执行一次状态转移，返回是否成功

    const std::string& current_state() const; // 获取当前状态
    long long step_count() const; // 获取当前步数
    const std::vector<Tape>& tapes() const; // 获取纸带集合
    bool halted() const; // 检查图灵机是否已停止
    const std::string& input() const; // 获取输入字符串

private:
    const turing::Transition* find_transition() const; // 查找当前状态和纸带符号对应的状态转移函数
    void apply_transition(const turing::Transition& transition); // 应用状态转移函数
    void init_tapes();// 初始化纸带集合

    turing::Program program_; // 图灵机程序
    std::string input_; // 输入字符串
    std::vector<Tape> tapes_; // 纸带集合
    std::string current_state_; // 当前状态
    long long step_count_ = 0; // 当前步数
    bool halted_ = false; // 图灵机是否已停止
    std::size_t first_invalid_index_ = std::string::npos; // 第一个无效输入的索引
    char invalid_symbol_ = '\0'; // 第一个无效输入的符号
};