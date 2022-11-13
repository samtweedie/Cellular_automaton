#include <iostream>
#include <bitset>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <time.h>
#include <fstream>


#define ONE_D_SIZE 64
#define x_dim 120
#define y_dim 60

//function templates
void display_generation_one_dim(std::bitset<ONE_D_SIZE> line);
void display_generation_two_dim(int grid[y_dim][x_dim]);
int one_dimensional_simulation(int rule_num);
int conways_game_of_life(bool is_three_d, bool is_random);
int count_neighbours(int grid[y_dim][x_dim], int x, int y, bool is_three_d);
int read_from_file(std::string file_name, int (&grid)[y_dim][x_dim]);

/*
	sleep for x milliseconds code from
	Webite: https://stackoverflow.com/questions/4184468/sleep-for-milliseconds
	Date: 11/11/22
*/


int main(){

	/*
	TO-DO
	-add functionality to let user decide if they want random seed or seed from file - DONE
	-create more files (see conways game of life wiki for seeds) - DONE
	-add error checking to read from file function (file not found or wrong formatting etc) ********
	-finish menu system
		-let user pick 1D, 2D or 3D simulation - DONE
		-let user change simulation variables (number of generations, time between generations, ruleset for 1D) - DONE
		 *x and y dimensions must stay constant*
	-add functionality to let user save seed from simulation **********

	whatever else you think of which would be good to add
	*/

	bool running = true;

	while(running){
		int option = 0;
		std::cout<<"Cellular Automaton" << std::endl << std::endl;
		std::cout<< "[1] 1-Dimensional Simulation" << std::endl;
		std::cout<< "[2] 2-D Conways Game of Life" << std::endl;
		std::cout<< "[3] 3-D Conways Game of Life" << std::endl;
		std::cout<< "[4] Quit" << std::endl;
		std::cout<< ">";
		std::cin >> option;

		switch(option){
			int rule;
			case 1:
				rule = 0;
				do{
				std::cout<<"Please enter a rule between 0 and 255 (30, 90 or 110 recommended): ";
				std::cin >> rule;
				if(rule < 0 || rule > 255)
					std::cout<<"Rule must be between 0 and 255!"<<std::endl;
				}while(rule < 0 || rule > 255);

				one_dimensional_simulation(rule);
				break;

			case 2:
				rule = 0;
				do{
					std::cout << "Enter 1 for a random seed or 2 to load seed from file: ";
					std::cin >> rule;
					if(rule != 1 && rule != 2){
						std::cout<<"You must enter either 1 or 2!"<<std::endl;
					}
				}while(rule != 1 && rule != 2);

				if(rule == 1){
					conways_game_of_life(false, true);
				}
				else{
					conways_game_of_life(false, false);
				}
				break;

			case 3:
				rule = 0;
				do{
					std::cout << "Enter 1 for a random seed or 2 to load seed from file: ";
					std::cin >> rule;
					if(rule != 1 && rule != 2){
						std::cout<<"You must enter either 1 or 2!"<<std::endl;
					}
				}while(rule != 1 && rule != 2);

				if(rule == 1){
					conways_game_of_life(true, true);
				}
				else{
					conways_game_of_life(true, false);
				}
				break;

			case 4:
				running = false;
				break;

			default:
				std::cout<< "Please pick from the options shown!";
				break;

		}


	}


	return 0;
}



//rule num determines rule set for 1D simulation, must be between 0 and 255 inclusive
int one_dimensional_simulation(int rule_num){

	#define ONE_D_NUM_GENERATIONS 32

	if(rule_num < 0 || rule_num > 255){
		return 1;
	}

	//create bitsets for the selected rule, parent and child grids, and the pattern
	//using bitset data structure to allow for easy conversion between decimal and binary, and for memory efficiency
	std::bitset<8> rule(rule_num);
	std::bitset<ONE_D_SIZE> parent;
	std::bitset<ONE_D_SIZE> child;
	std::bitset<3> pattern;

	//seed parent grid with 1 cell in the middle
	parent[ONE_D_SIZE/2] = 1;

	//display seed
	display_generation_one_dim(parent);
	std::cout<<std::endl;

	//loop for specified number of generations
	for(int k = 0; k < ONE_D_NUM_GENERATIONS; k++){

		//for every live cell in grid, mark the live neighbours in the pattern bitset
		for(int i = 0; i < ONE_D_SIZE; i++){
			for(int j = -1; j < 2; j++){
				if(parent[((i+j)%ONE_D_SIZE)] == 1){
					pattern[j+1] = 1;
				}
			}
			//when pattern is converted to decimal, it will match with one of the rules on the rule bitset
			//which will indicate whether child[i][j] will be love or dead
			child[i] = rule[(int)pattern.to_ulong()];

			//reset pattern for next cell
			pattern.reset();
		}

		//display child grid, swap parent and child grid then reset child grid
		display_generation_one_dim(child);
		std::cout<<std::endl;
		parent = child;
		child.reset();
	}

	return 0;
}


