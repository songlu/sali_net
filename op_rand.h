#include <iostream>
#include <ctime>
using namespace std;
class op_rand{
public:
	op_rand(){
		srand((unsigned)time(0));
	}
	int gene_rand_num(int maxnum){ return rand() % maxnum; }
	void gene_rand_box(int rows, int cols, int patchsize, int & begin_y, int &begin_x)
	{
		begin_y = rand()%(rows - patchsize + 1);
		begin_x = rand()%(cols - patchsize + 1);
	}
};