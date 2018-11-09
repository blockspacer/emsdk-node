import Vue from 'vue'
import App from './App.vue'

//import hello from '../dist/hello-func-cpp-wasm.js';
//console.log(hello);
//import createInstance from "../dist/hello-func-cpp-wasm.wasm"
//console.log(createInstance);

function initVue() {
  new Vue({
    el: '#app',
    render: h => h(App)
  });
}

const script = document.createElement('script');
script.src = (typeof WebAssembly === 'undefined') ? '/dist/hello-func-cpp-asmjs.js' : '/dist/hello-func-cpp-wasm.js';   // set script source
script.onload = () => {    // once script has loaded
  console.log("Loaded Emscripten.");
  if (typeof WebAssembly === 'undefined') {
    console.log("no WebAssembly support");
    initVue();
  } else {
      Module.onRuntimeInitialized = () => {
        initVue();
      };
  }
};
document.body.appendChild(script); // append script to DOM



/*var script = document.createElement('script')
script.src = "/dist/hello-func-cpp-wasm-asmjs.js"
document.body.appendChild(script)*/
/*var Module = Module || {
  wasmBinaryFile: '/dist/hello-func-cpp-wasm.wasm'
}*/
// loadWasm.js
/*let Module = {};    // create the Module object to hold the wasm code
function loadWASM() {
  
  return new Promise((resolve) => {
    
    fetch('/dist/hello-func-cpp-wasm.wasm')    // load the .wasm file
      .then(response => response.arrayBuffer())
      .then((buffer) => {    //return ArrayBuffer
        Module.wasmBinary = buffer;   // assign buffer to Module
        
        const script = document.createElement('script');
        script.src = '/dist/hello-func-cpp-wasm.js';   // set script source
        
        script.onload = () => {    // once script has loaded
          console.log("Loaded Emscripten.");
          resolve(Module);    // return Module
        };
        document.body.appendChild(script); // append script to DOM
      });
  });
};

loadWASM().then((m) => {    // 'm' now holds _myFunc()
  console.log("loadWASM", m);
  console.log(m.onRuntimeInitialized);
  initVue();
});*/

/*var xhr = new XMLHttpRequest();
xhr.open('GET', '/dist/hello-func-cpp-wasm.wasm', true);
xhr.responseType = 'arraybuffer';
xhr.onload = function() {
  Module.wasmBinary = xhr.response;
  var script = document.createElement('script');
  script.src = "/dist/hello-func-cpp-wasm.js";
  script.onload = function() {
    console.log("Module ", Module)
  };
  document.body.appendChild(script);
  if (typeof Module.onRuntimeInitialized === "undefined") {
    initVue();
  } else {
    Module.onRuntimeInitialized = () => {
      initVue();
    };
  }
};
xhr.send(null);*/


/*console.log(createInstance);

createInstance({
  'global': {},
  'env': {
    'memory': new WebAssembly.Memory({initial: 100, limit: 1000}),
    'table': new WebAssembly.Table({initial: 0, element: 'anyfunc'})
  }
}).then(instance => { console.log(1111111111); });*/
/*createInstance()
.then(m => {
  console.log(1111111111);
  console.log(m.instance.exports.add(1, 2));
  console.log(m.instance.exports.test());
});*/
/*Module.onRuntimeInitialized = _ => {

  let valTemplated = new Module.Templated();
  valTemplated.set(1);
  console.log("valTemplated", valTemplated.get());
  valTemplated.set("verylongstringmaybehere!verylongstringmaybehere!verylongstringmaybehere!verylongstringmaybehere!verylongstringmaybehere!");
  console.log("valTemplated", valTemplated.get());
}*/