all: configure compile
compile:
	mkdir -p cmake_build_debug &&\
	cd cmake_build_debug && cmake .. && cmake --build . -j4
	cp cmake_build_debug/openinjector .
configure:
	git clone https://github.com/google/sandboxed-api.git
clean:
	cd cmake_build_debug && make clean
