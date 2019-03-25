#include <iostream>
using namespace std;

/*
OOPʵ��һ���Թ�·����Ѱ������
�������Ҫ�����£�

�������Թ�·������������5 5
�������Թ��ľ���·����Ϣ(0��ʾ���ߣ�1��ʾ������)��
0 0 1 0 0
0 0 0 0 0
1 0 0 1 1
0 1 0 1 0
1 0 0 0 0
��ʼѰ���Թ�·��...

û�ҵ�����ӡ��ʾ��Ϣ���Թ������ڿ�ͨ�е�·����
�ҵ��ˣ����Թ�·����ӡһ��
* * 1 0 0
0 * * 1 0
1 0 * 0 1
0 1 * 1 0
1 0 * * *

git����Զ�̷�֧
git�������ط�֧
git�ڱ����л���֧
git�ύ���������
*/
// ��������
// ���巽��
const int RIGHT = 0;
const int DOWN = 1;
const int LEFT = 2;
const int UP = 3;
const int WAY_NUM = 4;
// ��������״̬�õ�
const int YES = 5;
const int NO = 6;

// �����Թ��ڵ�����
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
	int mval; // �ڵ��ֵ
	int mx; // �ڵ�����
	int my; // �ڵ�����
	int mstate[WAY_NUM]; // �ڵ��ĸ������״̬
};
// ����˳��ջ�ṹ
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


// �����Թ�����
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
			mpMaze[x][y].getY());//(0,0)��ջ
		while (x < mrow  && y < mcol )
		{
			int count = 0;
			for (int i = 0; i < WAY_NUM; i++)
			{
				if (mpMaze[x][y].getState()[i] == YES)
				{
					mpMaze[x][y].getState()[i] = NO;//��ǰ���ǰ��״̬ΪNO
					if (i == RIGHT){
						y += 1; mpMaze[x][y].getState()[LEFT] = NO; }//��һ���Ľ�㷵��״̬ΪNO//0 + 2
					
					if (i == DOWN) {
						x += 1;  mpMaze[x][y].getState()[UP] = NO; }//1 + 2
					
					if (i == LEFT)  { 
						y -= 1; mpMaze[x][y].getState()[RIGHT] = NO; }//2 + 2
					
					if (i == UP) {
						x -= 1; mpMaze[x][y].getState()[DOWN] = NO; }//3 + 2
 					
					mstack.push(mpMaze[x][y].getVal(), mpMaze[x][y].getX(),
 						mpMaze[x][y].getY());//��һ���Ľ����ջ
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
	//Ϊ�˷�ֹ����ǳ�����������⣬˽�л�����������
};


int main()
{
	cout << "�������Թ�·������������";
	int row = 5, col = 5;
	//cin >> row >> col;
	Maze maze(row, col);
	int a[] = { 0,0,1,0,0,
				0,0,0,0,0,
				1,0,0,1,1,
				0,1,0,1,0,
				1,0,0,0,0 };
	cout << "�������Թ��ľ���·����Ϣ(0��ʾ���ߣ�1��ʾ������)��" << endl;
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
	
	// �����Թ����нڵ�ķ��������״̬
	maze.adjustMaze();
	maze.show();
	// Ѱ���Թ�·��
	maze.findMazePath(0, 0);
	maze.showshack();
	// ��ӡѰ�ҽṹ
	maze.showResult();

	system("pause");
	return 0;
}
