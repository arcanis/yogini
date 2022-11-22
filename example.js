const fs = require(`fs`);
const wasm = fs.readFileSync(require.resolve(`yogini/yoga.wasm`));

function mainSync() {
    const Yoga = require(`yogini/sync`);

    const config = Yoga.Config.create(wasm);
    const node = config.createNode();
}

async function mainAsync() {
    const Yoga = require(`yogini/sync`);

    const config = await Yoga.Config.create(wasm);
    const node = config.createNode();
}

mainSync();
mainAsync();
