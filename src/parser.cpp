#include "parser.h"
#include <ctre.hpp>
#include <re2/re2.h>
#include <simdjson.h>

static const std::array Rgx{
    RE2(R"(\b[cs]\s*&&\s*[adf]\.set\([^,]+\s*,\s*encodeURIComponent\s*\(\s*([a-zA-Z0-9$]+)\()"),
    RE2(R"(\b[a-zA-Z0-9]+\s*&&\s*[a-zA-Z0-9]+\.set\([^,]+\s*,\s*encodeURIComponent\s*\(\s*([a-zA-Z0-9$]+)\()"),
    RE2(R"(\bm=([a-zA-Z0-9$]{2})\(decodeURIComponent\(h\.s\)\))"),
    RE2(R"(\bc&&\(c=([a-zA-Z0-9$]{2})\(decodeURIComponent\(c\)\))"),
    RE2(R"((?:\b|[^a-zA-Z0-9$])([a-zA-Z0-9$]{2})\s*=\s*function\(\s*a\s*\)\s*\{\s*a\s*=\s*a\.split\(\s*""\s*\);[a-zA-Z0-9$]{2}\.[a-zA-Z0-9$]{2}\(a,\d+\))"),
    RE2(R"((?:\b|[^a-zA-Z0-9$])([a-zA-Z0-9$]{2})\s*=\s*function\(\s*a\s*\)\s*\{\s*a\s*=\s*a\.split\(\s*""\s*\))"),
    RE2(R"(([a-zA-Z0-9$]+)\s*=\s*function\(\s*a\s*\)\s*\{\s*a\s*=\s*a\.split\(\s*""\s*\))")};

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

bool Parser::ParseJSON(std::string_view str) {

  // todo: impl
  return str.find(R"({"itag")");
}

std::optional<std::string> Parser::Parse(std::string_view sv) {

  re2::StringPiece s;

  for (auto &i : Rgx)
    if (RE2::PartialMatch(sv, i, &s))
      break;

  auto result = re2::RE2(s.ToString() + R"(=function\(.+?\)\{(.+?)\})");

  if (result.ok())
    RE2::PartialMatch(sv, result, &s);

  return result.ok() ? s.ToString() : std::optional<std::string>();
};