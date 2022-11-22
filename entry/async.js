const setupModule = require(`../dist/yoga-async`);

const {constants, prepare} = require(`./prepare`);

module.exports = {...constants};

module.exports.createContext = async wasmBinary => {
  const lib = prepare(await setupModule({wasmBinary}));

  return new lib.Config.create();
};
