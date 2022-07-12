#pragma once

#include <optional>
#include <string>

class Parser {

public:
  static bool ValidateInput(const std::string &str);

  static std::optional<std::string> GrabPlayerJs(std::string_view str);
}; // namespace Parser