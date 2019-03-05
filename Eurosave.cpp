#include "stdafx.h"
#include "Eurosave.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Eurosave::Eurosave(void)
{
}

Eurosave::~Eurosave(void)
{
}

void Eurosave::read_file(string cities, string hotel_rooms, string staff, string names){
	int temp;
	string ins;
	ifstream in;
	in.open(cities);
	if(in.fail()){
		cout << "Could not open first file. Return Error." << endl;
	}
	in >> nr_edges;
	in >> nr_cities;
	edge e;
	for(int i = 0; i < nr_edges; i++){
		in >> e.city1;
		in >> e.city2;
		in >> e.cost;
		e.ignore = false;
		e.used_edge = false;
		data.push_back(e);
	}
	
	in.close();
	in.open(hotel_rooms);
	if(in.fail()){
		cout << "Could not open second file. Return Error." << endl;
	}
	for(int i = 0; i < nr_cities; i++){
		in >> temp;
		in >> temp;
		hotel_costs.push_back(temp);
	}
	in.close();
	in.open(staff);
	if(in.fail()){
		cout << "Could not open third file. Return Error." << endl;
	}
	for(int i = 0; i < nr_cities; i++){
		in >> temp;
		in >> temp;
		//Next the data noted how many staff members went along to the meeting
		//Also, the assignment noted that the minister of finance went with his staff
		//so, to be correct, the data indicating how many staff members needed to attend
		//the meeting has to be incremented by one for each city.
		Staff.push_back(temp + 1);
	}
	in.close();
	
	in.open(names);
	if(in.fail()){
		cout << "Could not open the fourth file. Return Error." << endl;
	}

	
	label = new int[nr_cities];
	investigated = new bool[nr_cities];
	neighbour = new bool[nr_cities];
}

