all:
	cd cmake_build_debug && cmake .. && make -j4
configure:
	git clone https://github.com/google/sandboxed-api.git --branch v20190823
