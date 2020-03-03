/*

MIT License

Copyright (c) 2020 Alexander Chernenko (achernenko@mail.ru)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include "collog.h"

#if __cplusplus < 201402L
#define COLLOG_NO_STRING_LITERALS
#endif

using namespace collog;
using namespace concol;
using namespace concol_literals;
#ifndef COLLOG_NO_STRING_LITERALS
using namespace std::string_literals;
#endif
#ifndef CONCOL_NO_STRING_VIEW
using namespace std::string_view_literals;
#endif

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) try {
  color_log log("LOG NAME", true);
  color::set_enabled(true);
  log.set_enabled(true);
  log << "test: "
      << "OK\n"_green_bright;
  log << "test: "
      << "ERROR\n"_red_bright;

  log << color::to_string("test: {+green}OK{}\n");
  log << color::to_string("test: {+red}ERROR{}\n");

#ifndef CONCOL_NO_STRING_VIEW
  log << "test: " << color_type::green_bright << "OK\n"sv;
  log << "test: " << color_type::red_bright << "ERROR\n"sv;
#endif
  log << color::to_string("{+white}value{}: {+yellow}%d{}\n", 128);
  log << "{+white}value{}: " << color_type::yellow_bright << 128
      << color_ctrl::reset << '\n';

  log << color::to_string("{+white}value{}: {+yellow}%.2f{}\n", 3.14);
  log << "{+white}value{}: " << color_type::yellow_bright << 3.14
      << color_ctrl::reset << '\n';

  color::set_enabled(false);
  log << "noncolor\n"_red_bright;

  log.set_enabled(false);
  log << "log is disable\n";

  log.set_enabled(true);
  log << "log is enable\n";
  color::set_enabled(true);

  log.print_error("Error message\n");
  log.print_warning("Warning message\n");
  log.print_success("Success message\n");

  log << "{+white}message {+cyan}with header ";
  log.stream << "{+white}and message {+yellow}without header{}\n";
  log << color_tags::magenta_bright << "warning: ";
  log.stream << color_tags::cyan_bright << "test is over!";
  log.stream << color_tags::reset;
  return 0;
} catch (...) {
  std::cerr << "\nunexpected exception\n";
  return 0;
}
