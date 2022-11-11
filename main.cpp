#include <iostream>
#include <vector>
#include <bitset>


//one-dimensional size has a maximum of 64 due to bitset data structure limitations
#define ONE_D_SIZE 64

//function templates
void display_generation_one_dim(std::bitset<ONE_D_SIZE> line);
int one_dimensional_simulation(int rule_num);
int conways_game_of_life();

int main(){

	if (one_dimensional_simulation(30) == 1){
		std::cout<<"Rule not valid - rule must be between 0 and 255 inclusive!"<<std::endl;
	}

	//2D simulation



	//3D (toroidal) simulation

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

int conways_game_of_life(){

}


void display_generation_one_dim(std::bitset<ONE_D_SIZE> line){
	for(int i = 0; i < ONE_D_SIZE; i++){
		if(line[i] == 1){
			std::cout<<"#";
		}
		else{
			std::cout<<".";
		}
	}
}