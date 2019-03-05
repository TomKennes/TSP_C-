// Project TSP. Defines the entry point.
// Tom Kennes, I6081179
//

// Copyright � 2014 Tom Kennes All Rights Reserved

#include "stdafx.h"
#include <iostream>
#include <string>
#include "Pineapple.h"
#include <time.h>
#include <cstdlib>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(0));
	string filename;
	char choice, second_choice, third_choice;
	int time;
	Pineapple TSP;
	bool correct, Nearest = false, Farthest = false, Simple = false, Rand = false, SpanTree = false;
	clock_t start, end;


	
	//Initialization
	cout << "Please store the coordinates at the appropriate place. " << endl
		 << "Use the following format: " << endl
		 << "Line 1: number of cities. " << endl
		 << "line 2 to n: City number <space> x-coordinate <space> y-coordinate."
		 << endl << endl << endl;
	do{
		correct = true;
		cout << "What is the name of your file? " << endl
			 << "Remember to include: .txt  " << endl;
		cin >> filename;
		if(filename == "quit"){return 0;};
		if(!TSP.Processor(filename)){
			correct = false;
			cout << "1. Please check whether the file is located properly. "<< endl
				 << "2. Also make sure it is spelled correctly." << endl
				 << "Please try again." << endl << endl;
		}
		Simple = true;
	}while(!correct);

	
	
	//Do you wish to see the simplest solution, continue to the heuristics or maybe quit?
	cout << "The system has generated a preliminary simple solution, based on the data. " << endl
		 << "If you wish to view the path and its length, type 'y'." << endl
		 << "If you, however, wish to use a heuristic to find a perhaps better primal solution, type 'n' " << endl;
	do{
	cin  >> choice;
	switch(choice){
	case 'y':
	case 'Y':
		correct = true;
		TSP.getlength(TSP.Ssimple.order);
		cout << "If you wish to quit, type 'quit', otherwise press any key to continue to heuristic analysis. " << endl;
		cin >> filename;
		if(filename == "quit"){return 0;}
		break;
	case 'n':
	case 'N':
		correct = true;
		break;
	case 'q':
	case 'Q':
		return 0;
		break;
	default:
		correct = false;
		cout << "Not an accepted answer. " << endl 
			 << "For yes, type y." << endl
			 << "For no, type n." << endl
			 << "To quit, type q." << endl;
	}
	}while(!correct);
	

	// Which heuristic program do you want to execute? The answers are saved within the program.
	// It is also possible to execute multiple programs.
	do{
		cout << endl << "This system offers three heuristic approaches: " << endl
		 << "Press 'a' \t for the nearest neighbour approach. " << endl
		 << "Press 'b' \t for the farthest neighbour approach. " << endl
		 << "Press 'c' \t for the best random path approach. " << endl
		 << "Press 'd' \t for the minimal spanning tree approach. " << endl
		 << "Press 'q' \t to quit this program. " << endl;
		cin >> choice;
		correct = true;
	switch(choice)
	{
	case 'a':
	case 'A':
		cout << endl << "Nearest neighbour approach: " << endl;
		start = clock();
		TSP.Solve_NearestNeighbour();
		end = clock();
		cout << "Runtime: " <<  1000*((end-start)/CLOCKS_PER_SEC) << " ms" << endl;
		cout << endl << "If you wish to try another, or the same, heuristic, type 'y'. Otherwise, press any key." << endl;
		cin >> choice;
		Nearest = true;
		if(choice == 'y' || choice == 'Y'){correct = false;}
		break;
	case 'b':
	case 'B':
		cout << endl << "Farthest neighbour approach: " << endl;
		start = clock();
		TSP.Solve_FarthestNeighbour();
		end = clock();
		cout << "Runtime: " << 1000.0* ((end-start)/CLOCKS_PER_SEC) << " ms" << endl;
		cout << endl << "If you wish to try another, or the same, heuristic, type 'y'. Otherwise, press any key." << endl;
		cin >> choice;
		Farthest = true;
		if(choice == 'y' || choice == 'Y'){correct = false;}
		break;
	case 'c':
	case 'C':
		cout << endl << "Best random tour approach: " << endl;
		cout << "How many seconds do you want the program to execute random tours?" << endl;
		cin >> time;
		TSP.Solve_Randomtour(time);
		cout << endl << "If you wish to try another, or the same, heuristic, type 'y'. Otherwise, press any key." << endl;
		cin >> choice;
		Rand = true;
		if(choice == 'y' || choice == 'Y'){correct = false;}
		break;
	case 'd':
	case 'D':
		cout << endl << "Minimal spanning tree approach: " << endl;
		start = clock();
		TSP.Solve_Spanningtree();
		end = clock();
		cout << "Runtime: " << 1000.0* ((end-start)/CLOCKS_PER_SEC) << " ms" << endl;
		cout << endl << "If you wish to try another, or the same, heuristic, type 'y'. Otherwise, press any key." << endl;
		cin >> choice;
		SpanTree = true;
		if(choice == 'y' || choice == 'Y'){correct = false;}
		break;
	case 'q':
	case 'Q':
		return 0;
		break;
	default:
		correct = false;
		cout << "That is not a valid option. Come again." << endl << endl << endl;
	}
	}while(!correct);
	

	// Which heuristic solution do you want to improve? again the answers are saved within the program.
	// It is also possible to improve upon multiple heurisitcs.
	do{
		correct = true;
		cout << "You now have: " << endl;
		if(Simple){cout << "A simple solution." << endl;}
		if(Nearest){cout << "A solution according to the nearest neighbour heuristic." << endl;}
		if(Farthest){cout << "A solution according to the furthest neighbour heuristic." << endl;}
		if(Rand){cout << "A solution according to the best random tour heuristic." << endl;}
		if(SpanTree){cout << "A solution according to the minimal spanning tree. " << endl;}

		cout << "Do you want to improve the tour using a 2-opt mechanism? Press 'y'. " << endl;
		cout << "Do you not want to improve any tours using a 2-opt mechanism? Press 'n.'" << endl;
		cout << "Do you want to quit the program? Press any other key. " << endl;
		cin >> choice;
			if(choice == 'y' || choice == 'Y'){
				if(!(!Simple && !Nearest && !Farthest && !Rand && !SpanTree)){
				if(Simple){cout << "Press 'a' to improve upon the simple heuristic. " << endl;}
				if(Nearest){cout << "Press 'b' to improve upon the Nearest neighbour heuristic. " << endl;}
				if(Farthest){cout << "Press 'c' to improve upon the Farthest neighbour heuristic. " << endl;}
				if(Rand){cout << "Press 'd' to improve upon the best random tour heuristic. " << endl;}
				if(SpanTree){cout << "Press 'e' to improve upon the minimal spanning tree heuristic. " << endl;}
				cin >> second_choice;
				switch(second_choice){
					case 'a':
					case 'A':
						if(!Simple){correct = false; cout << "That's not a valid option. Come again." << endl; break;}
						start = clock();
						TSP.Improve_2opt(TSP.Ssimple.order);
						end = clock();
						cout << "Runtime: " << 1000.0* ((end-start)/CLOCKS_PER_SEC) << " ms" << endl << endl;
						cout << "Do you want to improve more tours? Press 'y'. Otherwise, press any key." << endl;
						cin >> third_choice;
						if(third_choice == 'y' || third_choice == 'Y'){correct = false; Simple = false;}
						break;
					case 'b':
					case 'B':
						if(!Nearest){correct = false; cout << "That's not a valid option. Come again." << endl; break;}
						start = clock();
						TSP.Improve_2opt(TSP.Snn.order);	
						end = clock();
						cout << "Runtime: " << 1000.0* ((end-start)/CLOCKS_PER_SEC) << " ms" << endl << endl;
						cout << "Do you want to improve more tours? Press 'y'. Otherwise, press any key." << endl;
						cin >> third_choice;
						if(third_choice == 'y' || third_choice == 'Y'){correct = false; Nearest = false;}
					break;
					case 'c':
					case 'C':
						if(!Farthest){correct = false; cout << "That's not a valid option. Come again." << endl; break;}
						start = clock();
						TSP.Improve_2opt(TSP.Sfn.order);
						end = clock();
						cout << "Runtime: " << 1000.0* ((end-start)/CLOCKS_PER_SEC) << " ms" << endl;
						cout << "Do you want to improve more tours? Press 'y'. Otherwise, press any key. " << endl;
						cin >> third_choice;
						if(third_choice == 'y' || third_choice == 'Y'){correct = false; Farthest = false;}
					break;
					case 'd':
					case 'D':
						if(!Rand){correct = false; cout << "That's not a valid option. Come again." << endl; break;}
						cout << "Note that only the last tour that has been found using this heuristic can be improved upon, as new ones replace old ones in the main memory. " << endl;
						start = clock();
						TSP.Improve_2opt(TSP.Srt.order);
						end = clock();
						cout << "Runtime: " << 1000.0* ((end-start)/CLOCKS_PER_SEC) << " ms" << endl << endl;
						cout << "Do you want to improve more tours? Press 'y'. Otherwise, press any key. " << endl;
						cin >> third_choice;
						if(third_choice == 'y' || third_choice == 'Y'){correct = false; Rand = false;}
					break;
					case 'e':
					case 'E':
						if(!SpanTree){correct = false; cout << "That's not a valid option. Come again." << endl; break;}
						start = clock();
						TSP.Improve_2opt(TSP.Stree.order);
						end = clock();
						cout << "Runtime: " << 1000.0* ((end-start)/CLOCKS_PER_SEC) << " ms" << endl;
						cout << "Do you want to improve more tours? Press 'y'. Otherwise, press any key. " << endl;
						cin >> third_choice;
						if(third_choice == 'y' || third_choice == 'Y'){correct = false; Farthest = false;}
					break;
					case 'q':
					case 'Q':
						return 0;
					break;
					default:
						correct = false;
						cout << "That's not a valid option. Come again." << endl;
					}
				}
				else{cout << "You already have improved all tours. " << endl;}
			}
			if(choice == 'n' || choice == 'N'){cout << endl;}
			if(choice != 'n' && choice != 'N' && choice != 'y' && choice != 'Y'){return 0;}
	}while(!correct);


	// Here you have the option to save your solution, which may come in hand if you want to glance over a solution which
	// incorporates more than 250 cities, like the luxemburg file.
	// Again you can save multiple tours, as long as you do not overwrite already saved solutions.
	do{
		correct = true;
		cout << "Do you want to save a tour? Press 'y'. " << endl;
		cout << "Do you want to quit the program? Press any other key. " << endl;
		cin >> choice;
		if(choice == 'y' || choice == 'Y'){
			if(Simple){cout << "Press 'a' to save the simple heuristic. " << endl;}
			if(Nearest){cout << "Press 'b' to save the Nearest neighbour heuristic. " << endl;}
			if(Farthest){cout << "Press 'c' to save upon the Farthest neighbour heuristic. " << endl;}
			if(Rand){cout << "Press 'd' to save the best random tour heuristic. " << endl;}
			if(SpanTree){cout << "Press 'e' to save the minimal spanning tree heurstic. " << endl;}
			cin >> choice;
			switch(choice){
			case 'a':
			case 'A':
				TSP.Save_solution(TSP.Ssimple.order, TSP.Ssimple.length);
				cout << "Do you want to save more tours? Press 'y'. Otherwise, press any key. " << endl;
						cin >> choice;
						if(choice == 'y' || choice == 'Y'){correct = false; Rand = false;}
				break;
			case 'b':
			case 'B':
				TSP.Save_solution(TSP.Snn.order, TSP.Snn.length);
				cout << "Do you want to save more tours? Press 'y'. Otherwise, press any key. " << endl;
				cin >> choice;
				if(choice == 'y' || choice == 'Y'){correct = false; Rand = false;}
				break;
			case 'c':
			case 'C':
				TSP.Save_solution(TSP.Sfn.order, TSP.Sfn.length);
				cout << "Do you want to save more tours? Press 'y'. Otherwise, press any key. " << endl;
				cin >> choice;
				if(choice == 'y' || choice == 'Y'){correct = false; Rand = false;}
				break;
			case 'd':
			case 'D':
				TSP.Save_solution(TSP.Srt.order, TSP.Srt.length);
				cout << "Do you want to save more tours? Press 'y'. Otherwise, press any key. " << endl;
				cin >> choice;
				if(choice == 'y' || choice == 'Y'){correct = false; Rand = false;}
				break;
			case 'e':
			case 'E':
				TSP.Save_solution(TSP.Stree.order, TSP.Stree.length);
				cout << "Do you want to save more tours? Press 'y'. Otherwise, press any key. " << endl;
				cin >> choice;
				if(choice == 'y' || choice == 'Y'){correct = false; Rand = false;}
				break;
			default:
				cout << "That's not an option. Do you ever learn?" << endl;
				correct = false;
				break;
			}
		}
		else{return 0;}
	}
	while(!correct);

	//The program is terminated.
	cout << "Program terminated." << endl << endl << endl
		 << "Copyright � 2014 Tom Kennes All Rights Reserved" << endl << endl;
	
	system("pause");
	return 0;
}

