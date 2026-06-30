#include "MyControllerTest.hpp"
#include "oatpp/Environment.hpp"

#include <print>

void runTests() {

  OATPP_RUN_TEST(MyControllerTest);
  
  // TODO - Add more tests here:
  // OATPP_RUN_TEST(MyAnotherTest);
  
}

int main() {

    oatpp::Environment::init();

    runTests();

    /* Print how much objects were created during app running, and what have left-probably leaked */
    /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
    std::println("\nEnvironment:");
    std::println("objectsCount = {}", oatpp::Environment::getObjectsCount());
    std::println("objectsCreated = {}", oatpp::Environment::getObjectsCreated());

    OATPP_ASSERT(oatpp::Environment::getObjectsCount() == 0);

    oatpp::Environment::destroy();

    return 0;
}
