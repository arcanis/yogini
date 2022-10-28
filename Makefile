CC=emcc

all: clean dir wasm

wasm:
	$(CC) yoga/yoga/*.cpp bindings/*.cc \
		--bind \
		-Iyoga \
		-g0 \
		-Os \
		-flto \
		--closure 1 \
		-msimd128 \
		-s WASM=1 \
		-s WASM_ASYNC_COMPILATION=1 \
		-s DISABLE_EXCEPTION_CATCHING=1 \
		-s ASSERTIONS=0 \
		-s ALLOW_MEMORY_GROWTH=1 \
		-s MODULARIZE=1 \
		-s DYNAMIC_EXECUTION=0 \
		-s TEXTDECODER=0 \
		-s ENVIRONMENT='web' \
		-s ERROR_ON_UNDEFINED_SYMBOLS=0 \
		-s FILESYSTEM=0 \
		-s MALLOC="emmalloc" \
		-s INCOMING_MODULE_JS_API=['instantiateWasm','locateFile']\
		-s EXPORT_NAME="yoga" \
		-s "DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=['memcpy','memset','malloc','free','strlen']" \
		-o dist/yoga.mjs

clean:
	rm -rf dist 

dir:
	mkdir -p dist
