// This project must be compiled with C++17
#include "STEFWideScreenMod.hpp"

#include <iostream>

using namespace std;

int main()
{
	STEFWideScreenMod modder;

	cout << "Welcome to the Star Trek Voyager Elite Force Wide Screen Modder." << endl
		 << endl;

	modder.detectConfigFile();
	modder.getGameDirectory();
	modder.getModDirectory();
	modder.getPreferredResolution();

	system("pause");

	cout << endl;

	modder.applyMod();

	cout << "Have fun!" << endl
		 << endl;

	cout << "PS. If you encounter any problems with the mod. Remember to visit this link for solutions:" << endl
		 << "https://github.com/fverco/STEFWideScreenMod#common-problems" << endl
		 << endl;

	system("pause");

	return 0;
}