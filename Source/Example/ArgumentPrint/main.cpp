
#include <stdio.h>
#include <stdlib.h>
#include <conio.H>
#include <iostream>
#include <string>

using namespace std;


void main(char argc, char *argv[])
{
	cout << "ArgumentPrint.exe";

	for (int i = 0; i < argc; ++i)
	{
		cout << ", ";
		cout << argv[i];
	}

	_getch();
}
