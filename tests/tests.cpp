#define DROGON_TEST_MAIN
#define ANKERL_NANOBENCH_IMPLEMENT
#include "api.h"
#include <drogon/HttpTypes.h>
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include <nanobench.h>

static const std::string test_ip = "http://127.0.0.1:3002";

DROGON_TEST(CoreAPITest) {

  /* Check default route */
  auto client = drogon::HttpClient::newHttpClient(test_ip);

  auto req = drogon::HttpRequest::newHttpRequest();

  req->setPath("/");

  client->sendRequest(req, [TEST_CTX](drogon::ReqResult res,
                                      const drogon::HttpResponsePtr &resp) {
    REQUIRE(res == drogon::ReqResult::Ok);

    REQUIRE(resp != nullptr);

    CHECK(resp->getStatusCode() == drogon::HttpStatusCode::k400BadRequest);

    CHECK(resp->contentType() == drogon::CT_TEXT_HTML);
  });
}

DROGON_TEST(QueryAPITest) {

  /* Check query route */
  auto client = drogon::HttpClient::newHttpClient(test_ip);

  auto req = drogon::HttpRequest::newHttpRequest();

  req->setPath("/query");

  client->sendRequest(req, [TEST_CTX](drogon::ReqResult res,
                                      const drogon::HttpResponsePtr &resp) {
    REQUIRE(res == drogon::ReqResult::Ok);

    REQUIRE(resp != nullptr);

    CHECK(resp->getStatusCode() == drogon::HttpStatusCode::k200OK);
  });
}

DROGON_TEST(BenchmarkQueryAPI){
    ankerl::nanobench::Bench().run("Query API Benchmark", [TEST_CTX] {
    auto client = drogon::HttpClient::newHttpClient(test_ip);

    auto req = drogon::HttpRequest::newHttpRequest();

    req->setPath("/query");

    client->sendRequest(req, [TEST_CTX](drogon::ReqResult res,
                                        const drogon::HttpResponsePtr &resp) {});
  });
}

int main(int argc, char **argv) {

  std::promise<void> p1;

  std::future<void> f1 = p1.get_future();

  std::jthread thr([&p1]() 
  {
    drogon::app().getLoop()->queueInLoop([&p1]() { p1.set_value(); });
  
    drogon::app().addListener("0.0.0.0", 3002).run();
  });

  f1.get();

  int status = drogon::test::run(argc, argv);

  drogon::app().getLoop()->queueInLoop([]() { drogon::app().quit(); });

  thr.join();

  return status;
}
