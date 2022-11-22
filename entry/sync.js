const setupModule = require(`../dist/yoga-sync`);

const {constants, prepare} = require(`./prepare`);

module.exports = {...constants};

module.exports.createContext = wasmBinary => {
  const lib = prepare(setupModule({wasmBinary}));

  return new lib.Config.create();
};
