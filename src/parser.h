#pragma once

#include <optional>
#include <string>
#include <string_view>

class Parser {

public:
  static bool ParseJSON(std::string_view str);

  static bool ValidateInput(const std::string &str);

  static std::optional<std::string> GrabPlayerJs(std::string_view str);

  static std::optional<std::string> Parse(std::string_view str);

}; // namespace Parser