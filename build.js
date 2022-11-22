const {execFile} = require(`child_process`);
const fs = require('fs');
const {build} = require('esbuild');
const flow = require('esbuild-plugin-flow');
const glob = require('fast-glob');
const {promisify} = require(`util`);

const execFileP = promisify(execFile);

const yogaSources = glob(`yoga/yoga/*.cpp`);
const bindingSources = glob(`bindings/*.cc`);

async function start(async) {
  const type = async
    ? `async`
    : `sync`;

  await execFileP(`emcc`, [
    ...await yogaSources,
    ...await bindingSources,
	  `--bind`,
	  `-Iyoga`,
	  `-g`,
	  `-flto`,
	  `-s`, `WASM=1`,
	  `-s`, `WASM_ASYNC_COMPILATION=${async ? 1 : 0}`,
	  `-s`, `USE_ES6_IMPORT_META=0`,
	  `-s`, `ASSERTIONS=0`,
	  `-s`, `ALLOW_MEMORY_GROWTH=1`,
	  `-s`, `DYNAMIC_EXECUTION=0`,
	  `-s`, `TEXTDECODER=0`,
	  `-s`, `MODULARIZE=1`,
	  `-s`, `ERROR_ON_UNDEFINED_SYMBOLS=0`,
	  `-s`, `FILESYSTEM=0`,
	  `-s`, `MALLOC="emmalloc"`,
	  `-s`, `INCOMING_MODULE_JS_API=['wasmBinary']`,
	  `-s`, `EXPORT_NAME="yoga"`,
	  `-o`, `dist/yoga-${type}.js`,
  ]);
}

async function main() {
  await Promise.all([
    start(false),
    start(true),
  ]);

  const [
    syncBuffer,
    asyncBuffer,
  ] = await Promise.all([
    fs.promises.readFile(`dist/yoga-sync.wasm`),
    fs.promises.readFile(`dist/yoga-async.wasm`),
  ]);

  if (Buffer.compare(syncBuffer, asyncBuffer) !== 0)
    throw new Error(`Expected the generated WASM file to be the same in sync / async modes`);

  await fs.promises.copyFile(
    `./dist/yoga-sync.wasm`,
    `./dist/yoga.wasm`,
  );

  await Promise.all([
    await fs.promises.rm(`dist/yoga-sync.wasm`),
    await fs.promises.rm(`dist/yoga-async.wasm`),
  ]);
}

main();
