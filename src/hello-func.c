#include <stdio.h>
#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
void hello(){
  printf("Hello, world!\n");
}

EMSCRIPTEN_KEEPALIVE
int daysInWeek() {
  return 7;
}