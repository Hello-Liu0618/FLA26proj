#include "tm.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\n\r\f\v");
    return s.substr(start, end - start + 1);
}

turing::Program turing::parse_program(const std::string& program_str) {

    Program program;

    std::ifstream fin(program_str);
    if ( !fin.is_open() ) {
        throw std::runtime_error("Failed to open program file: " + program_str);
    }

    std::string line = "";

    while ( std::getline(fin, line) ) {
        if ( !line.empty() && line.back() == '\r' ) {
            line.pop_back(); // 防止windows环境下\r\n换行符导致的解析错误
        }
        auto pos = line.find(';');
        if ( pos != std::string::npos ) {
            line = line.substr(0, pos); // 去掉';'及之后的注释部分
        }
        line = trim(line);
        if ( !line.empty() ) {
            
            if (line[0] == '#') {
                line = line.substr(1); // 去掉'#'，获取指令内容
                auto eq_pos = line.find('=');
                if (eq_pos == std::string::npos) {
                    throw std::runtime_error("Invalid directive: " + line);
                }
                std::string type = trim(line.substr(0, eq_pos));// 获取指令类型
                std::string value = trim(line.substr(eq_pos + 1));// 获取指令值

                if ( "Q" == type ) {// 解析状态集合
                    std::unordered_set<std::string> states;
                    value = value.substr(1, value.size() - 2); // 去掉大括号
                    for (std::size_t i = 0; i < value.size(); i++) {
                        if (value[i] == ',') {
                            states.insert(trim(value.substr(0, i)));
                            value = value.substr(i + 1);
                            i = 0;
                        }
                    }
                    if (!value.empty()) {
                        states.insert(trim(value));
                    }
                    program.states = std::move(states);
                }
                else if ( "S" == type ) { //解析输入字母表
                    std::unordered_set<char> Sigma;
                    value = value.substr(1, value.size() - 2); // 去掉大括号
                    for ( std::size_t i = 0 ; i < value.size() ; i++) {
                        if ( ',' != value[i] && ' ' != value[i] ) {
                            Sigma.insert(value[i]);
                        }
                    }
                    program.Sigma = std::move(Sigma);
                }
                else if ( "G" == type ) { // 解析纸带符号表
                    std::unordered_set<char> Gamma;
                    value = value.substr(1, value.size() - 2); // 去掉大括号
                    for ( std::size_t i = 0 ; i < value.size() ; i++ ) {
                        if ( ',' != value[i] && ' ' != value[i] ) {
                            Gamma.insert(value[i]);
                        }
                    }
                    program.Gamma = std::move(Gamma);
                }
                else if ( "F" == type ) {// 解析终止状态集合
                    std::unordered_set<std::string> q_finals;
                    value = value.substr(1, value.size() - 2); // 去掉大括号
                    for (std::size_t i = 0; i < value.size(); i++) {
                        if (value[i] == ',') {
                            q_finals.insert(trim(value.substr(0, i)));
                            value = value.substr(i + 1);
                            i = 0;
                        }
                    }
                    if (!value.empty()) {
                        q_finals.insert(trim(value));
                    }
                    program.q_finals = std::move(q_finals);
                }
                else if ( "q0" == type ) { // 解析初始状态
                    program.q0 = value;
                }
                else if ( "B" == type ) { // 解析空白符号（题目严格规定为 '_'）
                    if (value != "_") {
                        throw std::runtime_error("Invalid blank symbol: " + value);
                    }
                }
                else if ( "N" == type ) { // 解析纸带数量
                    program.num_tapes = std::stoi(value);
                }
                else {
                    throw std::runtime_error("Unknown directive: " + type);

                }
            }
            else {
                std::vector<std::string> parts;
                std::istringstream iss(line);
                std::string part;
                while (iss >> part) {
                    parts.push_back(part);
                }
                if ( parts.size() != 5 ) {
                    throw std::runtime_error("Invalid transition format: " + line);
                }
                std::string current_state = parts[0];
                std::vector<char> read(parts[1].begin(), parts[1].end());
                std::vector<char> write(parts[2].begin(), parts[2].end());
                std::vector<char> move_direction(parts[3].begin(), parts[3].end());
                std::string next_state = parts[4];
                program.transitions.push_back({current_state, read, write, move_direction, next_state});
            }
        }
    }

    fin.close();

    return program;
}