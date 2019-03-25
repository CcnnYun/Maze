#include <iostream>
using namespace std;

/*
OOP实现一个迷宫路径的寻找问题
软件运行要求如下：

请输入迷宫路径的行列数：5 5
请输入迷宫的具体路径信息(0表示能走，1表示不能走)：
0 0 1 0 0
0 0 0 0 0
1 0 0 1 1
0 1 0 1 0
1 0 0 0 0
开始寻找迷宫路径...

没找到，打印提示信息“迷宫不存在可通行的路径”
找到了，把迷宫路径打印一遍
* * 1 0 0
0 * * 1 0
1 0 * 0 1
0 1 * 1 0
1 0 * * *

git创建远程分支
git创建本地分支
git在本地切换分支
git提交代码的流程
*/
// 常量定义
// 定义方向
const int RIGHT = 0;
const int DOWN = 1;
const int LEFT = 2;
const int UP = 3;
const int WAY_NUM = 4;
// 定义行走状态用的
const int YES = 5;
const int NO = 6;

// 抽象迷宫节点类型
class MazeNode
{
public:
	MazeNode()
	{
		for (int i = 0; i < WAY_NUM; ++i)
		{
			mstate[i] = YES;
		}
	}
	void setVal(int data, int x, int y)
	{
		mval = data; 
		mx = x; 
		my = y;
	}

	int getVal()
	{
		return mval;
	}
	int getX()
	{
		return mx;
	}
	int getY()
	{
		return my;
	}
	int* getState()
	{
		return mstate;
	}
private:
	int mval; // 节点的值
	int mx; // 节点坐标
	int my; // 节点坐标
	int mstate[WAY_NUM]; // 节点四个方向的状态
};
// 抽象顺序栈结构
class CStack
{
public:
	CStack(int size = 10) 
		:msize(size),mtop(0)
	{
		mpstack = new MazeNode[msize];
	}
	~CStack()
	{
		delete[] mpstack;
		mpstack = nullptr;
	}
	void push(int val,int x,int y)
	{
		if (full())
		{
			resize();
		}
		mpstack[mtop++].setVal(val, x, y);
	}
	bool pop()
	{
		if (empty())
		{
			return false;
		}
		mtop--;
		return true;
	}
	bool findXY(int x, int y) 
	{
		bool flag = false;
		for (int i = 0; i < mtop; i++)
		{
			if (mpstack[i].getX() == x && mpstack[i].getY() == y)
			{
				flag = true;
				break;
			}
		}
		return flag;
	}
	bool full()
	{
		return (mtop == msize);
	}
	bool empty()
	{
		return (mtop == 0);
	}
	int gettopX()
	{
		return mpstack[mtop - 1].getX();
	}
	int gettopY()
	{
		return mpstack[mtop - 1].getY();
	}
	void show()
	{
		for (int i = 0; i < mtop; i++)
		{
			cout << "(" << mpstack[i].getX() << "," << mpstack[i].getY() << ")" << endl;
		}
	}
private:
	MazeNode *mpstack;
	int mtop;
	int msize;
	CStack(const CStack &src);
	void operator=(const CStack &src);
	void resize()
	{
		MazeNode *tmp = new MazeNode[msize * 2];
		for (int i = 0; i < msize; i++)
		{
			tmp[i].setVal(mpstack[i].getVal(), mpstack[i].getX(), mpstack[i].getY());
		}
	}
};


