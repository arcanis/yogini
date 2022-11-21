const entry = require(`./entry`);
const yoga = require(`./tmp/yoga-async`);

function bind(_, proto) {
  return proto
}

module.exports = async wasm => {
  const mod = await yoga({
    instantiateWasm(info, receive) {
      WebAssembly.instantiate(wasm, info).then((instance) => {
        receive(instance.instance || instance);
      });
      return {};
    },
  });

  return entry(bind, mod);
};

module.exports.initStreaming = async response => {
  const mod = await yoga({
    instantiateWasm(info, receive) {
      WebAssembly.instantiateStreaming(response, info).then((instance) => {
        receive(instance.instance || instance);
      });
      return {};
    },
  });

  return entry(bind, mod);
}
