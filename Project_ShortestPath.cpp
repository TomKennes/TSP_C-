// Operations_Research.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "Eurosave.h"
#include <vector>
#include "stdafx.h"
#include "Eurosave.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <istream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Copyright � 2015, Tom Kennes. All rights reserved. " << endl
		 << endl <<"You wouldn't steal a car. " << endl 
		 << "You wouldn't steal a purse. " << endl
		 << "You wouldn't steal a cellphone. " << endl
		 << "You wouldn't steal an algorithm. " << endl
		 << "Buying pirated algorithms is stealing! " << endl
		 << "Stealing is against the law!" << endl
		 << "************ PIRACY IS A CRIME!!! ********" << endl;


	system("pause");
	Eurosave Euro;
	Euro.read_file("Cities.txt", "Hotel-Costs.txt", "Staff.txt", "Names.txt");
	Euro.travel();
	Euro.optimize();

	system("pause");
	return 0;
}

