#include "api.h"
#include "drogon/HttpClient.h"
#include "drogon/HttpRequest.h"
#include "drogon/HttpResponse.h"
#include "drogon/HttpTypes.h"
#include "parser.h"
#include <optional>
#include <string>

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

  co_return Parser::GrabPlayerJs(result->getBody()).value_or("Error");
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

    if (!Parser::ValidateInput(str))
      co_return;

    auto Js = co_await GetPlayer(str);

    auto Out = co_await Decrypt(Js);

    resp->setBody(Out);
  }

  callback(resp);
}