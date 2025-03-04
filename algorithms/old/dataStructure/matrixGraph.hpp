﻿#pragma once
#include <iostream>

using namespace std;

/*
图的定义：图是由顶点的有穷非空集合和顶点之间的边的集合组成
无向图: 如果图中任意两个顶点之间的边都是无向边
有向图: 如果图中任意顶点之间的边都是有向边
无向完全图：在无向图中，如果任意两个顶点之间都存在边
有向完全图：在有向图中，如果任意两个顶点之间都存在方向互为相反的两条弧
总结: 图G的顶点数n和边数e的关系
（1）若G是无向图，则0≤e≤n(n-1)/2
	恰有n(n-1)/2条边的无向图称无向完全图(Undireet-ed Complete Graph)
（2）若G是有向图，则0≤e≤n(n-1)。
	恰有n(n-1)条边的有向图称为有向完全图
稀疏图: 有很少条边或弧的图。
稠密图: 有很多条边或弧的图。
权: 有时图的边或弧具有与它相关的数,这种与图的边或弧相关的数叫做权。
网：带权的图通常称为网。
度：顶点的度是指和该顶点关联的边的数目。
入度：有向图中以顶点（v）为头的弧的数目，称为（v）的入度。
出度：有向图中以顶点（v）为尾的弧的数目，称为（v）的出度。
邻接点：对于无向图，同一边上的两个顶点称为邻接点。
子图: 假设两个图G=(V,E)和G1=(V1,E1),如果V1⊆V且E1⊆E则G1为G的子图
路径的长度: 路径上的边或弧的数目。
连通图:如果对图中任意两个顶点Vi和Vj 属于E，则两个顶点是连通的
连通图的生成树是一个极小的连通子图，它含有图中全部的n个顶点，但只有足以构成一棵树的n-1条边。
*/


//邻接矩阵

template <class T>
class DMatrixGraph
{
public:
	DMatrixGraph(size_t vertexNum):m_vertexNum(vertexNum), m_edgeNum(0)
	{
		if (vertexNum > 0)
		{
			m_graph = new T*[m_vertexNum];
			memset(m_graph, 0x00, sizeof(T) * m_vertexNum);

			for (int i = 0; i < m_vertexNum; ++i)
			{
				m_graph[i] = new T[m_vertexNum];
				memset(m_graph[i], 0x00, sizeof(T) * m_vertexNum);
			}

		}
		else
		{
			return;
		}
	}

	DMatrixGraph(const DMatrixGraph& graph)
	{
		m_graph = new T* [graph.m_vertexNum];
		for (int i = 0; i < graph.m_vertexNum; ++i)
		{
			m_graph[i] = new T[graph.m_vertexNum];
		}

		for (int i = 0; i < graph.m_vertexNum; ++i)
		{
			for (int j = 0; j < graph.m_vertexNum; ++j)
			{
				m_graph[i][j] = graph.m_graph[i][j];
			}
		}
		
		m_vertexNum = graph.m_vertexNum;
		m_edgeNum = m_edgeNum;
	}

	~DMatrixGraph()
	{
		for (int i = 0; i < m_vertexNum; ++i)
		{
			delete[] m_graph[i];
		}
		delete[] m_graph;
	}

	//添加对称弧---无向图
	void insertSymArc(unsigned int v1, unsigned int v2, const T& value)
	{
		m_graph[v1][v2] = value;
		m_graph[v2][v1] = value;
		++m_edgeNum;
	}

	//添加弧---有向图
	void insertArc(unsigned int v1, unsigned int v2, const T& value)
	{
		m_graph[v1][v2] = value;
		++m_edgeNum;
	}

	//遍历
	void foreach(void (*print)(const T& val))
	{
		for (int i = 0; i < m_vertexNum; ++i)
		{
			cout << endl;
			for (int j = 0; j < m_vertexNum; ++j)
			{
				print(m_graph[i][j]);
			}
		}
		cout << endl;
	}



private:
	T** m_graph;

	size_t m_vertexNum;//顶点的个数

	size_t m_edgeNum;//边的条数

};