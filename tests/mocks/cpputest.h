/*
 * Copyright (C) 2020  Jordi Sánchez
 * This file is part of CPM Hub
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

#include <list>
#include <regex>
#include <vector>

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#define ASSERT_TRUE(condition)                          CHECK(condition)
#define ASSERT_FALSE(condition)                         CHECK_FALSE(condition)
#define ASSERT_EQUAL(expected, actual)                  CHECK_EQUAL(expected, actual)
#define ASSERT_STRING(expected, actual)                 STRCMP_EQUAL(std::string(expected).c_str(), std::string(actual).c_str())
#define ASSERT_THROWS(expected_exception, expression)   CHECK_THROWS(expected_exception, expression)


#define TEST_WITH_MOCK(testGroup, testName) \
  class TEST_##testGroup##_##testName##_TestShell; \
  extern TEST_##testGroup##_##testName##_TestShell TEST_##testGroup##_##testName##_TestShell_instance; \
  \
  class TEST_##testGroup##_##testName##_Test : public TEST_GROUP_##CppUTestGroup##testGroup { \
public: TEST_##testGroup##_##testName##_Test () : TEST_GROUP_##CppUTestGroup##testGroup () {} \
       void testBodyWrapped(); \
       void testBody(); \
}; \
  class TEST_##testGroup##_##testName##_TestShell : public UtestShell { \
      virtual Utest* createTest() _override { return new TEST_##testGroup##_##testName##_Test; } \
  } TEST_##testGroup##_##testName##_TestShell_instance; \
  static TestInstaller TEST_##testGroup##_##testName##_Installer(TEST_##testGroup##_##testName##_TestShell_instance, #testGroup, #testName, __FILE__,__LINE__); \
    void TEST_##testGroup##_##testName##_Test::testBody() { testBodyWrapped(); mock().checkExpectations(); mock().clear(); } \
    void TEST_##testGroup##_##testName##_Test::testBodyWrapped()


int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
