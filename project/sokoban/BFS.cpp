#include "BFS.h"
//#include "game.h"
#include "have.h"
int personX, personY, destinationX, destinationY, boxX, boxY, n = point[level].x, m = point[level].x;//�˵ĳ�ʼλ�ã����ӵĳ�ʼλ�ã�Ŀ�ĵصĳ�ʼλ�ã���ͼ�Ŀ�͸� 

void location()//��λ������ӡ�Ŀ�ĵ�
{
	for (int i = 0; i < 7; i++)//Ѱ������λ��
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
	temp.child = { personX, personY  };//���ó�ʼ�����λ��
	enQueue(bb, temp);//���
	QelemType down;
	while(!queueEmpty(bb))//Ѱ�����������������
	{
		temp = getHead(bb);//ȡ��ͷԪ��
		deQueue(bb, temp);//����
		if (map[temp.child.px][temp.child.py] == 4)//Ѱ�ҵ�����
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
			enQueue(bb, down);//���
		}
	}
	clearQueue(bb);//��ն���
	temp.child = { boxX, boxY };
	enQueue(bb, temp);
	while (!queueEmpty(bb))//Ѱ�������������Ŀ�ĵ�
	{
		temp = getHead(bb);//ȡ��ͷԪ��
		deQueue(bb, temp);//����
		if (map[temp.child.px][temp.child.py] == 3)//Ѱ�ҵ�Ŀ�ĵ�
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
			enQueue(bb, down);//���
		}
	}
}

void BFSdir()//��ȡ�ƶ��ķ���
{
	initQueue(qq);//��ʼ������
	location();//��λ����λ��
	int bfsStep = BFS()-1;//����BFS������¼����
	dir[bfsStep + 1] = '\0';
	QelemType pre = qq.base[stepN - 1];//��¼���һ���ƶ���λ�ýڵ�͸��ڵ�
	for (int i = stepN - 1; i >= 0; --i)
	{
		if (pre.parent.px == qq.base[i].child.px && pre.parent.py == qq.base[i].child.py && pre.parent.bx == qq.base[i].child.bx && pre.parent.by == qq.base[i].child.by)
		{//�ҵ����ڵ�
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

//�ƶ�����
bool Bound(int x, int y)//������ƶ������������������λ�÷�����  
{
	if (x < 0 || y < 0 || x >= n || y >= m || map[x][y] == 1)
		return true;
	else
		return false;
}
bool boxBound(int x, int y)//���ӵ��ƶ������������������λ�÷�����  
{
	if (x < 0 || y < 0 || x >= n || y >= m || map[x][y] == 1|| map[x][y]==4)
		return true;
	else
		return false;
}
bool pbBound(int x, int y,int m,int n,int bx,int by)//�ж����ӿɷ������ƶ�
{
	if ( map[x][y]==4 && (map[m][n] == 1 || map[m][n] == 4 || (bx==m && by==n)))
		return true;
	else
		return false;
}


int BFS()//���ù�������㷨Ѱ����̵���·
{
	state[personX][personY][boxX][boxY] = 1; //��ʼλ����Ϊ1
	QelemType temp;//��ʱ�洢��ǰλ����Ϣ
	temp.child = { personX, personY ,boxX ,boxY };
	enQueue(qq, temp);//���
	QelemType down;//
	int nextX, nextY;//�洢��һ����
	while (!queueEmpty(qq))
	{
		temp = getHead(qq);//ȡ��ͷԪ��
		deQueue(qq, temp);//����
		stepN++;
		if (temp.child.bx == destinationX && temp.child.by == destinationY)  //���ӵ���Ŀ�ĵ� 
		{
			return state[temp.child.px][temp.child.py][temp.child.bx][temp.child.by] - 1;//����������յ㣬���������ز���
			break;
		}
		for (int i = 0; i < 4; i++)//�ĸ��������	
		{ 
			nextX = temp.child.px + moves[i][0];//�����ƶ�
			nextY = temp.child.py + moves[i][1];
			if (Bound(nextX, nextY))  continue; //����ƶ���λ�ò��Ϸ� ��̽Ѱ�������� 
			if(pbBound(nextX, nextY, nextX + moves[i][0], nextY + moves[i][1] ,temp.child.bx, temp.child.by))
				continue;
			//���ǰ�������Ӳ����ƶ�����̽Ѱ��������
			if (nextX == temp.child.bx && nextY == temp.child.by)//������ƶ��������غ� ��˵�����ƶ�������
			{    
				if (boxBound(temp.child.bx + moves[i][0], temp.child.by + moves[i][1]))
					continue;
				if (state[nextX][nextY][temp.child.bx + moves[i][0]][temp.child.by + moves[i][1]]) continue;
				state[nextX][nextY][temp.child.bx + moves[i][0]][temp.child.by + moves[i][1]] = state[temp.child.px][temp.child.py][temp.child.bx][temp.child.by] + 1;
				//�����ƶ������˺�����λ�øı䣬��¼��״̬ 
				down.child = { nextX, nextY ,temp.child.bx + moves[i][0] ,temp.child.by + moves[i][1] };
				down.parent = { temp.child.px, temp.child.py ,temp.child.bx ,temp.child.by };
				enQueue(qq, down);   //��״̬���  
			}
			else   //��û�����������߿յ� 
			{
				if (state[nextX][nextY][temp.child.bx][temp.child.by]) continue; //����ƶ����״̬���ֹ��������������·���
				state[nextX][nextY][temp.child.bx][temp.child.by] = state[temp.child.px][temp.child.py][temp.child.bx][temp.child.by] + 1;
				down.child = { nextX, nextY ,temp.child.bx ,temp.child.by };
				down.parent = { temp.child.px, temp.child.py ,temp.child.bx ,temp.child.by };
				enQueue(qq, down);
			}
		}


	}
	return -1;//�����޷�����Ŀ�ĵط���-1
}


