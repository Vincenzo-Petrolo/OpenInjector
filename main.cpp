/**
 * Single injection example.
 */
#include <iostream>
#include <glog/logging.h>
#include "binary.h"
#include "injector.h"
#include "tester.h"

#define EXAMPLE 1

#if EXAMPLE == 0
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
		std::cout << std::to_string(i) << std::endl;
		b->regenerate(copy);
	}
	b->destroy_copy(copy);
	delete t;
	delete b;
	return 0;
}

#endif

#if EXAMPLE == 1
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
#if 0
	int i;
	int starting_text = 4198624;
	int ending_text = 4724416;
	/*Injects only in .text section of ELF binary*/
	for (i = starting_text; i < ending_text; i++) {
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
		std::cout << std::to_string(i) << std::endl;
		b->regenerate(copy);
	}
#endif
	int i;
	int starting_data = 49808;
	int ending_data = 50008;
	/*Injects only in .data section of ELF binary*/
	for (i = starting_data-50; i <= ending_data; i++) {
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
		std::cout << std::to_string(i) << std::endl;
		b->regenerate(copy);
	}
	b->destroy_copy(copy);
	delete t;
	delete b;
	return 0;
}

#endif
