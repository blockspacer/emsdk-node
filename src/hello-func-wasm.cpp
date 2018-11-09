#include <iostream>
#include <string>
#include <algorithm>
#include <optional>
#include <cstdlib>
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

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

// see https://github.com/kripken/emscripten/blob/b6b2c0ff37ed761a3ea8ad0371bf985f5004cfde/system/include/emscripten/val.h
std::vector<emscripten::val> vecFromJSArray(val v) {
    auto l = v["length"].as<unsigned>();
    //Object.keys()

    std::vector<emscripten::val> rv;
    for(unsigned i = 0; i < l; ++i) {
        rv.push_back(v[i]);
    }

    return rv;
};

static void each_arr(emscripten::val arr, emscripten::val callback) {
    for (emscripten::val& i: vecFromJSArray(arr)) callback(arr, i);
}

static void each_vec(emscripten::val arr, emscripten::val callback) {
    for (emscripten::val& i: vecFromJSArray(arr)) callback(i);
}

/*void call_callback(int num, emscripten::val callback) {
  std::cout << num << std::endl;
  callback();
}*/

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

// see https://medium.com/coinmonks/develop-w3c-web-components-with-webassembly-d65938284255
class Code39 {

public:
  Code39()
  {
	inputData="12345678";
	checkDigit=1;
  }

  Code39(std::string inputData, int checkDigit)
    : checkDigit(checkDigit)
    , inputData(inputData)
  {}

  std::string encode() {  

	std::string filteredData=filterInput(inputData);

	int filteredlength = filteredData.length();
	std::string result;
	if (checkDigit==1)
		result="*"+filteredData+generateCheckDigit(filteredData)+"*";
	else
		result="*"+filteredData+"*";

	std::string mappedResult;
	for (int x=0;x<result.length();x++)
	{	       
	        mappedResult=mappedResult+"&#"+std::to_string((unsigned char)result[x])+";";	
    	}
	result=mappedResult;

	human_readable_text=result;
	return result;
  }
  int getCheckDigit() const { return checkDigit; }
  void setCheckDigit(int checkDigit_) { checkDigit = checkDigit_; }

  std::string getInputData() const { return inputData; }
  void setInputData(std::string inputData_) { inputData = inputData_; }

  std::string getHumanReadableText() const { return human_readable_text; }

private:

  std::string inputData;
  std::string human_readable_text;
  int checkDigit;
  std::string result;
  std::string CODE39MAP[43]={"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","-","."," ","$","/","+","%"};

  int getCode39Value(char inputchar) {
	int RVal=-1;
	int i=0;
	for (i=0;i<43;i++)
	{
		if (inputchar==CODE39MAP[i][0])
		{
			RVal=i;
		}
	}
	return RVal;
  }

  std::string generateCheckDigit(std::string data)
  {
	std::string checkDigit;
	int datalength=data.length();
	int sumValue=0;
	int x=0;
	for (x=0;x<datalength;x++)
	{
		sumValue=sumValue+getCode39Value(data[x]);
	}
	sumValue=sumValue % 43;
	return CODE39MAP[sumValue];
  }

  std::string filterInput(std::string data)
  {
	std::string result;
	int x=0;
	int y=0;
	for (x=0; x < data.length() && y < 255; x++)
	{
		if (getCode39Value(data[x]) != -1)
		{
			result=result+data[x];
			y++;	
		}
	}
	return result;
  }

};

class Templated {
private:
    emscripten::val value;

    //template<typename U, typename Fn, class... Args>
    //Templated<U> link(Fn fn, Args&&... args);

public:
    Templated() : value(0) {};

    void set(emscripten::val val);

    emscripten::val get();
};

/*template<typename T>
Templated<T>::Templated(T value)() {
    value = value;
}*/

void Templated::set(emscripten::val val) {
    value = val;
}

emscripten::val Templated::get() {
    return value;
}

template<typename T> 
class TemplatedConstructor {
private:
    T value;
    int p1;
    int p2;
    std::string str;

    //template<typename U, typename Fn, class... Args>
    //TemplatedConstructor<U> link(Fn fn, Args&&... args);

public:
    TemplatedConstructor() {
        std::cout << "TemplatedConstructor() called" << std::endl;
    };
    TemplatedConstructor(T val) : value(val) {
        std::cout << "TemplatedConstructor(T val) called" << std::endl;
    };
    TemplatedConstructor(std::string val) : str(val) {
        std::cout << "TemplatedConstructor(std::string val) called" << std::endl;
    };
    TemplatedConstructor(int a1, int a2) { p1 = a1; p2 = a2; };

