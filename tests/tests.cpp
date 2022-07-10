#define DROGON_TEST_MAIN
#define ANKERL_NANOBENCH_IMPLEMENT
#include <drogon/HttpTypes.h>
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include <nanobench.h>


//Make sure tubedl-core is running in order to run API tests correctly
DROGON_TEST(RemoteAPITest) {
  auto client = drogon::HttpClient::newHttpClient("http://127.0.0.1:3002");
  auto req = drogon::HttpRequest::newHttpRequest();
  req->setPath("/");
  client->sendRequest(req, [TEST_CTX](drogon::ReqResult res,
                                      const drogon::HttpResponsePtr &resp) {
    REQUIRE(res == drogon::ReqResult::Ok);
    REQUIRE(resp != nullptr);

    CHECK(resp->getStatusCode() == drogon::HttpStatusCode::k200OK);
  });
}

int main(int argc, char **argv) {
  std::promise<void> p1;
  std::future<void> f1 = p1.get_future();

  std::jthread thr([&]() {
    drogon::app().getLoop()->queueInLoop([&p1]() { p1.set_value(); });
    drogon::app().addListener("0.0.0.0", 3002).run();
  });

  f1.get();
  int status = drogon::test::run(argc, argv);

  drogon::app().getLoop()->queueInLoop([]() { drogon::app().quit(); });
  thr.join();
  return status;
}
