#include "api.h"
#include "drogon/HttpClient.h"
#include "drogon/HttpRequest.h"
#include "drogon/HttpResponse.h"
#include "drogon/HttpTypes.h"
#include "parser.h"
#include <optional>
#include <string>

drogon::HttpResponsePtr
controllers::API::simpleResponse(drogon::HttpStatusCode E) {

  auto resp = drogon::HttpResponse::newHttpResponse();

  resp->setStatusCode(E);

  return resp;
}

drogon::HttpRequestPtr controllers::API::buildRequest(const std::string &str,
                                                      bool first) {

  auto req = drogon::HttpRequest::newHttpRequest();

  first ? req->setPath(str) : req->setPath("/watch");

  req->setParameter("v", str);

  return req;
};

drogon::Task<std::string> controllers::API::Decrypt(std::string str) const {

  auto cli = drogon::HttpClient::newHttpClient("https://www.youtube.com");

  auto result = co_await cli->sendRequestCoro(buildRequest(str));

  co_return (Parser::Parse(result->getBody())).value_or("Error");
}

drogon::Task<std::string> controllers::API::GetPlayer(std::string str) const {
  auto cli = drogon::HttpClient::newHttpClient("https://www.youtube.com");

  auto result = co_await cli->sendRequestCoro(buildRequest(str, false));

  // if(Parser::ParseJSON(result->getBody())){}

  co_return Parser::GrabPlayerJs(result->getBody()).value_or("Error");
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