void Eurosave::travel()
{
	int cost_total = 0;
	int optimal = INT_MAX;
	int total_staff = 0;

	for(int i = 0; i < nr_cities; i++){
		total_staff = total_staff + Staff[i];
	}

	//1. First, let's get all the distance matrices in a handsome format
	for(int i = 0; i < nr_cities; i++){
		Prepare_Dijkstra(i);
		Dijkstra();
		Dijk.push_back(Dijkstra_Matrix);
	}

	//2. Now, we can check what the costs will be:
	//   Variable: Location of meeting
	for(int i = 0; i < nr_cities; i++){
		for(int j = 0; j < nr_cities; j++){
			cost_total = cost_total + Dijk[i][nr_cities - 1][j][0]*Staff[j];
		}
		cost_total = cost_total + hotel_costs[i]*(total_staff);
		if(cost_total < optimal){
			optimal = cost_total;
			unrestricted_solution = i;
		}
		cost_total = 0;
	}
	cout << "The optimal city is " << unrestricted_solution << endl;
	cout << "The number of this city corresponds to "; 
	names(unrestricted_solution); 
	cout << "The corresponding costs are " << optimal << endl;
	cout << endl;
}
void Eurosave::Prepare_Dijkstra(int a){
	for(int i = 0; i < nr_cities; i++){
		label[i] = INT_MAX;

	}
	label[a] = 0;

	Dijkstra_Matrix = new int**[nr_cities];
	for(int i = 0; i < nr_cities; i++){
		Dijkstra_Matrix[i] = new int*[nr_cities];
		for(int j = 0; j < nr_cities; j++){
			Dijkstra_Matrix[i][j] = new int[2];
			if(j == a){
			Dijkstra_Matrix[i][j][0] = 0;		//The first entry indicating cheapest cost
			Dijkstra_Matrix[i][j][1] = a;		//The second entry indicating the predecessor
			}
			else
			{
			Dijkstra_Matrix[i][j][0] = INT_MAX;
			}

		}

	}
	reset_investigated();
}
void Eurosave::reset_neighbour(){
	for(int i = 0; i < nr_cities; i++){
		neighbour[i] = false;
	}

}
void Eurosave::reset_investigated(){
	for(int i = 0; i < nr_cities; i++){
		investigated[i] = false;
	}

}
void Eurosave::Dijkstra(){
	int min = INT_MAX, indicator;

	//0. We have to make maximal z iterations to investigate all nodes, namely each iteration of this loop will investigate on one node which is not 
	//   yet investigated upon.
	for(int z = 0; z < nr_cities; z++){
	//1.A Find the smallest label
	for(int i = 0; i < nr_cities; i++){
		if(label[i] <= min && !investigated[i]){
			min = label[i];
			indicator = i;
		}
	}
	min = INT_MAX;
	//1.B Make sure this node will not be revisited by the algorithm, also exclude cycles containing one node
	investigated[indicator] = true;
	//1. The smallest label is the city with indicator i
	//2. Find neighbours of smallest label indicated by i, and if they have a higher label than smallest label + cost of the edge between the nodes
	//	 Update its label and indicate it as a neighbour, meaning it is adjacent to the smallest lable and we can upgrade him
	for(int i = 0; i < nr_edges; i++){
		if(data[i].city1 == indicator && !investigated[data[i].city2] && !data[i].ignore){
			if(label[data[i].city2] > label[data[i].city1] + data[i].cost){
				label[data[i].city2] = label[data[i].city1] + data[i].cost;
				neighbour[data[i].city2] = true;
			}
		}
		if(data[i].city2 == indicator && !investigated[data[i].city1] && !data[i].ignore){
			if(label[data[i].city1] > label[data[i].city2] + data[i].cost){
				label[data[i].city1] = label[data[i].city2] + data[i].cost;
				neighbour[data[i].city1] = true;
			}
		}
	}

	//3. Update the information regarding shortest paths, or in this case cheapest paths
	//3.A	We want to update row afer row, based on the previous row and on 
	//		the previous test, e.g. is data[i].city1 (or 2) a neighbour of
	//		smallest lable that has not yet been visited
		if(z == 0){
			for(int i = 0; i < nr_cities; i++){
				if(neighbour[i] == true){
					Dijkstra_Matrix[z][i][1] = indicator;
					Dijkstra_Matrix[z][i][0] = label[i];
				}
			}
		}
		else
		{
			for(int i = 0; i < nr_cities; i++){
				if(neighbour[i] == true){
					Dijkstra_Matrix[z][i][1] = indicator;
					Dijkstra_Matrix[z][i][0] = label[i];
				}
				else
				{
					Dijkstra_Matrix[z][i][1] = Dijkstra_Matrix[z-1][i][1];
					Dijkstra_Matrix[z][i][0] = Dijkstra_Matrix[z-1][i][0];
				}

			}

		}
		reset_neighbour();
	}

	//print_thingy();
	


	
}
void Eurosave::print_thingy(){
	for(int i = 11; i < nr_cities; i++){
		for(int j = 0; j < nr_cities; j++){
			cout << i << ", " << j << ": " << Dijkstra_Matrix[i][j][0] << "..." << Dijkstra_Matrix[i][j][1] << "    ";
		}
		cout << endl;
	}
}


void Eurosave::optimize(){
	int congestion;
	bool possibility_of_congestion = true;
	bool congested = true;
	int counter = 1;
	
	initialize_optimize(unrestricted_solution);
	cout << endl << "********** - Initialisation complete: Starting process - *************" << endl;
	while(balance[unrestricted_solution] != 0)
	{
	cout << "######################### -"  << counter <<"th cycle- ###################" << endl;
	//Find pathways to unrestricted_solution
	get_pathways();

	//Who is travelling over which path?
	assess_paths(unrestricted_solution);

	//What is the most congested path.
	congestion = get_most_overloaded_edge();

	/*cout << congestion << " : most overloaded path!" << endl;
	cout << "E.G.: capacity: " << data[congestion].residual_edge << endl; 
	cout << "THe edge is: " << data[congestion].city1  << " -> " << data[congestion].city2 << ". " << endl;
	cout << "***************** balances ****************" << endl;
	for(int i = 0; i < balance.size(); i++)
	{
			cout << "i: " << i << "\t balance: " << balance[i] << endl;
	}*/
	if(data[congestion].residual_capacity < 0)
	{
		data[congestion].ignore = true;
		Prepare_Dijkstra(congestion);
		Dijkstra();
		priority_boarding(congestion);
		process_travellers(congestion);
		data[congestion].residual_capacity = data[congestion].residual_edge;
		/*cout << "***************** balances ****************" << endl;
		for(int i = 0; i < balance.size(); i++)
		{
			cout << "i: " << i << "\t balance: " << balance[i] << endl;
		}*/
		Dijk[unrestricted_solution] = Dijkstra_Matrix;
	}
	else
	{
		process_travellers(congestion);
		for(int i = 0; i < balance.size(); i++)
		{
			cout << "i: " << i << "\t balance: " << balance[i] << endl;
		}
		data[congestion].residual_capacity = data[congestion].residual_edge;
		data[congestion].used_edge = true;
	}
	
	if (pathways.size() > 0)
	{
		pathways.clear();
	}
	counter++;
	}
	print_solution();
	cout << "THIS IS FINISHED!" << endl;
}

