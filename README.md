# collog
Color logger it is a simple crossplatform (Windows/Linux) C++ header only library for console colorized log output (depend on concol library).

## Example

```c

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

  color_log log("{cyan}LOG NAME{+black} | {}", true);
  color::set_enabled(true);
  log.set_enabled(true);

  const std::string test_header{"test: "};

  log << test_header << "OK\n"_green_bright;
  log << test_header << "ERROR\n"_red_bright;

  log << test_header + color::to_string("{+green}OK{}\n");
  log << test_header + color::to_string("{+red}ERROR{}\n");

#ifndef CONCOL_NO_STRING_VIEW
  log << test_header.c_str() << color_type::green_bright << "OK\n"sv;
  log << test_header.c_str() << color_type::red_bright << "ERROR\n"sv;
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
  log.print_info("Info message\n");
  log.print_critical("Critical error message\n");

  log << "{+white}message {+cyan}with header ";
  log.stream << "{+white}and message {+yellow}without header{}\n";

  log.print("Progress test: \n");

  for (int step{}, max_step{50}; step <= max_step; ++step) {
    log.print_progress(step, max_step);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  for (int step{0}; step <= 100; ++step) {
    log.print_progress(step);
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
  }

  color color("Progress test ");
  color.add_green_bright("Ok!\n");
  log << color.to_string();

  log << color_type::magenta_bright << "test ";
  log.stream << color_type::cyan_bright << "is {+yellow}over{+red}!!!";
  log.stream << color_ctrl::reset;

  log.print_nl();


```