    void set(T val) {
        value = val;
        str = std::to_string(value);
    }

    T get() {
        std::cout << "str:" << str << std::endl;
        std::cout << "p1:" << p1 << std::endl;
        std::cout << "p2:" << p2 << std::endl;
        return value;
    }
};

//TemplatedConstructor<int>* makeMyClass(int, int); //Factory function.

/*template<typename T>
TemplatedConstructor<T>::TemplatedConstructor(T value)() {
    value = value;
}*/


// see https://github.com/charto/nbind

//using emscripten::val;

EMSCRIPTEN_BINDINGS(my_module) {
    // Templates
class_<Templated>("Templated")
    .constructor<>()
    .function("set", &Templated::set)
    .function("get", &Templated::get);
class_<TemplatedConstructor<int>>("TemplatedConstructor")
    .constructor<>()
    //.constructor<int>()
    .constructor<emscripten::val>( select_overload<TemplatedConstructor<int>(emscripten::val)>([](emscripten::val c) {
       std::cout << "int constructor wrapped with emscripten::val" << std::endl;
       std::cout << "c.typeof().as<std::string>() is " << c.typeof().as<std::string>() << std::endl;
       if (c.typeof().as<std::string>() == "string") {
           return TemplatedConstructor<int>("1");
       } else if (c.typeof().as<std::string>() == "boolean") {
           return TemplatedConstructor<int>(true);
       }
       return TemplatedConstructor<int>(1);
    }))
    //.constructor<std::string>()
    //.constructor<std::string>(select_overload<void(std::string)>(&TemplatedConstructor<std::string>::TemplatedConstructor<std::string>))
    .constructor<int, int>()
    //.constructor(&makeMyClass, allow_raw_pointers())
    //.constructor<int, int>("TemplatedConstructor_2")
    /*.constructor<std::string>( select_overload<TemplatedConstructor<int>(std::string, std::string, std::string)>([](std::string a, std::string b, std::string c) {
       std::cout << "wrap constructor in lambda 1" << std::endl;
       return TemplatedConstructor<int>("wrapped constructor");
    }))*/

    // Idea: use emscripten::val && hasOwnProperty to create overloaded constructors. We can call native constructor based on Properties
    .constructor<std::string>( select_overload<TemplatedConstructor<int>(std::string, std::string, emscripten::val)>([](std::string a, std::string b, emscripten::val c) {
       std::cout << "wrap constructor in lambda 1" << std::endl;
       std::cout << "c.typeof().as<std::string>() is " << c.typeof().as<std::string>() << std::endl;
       if (c.hasOwnProperty("isStr")){
           std::cout << "hasOwnProperty isStr" << std::endl;
           std::cout << "c[\"isStr\"].typeof().as<std::string>() is " << c["isStr"].typeof().as<std::string>() << std::endl;
           std::cout << "c[\"value\"].typeof().as<std::string>() is " << c["value"].typeof().as<std::string>() << std::endl;
           //emscripten::val strVal = c["isStr"];
           //std::string getVal;
           std::cout << "strVal: " << c["value"].as<std::string>() << std::endl;
           
           return TemplatedConstructor<int>(c["value"].as<std::string>());
       }
       if (c.hasOwnProperty("isNumber")){
           std::cout << "hasOwnProperty isNumber" << std::endl;
           return TemplatedConstructor<int>("wrapped constructor");
       }
       return TemplatedConstructor<int>("wrapped constructor");
    }))
    //.constructor<int>("TemplatedConstructor_int")
    .function("set", &TemplatedConstructor<int>::set)
    .function("get", &TemplatedConstructor<int>::get);
    //
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
  function("each_arr", &each_arr);
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

    // see https://medium.com/coinmonks/develop-w3c-web-components-with-webassembly-d65938284255
    class_<Code39>("Code39")
    .constructor<>()
    .constructor<std::string, int>()
    .function("encode", &Code39::encode)
    .property("checkDigit", &Code39::getCheckDigit, &Code39::setCheckDigit)
    .property("inputData", &Code39::getInputData, &Code39::setInputData)
    .property("humanReadableText", &Code39::getHumanReadableText)
    ;
}