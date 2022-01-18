#include <iostream>

#include "sqList.hpp"

#define MAXVERTEX 1024
using namespace std;


//邻接表
template<class T>
class DListGraph
{
public:
	DListGraph(size_t vertexNum):m_vertexNum(vertexNum)
	{
		head = new Vertex[m_vertexNum];
		if (head == NULL)
		{
			cerr << "内存申请失败" << endl;
			return;
		}
		for (int i = 0; i < m_vertexNum; ++i)
		{
			head[i].first = new EdgeNode;
			head[i].first->next = nullptr;
		}
	

	}

	~DListGraph()
	{
		for (int i = 0; i < m_vertexNum; ++i)
		{
			EdgeNode* curNode = head[i].first;
			while (1)
			{
				if (curNode == nullptr)
					break;
				EdgeNode* nextNode = curNode->next;
				delete curNode;
				curNode = nextNode;
			}

		}

		delete[] head;
	}

	//添加弧
	void insertArc(unsigned int a, unsigned int b, const T& val)
	{
		head[a].name = to_string(a);
		EdgeNode* newEdge = new EdgeNode;
		newEdge->nextArc = b;
		newEdge->data = val;
		newEdge->next = nullptr;

		EdgeNode* curEdge = head[a].first->next;

		newEdge->next = curEdge;
		head[a].first->next = newEdge;
		
		++m_edgeNum;
	}

	void insertSymArc(unsigned int a, unsigned int b, const T& val)
	{

		head[a].name = to_string(a);
		EdgeNode* newEdge = new EdgeNode;
		newEdge->nextArc = b;
		newEdge->data = val;
		newEdge->next = nullptr;

		EdgeNode* curEdge = head[a].first->next;

		newEdge->next = curEdge;
		head[a].first->next = newEdge;


		head[b].name = to_string(b);
		newEdge = new EdgeNode;
		newEdge->nextArc = a;
		newEdge->data = val;
		newEdge->next = nullptr;

		curEdge = head[b].first->next;

		newEdge->next = curEdge;
		head[b].first->next = newEdge;

		m_edgeNum += 2;
	}

	//遍历
	void foreach(void (*print)(const T& val))
	{
		EdgeNode* curNode;
		for (int i = 0; i < m_vertexNum; ++i)
		{
			if (head[i].name != "")
			{
				cout << head[i].name << ":";
				curNode = head[i].first->next;
				while (curNode != nullptr)
				{
					cout << "[" << curNode->nextArc << "]";
					print(curNode->data);
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
	void DFS()
	{
		bool* visited = new bool[m_vertexNum];//标记是否被访问过
		for (int i = 0; i < m_vertexNum; ++i)
		{
			visited[i] = false;
		}

		function<> func = []() 
		{
			
		}
		


	}

private:


	struct EdgeNode
	{
		unsigned int nextArc;//指向顶点的位置

		T data;//权

		EdgeNode* next;//存储下一条边
	};

	//顶点结构体
	struct Vertex
	{
		string name;// 点名

		EdgeNode* first;//指向第一条依附该顶点的边的指针
	};

private:

	Vertex* head;

	size_t m_vertexNum;//顶点数

	size_t m_edgeNum;//边数
};