int conways_game_of_life(bool is_three_d, bool is_random){

	//initialise parent and child grids to all zeros
	int parent_grid[y_dim][x_dim] = {{0}};
	int child_grid[y_dim][x_dim] = {{0}};

	//seed the randomiser with current time
	srand(time(0));

	int neighbour_count;


	
	if(is_random){
		//fill parent grid with random values
		for(int i = 0; i < y_dim; i++){
			for(int j = 0; j < x_dim; j++){
				parent_grid[i][j] = std::rand()%2;
			}
		}
	}
	else{
		std::string file_name;
		std::cout<<"Please enter a file name to load as a seed (must be 120 columns by 60 rows): ";
		std::cin >> file_name;
		//read seed from file
		read_from_file(file_name, parent_grid);

		//display initial generation for 500 milliseconds
		display_generation_two_dim(parent_grid);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	//display initial generation for 500 milliseconds
	display_generation_two_dim(parent_grid);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));


	//loop for 32 generations
	for(int num_gen = 0; num_gen < 200; num_gen++){

		//for every cell in parent grid, count its neighbours and then check against the rules to 
		//assign state to corresponding cell in child grid
		//see conways game of life wikipedia for ruleset
		for(int i = 0; i < y_dim; i++){
			for(int j = 0; j < x_dim; j++){
				neighbour_count = 0;
				neighbour_count = count_neighbours(parent_grid, i, j, is_three_d);

				//if current cell is live
				if(parent_grid[i][j] == 1){
					if(neighbour_count < 2 || neighbour_count > 3){
						child_grid[i][j] = 0;
					}
					else{
						child_grid[i][j] = 1;
					}
				}

				//if current cell is dead
				if(parent_grid[i][j] == 0){
					if(neighbour_count == 3){
						child_grid[i][j] = 1;
					}
					else{
						child_grid[i][j] = 0;
					}
				}
				
			}
		}

		//display generated grid
		display_generation_two_dim(child_grid);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		//swap parent and child grid and reset child grid
		for(int i = 0; i < y_dim; i++){
			for(int j = 0; j < x_dim; j++){
				parent_grid[i][j] = child_grid[i][j];
				child_grid[i][j] = 0;
			}
		}
	}

	return 0;

}

int count_neighbours(int grid[y_dim][x_dim], int y, int x, bool is_three_d){

	int count = 0;

	//counts love cells around cell at grid[y][x]
	//if is_three_d is true, the cells at edge will have neighbours at the opposing edge of the grid
	//creating a toroidal grid
	for(int i = y-1; i <= y+1; i++){
		for(int j = x-1; j <= x+1; j++){
			if(is_three_d){
				if(i == y && j == x){
					continue;
				}
				if(grid[(i+y_dim)%y_dim][(j+x_dim)%x_dim] == 1){
					count++;
				}
			}
			else{
				//if x is on left boundry, do not count left hand neighbours
				if(i < 0){
					continue;
				}
				//if x is on right boundry, do not count right hand neighbours
				else if(i > y_dim){
					continue;
				}

				//if y is on top boundry, do not count neighbours above
				if(j < 0){
					continue;
				}
				//if y is on bottom boundry, do not count neighbours below
				else if(j > x_dim){
					continue;
				}

				//do not count self as neighbour
				if(i == y && j == x){
					continue;
				}
				else if(grid[i][j] == 1){
					count++;
				}

			}
		}
	}

	return count;

}


void display_generation_one_dim(std::bitset<ONE_D_SIZE> line){
	for(int i = 0; i < ONE_D_SIZE; i++){
		if(line[i] == 1){
			std::cout<<"#";
		}
		else{
			std::cout<<" ";
		}
	}
}

void display_generation_two_dim(int grid[y_dim][x_dim]){
	system("clear");
	std::cout << std::string(x_dim+2, '-') << std::endl;
	for(int i = 0; i < y_dim; i++){
		std::cout<<"|";
		for(int j = 0; j < x_dim; j++){
			if(grid[i][j] == 1){
				std::cout<<"#";
			}
			else{
				std::cout<<" ";
			}
		}
		std::cout<<"|";
		std::cout<<std::endl;
	}
	std::cout << std::string(x_dim+2, '-') << std::endl;
}

//file needs to be 60 rows and 120 columns, non-delimited
int read_from_file(std::string file_name, int (&grid)[y_dim][x_dim]){
	FILE *input_file = fopen(file_name.c_str(), "r");
	if(input_file == NULL) //if file does not exist or error accessing file
	{
		return 1;
	}
	for(int i = 0; i < y_dim; i++){
		for(int j = 0; j < x_dim; j++){
			grid[i][j] = fgetc(input_file) - '0';
		}
		fgetc(input_file); // throw away new line
	}

}
