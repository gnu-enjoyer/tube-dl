#include "parser.h"
#include <ctre.hpp>

static constexpr auto ValidateInputInternal(const std::string &str) {
  return ctre::match<"([a-zA-Z0-9]+)">(str);
}

bool Parser::ValidateInput(const std::string &str) {
  return ValidateInputInternal(str);
}

std::optional<std::string> Parser::GrabPlayerJs(std::string_view str) {
  auto match =
      ctre::search<R"((?:PLAYER_JS_URL|jsUrl)\"\s*:\s*\"([^"]+))">(str);

  return match ? match.get<1>().to_string() : std::optional<std::string>{};
}