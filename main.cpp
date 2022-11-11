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
int conways_game_of_life(bool is_three_d);
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
	-add functionality to let user decide if they want random seed or seed from file
	-create more files (see conways game of life wiki for seeds)
	-add error checking to read from file function (file not found or wrong formatting etc)
	-finish menu system
		-let user pick 1D, 2D or 3D simulation
		-let user change simulation variables (number of generations, time between generations, ruleset for 1D)
		 *x and y dimensions must stay constant*
	-add functionality to let user save seed from simulation

	whatever else you think of which would be good to add
	*/

	
	//1D simulation
	system("clear");
	std::cout<<"-------- One dimensional cellular automaton --------" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	if (one_dimensional_simulation(30) == 1){
		std::cout<<"Rule not valid - rule must be between 0 and 255 inclusive!" << std::endl;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));	

	//2D simulation
	system("clear");
	std::cout<<"-------- Two dimensional Conways Game of Life --------" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	conways_game_of_life(false);

	//3D (toroidal) simulation
	system("clear");
	std::cout<<"-------- Three dimensional (toroidal) Conways Game of Life --------" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	conways_game_of_life(true);
	return 0;
}



//rule num determines rule set for 1D simulation, must be between 0 and 255 inclusive
int one_dimensional_simulation(int rule_num){

	#define ONE_D_NUM_GENERATIONS 32

	//1D simulation

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


int conways_game_of_life(bool is_three_d){

	//initialise parent and child grids to all zeros
	int parent_grid[y_dim][x_dim] = {{0}};
	int child_grid[y_dim][x_dim] = {{0}};

	//seed the randomiser with current time
	srand(time(0));

	int neighbour_count;


	/*
	UNCOMMENT THIS AND COMMENT OUT READ_FROM_FILE LINE TO SEED WITH RANDOM VALUES

	//fill parent grid with random values
	for(int i = 0; i < y_dim; i++){
		for(int j = 0; j < x_dim; j++){
			parent_grid[i][j] = std::rand()%2;
		}
	}
	*/

	//read seed from file
	read_from_file("quaser.txt", parent_grid);

	//display initial generation for 500 milliseconds
	display_generation_two_dim(parent_grid);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));


	//loop for 20 generations
	for(int num_gen = 0; num_gen < 20; num_gen++){

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
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

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
	for(int i = -1; i < 2; i++){
		for(int j = -1; j < 2; j++){
			if(is_three_d){
				if(i == 0 && j == 0){
					continue;
				}
				if(grid[(y+i)%y_dim][(x+j)%x_dim] == 1){
					count++;
				}
			}
			else{
				if(x == 0 && j < 0){
					continue;
				}
				else if(x == x_dim-1 && j > 0){
					continue;
				}
				if(y == 0 && i < 0){
					continue;
				}
				else if(y == y_dim-1 && i > 0){
					continue;
				}
				if(grid[y+i][x+j] == 1){
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
	for(int i = 0; i < y_dim; i++){
		for(int j = 0; j < x_dim; j++){
			grid[i][j] = fgetc(input_file) - '0';
		}
		fgetc(input_file); // throw away new line
	}

}
