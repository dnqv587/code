#include <iostream>

#define MAXVERTEX 1024
using namespace std;


//�ڽӱ�
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

	//��ӻ�
	void insertArc()

private:
	//����ṹ��
	struct Vertex
	{
		T name;//��������

		EdgeNode* first;//ָ���һ�������ö���ıߵ�ָ��
	};

	struct EdgeNode
	{
		unsigned int pos;//�洢����λ��

		EdgeNode* next;//�洢��һ����
	};


private:

	Vertex head[MAXVERTEX];

	size_t m_vertexNum;//������

	size_t m_edgeNum;//����
};