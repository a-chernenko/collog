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

#pragma once

#if __cplusplus < 201103L
#error \
    "This file requires compiler and library support for the ISO C++ 2011 standard or later."
#endif

#include <iostream>
#include <string>
#ifndef CONCOL_NO_STRING_VIEW
#include <string_view>
#endif
#include "concol.h"

namespace collog {

namespace detail {

class logstream {
  bool _enabled{};

 protected:
  std::ostream _log;

  explicit logstream(bool log_enable)
      : _log{(log_enable) ? std::cerr.rdbuf() : nullptr} {
    concol::color::set_ostream(stderr);
  }
  ~logstream() noexcept = default;

 public:
  template <typename Type>
  logstream &operator<<(Type &&rhs) {
    if (_enabled) {
      _log << std::forward<Type>(rhs);
    }
    return *this;
  }
  logstream &operator<<(std::string &&rhs) {
    if (_enabled) {
      concol::color::printf(rhs);
    }
    return *this;
  }
#ifndef CONCOL_NO_STRING_VIEW
  logstream &operator<<(std::string_view &&rhs) {
    if (_enabled) {
      concol::color::printf(rhs);
    }
    return *this;
  }
#endif
  logstream &operator<<(const char *rhs) {
    if (_enabled) {
      concol::color::printf(rhs);
    }
    return *this;
  }
  void set_enabled(bool enabled) noexcept { _enabled = enabled; }
  bool is_enabled() const noexcept { return _enabled; }
};

}  // namespace detail

class color_log : public detail::logstream {
#ifndef CONCOL_NO_STRING_VIEW
  const std::string_view _header_str;
#else
  const std::string _header_str;
#endif

  void _print_header() { concol::color::printf(_header_str); }

 public:
#ifndef CONCOL_NO_STRING_VIEW
  explicit color_log(const std::string_view &header_str, bool enable = true)
      : logstream{enable}, _header_str{header_str} {}
#else
  explicit color_log(const std::string header_str, bool enable = true)
      : logstream{enable}, _header_str{header_str} {}
#endif
  ~color_log() noexcept = default;
  logstream &stream{*this};
  template <typename Type>
  logstream &operator<<(Type &&rhs) {
    if (is_enabled()) {
      _print_header();
      _log << std::forward<Type>(rhs);
    }
    return stream;
  }
  logstream &operator<<(std::string &&rhs) {
    if (is_enabled()) {
      _print_header();
      concol::color::printf(rhs);
    }
    return stream;
  }
#ifndef CONCOL_NO_STRING_VIEW
  logstream &operator<<(std::string_view &&rhs) {
    if (is_enabled()) {
      _print_header();
      concol::color::printf(rhs);
    }
    return stream;
  }
#endif
  logstream &operator<<(const char *rhs) {
    if (is_enabled()) {
      _print_header();
      concol::color::printf(rhs);
    }
    return stream;
  }
  void print(const std::string &str) {
    if (is_enabled()) {
      _print_header();
      concol::color::printf(str);
    }
  }
  void print_error(const std::string &str) {
    if (!is_enabled()) {
      return;
    }
    _print_header();
    concol::color color;
    color.add_red_bright("ERROR: ");
    color.add(str);
    color.print();
  }
  void print_warning(const std::string &str) {
    if (!is_enabled()) {
      return;
    }
    _print_header();
    concol::color color;
    color.add_yellow_bright("WARNING: ");
    color.add(str);
    color.print();
  }
  void print_success(const std::string &str) {
    if (!is_enabled()) {
      return;
    }
    _print_header();
    concol::color color;
    color.add_green_bright("SUCCESS: ");
    color.add(str);
    color.print();
  }
  void print_info(const std::string &str) {
    if (!is_enabled()) {
      return;
    }
    _print_header();
    concol::color color;
    color.add_cyan_bright("INFO: ");
    color.add(str);
    color.print();
  }
  void print_critical(const std::string &str) {
    if (!is_enabled()) {
      return;
    }
    _print_header();
    concol::color color;
    color.add_red_bright("FATAL ERROR: ");
    color.add(str);
    color.print();
  }
  void print_progress(const int percentage, const int max_percentage = 100) {
    if (!is_enabled()) {
      return;
    }
    _print_header();
    int complete{max_percentage};
    if (complete > 100) {
      complete = 100;
    }
    int progress{percentage};
    if (progress < 0) {
      progress = 0;
    }
    if (progress <= complete) {
      concol::color color;
      color.add_cyan_bright("[");
      std::string head(progress, '.');
      std::string tail(complete - percentage, ' ');
      if (progress < complete) {
        color.add_yellow_bright(head + tail);
      } else {
        color.add_green_bright(head + tail);
      }
      color.add_cyan_bright("] ");
      color.add_white_bright(progress * 100 / complete);
      color += "%";
      if (progress < complete) {
        color += '\r';
      } else {
        color += '\n';
      }
      color.print();
    }
  }
  void print_nl() {
    if (is_enabled()) {
      _log << '\n';
    }
  }
  void print_rl() {
    if (is_enabled()) {
      _log << '\r';
    }
  }
};

}  // namespace collog
