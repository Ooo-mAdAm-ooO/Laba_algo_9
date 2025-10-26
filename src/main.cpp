#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <random>
#include <stack>
#include <string>
#include <iomanip>

using namespace std;
using namespace std::chrono;

class Graph
{
public:
	Graph() : vertexCount(0), edgeCount(0)
	{
	}
	void genAdjMatrix(int vCount, double probability = 0.4)
	{
		matrix.assign(vCount, vector<int>(vCount, 0));
		adjList.assign(vCount, {});
		visited.assign(vCount, false);

		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<> dist(0.0, 1.0);
		vertexCount = vCount;
		// Генерация ребер
		for (int i = 0; i < vertexCount; i++)
		{
			for (int j = i + 1; j < vertexCount; j++)
			{
				if (dist(gen) < probability)
				{
					matrix[i][j] = matrix[j][i] = 1;
					++edgeCount;
				}
			}
		}
		genAdjList();
	}
	void DFS(int start, bool show)
	{
		resetVisited();
		stack<int> st;
		st.push(start);

		vector<int> dist(vertexCount, -1);
		dist[start] = 0;

		while (!st.empty())
		{
			int v = st.top();
			st.pop();

			if (!visited[v])
			{
				visited[v] = true;
			}

			for (int i = vertexCount - 1; i >= 0; i--)
			{
				if (matrix[v][i] == 1 && !visited[i])
				{
					if (dist[i] == -1)
						dist[i] = dist[v] + 1;
					st.push(i);
				}
			}
		}
		cout << "DFS (матрица смежности):\n";
		if (show)
			showDist(dist);
	}
	void BFS(int start, bool show)
	{
		vector<int> dist(vertexCount, -1);
		queue<int> q;
		dist[start] = 0;
		q.push(start);

		while (!q.empty())
		{
			int v = q.front();
			q.pop();

			for (int i = 0; i < vertexCount; i++)
			{
				if (matrix[v][i] == 1 && dist[i] == -1)
				{
					dist[i] = dist[v] + 1;
					q.push(i);
				}
			}
		}
		cout << "BFS (матрица смежности):\n";
		if(show)
			showDist(dist);
	}
	void DFS_adj(int start, bool show)
	{
		resetVisited();
		stack<int> st;
		st.push(start);

		vector<int> dist(vertexCount, -1);
		dist[start] = 0;

		while (!st.empty())
		{
			int v = st.top();
			st.pop();

			if (!visited[v])
			{
				visited[v] = true;
			}

			for (int nb : adjList[v])
			{
				if (!visited[nb])
				{
					if (dist[nb] == -1)
						dist[nb] = dist[v] + 1;
					st.push(nb);
				}
			}
		}

		cout << "DFS (список смежности):\n";
		if (show)
			showDist(dist);
	}
	void BFS_adj(int start, bool show)
	{
		vector<int> dist(vertexCount, -1);
		queue<int> q;
		dist[start] = 0;
		q.push(start);

		while (!q.empty())
		{
			int v = q.front();
			q.pop();

			for (int nb : adjList[v])
			{
				if (dist[nb] == -1)
				{
					dist[nb] = dist[v] + 1;
					q.push(nb);
				}
			}
		}
		
		cout << "BFS (список смежности):\n";
		if (show)
			showDist(dist);
	}

	void print() const {
		cout << "\n=== Матрица смежности ===\n   ";
		for (int j = 0; j < vertexCount; j++) cout << setw(2) << j << " ";
		cout << "\n";
		for (int i = 0; i < vertexCount; i++) {
			cout << setw(2) << i << " ";
			for (int j = 0; j < vertexCount; j++)
				cout << setw(2) << matrix[i][j] << " ";
			cout << "\n";
		}

		cout << "\n=== Список смежности ===\n";
		for (int i = 0; i < vertexCount; i++) {
			cout << setw(2) << i << ": ";
			if (adjList[i].empty()) cout << "пусто";
			else for (int j : adjList[i]) cout << j << "(" << matrix[i][j] << ") ";
			cout << "\n";
		}
		cout << "Вершин: " << vertexCount << ", Рёбер: " << edgeCount << "\n\n";
	}
private:
	void resetVisited()
	{
		fill(visited.begin(), visited.end(), false);
	}
	void genAdjList()
	{
		for (int i = 0; i < vertexCount; i++)
		{
			adjList[i].clear();
			for (int j = 0; j < vertexCount; j++)
			{
				if (matrix[i][j])
					adjList[i].push_back(j);
			}
		}
	}
	void showDist(vector<int>& dist)
	{
		for (int i = 0; i < vertexCount; i++)
			cout << "DIST[" << i << "] = " << (dist[i] == -1 ? "не имеет связи" : std::to_string(dist[i])) << "\n";
	}
	vector<vector<int>> matrix;
	vector<vector<int>>	adjList;
	vector<bool> visited;
	int vertexCount;
	int edgeCount;
};

// Обёртка для метода класса
void TimeToTime(void (Graph::* func)(int, bool), Graph& obj, int param, bool show)
{
	auto start = high_resolution_clock::now();
	(obj.*func)(param, show); // вызываем метод
	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<milliseconds>(stop - start);
	cout << "Время выполнения метода: " << duration.count() << " миллисекунд\n";
}

void testTime()
{
	std::vector<int> capacity{100, 200, 400, 800, 1600};

	for (int i = 0; i < capacity.size(); i++)
	{
		cout << setfill('-') << setw(10) << '\n';
		cout << "Колвичество элементов: " << capacity[i] << "\n\n";
		Graph gp;
		gp.genAdjMatrix(capacity[i]);
		TimeToTime(&Graph::DFS, gp, 0, false);
		cout << "\n";
		TimeToTime(&Graph::BFS, gp, 0, false);
		cout << "\n";
		TimeToTime(&Graph::DFS_adj, gp, 0, false);
		cout << "\n";
		TimeToTime(&Graph::BFS_adj, gp, 0, false);
		cout << "\n";
	}
}

int main()
{
	setlocale(LC_ALL, "RUSSIAN");

	Graph gp;

	int en_Vt = 0;
	float en_Proab = 0.0f;

	cout << "Ведите количество вершин" << "\n";
	cin >> en_Vt;
	cout << "Введите вероятность связи (0.1 - 0.9" << "\n";
	cin >> en_Proab;

	gp.genAdjMatrix(en_Vt);
	gp.print();

	TimeToTime(&Graph::DFS, gp, 0, true);
	cout << "\n";
	TimeToTime(&Graph::BFS, gp, 0, true);
	cout << "\n";
	TimeToTime(&Graph::DFS_adj, gp, 0, true);
	cout << "\n";
	TimeToTime(&Graph::BFS_adj, gp, 0, true);
	cout << "\n";



	return 0;
}
