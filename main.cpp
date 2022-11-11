#include <iostream>
#include <vector>
#include <bitset>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <time.h>


#define ONE_D_SIZE 64
#define x_dim 64
#define y_dim 32

//function templates
void display_generation_one_dim(std::bitset<ONE_D_SIZE> line);
void display_generation_two_dim(int grid[y_dim][x_dim]);
int one_dimensional_simulation(int rule_num);
int conways_game_of_life(bool is_three_d);
int count_neighbours(int grid[y_dim][x_dim], int x, int y, bool is_three_d);

int main(){

	//1D simulation
	system("clear");
	std::cout<<"-------- One dimensional cellular automaton --------" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	if (one_dimensional_simulation(90) == 1){
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
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	return 0;
}


int one_dimensional_simulation(int rule_num){

	#define ONE_D_NUM_GENERATIONS 32

	//1D simulation

	if(rule_num < 0 || rule_num > 255){
		return 1;
	}

	std::bitset<8> rule(rule_num);
	std::bitset<ONE_D_SIZE> parent;
	std::bitset<ONE_D_SIZE> child;
	std::bitset<3> pattern;

	parent[ONE_D_SIZE/2] = 1;

	display_generation_one_dim(parent);
	std::cout<<std::endl;

	for(int k = 0; k < ONE_D_NUM_GENERATIONS; k++){
		for(int i = 0; i < ONE_D_SIZE; i++){
			for(int j = -1; j < 2; j++){
				if(parent[((i+j)%ONE_D_SIZE)] == 1){
					pattern[j+1] = 1;
				}
			}
			child[i] = rule[(int)pattern.to_ulong()];
			pattern.reset();
		}
		display_generation_one_dim(child);
		std::cout<<std::endl;
		parent = child;
		child.reset();
	}

	return 0;
}

int conways_game_of_life(bool is_three_d){

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	int parent_grid[y_dim][x_dim] = {0};
	int child_grid[y_dim][x_dim] = {0};

	srand(time(0));

	int neighbour_count;

	for(int i = 0; i < y_dim; i++){
		for(int j = 0; j < x_dim; j++){
			parent_grid[i][j] = std::rand()%2;
		}
	}

	display_generation_two_dim(parent_grid);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	for(int num_gen = 0; num_gen < 20; num_gen++){

		for(int i = 0; i < y_dim; i++){
			for(int j = 0; j < x_dim; j++){
				neighbour_count = 0;
				neighbour_count = count_neighbours(parent_grid, i, j, is_three_d);

				if(parent_grid[i][j] == 1){
					if(neighbour_count < 2 || neighbour_count > 3){
						child_grid[i][j] = 0;
					}
					else{
						child_grid[i][j] = 1;
					}
				}

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

		display_generation_two_dim(child_grid);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));


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
	for(int i = 0; i < y_dim; i++){
		for(int j = 0; j < x_dim; j++){
			if(grid[i][j] == 1){
				std::cout<<"#";
			}
			else{
				std::cout<<" ";
			}
		}
		std::cout<<std::endl;
	}
}
