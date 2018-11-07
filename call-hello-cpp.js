const path = require("path");
// to resolve wasm file
process.chdir(path.join(__dirname, "out"));
const Module = require("./out/hello-func-cpp");
console.log(Module);
console.log(Module.add(1, 2.3));
console.log(Module.exclaim("hello world"));