// 抽象迷宫类型
class Maze
{
public:
	Maze(int row, int col)
		:mrow(row),mcol(col)
	{
		mpMaze = new MazeNode *[mrow];
		for (int i = 0; i < mrow; i++)
		{
			mpMaze[i] = new MazeNode[mcol];
		}
	}
	~Maze()
	{
		for (int i = 0; i < mcol; i++)
		{
			delete[] mpMaze[i];
		}
		delete[] mpMaze;
	}
	void initMazeNode(int data, int row, int col)
	{
		mpMaze[row][col].setVal(data, row, col);
	}
	void adjustMaze()
	{
		
		for (int x = 0; x < mrow; x++)
		{
			for (int y = 0;  y < mcol; y++)
			{
				if (!mpMaze[x][y].getVal())
				{
					if (y + 1 >= mcol || mpMaze[x][y + 1].getVal() == 1)
					{ mpMaze[x][y].getState()[RIGHT] = NO; }

					if (x + 1 >= mrow || mpMaze[x + 1][y].getVal() == 1)
					{ mpMaze[x][y].getState()[DOWN] = NO; }

					if (y - 1 < 0 || mpMaze[x][y - 1].getVal() == 1)
					{ mpMaze[x][y].getState()[LEFT] = NO; }

					if (x - 1 < 0 || mpMaze[x - 1][y].getVal() == 1)
					{ mpMaze[x][y].getState()[UP] = NO; }
				}
			}
		}
	}
	void findMazePath(int x, int y)
	{
		mstack.push(mpMaze[x][y].getVal(), mpMaze[x][y].getX(),
			mpMaze[x][y].getY());//(0,0)入栈
		while (x < mrow  && y < mcol )
		{
			int count = 0;
			for (int i = 0; i < WAY_NUM; i++)
			{
				if (mpMaze[x][y].getState()[i] == YES)
				{
					mpMaze[x][y].getState()[i] = NO;//当前结点前进状态为NO
					if (i == RIGHT){
						y += 1; mpMaze[x][y].getState()[LEFT] = NO; }//下一步的结点返回状态为NO//0 + 2
					
					if (i == DOWN) {
						x += 1;  mpMaze[x][y].getState()[UP] = NO; }//1 + 2
					
					if (i == LEFT)  { 
						y -= 1; mpMaze[x][y].getState()[RIGHT] = NO; }//2 + 2
					
					if (i == UP) {
						x -= 1; mpMaze[x][y].getState()[DOWN] = NO; }//3 + 2
 					
					mstack.push(mpMaze[x][y].getVal(), mpMaze[x][y].getX(),
 						mpMaze[x][y].getY());//下一步的结点入栈
					break;
				}
				else
				{
					count++;
				}
			}
			if (count >= WAY_NUM)
			{
				mstack.pop();
				x = mstack.gettopX();
				y = mstack.gettopY();
			}
			if (x == mrow - 1)
			{ if (y == mcol - 1) { break; } }
		}
	}
	void show()
	{
		for (int i = 0; i < mrow; i++)
		{
			for (int j = 0; j < mcol; j++)
			{
				cout << mpMaze[i][j].getVal()
					<< " (" << mpMaze[i][j].getX() << "," << mpMaze[i][j].getY() << ") ("
					<< mpMaze[i][j].getState()[0] << mpMaze[i][j].getState()[1]
					<< mpMaze[i][j].getState()[2] << mpMaze[i][j].getState()[3] << ") ";
			}
			cout << endl;
		}
		cout << "void show()" << endl;
	}
	void showshack()
	{
		mstack.show();
	}
	void showResult()
	{
		for (int i = 0; i < mrow; i++)
		{
			for (int j = 0; j < mcol; j++)
			{
				if (mstack.findXY(i,j))
				{
					cout << "* ";
				}
				else
				{
					cout << mpMaze[i][j].getVal() << " ";
				}
			}
			cout << endl;
		}
	}
private:
	int mrow;
	int mcol;
	MazeNode **mpMaze;
	CStack mstack;
	//为了防止对象浅拷贝出现问题，私有化那两个函数
};


int main()
{
	cout << "请输入迷宫路径的行列数：";
	int row = 5, col = 5;
	//cin >> row >> col;
	Maze maze(row, col);
	int a[] = { 0,0,1,0,0,
				0,0,0,0,0,
				1,0,0,1,1,
				0,1,0,1,0,
				1,0,0,0,0 };
	cout << "请输入迷宫的具体路径信息(0表示能走，1表示不能走)：" << endl;
	int k = 0;
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			int data = a[k];
			//cin >> data;
			cout << a[k] << " ";
			maze.initMazeNode(data, i, j);
			k++;
		}
		cout << endl;
	}
	
	// 调整迷宫所有节点的方向的行走状态
	maze.adjustMaze();
	maze.show();
	// 寻找迷宫路径
	maze.findMazePath(0, 0);
	maze.showshack();
	// 打印寻找结构
	maze.showResult();

	system("pause");
	return 0;
}
