const {constants, prepare} = require(`./prepare`);
const yoga = require(`../dist/yoga-async`);

function bind(_, proto) {
  return proto
}

module.exports = {
  ...constants,

  Config: {
    create: async wasmBinary => {
      return prepare(bind, await yoga({wasmBinary})).Config.create();
    },
  },
};
