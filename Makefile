
compile: build
	gcc ./src/*.c -o ./build/main -Iinclude -lraylib

build:
	mkdir -p ./build

run: compile
	./build/main

.PHONY: build compile run
