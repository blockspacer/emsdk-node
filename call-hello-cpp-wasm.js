const path = require("path");
// to resolve wasm file
process.chdir(path.join(__dirname, "out"));
const Module = require("./out/hello-func-cpp-wasm");

// see https://searchcode.com/codesearch/view/57974704/

Module.onRuntimeInitialized = _ => {
    let valTemplatedConstructor = new Module.TemplatedConstructor("1", "2", {isStr: true, value: "customStr"});
    console.log("valTemplatedConstructor", valTemplatedConstructor.get());
    valTemplatedConstructor.set(1);
    console.log("valTemplatedConstructor", valTemplatedConstructor.get());
    valTemplatedConstructor.set(4356236);
    console.log("valTemplatedConstructor", valTemplatedConstructor.get());
    
    let valTemplated = new Module.Templated();
    valTemplated.set(1);
    console.log("valTemplated", valTemplated.get());
    valTemplated.set("verylongstringmaybehere!verylongstringmaybehere!verylongstringmaybehere!verylongstringmaybehere!");
    console.log("valTemplated", valTemplated.get());
           // prints 123456789123456780 !!!
    valTemplated.set(123456789123456789);
    console.log("valTemplated", valTemplated.get());

    // test etd::optional, if(n == 0) => return std::nullopt;
    // see https://qiita.com/yumetodo/items/32eb162faa78f36009a6
    const op = Module.OptionalTest.foo(2);
    if(op.has_value()) {
        console.log("op.value(): ", op.value());
    }
    else{
        console.log("not set op.value()");
    }
    const op2 = Module.OptionalTest.foo(0);
    if(op2.has_value()) {
        console.log("op.value(): ", op2.value());
    }
    else{
        console.log("not set op.value()");
    }

    console.log("add: ", Module.add(1, 2.3));
    console.log("exclaim: ", Module.exclaim("hello world"));
    console.log("_daysInWeek: ", Module._daysInWeek());
    console.log("_hoursInDay: ", Module._hoursInDay());
    console.log("set buffer to 999");
    //Module.makeBuffer(999);
    console.log("_getBuffer: ", Module._getBuffer());
    console.log("SOME_CONSTANT: ", Module.SOME_CONSTANT);

    const c = new Module.Counter(22);
    console.log(c.counter); // prints 22
    c.increase();
    console.log(c.counter); // prints 23
    console.log(c.squareCounter()); // prints 529
    console.log( "getStringFromInstance", Module.Counter.getStringFromInstance(c) );

    console.log("processMessage", Module.processMessage({x: 50, y: 60, width:250, height:10, color: 77}));

    //var rect = new Module.Rectangle( 50, 60, 250, 10, 77 );
    //var rect = new Module.Rectangle{x: 50, y: 60, width:250, height:10, color: 0xFF000000};
    //console.log("rect: ", rect);

    
    // TODO


    //const someClass = new Module.SomeClass();
    //someClass.x = 4;
    /*console.log("someClass.x: ", Module.SomeClass);
    const testConstructor = new Module.HasConstructor({x: 777});
    console.log("testConstructor: ", testConstructor.x);*/

    /*var callbackPointer = Runtime.addFunction(function() {
        console.log('In the callback');
    });*/

    Module.call_callback(1, function() {
        console.log('In the callback');
    });

    //Module.ccall('call_callback', 'number', ['number'], [callbackPointer]);

    //var shape = new Module.Shape( {x: 50, y: 60, width:250, height:10, color: 77} );
    //console.log("shape: ", shape);

    var vectorHolder = new Module.VectorHolder();
    var vec = vectorHolder.get();
    console.log("vec.size: ", vec.size());

    var str = new Module.StringHolder('string #3');
    console.log("str: ", str.get());

    vec.push_back(str);
    //str.delete();

    // based on https://searchcode.com/file/75814069/tests/embind/embind.test.js
    console.log("vec.size: ", vec.size());
    console.log("vec.get(0): ", vec.get(0).get());
    console.log("vec.get(1): ", vec.get(1).get());
    console.log("vec.get(2): ", vec.get(2).get());

    str.delete();
    vec.delete();
    vectorHolder.delete();
};