// This project must be compiled with C++17
#include "STEFWideScreenMod.hpp"

#include <iostream>

using namespace std;

int main()
{
	STEFWideScreenMod modder;

	cout << "Welcome to the Star Trek Voyager Elite Force Wide Screen Modder." << endl
		 << endl;

	modder.detectGameDirectory();
	modder.detectModDirectory();
	modder.getPreferredResolution();

	system("pause");

	modder.applyMod();

	cout << "Have fun!" << endl
		 << endl;

	return 0;
}