//Functions used in successive shortest path
void Eurosave::initialize_optimize(int end){
	set_capacities();
	set_b(end);
	set_residuals();
}

//Initialisation functions
void Eurosave::set_residuals(){
	for(int i = 0; i < nr_edges; i++){
		data[i].residual_edge = data[i].initial_capacity;
	}
}
void Eurosave::set_capacities(){
	for(int i = 0; i < nr_edges; i++){
		switch(i)
		{
		case 2:
			data[i].initial_capacity = 22;
			data[i].residual_capacity = data[i].initial_capacity;
			data[i].residual_edge = data[i].residual_capacity;
			break;
		case 7:
			data[i].initial_capacity = 19;
			data[i].residual_capacity = data[i].initial_capacity;
			data[i].residual_edge = data[i].residual_capacity;
			break;
		case 11:
			data[i].initial_capacity = 22;
			data[i].residual_capacity = data[i].initial_capacity;
			data[i].residual_edge = data[i].residual_capacity;
			break;
		case 18:
			data[i].initial_capacity = 21;
			data[i].residual_capacity = data[i].initial_capacity;
			data[i].residual_edge = data[i].residual_capacity;
			break;
		default:
			data[i].initial_capacity = INT_MAX;
			data[i].residual_capacity = data[i].initial_capacity;
			data[i].residual_edge = data[i].residual_capacity;
			break;
		}
	}

}
void Eurosave::set_b(int end){
	int total_staff = 0;
	for(int i = 0; i < nr_cities; i++){
		total_staff = total_staff + Staff[i];
	}
	for(int i = 0; i < nr_cities; i++){
		if(i == end){
			balance.push_back(Staff[i] - total_staff);
		}
		else
		{
			balance.push_back(Staff[i]);
		}

	}

}


