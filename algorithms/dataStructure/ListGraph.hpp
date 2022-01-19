#pragma once
#include <iostream>
#include<functional>

#include "sqList.hpp"
#include "sqQueue.hpp"

#define MAXVERTEX 1024
using namespace std;


//邻接表
template<class T>
class DListGraph
{
public:
	DListGraph(size_t vertexNum):m_vertexNum(vertexNum)
	{
		m_head = new Vertex[m_vertexNum];
		if (m_head == NULL)
		{
			cerr << "内存申请失败" << endl;
			return;
		}
		for (int i = 0; i < m_vertexNum; ++i)
		{
			m_head[i].first = new EdgeNode;
			m_head[i].first->next = nullptr;
		}
	

	}

	~DListGraph()
	{
		for (int i = 0; i < m_vertexNum; ++i)
		{
			EdgeNode* curNode = m_head[i].first;
			while (1)
			{
				if (curNode == nullptr)
					break;
				EdgeNode* nextNode = curNode->next;
				delete curNode;
				curNode = nextNode;
			}

		}

		delete[] m_head;
	}

	//添加弧
	void insertArc(unsigned int a, unsigned int b, const T& val,const T& auth)//顶点，邻接点，顶点数据，权
	{
		m_head[a].data = val;
		EdgeNode* newEdge = new EdgeNode;
		newEdge->nextArc = b;
		newEdge->info = auth;
		newEdge->next = nullptr;

		EdgeNode* curEdge = m_head[a].first->next;

		newEdge->next = curEdge;
		m_head[a].first->next = newEdge;
		
		++m_edgeNum;
	}

	void insertSymArc(unsigned int a, unsigned int b, const T& val,const T& auth)
	{

		m_head[a].data = val;
		EdgeNode* newEdge = new EdgeNode;
		newEdge->nextArc = b;
		newEdge->info = auth;
		newEdge->next = nullptr;

		EdgeNode* curEdge = m_head[a].first->next;

		newEdge->next = curEdge;
		m_head[a].first->next = newEdge;


		m_head[b].data = val;
		newEdge = new EdgeNode;
		newEdge->nextArc = a;
		newEdge->info = auth;
		newEdge->next = nullptr;

		curEdge = m_head[b].first->next;

		newEdge->next = curEdge;
		m_head[b].first->next = newEdge;

		m_edgeNum += 2;
	}

	//遍历
	void foreach(void (*print)(const T& val))
	{
		EdgeNode* curNode;
		for (int i = 0; i < m_vertexNum; ++i)
		{
			if (m_head[i].first->next != nullptr)
			{
				cout << m_head[i].data << ":";
				curNode = m_head[i].first->next;
				while (curNode != nullptr)
				{
					cout << "[" << curNode->nextArc << "]";
					print(curNode->info);
					curNode = curNode->next;
				}
				cout << endl;
			}
			
		}
		cout << endl;
	}

	//深度优先搜索

	/*为了实现深度优先搜索，首先选择一个起始顶点并需要遵守三个规则：
	1.如果可能，访问一个邻接的未访问顶点，标记它，并把它放入栈中。
	2.当不能执行规则1时，如果栈不空，就从栈中弹出一个顶点。
	3.如果不能执行规则1和规则2，就完成了整个搜索过程
	*/
	void DFS(unsigned int v,void (*print)(const T& val))//v为出发点
	{
		bool* visited = new bool[m_vertexNum];//标记是否被访问过
		for (int i = 0; i < m_vertexNum; ++i)
		{
			visited[i] = false;
		}
		
		//递归遍历
		std::function<void (const int pos)> func = [&](const int pos)->void
		{
			//print(m_head[pos].data);
			cout << pos << " ";
			visited[pos] = true;//访问标志
			EdgeNode* curEdge = m_head[pos].first->next;//
			while (curEdge != nullptr)
			{
				int adjoinPos = curEdge->nextArc;//邻接点
				if (!visited[adjoinPos])
				{
					func(adjoinPos);//递归
				}
				curEdge = curEdge->next;
			}

		};

		func(v);

		delete[] visited;
	}

	/*
	实现广度优先搜索，也要遵守三个规则：
	访问下一个未来访问的邻接点，这个顶点必须是当前顶点的邻接点，标记它，并把它插入到队列中。
	如果因为已经没有未访问顶点而不能执行规则1时，那么从队列头取一个顶点，并使其成为当前顶点。
	如果因为队列为空而不能执行规则2，则搜索结束。
	*/
	//广度优先搜索
	void BFS(unsigned int v, void (*print)(const T& val))
	{
		bool* visited = new bool[m_vertexNum];//标记是否被访问过
		for (int i = 0; i < m_vertexNum; ++i)
		{
			visited[i] = false;
		}

		DsqQueue< int>que(m_vertexNum);

		que.push(v);
		//print(m_head[v].data);
		cout << v << " ";
		visited[v] = true;

		while (!que.isEmpty())
		{
			EdgeNode* curNode = m_head[que.front()].first->next;
			while (curNode != nullptr)
			{
				if (!visited[curNode->nextArc])
				{
					que.push(curNode->nextArc);
					//print(m_head[curNode->nextArc].data);
					cout << curNode->nextArc << " ";
					visited[curNode->nextArc] = true;
				}
				curNode = curNode->next;
			}
			
			que.pop();
		}


		delete[] visited;

	}

	//最短路径---迪杰斯特拉算法
	void shortestPath_DIJ(int v)
	{
		//初始化
		int dist[m_vertexNum] ;//出发点到各个点的最短距离
		int path[m_vertexNum] ;//到当前点的最近邻接点
		bool* visited = new bool[m_vertexNum];//标记是否被访问过
		for (int i = 0; i < m_vertexNum; ++i)
		{
			visited[i] = false;
			if (i != v)//排除顶点为出发点
			{
				path[i] 
			}
		}

	}

private:


	struct EdgeNode
	{
		unsigned int nextArc;//指向顶点的位置

		T info;//权

		EdgeNode* next;//存储下一条边
	};

	//顶点结构体
	struct Vertex
	{
		T data;// 点名

		EdgeNode* first;//指向第一条依附该顶点的边的指针
	};

private:

	Vertex* m_head;

	size_t m_vertexNum;//顶点数

	size_t m_edgeNum;//边数
};