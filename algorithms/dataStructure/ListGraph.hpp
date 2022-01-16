#include <iostream>

#define MAXVERTEX 1024
using namespace std;


//邻接表
template<class T>
class ListGraph
{
public:
	ListGraph() 
	{
	}

	~ListGraph()
	{

	}

	//添加弧
	void insertArc()

private:
	//顶点结构体
	struct Vertex
	{
		T name;//顶点数据

		EdgeNode* first;//指向第一条依附该顶点的边的指针
	};

	struct EdgeNode
	{
		unsigned int pos;//存储顶点位置

		EdgeNode* next;//存储下一条边
	};


private:

	Vertex head[MAXVERTEX];

	size_t m_vertexNum;//顶点数

	size_t m_edgeNum;//边数
};