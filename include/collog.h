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
      : logstream{(log_enable) ? std::cout.rdbuf() : nullptr} {}
  explicit logstream(std::streambuf *buf_ptr = {}) : _log{buf_ptr} {}
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
  logstream &operator<<(const std::string_view &rhs) {
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
  const std::string_view _str;
#else
  const std::string _str;
#endif

  void print_header() {
    _log << concol::color_type::black_bright << "[" << _str << "] "
         << concol::color_ctrl::reset;
  }

 public:
#ifndef CONCOL_NO_STRING_VIEW
  explicit color_log(const std::string_view &str, bool enable = true)
      : logstream((enable) ? std::cout.rdbuf() : nullptr), _str{str} {}
#else
  explicit color_log(const std::string str, bool enable = true)
      : logstream((enable) ? std::cout.rdbuf() : nullptr), _str{str} {}
#endif
  ~color_log() noexcept = default;
  logstream &stream{*this};
  template <typename Type>
  logstream &operator<<(Type &&rhs) {
    if (is_enabled()) {
      print_header();
      _log << std::forward<Type>(rhs);
    }
    return *this;
  }
  logstream &operator<<(std::string &&rhs) {
    if (is_enabled()) {
      print_header();
      concol::color::printf(rhs.c_str());
    }
    return *this;
  }
#ifndef CONCOL_NO_STRING_VIEW
  logstream &operator<<(std::string_view &&rhs) {
    if (is_enabled()) {
      print_header();
      concol::color::printf(rhs.data());
    }
    return *this;
  }
#endif
  logstream &operator<<(const char *rhs) {
    if (is_enabled()) {
      print_header();
      concol::color::printf(rhs);
    }
    return *this;
  }
  void print_error(const std::string &str) {
    *this << concol::color_type::red_bright << str << concol::color_ctrl::reset;
  }
  void print_warning(const std::string &str) {
    *this << concol::color_type::yellow_bright << str
          << concol::color_ctrl::reset;
  }
  void print_success(const std::string &str) {
    *this << concol::color_type::green_bright << str
          << concol::color_ctrl::reset;
  }
};

}  // namespace collog
