#include "stdafx.h"
#include "Pineapple.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <stack>

using namespace std;
// Constructor
Pineapple::Pineapple(void)
{
	cities = 0;
	schedule = new int [0];
	Distance = new double* [0];
	Distance[0] = new double [0];
	Order_std = new int [0];
	Snn.length = 0;
	Sfn.length = 0;
	Ssimple.length = 0;
	Srt.length = 0;
	Stree.length = 0;
	Snn.order = new int[0];
	Sfn.order = new int[0];
	Ssimple.order = new int[0];
	Srt.order = new int[0];
	Stree.order = new int[0];
	}

// Destructor
Pineapple::~Pineapple(void)
{
	for(int i = 0; i < cities; i++){
		delete[] Distance[i];
	}
	delete[] schedule;
	delete[] Distance;
	delete[] Order_std;
	delete[] Ssimple.order;
	delete[] Sfn.order;
	delete[] Snn.order;
	delete[] Srt.order;
	delete[] Stree.order;
	Coordinates.clear();
	all_edges.clear();
	label.clear();
	spanning_set.clear();
	tour.clear();
	visit.clear();
	plan.clear();
	cities = 0;
	length = 0;
	Ssimple.length = 0;
	Sfn.order = 0;
	Snn.order = 0;
	Srt.order = 0;
	Stree.order = 0;
}

// Iniatilisator
bool Pineapple::Processor(string file){
    ifstream in;
    in.open(file);
    if(in.fail()){cout << "Could not open file!"<< endl; return false;}
    else
    {
        cout << endl <<  "Succesfully opened file." << endl << endl;
        in >> cities;
        Distance = new double*[cities];
        Order_std = new int [cities];
        Position P;
        for(int i = 0; i < cities; i++){
            in >> Order_std[i];
            in >> P.X_coordinate;
            in >> P.Y_coordinate;
            Coordinates.push_back(P);
            Distance[i] = new double[cities];
        }
        
        for(int i = 0; i < cities; i++){
            for(int j = 0; j < cities; j++){
                //Euclidean distance
                Distance[i][j] = sqrt(pow((Coordinates[i].X_coordinate-Coordinates[j].X_coordinate), 2) + pow((Coordinates[i].Y_coordinate-Coordinates[j].Y_coordinate), 2));
            }
        }
        
        //Set non-constant variables
        //Find first possible solution: the simple one. e.g.: 0-1-2-3-4...
        schedule = new int[cities + 1]; // [cities + 1] since the first and the last must be the same
        Ssimple.order = new int[cities + 1];
        visit.resize(cities);
        for(int i = 0; i < cities; i++){
            schedule[i] = Order_std[i] - 1;
            Ssimple.order[i] = schedule[i];
            visit[i] = false;
        }
        schedule[cities] = 0;
        Ssimple.order[cities]= 0;
        Ssimple.length = find_length(Ssimple.order);
        return true;
    }
    in.close();
}

