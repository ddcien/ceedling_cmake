#include "unity.h"
#include "unity_fixture.h"

#include "foo.h"
#include "mock_bar.h"

#include <string.h>

TEST_GROUP(foo);

TEST_SETUP(foo) {}

TEST_TEAR_DOWN(foo) {}

TEST(foo, ReturnThruPtr) {
  int x = 10;
  int y = 10;
  int ret;

  bar_Expect(&x, y);
  bar_IgnoreArg_x();

  x += y;
  bar_ReturnThruPtr_x(&x);

  ret = foo(y);
  TEST_ASSERT_EQUAL(x, ret);
}

TEST(foo, Array0) {
  int ret;
  int x[16] = {0};
  int y[16] = {0};
  bar_array_ExpectWithArray(x, 16, 16);
  ret = foo_array();
  TEST_ASSERT_EQUAL(16, ret);
  TEST_ASSERT_EQUAL_INT_ARRAY(y, x, 16);
}

TEST(foo, Array1) {
  int ret;
  int x[16] = {0};
  int y[16] = {0};

  bar_array_ExpectWithArray(x, 16, 16);
  bar_array_IgnoreArg_x();

  memset(x, 0xff, sizeof(x));
  bar_array_ReturnArrayThruPtr_x(x, 16);

  memset(y, 0xff, sizeof(y));

  ret = foo_array();
  TEST_ASSERT_EQUAL(16, ret);
  TEST_ASSERT_EQUAL_INT_ARRAY(y, x, 16);
}
