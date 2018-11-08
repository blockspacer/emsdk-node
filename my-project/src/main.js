import Vue from 'vue'
import App from './App.vue'



/*import createInstance from "./hello-func-cpp-wasm.wasm"
import Module from './hello-func-cpp-wasm.js';
*/
console.log(Module);
Module.onRuntimeInitialized = () => {
    new Vue({
      el: '#app',
      render: h => h(App)
    })
};
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