// Solve from scratch
void Pineapple::Solve_NearestNeighbour(){
    int remember;
    double smallest = INT_MAX;
    Snn.length = INT_MAX;
    Snn.order = new int[cities+1];
    
    //The loop create tours starting from each city
    for(int i = 0; i < cities; i++){
        //set first and last cities the same since its a tour
        schedule[0] = i;
        schedule[cities] = i;
        visit[i] = true;
        for(int j = 0; j < cities; j++){ //row of the schedule
            for(int k = 0; k < cities; k++){
                if(visit[k] == false){ //We know that schedule[j] != k if visit[k] == false since schedule[j] is already visited
                    if(Distance[schedule[j]][k] < smallest){ //looking for the smallest distance
                        smallest = Distance[schedule[j]][k];
                        remember = k;
                    }
                }
            }
            if(j != cities - 1){
                schedule[j + 1] = remember;
                smallest = INT_MAX;
                visit[remember] = true;
            }
        }
        length = find_length(schedule);
        if(length < Snn.length){
            Snn.length = length;
            //duplicate the schedule into the struct
            for(int j = 0; j < cities + 1; j++){
                Snn.order[j] = schedule[j];
            }
        }
        reset_visit();
    }
    
    //Load solutions into semi-long-term memory
    getlength(Snn.order);
}
void Pineapple::Solve_FarthestNeighbour(){
    //Similar to the nearest neighbor heuristic
    int remember;
    double largest = INT_MIN;
    Sfn.length = INT_MAX;
    Sfn.order = new int[cities+1];
    
    
    for(int i = 0; i < cities; i++){
        schedule[0] = i;
        schedule[cities] = i;
        visit[i] = true;
        for(int j = 0; j < cities; j++){
            for(int k = 0; k < cities; k++){
                if(schedule[j] != k && visit[k] == false){
                    if(Distance[schedule[j]][k] > largest){ // Only difference : " > " instead of " < "
                        largest = Distance[schedule[j]][k];
                        remember = k;
                    }
                }
            }
            if(j != cities - 1){
                schedule[j + 1] = remember;
                largest = INT_MIN;
                visit[remember] = true;
            }
        }
        length = find_length(schedule);
        if(length < Sfn.length){
            Sfn.length = length;
            for(int j = 0; j < cities + 1; j++){
                Sfn.order[j] = schedule[j];
            }
        }
        reset_visit();
    }
    
    //Load solutions into semi-long-term memory
    getlength(Sfn.order);
}
void Pineapple::Solve_Randomtour(int seconds){
    vector<int> v;
    int trials = 0;
    double travel;
    Srt.order = new int[cities + 1];
    Srt.length = INT_MAX;
    
    //Set an initial tour with cities' id in increasing order
    for(int i = 0; i < cities; i++){
        v.push_back(i);
    }
    clock_t start = clock();
    clock_t end = clock();
    
    while(((end-start)/CLOCKS_PER_SEC) < seconds){
        //randomly mix all the entries of the vector
        random_shuffle(v.begin(), v.end());
        v.push_back(v[0]); //first city = last
        travel = 0;
        
        //finding length
        for(int i = 0; i < cities; i++)
        {
            travel = travel + Distance[v[i]][v[i+1]];
        }
        
        //Comparing Length
        if(travel < Srt.length){
            Srt.length = travel;
            for(int i = 0; i < cities + 1; i++){
                Srt.order[i] = v[i];
            }
        }
        v.pop_back();
        end = clock();
        trials++;
    }
    //Print out the best one
    getlength(Srt.order);
    cout << "In " << seconds << " seconds, the algorithm completed: " << trials <<" Trials." << endl;
}
void Pineapple::Solve_Spanningtree(){
	edge e;
	double lengt_best = INT_MAX;

	//Create a vector of the edges for later usage
	for(int i = 0; i < cities; i++){
		for(int j = i + 1; j < cities; j++){
			e.city1 = i;
			e.city2 = j;
			e.space = Distance[i][j];
			all_edges.push_back(e);   //e is the vector of all edges with i < j, as it is unnecessary to include the symmetric edges due to efficiency issues.
		}
	}
	
	// Sort the vector based on the lenghts of the edges, e.g. space;
	Quicksort(all_edges, 0, all_edges.size()-1);

	// Get a minimal spanning tree
	Get_spanningtree();

	// Improve on the Spanning tree
	Improve_Spanningtree();

	// Print out the tour for the user
	cout << "The minimal spannign tree tour: " << endl;
	for(int i = 0; i < tour.size(); i++){
		cout << i << "\t" << tour[i] << endl;
	}	
	
	// Save the tour in the standard format we adopted. Also for potential later usage.
	Stree.length = 0;
	Stree.order = new int[tour.size()];
	for(int i = 0; i < tour.size(); i++){
		Stree.order[i] = tour[i];
	}
	Stree.length = find_length(Stree.order);

}

