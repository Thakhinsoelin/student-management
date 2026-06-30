#pragma once

#include <oatpp-test/UnitTest.hpp>

class MyControllerTest : public oatpp::test::UnitTest {
public:

  MyControllerTest() : UnitTest("TEST[MyControllerTest]" /* Test TAG for logs */){}
  void onRun() override;

};
