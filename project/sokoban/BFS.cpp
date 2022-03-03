#include "BFS.h"
//#include "game.h"
#include "have.h"
int personX, personY, destinationX, destinationY, boxX, boxY, n = point[level].x, m = point[level].x;//人的初始位置，箱子的初始位置，目的地的初始位置，地图的宽和高 

void location()//定位人物、箱子、目的地
{
	for (int i = 0; i < 7; i++)//寻找人物位置
	{
		for (int j = 0; j < 10; j++)
		{
			if (map[i][j] == 5) //|| map[i][j] == 8)
			{
				personX = i;
				personY = j;
			}

		}
	}
	sqQueue bb;
	initQueue(bb);
	QelemType temp;
	temp.child = { personX, personY  };//设置初始人物的位置
	enQueue(bb, temp);//入队
	QelemType down;
	while(!queueEmpty(bb))//寻找离人物最近的箱子
	{
		temp = getHead(bb);//取队头元素
		deQueue(bb, temp);//出队
		if (map[temp.child.px][temp.child.py] == 4)//寻找到箱子
		{
			boxX = temp.child.px;
			boxY = temp.child.py;
			break;
		}
		for (int j = 0; j < 4; j++)
		{
			int nextX = temp.child.px + moves[j][0];
			int nextY = temp.child.py + moves[j][1];
			if (Bound(nextX, nextY))  continue;
			down.child = { nextX ,nextY };
			enQueue(bb, down);//入队
		}
	}
	clearQueue(bb);//清空队列
	temp.child = { boxX, boxY };
	enQueue(bb, temp);
	while (!queueEmpty(bb))//寻找离箱子最近的目的地
	{
		temp = getHead(bb);//取队头元素
		deQueue(bb, temp);//出队
		if (map[temp.child.px][temp.child.py] == 3)//寻找到目的地
		{
			destinationX = temp.child.px;
			destinationY = temp.child.py;
			break;
		}
		for (int j = 0; j < 4; j++)
		{
			int nextX = temp.child.px + moves[j][0];
			int nextY = temp.child.py + moves[j][1];
			if (Bound(nextX, nextY))  continue;
			down.child = { nextX ,nextY };
			enQueue(bb, down);//入队
		}
	}
}

void BFSdir()//提取移动的方向
{
	initQueue(qq);//初始化队列
	location();//定位各个位置
	int bfsStep = BFS()-1;//运行BFS，并记录步数
	dir[bfsStep + 1] = '\0';
	QelemType pre = qq.base[stepN - 1];//记录最后一次移动的位置节点和父节点
	for (int i = stepN - 1; i >= 0; --i)
	{
		if (pre.parent.px == qq.base[i].child.px && pre.parent.py == qq.base[i].child.py && pre.parent.bx == qq.base[i].child.bx && pre.parent.by == qq.base[i].child.by)
		{//找到父节点
			int a[2] = { pre.child.px - pre.parent.px, pre.child.py - pre.parent.py };//
			if (pre.child.px - pre.parent.px == 0)
			{
				if (pre.child.py - pre.parent.py == 1)
				{
					Sleep(300);
					userkey = 'd';
					keyDown(map);
				}
				else
				{
					Sleep(300);
					userkey = 'a';
					keyDown(map);
				}
			}
			else
			{
				if (pre.child.px - pre.parent.px == 1)
				{
					Sleep(300);
					userkey = 's';
					keyDown(map);
				}
				else
				{
					Sleep(300);
					userkey = 'w';
					keyDown(map);
				}
			}
			bfsStep--;
			pre = qq.base[i];

		}
	}
}

//移动条件
bool Bound(int x, int y)//人物的移动条件，遇到不合理的位置返回真  
{
	if (x < 0 || y < 0 || x >= n || y >= m || map[x][y] == 1)
		return true;
	else
		return false;
}
bool boxBound(int x, int y)//箱子的移动条件，遇到不合理的位置返回真  
{
	if (x < 0 || y < 0 || x >= n || y >= m || map[x][y] == 1|| map[x][y]==4)
		return true;
	else
		return false;
}
bool pbBound(int x, int y,int m,int n,int bx,int by)//判断箱子可否被人物推动
{
	if ( map[x][y]==4 && (map[m][n] == 1 || map[m][n] == 4 || (bx==m && by==n)))
		return true;
	else
		return false;
}


int BFS()//利用广度优先算法寻找最短的线路
{
	state[personX][personY][boxX][boxY] = 1; //初始位置设为1
	QelemType temp;//临时存储当前位置信息
	temp.child = { personX, personY ,boxX ,boxY };
	enQueue(qq, temp);//入队
	QelemType down;//
	int nextX, nextY;//存储下一个点
	while (!queueEmpty(qq))
	{
		temp = getHead(qq);//取队头元素
		deQueue(qq, temp);//出队
		stepN++;
		if (temp.child.bx == destinationX && temp.child.by == destinationY)  //箱子到达目的地 
		{
			return state[temp.child.px][temp.child.py][temp.child.bx][temp.child.by] - 1;//如果箱子在终点，结束，返回步数
			break;
		}
		for (int i = 0; i < 4; i++)//四个方向遍历	
		{ 
			nextX = temp.child.px + moves[i][0];//人物移动
			nextY = temp.child.py + moves[i][1];
			if (Bound(nextX, nextY))  continue; //如果移动的位置不合法 ，探寻其它方向 
			if(pbBound(nextX, nextY, nextX + moves[i][0], nextY + moves[i][1] ,temp.child.bx, temp.child.by))
				continue;
			//如果前方的箱子不可推动，则探寻其他方向
			if (nextX == temp.child.bx && nextY == temp.child.by)//如果人移动和箱子重合 ，说明人推动了箱子
			{    
				if (boxBound(temp.child.bx + moves[i][0], temp.child.by + moves[i][1]))
					continue;
				if (state[nextX][nextY][temp.child.bx + moves[i][0]][temp.child.by + moves[i][1]]) continue;
				state[nextX][nextY][temp.child.bx + moves[i][0]][temp.child.by + moves[i][1]] = state[temp.child.px][temp.child.py][temp.child.bx][temp.child.by] + 1;
				//箱子推动，则人和箱子位置改变，记录新状态 
				down.child = { nextX, nextY ,temp.child.bx + moves[i][0] ,temp.child.by + moves[i][1] };
				down.parent = { temp.child.px, temp.child.py ,temp.child.bx ,temp.child.by };
				enQueue(qq, down);   //新状态入队  
			}
			else   //人没有推箱子则走空地 
			{
				if (state[nextX][nextY][temp.child.bx][temp.child.by]) continue; //如果移动后的状态出现过，则重新搜索新方向
				state[nextX][nextY][temp.child.bx][temp.child.by] = state[temp.child.px][temp.child.py][temp.child.bx][temp.child.by] + 1;
				down.child = { nextX, nextY ,temp.child.bx ,temp.child.by };
				down.parent = { temp.child.px, temp.child.py ,temp.child.bx ,temp.child.by };
				enQueue(qq, down);
			}
		}


	}
	return -1;//箱子无法到达目的地返回-1
}