// Improve a solution
void Pineapple::Improve_2opt(int* improve_this){
	// 1. Initialization of variables	
	bool possibility = true;
	int repetition = 0;
	double initial_length = 0;
	double difference = 0;
	double store;
	int index_i, index_j;
	double  value = 0;

	// In this method plan is used as a container for temporary storage and data manipulation. It isn't necessary but it is helpfull.
	for(int i = 0; i < cities; i++){
		plan.push_back(improve_this[i]);
	}
	// Also we set an initial length to compare later. Using an initial length as a container for that we can put any sequence of cities into
	// the method next to the standard solutions: simple, nearest neighbour, farthest neighbour and minimal spanning tree.
	for(int i = 0; i < cities - 1; i++){
		initial_length = initial_length + Distance[plan[i]][plan[i+1]];
	}
	initial_length = initial_length + Distance[plan[cities - 1]][plan[0]];

	// 2. The start of the loop. The loop continues untill it is unable to find a possible improvement. It is therefore set at false and during
	// the loop it must be proven to be true in order to repeat the loop.
	do{
		possibility = false;
		for(int i = 0; i < cities - 2; i++){
			for(int j = i + 2; j < cities; j++){
				// New tour finds out wether the supposed 2-opt of i and j would generate a better tour. It does so by examining the difference
				// between added edges and removed edges.
				store = new_tour(i, j);
				// Then we want to find the 2-opt which generates the biggest difference in length. ALso because we found one improvement when
				// the if statement can be entered, possibitity becomes true again. index_i and index_j are carriers to find the specific cities
				// for the 2-opt in a later stadium where we actually generate the new tour after the 2-opt.
				if(store < difference){
					difference = store;
					index_i = i;
					index_j = j;
					possibility = true;
				}
			}
		}
		// Then, as we should only perform a 2-opt when it actually makes sense we check again for positive possibility.
		// Then we adjust the tour using perform_2opt, the length of this tour and the total increased value by the 2-opt
		// algorithm.
		if(possibility){
			perform_2opt(index_i, index_j);
			initial_length = initial_length + difference;
			value = value - difference;
			repetition++;
			difference = 0;
		}
	}while(possibility);
	// 3. Again to make the program as user-friendly as possible we include some print-commands in order to expose the data
	// and check whether the program behaved accordingly.
	cout << "The initial tour compared to the new tour: " << endl << "Stop \t      Initial \t      New" << endl;
	for(int i = 0; i < cities; i++){
		cout << i + 1 << "\t \t" << improve_this[i] << "\t \t" << plan[i] + 1 << endl;
	}
	cout << cities + 1  << "\t \t" << improve_this[0] << "\t \t" << plan[0] +1 << endl;
	cout << endl << "Using the 2-opt method, the solution is improved by: " << value << endl
		 << "The length of the solution is: " << initial_length << endl
		 << "The program has executed " << repetition
		 << " 2-opt exchanges! " << endl;
		
	// 4. Because without output the worth of this algorithm becomes quite minimal, we return, as requested, the new route.
	for(int i = 0; i  < cities; i++){
		improve_this[i] = plan[i];
	}
	improve_this[cities] = improve_this[0];
}

// Save a solution
void Pineapple::Save_solution(int* tour, long double interspace){
	string file;
	ofstream out;
	cout << "What is the name of the file? " << endl
		 << "There is a standard file to for output present: 'output.txt'" << endl;
	cin  >> file;
	out.open(file);
	if(out.fail()){cout << "Could not open file, please check if it is located properly. " << endl;}
	else
	{
	out << cities << endl;
	for(int i = 0; i < cities + 1; i++){
		out << "City: \t" << i + 1 << "\t \t \t" << tour[i] << endl;
	}
	out << "Length of the tour: " << "\t" << interspace;
	out.close();
	}
}

