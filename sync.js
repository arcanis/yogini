const entry = require(`./entry`);
const yoga = require(`./tmp/yoga-async`);

function bind(_, proto) {
  return proto
}

module.exports = async wasm => {
  const mod = await yoga({
    instantiateWasm(info, receive) {
      const module = new WebAssembly.Module(wasm);
      return new WebAssembly.Instance(module, {});
    },
  });

  return entry(bind, mod);
};