void Eurosave::get_pathways(){
	cout << "*************** - get_pathways - ********************" << endl;
	bool finish = false;
	int k = nr_cities -1;
	int pred;
	vector<int> v;
	vector<vector<int>> w;

	for(int i = 0; i < nr_cities; i++){
		w.clear();
		for(int j = 0; j < nr_cities; j++){
			finish = false;
			k = nr_cities - 1;
			v.clear();
			pred = Dijk[i][k][j][1];
			v.push_back(pred);
			while(!finish){
				if(pred == i){
					finish = true;
				}
				else
				{
					k--;
					pred = Dijk[i][k][pred][1];
					v.push_back(pred);
				}
			}
			w.push_back(v);
		}
		pathways.push_back(w);
	}
}
void Eurosave::assess_paths(int end){
	cout << "Unrestricted solution: " << unrestricted_solution << endl;
	cout << "**************** - assess paths - **********************" << endl;
	int k;
	//Initially clear the travellers of the pathway
	for(int i = 0; i < nr_edges; i ++){
		data[i].travellers_origins.clear();
	}
	for(int i = 0; i < pathways[end].size(); i++){
		if(balance[i] != 0){
		if(pathways[end][i].size() < 2){
			for(int j = 0; j < nr_edges; j++){
				if((data[j].city1 == pathways[end][i][0] && data[j].city2 == i)){
					data[j].travellers_origins.push_back(i);
					data[j].residual_capacity = data[j].residual_capacity - balance[i]; 
					cout << "-> " << j << ": " << data[j].residual_capacity << endl;
				}
				if(data[j].city2 == pathways[end][i][0] && data[j].city1 == i){
					data[j].travellers_origins.push_back(i);
					data[j].residual_capacity = data[j].residual_capacity - balance[i];
					cout << "-> " << j << ": " << data[j].residual_capacity << endl;

				}
				
			}
		}
		else
		{
			for(int j = 0; j < nr_edges; j++){
				if((data[j].city1 == pathways[end][i][0] && data[j].city2 == i)){
					data[j].travellers_origins.push_back(i);
					data[j].residual_capacity = data[j].residual_capacity - balance[i];
					cout << "-> " << j << ": " << data[j].residual_capacity << endl;

				}
				if((data[j].city2 == pathways[end][i][0] && data[j].city1 == i)){
					data[j].travellers_origins.push_back(i);
					data[j].residual_capacity = data[j].residual_capacity - balance[i];
					cout << "-> " << j << ": " << data[j].residual_capacity << endl;
				}
			}
			k = 0;
			while(pathways[end][i][k] != end){
				for(int j = 0; j < nr_edges; j++){
					if(data[j].city1 == pathways[end][i][k] && data[j].city2 == pathways[end][i][k+1]){
					data[j].travellers_origins.push_back(i);
					data[j].residual_capacity = data[j].residual_capacity - balance[i];
					cout << "-> " << j << ": " << data[j].residual_capacity << endl;

				}
				if(data[j].city2 == pathways[end][i][k] && data[j].city1 == pathways[end][i][k+1]){
					data[j].travellers_origins.push_back(i);
					data[j].residual_capacity = data[j].residual_capacity - balance[i];
					cout << "-> " << j << ": " << data[j].residual_capacity << endl;

				}
				}
				k += 1;

			}

		}
	}
	}
	for(int i = 0; i < nr_edges;i++){
		for(int j = 0; j < data[i].travellers_origins.size(); j++){
			cout << data[i].travellers_origins[j] << "\t ";
		}
		cout << ": use edge: " << data[i].city1 << " <-> " << data[i].city2 << endl;
	}

}
int Eurosave::get_most_overloaded_edge(){
	cout << "*************** - get most overloaded edge - ************" << endl;
	int t = INT_MAX;
	int ind;
	for(int i = 0; i < nr_edges; i++){
		cout << i << "th: residual capacity: " << data[i].residual_capacity << endl;
		if(data[i].residual_capacity <= t && !data[i].ignore && !data[i].used_edge){
			t = data[i].residual_capacity;
			cout << "Minimal = " << t << endl;
			ind = i;
		}
	}
	cout << ind << endl;
	return ind;
}
void Eurosave::priority_boarding(int congestion){
	cout << "********* Priority-Boarding ********" << endl;
	int max = INT_MIN;
	int ind;
	int erase_this;
	vector<int> r;
	int p = data[congestion].travellers_origins.size();
	Prepare_Dijkstra(unrestricted_solution);
	Dijkstra();
	for(int j = 0; j < p; j++){
		for(int i = 0; i < data[congestion].travellers_origins.size(); i++){
			if((Dijkstra_Matrix[11][data[congestion].travellers_origins[i]][0] - Dijk[unrestricted_solution][11][data[congestion].travellers_origins[i]][0]) > max){
				max = (Dijkstra_Matrix[11][data[congestion].travellers_origins[i]][0] - Dijk[unrestricted_solution][11][data[congestion].travellers_origins[i]][0]);
				ind = data[congestion].travellers_origins[i];
				cout << "ind " << ind << "\t";
				erase_this = i;
			}		
		}
		cout << "Costs of deviation from path, per person depending on origin: " << max << endl;
		max = INT_MIN;
		r.push_back(ind);
		cout << "length of vector: "  << data[congestion].travellers_origins.size() << endl;
		data[congestion].travellers_origins.erase(data[congestion].travellers_origins.begin() + erase_this);
		for(int k = 0; k < data[congestion].travellers_origins.size(); k++)
		{
			cout << data[congestion].travellers_origins[k] << "  ";
		}
		cout << endl;
	}
	cout << "******** r-size **********" << endl;
	for(int i = 0; i < r.size(); i++){
		data[congestion].travellers_origins.push_back(r[i]);
		cout << i << " " << r[i] << endl;
	}
	r.clear();
}
void Eurosave::process_travellers(int focus){
	cout << "*************** - process travellers - **************" << endl;
	solution s;
	for(int i = 0; i < data[focus].travellers_origins.size(); i++){
		if((data[focus].residual_edge - balance[data[focus].travellers_origins[i]]) > 0){
			s.people = balance[data[focus].travellers_origins[i]];
			s.path = pathways[unrestricted_solution][data[focus].travellers_origins[i]];
			s.origin = data[focus].travellers_origins[i];
			solution_set.push_back(s);
			balance[unrestricted_solution] = balance[unrestricted_solution] + balance[data[focus].travellers_origins[i]];
			data[focus].residual_edge = data[focus].residual_edge - balance[data[focus].travellers_origins[i]];
			balance[data[focus].travellers_origins[i]] = 0;
		}
		else
		{
			if((data[focus].residual_edge > 0) && (data[focus].residual_edge - balance[data[focus].travellers_origins[i]] < 0)){
				s.people = data[focus].residual_edge;
				s.path = pathways[unrestricted_solution][data[focus].travellers_origins[i]];
				s.origin = data[focus].travellers_origins[i];
				solution_set.push_back(s);
				balance[unrestricted_solution] = balance[unrestricted_solution] + data[focus].residual_edge;
				balance[data[focus].travellers_origins[i]] = balance[data[focus].travellers_origins[i]] - data[focus].residual_edge;
				data[focus].residual_edge = 0;
			}
		}
	}


	for(int i = 0; i < solution_set.size(); i ++){
		cout << "People: " << solution_set[i].people << "\t Originated from: " << solution_set[i].origin << endl;
		for(int j = 0; j < solution_set[i].path.size(); j++){
			cout << j << "th: " << solution_set[i].path[j] << endl;
		}
	}




}

