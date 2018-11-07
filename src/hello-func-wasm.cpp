#include <iostream>
#include <string>
#include <algorithm>
#include <optional>

#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

using namespace emscripten;

// see https://developers.google.com/web/updates/2018/08/embind

extern "C" {
  EMSCRIPTEN_KEEPALIVE
  extern int daysInWeek();

  EMSCRIPTEN_KEEPALIVE
  extern int hoursInDay();

  EMSCRIPTEN_KEEPALIVE
  int getBuffer();
}

static int buffer;

int daysInWeek() {
  return 7;
}

int hoursInDay() {
  return 24;
}

int getBuffer() {
  return buffer;
}

double add(double a, double b) {
  return a + b;
}

std::string exclaim(std::string message) {
  return message + "!";
}

std::vector<std::uint8_t> makeBuffer(int input)
{
    std::cout << "makeBuffer" << std::endl;
    buffer = input;
    return std::vector<std::uint8_t>({
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B,
        0x0C, 0x0D, 0x0E, 0x0F,
    });
}

class Counter {
public:
  int counter;
  static int same_counter;

  Counter(int init) :
    counter(init) {
  }

  void foo() {
    std::cout << "foo\n";
  }

  void foo(int i) {
    std::cout << "foo(int i)\n";
  }

  void foo(float f) const  {
    std::cout << "foo(float f)\n";
  }

  void increase() {
    counter++;
    same_counter = counter;
  }

  int squareCounter() {
    same_counter = counter * counter;
    return counter * counter;
  }

  static std::string getStringFromInstance(const Counter& instance) {
    return std::to_string(same_counter);
  }
};

int Counter::same_counter = 0;

const double SOME_CONSTANT = 3.14;

struct Rectangle {
   int x = 0;
   int y = 0;
   int width = 100;
   int height = 100;
   int color = 5;
};

/*class Shape {
public: 
    Shape(Rectangle rect);  
};*/

std::string processMessage(Rectangle opts) {
  std::string rect_to_str = std::to_string(opts.x);
  std::cout << rect_to_str << std::endl;
  return rect_to_str;
}

// see https://github.com/manaflair/emmagic
// see https://searchcode.com/codesearch/view/25074422/
// see https://www.slideshare.net/chadaustin/connecting-c-and-javascript-on-the-web-with-embind

struct SomeClass {
   int x = 0;
};

struct HasConstructor {
    HasConstructor(SomeClass) {
    }
};

void call_callback(int num, emscripten::val callback) {
  std::cout << num << std::endl;
  callback();
}

class StringHolder {
public:
    StringHolder(const std::string& s)
        : str_(s)
    {}

    void set(const std::string& s) {
        str_ = s;
    }

    std::string get() const {
        return str_;
    }

    std::string& get_ref() {
        return str_;
    }

    const std::string& get_const_ref() const {
        return str_;
    }

private:
    std::string str_;
};

class VectorHolder {
public:
    VectorHolder() {
        v_.push_back(StringHolder("string #1"));
        v_.push_back(StringHolder("string #2"));
    }

    std::vector<StringHolder> get() const {
        return v_;
    }

    void set(std::vector<StringHolder> vec) {
        v_ = vec;
    }

private:
    std::vector<StringHolder> v_;
};

// see https://codertw.com/%E7%A8%8B%E5%BC%8F%E8%AA%9E%E8%A8%80/101846/
enum class NewStyle {
  ONE,
  TWO
};

template<typename T>
emscripten::class_<std::optional<T>> register_optional(const char* name) {
    using emscripten::val;
    using OptionalType = std::optional<T>;
    bool (*has_value)(const OptionalType&) = [](const OptionalType& op){ return op.has_value(); };
    val (*value)(const OptionalType&) = [](const OptionalType& op) { return val(op.value()); };
    void (*reset)(OptionalType&) = [](OptionalType& op) { op.reset(); };
    return emscripten::class_<OptionalType>(name)
        .template constructor<>()
        .function("has_value", has_value)
        .function("value", value)
        .function("reset", reset)
        ;
}

// https://qiita.com/yumetodo/items/32eb162faa78f36009a6
class OptionalTest{
public:
    static std::optional<double> foo(int n) {
        if(n == 0){
            return std::nullopt;
        }
        else{
            return { 7.2 / n};
        }
    }
};

// see https://github.com/charto/nbind

EMSCRIPTEN_BINDINGS(my_module) {
  emscripten::class_<OptionalTest>("OptionalTest")
    .constructor<>()
    .class_function("foo", &OptionalTest::foo);
    register_optional<double>("optional<double>");
  // enum
  enum_<NewStyle>("NewStyle")
    .value("ONE", NewStyle::ONE)
    .value("TWO", NewStyle::TWO)
  ;
  // constants
  constant("SOME_CONSTANT", SOME_CONSTANT);
  // functions
  function("call_callback", &call_callback);
  function("add", &add);
  function("exclaim", &exclaim);
  function("makeBuffer", &makeBuffer);
  function("processMessage", &processMessage);
  // class
  class_<Counter>("Counter")
    .constructor<int>()
    // static
    .class_function("getStringFromInstance", &Counter::getStringFromInstance)
    .function("increase", &Counter::increase)
    .function("squareCounter", &Counter::squareCounter)
    .function("foo", select_overload<void()>(&Counter::foo))
    .function("foo_int", select_overload<void(int)>(&Counter::foo))
    .function("foo_float", select_overload<void(float)const>(&Counter::foo))
    .property("counter", &Counter::counter);
  //
  /*class_<Shape>("Shape")
    .constructor<Rectangle>();*/
  value_object<Rectangle>("Rectangle")
      .field("x", &Rectangle::x)
      .field("y", &Rectangle::y)
      .field("width", &Rectangle::width)
      .field("height", &Rectangle::height)
      .field("color", &Rectangle::color);
  /*value_object<SomeClass>("SomeClass")
      .field("x", &SomeClass::x);*/
  /*class_<Shape>("Shape")
      .constructor<Rectangle>();*/
  /*class_<HasConstructor>("HasConstructor")
        .constructor<SomeClass>();*/

  class_<StringHolder>("StringHolder")
      .smart_ptr<std::shared_ptr<StringHolder>>("shared_ptr<StringHolder>")
      .constructor<std::string>()
      .function("set", &StringHolder::set)
      .function("get", &StringHolder::get)
      .function("get_const_ref", &StringHolder::get_const_ref)
      ;

    register_vector<StringHolder>("StringHolderVector");
    class_<VectorHolder>("VectorHolder")
        .constructor<>()
        .function("get", &VectorHolder::get)
        .function("set", &VectorHolder::set)
        ;
}