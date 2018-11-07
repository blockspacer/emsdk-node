#include <iostream>
#include <string>

#include <emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

double add(double a, double b) {
  return a + b;
}

std::string exclaim(std::string message) {
  return message + "!";
}

EMSCRIPTEN_BINDINGS(my_module) {
  function("add", &add);
  function("exclaim", &exclaim);
}