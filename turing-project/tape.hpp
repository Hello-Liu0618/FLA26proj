#pragma once
#include <string>
#include <unordered_map>

class Tape {
public:
    char read() const; // 读取当前纸带符号
    void write(char symbol); // 写入纸带符号
    void move(char direction); // 移动纸带头

    long long get_head_position() const; // 获取纸带头位置

    char at(long long position) const; // 获取指定位置的纸带符号
    
    long long min_non_blank_position() const; // 获取最小非空白符号位置
    long long max_non_blank_position() const; // 获取最大非空白符号位置
    bool has_non_blank() const; // 检查纸带是否有非空白符号(即检查纸带是否非空)

    std::string to_string() const; // 将纸带内容转换为字符串表示

    void init_tape(const std::string& input); // 初始化纸带内容

private:
    std::unordered_map<long long, char> tape_; // 纸带内容
    long long head_position_ = 0; // 纸带头位置

};