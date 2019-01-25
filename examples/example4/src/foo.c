#include "foo.h"

#include "bar.h"

int foo(int y) {
  int x = 0;
  bar(&x, y);
  return x;
}

int foo_array(void) {
  int x[16] = {0};
  bar_array(x, 16);
  return 16;
}
