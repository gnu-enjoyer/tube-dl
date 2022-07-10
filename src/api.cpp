#include "api.h"
#include "drogon/HttpClient.h"
#include "drogon/HttpRequest.h"
#include "drogon/HttpResponse.h"
#include "drogon/HttpTypes.h"
#include <ctre.hpp>
#include <optional>
#include <string>

static constexpr auto id_pattern = ctll::fixed_string{"([a-zA-Z0-9]+)"};

static constexpr bool ValidateInput(const std::string &str) {
  return ctre::match<id_pattern>(str);
};

std::optional<std::string>
controllers::API::GrabPlayerJs(std::string_view str) {
  auto m = ctre::search<R"((?:PLAYER_JS_URL|jsUrl)\"\s*:\s*\"([^"]+))">(str);

  return m ? m.get<1>().to_string() : std::optional<std::string>{};
}

drogon::Task<std::string> controllers::API::Decrypt(std::string str) const {
  auto cli = drogon::HttpClient::newHttpClient("https://www.youtube.com");

  auto req = drogon::HttpRequest::newHttpRequest();

  req->setPath(str);

  auto result = co_await cli->sendRequestCoro(req);

  // TODO Add CTRE class for processing Js into final form
  // co_return std::string(result->getBody());
  co_return str;
}

drogon::Task<std::string> controllers::API::GetPlayer(std::string str) const {
  auto cli = drogon::HttpClient::newHttpClient("https://www.youtube.com");

  auto req = drogon::HttpRequest::newHttpRequest();

  req->setPath("/watch");

  req->setParameter("v", str);

  auto result = co_await cli->sendRequestCoro(req);

  co_return GrabPlayerJs(result->getBody()).value_or("Error");
}

void controllers::API::Entry(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {

  auto resp = drogon::HttpResponse::newHttpResponse();

  resp->setStatusCode(drogon::HttpStatusCode::k400BadRequest);

  callback(resp);
}

drogon::AsyncTask controllers::API::Query(
    const drogon::HttpRequestPtr req,
    std::function<void(const drogon::HttpResponsePtr &)> callback) {

  auto resp = drogon::HttpResponse::newHttpResponse();

  if (std::string str = req->getParameter("v"); str.size() == 11) {

    if (!ValidateInput(str))
      co_return;

    auto Js = co_await GetPlayer(str);

    auto Out = co_await Decrypt(Js);

    resp->setBody(Out);
  }

  callback(resp);
}