all: configure compile
compile:
	@echo "\n\033[1;93m█ Compiling...\033[0m\n\n" &&\
	mkdir -p cmake_build_debug &&\
	cd cmake_build_debug && cmake .. && cmake --build . -j4
	cp cmake_build_debug/openinjector .
configure:
	@echo "\n\033[1;93m█ Downloading library dependencies...\033[0m\n\n" &&\
	git clone https://github.com/google/sandboxed-api.git
docs:
	@mkdir -p documentation
	@echo "\n\033[1;93m█ Generating documentation...\033[0m\n\n" &&\
	doxygen Doxyfile &&\
	xdg-open documentation/html/index.html &>/dev/null	
clean:
	cd cmake_build_debug && make clean
