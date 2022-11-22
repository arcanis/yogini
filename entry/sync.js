const {constants, prepare} = require(`./prepare`);
const yoga = require(`../dist/yoga-sync`);

function bind(_, proto) {
  return proto
}

module.exports = {
  ...constants,

  Config: {
    create: wasmBinary => {
      return prepare(bind, yoga({wasmBinary})).Config.create();
    },
  },
};
