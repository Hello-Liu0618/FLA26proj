#pragma once

#include <cstddef>
#include <string>

class Simulator;

namespace out {

// 普通模式：非法输入与最终纸带内容
void print_illegal_normal();
void print_plain_result(const std::string& content);

// verbose 模式：ERR / RUN / Step / Result
void print_input_error(const std::string& input, std::size_t pos, char bad);
void print_run_header(const std::string& input);
void print_snapshot(const Simulator& sim);
void print_result(const std::string& content);

} // namespace out
