#include <iostream>
#include <cstdlib>
#include <ctime>

#define MAX2(a,b)			((a)>(b)?(a):(b))
#define MAX3(a,b,c)			(MAX2(MAX2(a,b),(c)))
#define MAX4(a,b,c,d)		(MAX2(MAX3(a,b,c),(d)))

class CellData
{
public:
	double q_[4] = { 0.0 ,0.0 ,0.0 ,0.0 };//up down left right
	double reward_ = 0.0;
	bool is_wall = false;
	CellData() {}
	double getMaxQ()
	{
		return MAX4(q_[0], q_[1], q_[2], q_[3]);
	}
};
class GridWorld
{
public:
	
	int i_res_, j_res_;
	
	CellData *q_arr2d_ = nullptr;
	
	GridWorld(const int& i_res, const int& j_res)
		:i_res_(i_res), j_res_(j_res)
	{q_arr2d_ = new CellData[i_res*j_res];}

	CellData& getCell(const int& i, const int& j)
	{return q_arr2d_[i + i_res_*j];}


	bool isInside(const int& i, const int& j)
	{
		if (i < 0) return false;
		if (i >= i_res_)return false;
		if (j < 0)return false;
		if (j >= j_res_)return false;
		if (q_arr2d_[i + i_res_*j].is_wall == true)
			return false;
		return true;
	}

	void printSigned(const float& v)
	{
		if (v > 0.0f)printf("+%1.2f", v);
		else if (v < 0.0f)printf("%1.2f", v);
		else
			printf(" 0.0");
	}

	void print()
	{
		for (int j = j_res_ - 1; j >= 0; j--)
		{
			for (int i = 0; i < i_res_; i++)
			{
				CellData &cell = getCell(i, j);
				printf("      "); printSigned(cell.q_[0]); printf("            ");
				
			}
			printf("\n");
			for (int i = 0; i < i_res_; i++)
			{
				CellData &cell = getCell(i, j);
				printSigned(cell.q_[2]);	printf("       "); printSigned(cell.q_[3]);
				printf("      ");
			}
			printf("\n");
			for (int i = 0; i < i_res_; i++)
			{
				CellData &cell = getCell(i, j);
				printf("      "); printSigned(cell.q_[1]); printf("            ");
				
			}
			printf("\n\n\n");
		}
	}
};

class Agent
{
public:
	int i_, j_;
	double reward_;

	Agent()
		:i_(0), j_(0), reward_(0.0)
	{}
	void move(int i, int j)
	{
		i_ = i;
		j_ = j;
	}
	void reset()
	{
		i_ = 0;
		j_ = 0;
		reward_ = 0;
	}
};




void grid_world2()
{
	struct point
	{
		int x, y;
	};

	srand((unsigned int)time(NULL));
	std::cout << "Hello, grid world2!" << std::endl;
	const int world_res_i = 4, world_res_j = 3;
	GridWorld world(world_res_i, world_res_j);
	double alpha = 0.8;
	double gamma = 0.9;

	point pos_reward[1] = { 3, 2 };
	point nav_reward[1] = { 3, 1 };


	for (int j = 0; j < world_res_j; j++)
	{
		for (int i = 0; i < world_res_i; i++)
			world.getCell(i, j).reward_ = -0.1;
	}

	for (auto itr : pos_reward)
	{
		world.getCell(itr.x, itr.y).reward_ = 1.0;
	}
	for (auto itr : nav_reward)
	{
		world.getCell(itr.x, itr.y).reward_ = -1.0;
	}

	world.getCell(1, 1).is_wall = true;
	
	Agent my_agent;

	for (int t = 0; t < 100000; t++)
	{
		const int action = rand() % 4;
		int i = my_agent.i_, j = my_agent.j_;
		int i_old = i, j_old = j;
		switch (action)
		{
		case 0:
			j++;//up
			break;
		case 1:
			j--;//down
			break;
		case 2:
			i--;//left
			break;
		case 3:
			i++;//right
			break;
		}
		if (world.isInside(i, j) == true)//move robot if avaiable
		{
			//move agent // state를 업뎃 해주고 
			my_agent.move(i, j);

			
			//update reward(r_t) rt계산 바꾸고
			my_agent.reward_ -= 0.001;

			//update q values of previous cell(q_t) qt계산해서 바꾸고
			world.getCell(i_old, j_old).q_[action] +=
				alpha*(
					world.getCell(i, j).reward_
					+ gamma*world.getCell(i, j).getMaxQ() + my_agent.reward_
					- world.getCell(i_old, j_old).q_[action]
					);

			//reset if agent is in final cells 만약  파이널에 도달했으면 리셋후 훈련시작
			for (auto itr : pos_reward)
			{
				if (itr.x == i && itr.y == j)
					my_agent.reset();
			}
			for (auto itr : nav_reward)
			{
				if (itr.x == i && itr.y == j)
					my_agent.reset();
			}
			
		}
		else //만약 벗어난다면?
		{
			world.getCell(i_old, j_old).q_[action] = -0.01;
		}
		system("cls");
		world.print();

	}
	world.print();


}

int main()
{
	//grid_world1();

	grid_world2();
	return 0;

}