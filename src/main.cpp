/**
 * Single injection example.
 */
#include <iostream>
#include "binary.h"
#include "injector.h"
#include "tester.h"

int main(int argc, char **argv) {
	Binary *b;
	Tester *t;
	size_t size;
	try {
		std::string pathname(argv[1]);
		b = new Binary(pathname);
	} catch (std::invalid_argument e) {
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE);
	} catch (std::bad_alloc e) {
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	binary_t *copy = b->get_copy();
	
	size = b->get_size();
	
	try {
	
		t = new Tester(copy,size);
	
	} catch (std::string error_msg) {
	
		std::cout << error_msg << std::endl;
	}
	int i = 0;

#if 0
	for (i = 0; i < b->get_size()*8; i++) {
		Injector::singleInjection(i,copy);
		try {
			t->testFaulty(copy,size);
		} catch (int e) {
			std::cout <<
			"Caught exception : "
			<< std:: endl
			<< e
			<< std::endl;
		}
		b->regenerate(copy);
	}
#endif
	b->destroy_copy(copy);
	t->stringifyStats();
	t->writeCSV(Tester::O_SHORT);
	delete t;
	delete b;
	return 0;
}
