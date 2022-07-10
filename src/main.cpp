#include <drogon/drogon.h>

int main() {
  drogon::app()
      .setLogPath("./")
      .setLogLevel(trantor::Logger::kWarn)
      .addListener("0.0.0.0", 3001)
      .setThreadNum(8)
      .run();
}
