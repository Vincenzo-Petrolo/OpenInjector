compile:
	cd cmake_build_debug && cmake ..
	cmake --build cmake_build_debug -j4
configure:
	git clone https://github.com/google/sandboxed-api.git --branch v20190823
clean:
	cd cmake_build_debug && make clean
