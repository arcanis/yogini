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

  await fs.promises.mkdir(`tmp`, {recursive: true});
  await fs.promises.mkdir(`dist`, {recursive: true});

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
	  `-s`, `INCOMING_MODULE_JS_API=['instantiateWasm','locateFile']`,
	  `-s`, `EXPORT_NAME="yoga"`,
	  `-o`, `tmp/yoga-${type}.js`,
  ]);

  await build({
    bundle: true,
    sourcemap: false,
    format: `cjs`,
    target: `esnext`,
    platform: `node`,
    loader: {[`.js`]: `ts`},
    entryPoints: [`./${type}.js`],
    outfile: `./dist/${type}.js`,
    plugins: [flow(/\.js$/, true)],
  });

  await fs.promises.copyFile(
    `./${type}.d.ts`,
    `./dist/${type}.d.ts`,
  );
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
    fs.promises.readFile(`tmp/yoga-sync.wasm`),
    fs.promises.readFile(`tmp/yoga-async.wasm`),
  ]);

  if (Buffer.compare(syncBuffer, asyncBuffer) !== 0)
    throw new Error(`Expected the generated WASM file to be the same in sync / async modes`);

  await fs.promises.copyFile(
    `./tmp/yoga-sync.wasm`,
    `./dist/yoga.wasm`,
  );
}

main();
