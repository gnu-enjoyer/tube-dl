#pragma once

#include <drogon/HttpController.h>

namespace controllers {

class API : public drogon::HttpController<API> {

  static drogon::HttpResponsePtr simpleResponse(drogon::HttpStatusCode E);

  static drogon::HttpRequestPtr buildRequest(const std::string &str,
                                             bool first = true);

  drogon::Task<std::string> Decrypt(std::string str) const;

  drogon::Task<std::string> GetPlayer(std::string str) const;

public:
  METHOD_LIST_BEGIN
  ADD_METHOD_TO(API::Entry, "/", drogon::Get);
  ADD_METHOD_TO(API::Query, "/query", drogon::Get);
  METHOD_LIST_END

  void Entry(const drogon::HttpRequestPtr &req,
             std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    callback(simpleResponse(drogon::HttpStatusCode::k400BadRequest));
  };

  drogon::AsyncTask
  Query(const drogon::HttpRequestPtr req,
        std::function<void(const drogon::HttpResponsePtr &)> callback);
};
} // namespace controllers