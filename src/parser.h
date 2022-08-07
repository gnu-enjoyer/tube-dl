#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace Parser {

  bool ParseJSON(std::string_view str);

  bool ValidateInput(const std::string &str);

  std::optional<std::string> GrabPlayerJs(std::string_view str);

  std::optional<std::string> Parse(std::string_view str);

}; // namespace Parser