void Eurosave::names(int index)
{
	if (index == 0)
		cout << "Athens";
	else if (index == 1)
		cout << "Berlin";
	else if (index == 2)
		cout << "Brussels";
	else if (index == 3)
		cout << "Dublin" ;
	else if (index == 4)
		cout << "The Hague" ;
	else if (index == 5)
		cout << "Helsinki" ;
	else if (index == 6)
		cout << "Lisbon" ;
	else if (index == 7)
		cout << "Luxembourg" ;
	else if (index == 8)
		cout << "Madrid" ;
	else if (index == 9)
		cout << "Paris" ;
	else if (index == 10)
		cout << "Rome" ;
	else if (index == 11)
		cout << "Vienna" ;

	
}
void Eurosave::print_solution(){
	int costs = 0, totalcosts = 0, totalstaff = 0;
	
	for(int z = 0; z < nr_cities; z++){
		totalstaff = totalstaff + Staff[z];
		for(int i = 0; i < solution_set.size(); i ++){
			if(z == solution_set[i].origin){
			cout << "People: " << solution_set[i].people << "\t Originated from: ";
			names(solution_set[i].origin);
			cout << endl;
			costs = costs + find_cost(solution_set[i].origin, solution_set[i].path[0]);
				for(int j = 0; j < solution_set[i].path.size(); j++){
					if(j == solution_set[i].path.size() - 1){
					cout << "and from there directly to: ";
					names(solution_set[i].path[j]);
					cout << endl;
					}
					else
					{
					cout << "via: ";
					names(solution_set[i].path[j]);
					cout << endl;
						costs = costs + find_cost(solution_set[i].path[j], solution_set[i].path[j+1]);
					}
				}
				cout << "With a personal cost of: " << costs << ". " << endl;
				cout << "Total cost for this group: " << costs*solution_set[i].people << ". " << endl;
				totalcosts = totalcosts + costs*solution_set[i].people;
				costs = 0;
			}
		}
	}
	cout << endl << "The total costs of travelling of this solution: " << totalcosts << ". " << endl;
	cout << "The total hotel costs of this solution: " << totalstaff*hotel_costs[unrestricted_solution] << " ." << endl;
	cout << "The total travelling and hotel costs of this solution: " << totalcosts + totalstaff*hotel_costs[unrestricted_solution] << " ." << endl;
}
int Eurosave::find_cost(int a, int b){
	for(int i = 0; i < nr_edges; i++){
		if(data[i].city1 == a && data[i].city2 == b){
			return data[i].cost;
		}
		if(data[i].city1 == b && data[i].city2 == a){
			return data[i].cost;
		}
	}
}