// Support-methods
void Pineapple::getlength(int* tour){
	// This is a simple support-method in order to show the user the length of the tour and the order.
	// It is different from find_length because it does not return anything. getlength should be considered
	// as a print_command whereas find_length should be considered as a pure-support method
	length = 0;
	cout << "City 1.:\t" << tour[0] + 1 << endl;
	for(int i = 1; i < cities; i++){
		length = length + Distance[tour[i - 1]][tour[i]];
		cout << "City " << i + 1 << ".:\t" << tour[i] + 1 <<". \t Distance traveled so far: " << length << endl;
	}
	length = length + Distance[tour[cities -1]][tour[0]];
	cout << "City 1"<<  ".:\t" << tour[0] + 1 <<". \t Distance traveled: " << length << endl;
	cout << "Total distance traveled: " << length << endl;
}
double Pineapple::find_length(int* tour){
	// Very similar to getlength. The difference is the fact that this method returns a number to the program
	// instead of output to the user.
	length = 0;
	for(int i = 1; i < cities; i++){
		length = length + Distance[tour[i-1]][tour[i]];
	}
	length = length + Distance[tour[cities - 1]][tour[0]];
	return(length);
}
void Pineapple::reset_visit(){
	// This method supports Solve_Nearestneighbour and Solve_FarthestNeighbour. It was not necessary actually to put it as a method, but
	// again it is convenient and it helps us to grasp the complexity C++ is able to exhibit.
	for(int i = 0; i < cities; i++){
		visit[i] = false;
	}
}
double Pineapple::new_tour(int a, int b){
	// Support of the Improve_2opt method. Essentially it only calculates the potential difference after a 2-opt and returns it to the actual
	// method that needs it. Again, we think this works convenient and splits the complex into more understandable pieces. 
	if(a==0 && b < cities - 1){
		return(Distance[plan[a]][plan[b]] + Distance[plan[a + 1]][plan[b + 1]] - Distance[plan[a]][plan[a + 1]] - Distance[plan[b]][plan[b + 1]]);
	}
	else
	{
		if(b == cities -1 && a > 0){
			return(Distance[plan[a]][plan[b]] + Distance[plan[a + 1]][plan[0]] - Distance[plan[a]][plan[a+1]] - Distance[plan[b]][plan[0]]);
		}
		else
		{
			if(!(b == cities - 1 && a == 0)){
				return(Distance[plan[a]][plan[b]] + Distance[plan[a + 1]][plan[b + 1]] - Distance[plan[a]][plan[a + 1]] - Distance[plan[b]][plan[b + 1]]);
			}
			else
			{
				return 0;
			}

		}

	}


}
void Pineapple::perform_2opt(int x, int y){
	// A simple method part of the C++ library class algorithm which does exactly what it says. It's makes our lives much easier.
	// Furthermore, it is also quite efficient. Or at least, more efficient than other ways to reverse the elements.
		reverse(plan.begin() + x + 1, plan.begin() + y + 1);
}
void Pineapple::Quicksort(vector<edge>& V, int first, int last){
	// This is a standard quicksort algorithm. The only thing that makes it a little more intermediate level is the
	// fact that it sorts based on a quality of a struct (length) and then moves the whole struct according to his
	// length compared to the lengths of other structs.
	// The struct has three components. Starting point, ending point and a distance.
	int pivot, i, j;
	edge store;
	if(first < last){
		pivot = first;
		i = first;
		j = last;

		while(i < j){
			while(V[i].space <= V[pivot].space && i < last)
				i++;
			while(V[j].space > V[pivot].space)
				j--;
			if(i < j){
				store = V[i];
				V[i] = V[j];
				V[j] = store;
			}
		}

		store = V[pivot];
		V[pivot] = V[j];
		V[j] = store;

		Quicksort(V, first, j-1);
		Quicksort(V, j + 1, last);
	}

}
void Pineapple::Get_spanningtree(){
	//Initilization of variables
	// 1.A. it is important to note that the initial label is different for each city and always higher than the number of cities
	int i = 0, label_nr = 0, store;
	for(int i = 0; i < cities; i++){
		label.push_back(cities + i);
	}

	//2. A start the loop over all the edges to find spanning tree.
	for(int i = 0; i < all_edges.size(); i++){
		if(label[all_edges[i].city1] != label[all_edges[i].city2]){
			//The labels are not the same so give them both a new label. That is, the lowest label available that is not in use by 
			// any other city.
			if(label[all_edges[i].city1] >= cities && label[all_edges[i].city2] >= cities){
				label[all_edges[i].city1] = label_nr;
				label[all_edges[i].city2] = label_nr;
				all_edges[i].indicator = label_nr;
				label_nr++;
			}
			else 
			// not both are bigger than cities. Find out which city has the smallest label and make sure all the cities that have the 
			// highest label get the smallest label as well. If only one of the two cities has a label that is lower than the number 
			// of cities, this is easy. Than we only have to attach that label to the city with the higher than number of cities label.
			// otherwise we have to run through all cities that have been labeled before e.g. those before i and makes sure that those 
			// who are tagged with the higher label get the smaller label as well. No big deal.
			{
				if(label[all_edges[i].city1] < cities && label[all_edges[i].city2] < cities){
					if(label[all_edges[i].city1] < label[all_edges[i].city2]){
						//label city1 is smaller than the label for city2
						store = label[all_edges[i].city2];
						label[all_edges[i].city2] = label[all_edges[i].city1];
						all_edges[i].indicator = label[all_edges[i].city1];
						for(int j = 0; j < i; j++){
							if(all_edges[j].indicator == store){
								all_edges[j].indicator = label[all_edges[i].city1];
								label[all_edges[j].city1] = label[all_edges[i].city1];
								label[all_edges[j].city2] = label[all_edges[i].city1];
							}

						}
						
					}
					else // label city2 is smaller than the label for city1
					{
						store = label[all_edges[i].city1];
						label[all_edges[i].city1] = label[all_edges[i].city2];
						all_edges[i].indicator = label[all_edges[i].city2];
						for(int j = 0; j < i; j++){
							if(all_edges[j].indicator == store){
								all_edges[j].indicator = label[all_edges[i].city2];
								label[all_edges[j].city2] = label[all_edges[i].city2];
								label[all_edges[j].city1] = label[all_edges[i].city2];
							}
						}
					}
				}
				else
				// Only one of them is smaller than cities. As earlier explained. Give the label of the one with the label
				// smaller than the number cities to the other city. Such that they both have the smaller label.
				// In the process we made sure to label edges as well, as you can later on filter them from the whole set.
				// The labeling is done simultaneously as can be seen from the code.
				{
					if(label[all_edges[i].city1] < cities && label[all_edges[i].city2] > cities)
					{
						label[all_edges[i].city2] = label[all_edges[i].city1];
						all_edges[i].indicator = label[all_edges[i].city1];
					}
					else
					{
						label[all_edges[i].city1] = label[all_edges[i].city2];
						all_edges[i].indicator = label[all_edges[i].city2];
					}
				}
			}

		}
	}
	// 3. Than all that is left to filter those edges with label 0 from the whole set of edges and put them together
	// We have put them together in the vector spanning_set
	edge e;
	for(int z = 0; z < all_edges.size(); z++){
		if(all_edges[z].indicator == 0){
			e.city1 = all_edges[z].city1;
			e.city2 = all_edges[z].city2;
			e.space = all_edges[z].space;
			e.indicator = all_edges[z].indicator;
			spanning_set.push_back(e);
		}
	}
	// 4. To make it user friendly and to return some data, we've added some printcommands!
	tree_distance = 0;
	cout << endl << "The following connections are part of the minimal spanning tree: " << endl;
	for(int i = 0; i < spanning_set.size(); i++){
		tree_distance = tree_distance + Distance[spanning_set[i].city1][spanning_set[i].city2];
		cout << "City1: " << spanning_set[i].city1 
			<< "\t City2: " << spanning_set[i].city2
			<< " Total Distance: " << tree_distance << endl;
	}
	cout << "Size of the set: " << spanning_set.size() << endl;
	cout << "So this means that there is a lower  bound for the optimal length of: "
		 << tree_distance*2 << endl;
}
void Pineapple::Improve_Spanningtree(){
	//Declaration of Variables
	vector<bool> visited;
	vector<bool> in_stack;
	vector<int> neighbours;
	stack<int> stuff;
	int memory;
	bool indicator;

	// 1.A: First we set the initial values and check how many neighbours each city has.
	// We set all visited cities on false and indicate all the cities as not being in the stack.
	int counter = 0;
	for(int i = 0; i < cities; i++){
		visited.push_back(false);
		in_stack.push_back(false);
		for(int j = 0; j < spanning_set.size(); j++){
			// For every edge of the spanning tree we check whether city i is either the starting point or the ending point.
			// As the set of spanning edges does not hold both the edge from city a to city b and city b to city a we do not
			// have to account for symmetry.
			if(spanning_set[j].city1 == i){
				counter++;
			}
			if(spanning_set[j].city2 == i){
				counter++;
			}
		}
		//As neighbours is a vector, we can simply push it at the end. Also we have to make sure to reset counter.
		neighbours.push_back(counter);
		counter = 0;
	}
	
	// 1.B: We set the starting point of the tour to be city 0, but essentially it is not important which city we chose to start from.
	// Also, we make sure to put city 0 on the bottom of the stack, equivalently one could leave the stack empty and check for emptiness in the
	// while-loops but we chose for this option.
	tour.push_back(0);
	stuff.push(0);
	in_stack[0] = true;
	visited[0] = true;
	
	

	// 2. Then we enter the loops.
	do{
		// 2.A First of all, when the city on top has more than one neighbour it will not be at the end of a leaf, thus we have to add any neighbouring cities to the stack
		// in order to make sure all cities are involved in the computation.
		while(neighbours[stuff.top()] > 1){
			// 2.A.1 First of all, remember what used to be on top. If we can't find another neighbour, are at what is supposed to be the last place in the tour.
			memory = stuff.top();
			// 2.A.2 Check whether the city in the memory is a city on either side of the set of spanning edges. If so, put it on the stack and let in_stack be true, because
			// we don't want to put it twice in the stack.
			for(int i = 0; i < spanning_set.size(); i++){
				if(spanning_set[i].city1 == memory && !visited[spanning_set[i].city2] && !in_stack[spanning_set[i].city2]){
				stuff.push(spanning_set[i].city2);
				in_stack[spanning_set[i].city2] = true;
				}
				if(spanning_set[i].city2 == memory && !visited[spanning_set[i].city1] && !in_stack[spanning_set[i].city1]){
				stuff.push(spanning_set[i].city1);
				in_stack[spanning_set[i].city1] = true;
				}
			}
			if(stuff.top() == memory){neighbours[stuff.top()] = neighbours[stuff.top()] - 1;}
		}
		// 2.C Then the second while loop: it start only for those who are on top of the stack so those who have no neighbours
		// The indicator makes sure the while halts whenever we try to take a city from the stack that is connected to a path
		// we have not on before. If that happens we cannot work backwards but we should first add the cities that lie on the 
		// path.
		indicator = true;
		while(indicator){
			tour.push_back(stuff.top());
			visited[tour.back()] = true;
			stuff.pop();
			for(int i = 0; i< spanning_set.size(); i++){
				if(!((spanning_set[i].city1 == tour.back() && spanning_set[i].city2 == stuff.top()) || (spanning_set[i].city2 == tour.back() && spanning_set[i].city1 == stuff.top()))){
					indicator = false;
				}
			}
		}
	}while(stuff.top() != 0);
	//To be consistent, I add the start point to the end of the tour. This changes nothing. It is just convenient.
	tour.push_back(tour[0]);
}