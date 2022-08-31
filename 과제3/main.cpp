#include <iostream>
#include "Str.h"
#include <string.h>

int main() {
	// sample main code for Str class
	// test all methods in Str class: constructor, contents, operator =, length
	Str a("I'm a girl");
	std::cout << a.contents();
	a = "I'm a boy\n";
	std::cout << a.contents();
	std::cout << "Compare a with some string: " << a.compare("I'm a a") << std::endl;
	std::cout << "length of a; " <<  a.length() << std::endl;

	Str b("Too many reports...\n");
	std::cout << "Length of b: " << b.length() << "/ Contents of b: " << b.contents();

	Str c(20);
	std::cout << "Length of c: " << c.length() << "/ Contents of c: " << c.contents() << "check" << std::endl;
	c = "Why so many Assignments for me?";
	std::cout << "Length of c: " << c.length() << "/ Contents of c: " << c.contents() << std::endl;
	std::cout << "Compare c with b: " << c.compare(b) << std::endl;

	c = a;
    std::cout << "Length of new c: " << c.length() << "/ Contents of new c: " << c.contents() << std::endl; 
	Str d("Because of 6 major lecture... somebody help me..");
	std::cout << "Length of d: " << d.length() << "/ Contents of d: " << d.contents() << std::endl;
	
	std::cout << "Compare d with c: " << d.compare(c) << std::endl;
	
	Str er(-3); // error message should be printed and program will be ended
	return 0;
}
