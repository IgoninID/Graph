#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <limits>
#include <stdexcept>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

/// <summary>
/// Реализация ориентированного графа на основе матрицы смежности
/// </summary>
/// <typeparam name="T">тип вершин</typeparam>
/// <typeparam name="W">тип весов рёбер</typeparam>
template <typename T, typename W>
class DGraph {
private:

    /// <summary>
    /// список вершин
    /// </summary>
    vector<T> vertices;

    /// <summary>
    /// матрица смежности, adj[i][j] = вес ребра, направленного из i в j
    /// </summary>
    vector<vector<W>> adj;

    /// <summary>
    /// отображение вершины в индекс
    /// </summary>
    unordered_map<T, int> indexMap;

    /// <summary>
    /// обозначение "нет ребра"
    /// </summary>
    W INF;

public:

    /// <summary>
    /// Конструктор по умолчанию
    /// </summary>
    /// <param name="InVal">обозначение "нет ребра"</param>
    DGraph(W InVal) : INF(InVal) {}

    /// <summary>
    /// Размер графа
    /// </summary>
    /// <returns>количество вершин в графе</returns>
    int Size() const {
        return static_cast<int>(vertices.size());
    }

    /// Проверяет, существует ли вершина

    /// <summary>
    /// Проверка существования вершины
    /// </summary>
    /// <param name="v">Вершина графа</param>
    /// <returns>True - существует, False - не существует</returns>
    bool HasVert(const T& v) const {
        return indexMap.find(v) != indexMap.end();
    }

    /// Проверяет, существует ли ребро между двумя вершинами

    /// <summary>
    /// Проверка существования ребра между двумя вершинами
    /// </summary>
    /// <param name="from">1 вершина</param>
    /// <param name="to">2 вершина</param>
    /// <returns>True - существует, False - не существует</returns>
    bool HasEdge(const T& from, const T& to) const {
        auto it1 = indexMap.find(from);
        auto it2 = indexMap.find(to);
        if (it1 == indexMap.end() || it2 == indexMap.end())
            return false;
        return adj[it1->second][it2->second] != INF;
    }


    /// <summary>
    /// Вес ребра
    /// </summary>
    /// <param name="from">1 вершина</param>
    /// <param name="to">2 вершина</param>
    /// <exception cref="runtime_error - вершина не существует или ребро не существует">
    /// </exception>
    /// <returns>Вес ребра между двумя вершинами</returns>
    W GetEdge(const T& from, const T& to) const {
        auto it1 = indexMap.find(from);
        auto it2 = indexMap.find(to);
        if (it1 == indexMap.end() || it2 == indexMap.end())
            throw runtime_error("getEdge: вершина не найдена"); // Бросаем исключение ошибки времени выполнения
        W val = adj[it1->second][it2->second];
        if (val == INF)
            throw runtime_error("getEdge: ребро не существует"); // Бросаем исключение ошибки времени выполнения
        return val;
    }


    /// <summary>
    /// Добавление вершины
    /// Сложность: O(V ^ 2)
    /// </summary>
    /// <param name="value">Значение вершины</param>
    /// <exception cref="runtime_error - вершина уже существует">
    /// </exception>
    void AddV(const T& value) {
        if (indexMap.find(value) != indexMap.end()) { // если вершина существует
            throw runtime_error("Вершина уже существует");
        }

        // Добавляем её в список вершин и карту индексов.
        vertices.push_back(value);
        indexMap[value] = vertices.size() - 1;

        // Расширяем матрицу смежности: добавляем новый столбец и строку.

        // Добавляем столбец INF в каждую существующую строку
        for (size_t i = 0; i < adj.size(); ++i) {
            adj[i].push_back(INF);
        }

        // Добавляем новую строку (все INF)
        adj.push_back(vector<W>(vertices.size(), INF));
    }
    
    /// <summary>
    /// Удаление вершины
    /// Сложность: O(V^2)
    /// </summary>
    /// <param name="value">Значение вершины</param>
    /// <exception cref="runtime_error - вершина не существует">
    /// </exception>
    void DeleteV(const T& value) {
        if (indexMap.find(value) == indexMap.end()) { // если вершина не существует
            throw runtime_error("Вершина не найдена");
        }

        int idx = indexMap[value];

        // Удаляем вершину 
        vertices.erase(vertices.begin() + idx);

        // Удаляем соответствующую строку из матрицы смежности
        adj.erase(adj.begin() + idx);

        // Удаляем соответствующий столбец из матрицы смежности
        for (size_t i = 0; i < adj.size(); ++i)
            adj[i].erase(adj[i].begin() + idx);

        // Перестраиваем indexMap
        indexMap.clear();
        for (size_t i = 0; i < vertices.size(); ++i)
            indexMap[vertices[i]] = i;
    }

    /// <summary>
    /// Добавление или редактирование ребра 
    /// Сложность: O(1)
    /// </summary>
    /// <param name="from">1 вершина</param>
    /// <param name="to">2 вершина</param>
    /// <param name="weight">Вес ребра</param>
    /// <exception cref="runtime_error - одна из вершин не существует">
    /// </exception>
    void AddE(const T& from, const T& to, W weight) {

        if (indexMap.find(from) == indexMap.end() || indexMap.find(to) == indexMap.end())
            throw runtime_error("Одна из вершин не найдена");

        int u = indexMap[from];
        int v = indexMap[to];
        adj[u][v] = weight; // создаём или перезаписываем ребро из u в v
    }
    
    /// <summary>
    /// Удаление ребра 
    /// Сложность: O(1)
    /// </summary>
    /// <param name="from">1 веершина</param>
    /// <param name="to">2 вершина</param>
    /// <exception cref="runtime_error - одна из вершин не существует">
    /// </exception>
    void DeleteE(const T& from, const T& to) {

        if (indexMap.find(from) == indexMap.end() || indexMap.find(to) == indexMap.end()) // если одна из вершин не существует
            throw runtime_error("Одна из вершин не найдена");

        int u = indexMap[from];
        int v = indexMap[to];
        adj[u][v] = INF; // перезаписываем ребро из u в v на значение бесконечности
    }

    /// <summary>
    /// Обход в ширину (BFS)
    /// Сложность: O(V ^ 2)
    /// </summary>
    /// <param name="start">Начальная вершина</param>
    /// <returns>Массив с значениями вершин</returns>
    /// <exception cref="runtime_error - начальная вершина не существует">
    /// </exception>
    vector<T> BFS(const T& start) const {
        auto it = indexMap.find(start);
        if (it == indexMap.end()) // если начальная вершина не существует
            throw runtime_error("BFS: начальная вершина не найдена");

        int s = it->second; // индекс стартовой вершины
        int n = Size();

        vector<char> visited(n, 0); // посещенные вершины
        vector<char> in_queue(n, 0); // вершины в очереди
        queue<int> q;
        vector<T> order;

        // Поместить стартовую вершину в очередь.
        q.push(s);
        in_queue[s] = 1;

        while (!q.empty()) {

            // Извлечь вершину, отметить как посещённую.
            int u = q.front();
            q.pop();

            if (visited[u])
                continue;

            visited[u] = 1;
            order.push_back(vertices[u]);

            // Проверяем всех возможных соседей
            for (int v = 0; v < n; ++v) {
                if (adj[u][v] != INF && !visited[v] && !in_queue[v]) {
                    q.push(v);
                    in_queue[v] = 1;
                }
            }
        }
        return order;
    }

    /// <summary>
    /// Обход в глубину (DFS)
    /// Сложность: O(V ^ 2)
    /// </summary>
    /// <param name="start">Начальная вершина</param>
    /// <returns>Массив с значениями вершин</returns>
    /// <exception cref="runtime_error - начальная вершина не существует">
    /// </exception>
    vector<T> DFS(const T& start) const {

        auto it = indexMap.find(start);
        if (it == indexMap.end()) // Если начальная вершина не найдена
            throw runtime_error("DFS: начальная вершина не найдена"); // Бросаем исключение ошибки времени выполнения

        int s = it->second; // индекс стартовой вершины
        int n = Size();

        vector<char> visited(n, 0); // посещенные вершины
        vector<T> order;
        stack<int> st;

        st.push(s);

        while (!st.empty()) {
            int u = st.top();
            st.pop();

            if (visited[u])
                continue;

            visited[u] = 1;
            order.push_back(vertices[u]);

            // Добавляем всех соседей вершины u в стек
            for (int v = 0; v < n; ++v) {
                if (adj[u][v] != INF && !visited[v]) {
                    st.push(v);
                }
            }
        }

        return order;
    }

    /// <summary>
    /// Алгоритм Форда-Беллмана для поиска кратчайшего пути от стартовой вершины ко всем остальным
    /// Сложность:
    /// пустой граф O(V ^ 2)
    /// разреженный граф O(V* E)
    /// плотный граф O(V ^ 3)
    /// </summary>
    /// <param name="start">Начальная вершина</param>
    /// <returns>список длин кратчайших путей от стартовой вершины до всех остальных по порядку</returns>
    /// <exception cref="runtime_error - начальная вершина не существует или обнаруживается отрицательный цикл">
    /// </exception>
    vector<W> BellmanFord(const T& start) const {

        if (indexMap.find(start) == indexMap.end()) // Если вершина не существует
            throw runtime_error("Вершина не найдена");

        int n = vertices.size();
        int s = indexMap.at(start); // индекс стартовой вершины

        // Инициализация: все расстояния = INF, кроме стартовой (0)
        vector<W> dist(n, INF);
        dist[s] = 0;

        // Основной цикл 
        // для каждого ребра u->v обновляем dist[v] = min(dist[v], dist[u] + w)
        for (int k = 0; k < n - 1; ++k) {
            for (int u = 0; u < n; ++u) {
                for (int v = 0; v < n; ++v) {
                    if (adj[u][v] != INF && dist[u] != INF) {
                        if (dist[v] > dist[u] + adj[u][v]) { // если существует более короткий путь в вершину v
                            dist[v] = dist[u] + adj[u][v]; // перезаписываем длину кратчайшего пути в вершину v
                        }
                    }
                }
            }
        }

        //  Проверка на отрицательные циклы 
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                // если после основного цикла остаются более короткие пути
                if (adj[u][v] != INF && dist[u] != INF && dist[v] > dist[u] + adj[u][v]) {
                    throw runtime_error("Обнаружен отрицательный цикл!");
                }
            }
        }

        return dist;
    }

    ///  Печать матрицы смежности 

    /// <summary>
    /// Печать матрицы смежности 
    /// </summary>
    void printM() const {
        cout << "\nМатрица смежности (INF = нет ребра):\n";
        cout << setw(8) << "";
        for (size_t i = 0; i < vertices.size(); ++i)
            cout << setw(8) << vertices[i];
        cout << endl;

        for (size_t i = 0; i < vertices.size(); ++i) {
            cout << setw(8) << vertices[i];
            for (size_t j = 0; j < vertices.size(); ++j) {
                if (adj[i][j] == INF) cout << setw(8) << "INF";
                else cout << setw(8) << adj[i][j];
            }
            cout << endl;
        }
        cout << endl;
    }

    /// <summary>
    /// Возвращает список соседей вершины
    /// Сложность: O(V)
    /// </summary>
    /// <param name="vertex">Вершина</param>
    /// <returns>Массив вершин, с которыми у данной вершины есть связь в любом направлении</returns>
    /// <exception cref="runtime_error - вершина не существует">
    /// </exception>
    vector<T> GetNeighbors(const T& vertex) const {

        auto it = indexMap.find(vertex);
        if (it == indexMap.end()) // если вершины не существует
            throw runtime_error("GetNeighbors: вершина не найдена");

        int idx = it->second;
        vector<T> neighbors;

        for (size_t j = 0; j < vertices.size(); ++j) {

            // исходящие рёбра
            if (adj[idx][j] != INF) {
                neighbors.push_back(vertices[j]);
            }

            // входящие рёбра
            else if (adj[j][idx] != INF) {
                neighbors.push_back(vertices[j]);
            }
        }
        return neighbors;
    }

    /// <summary>
    /// Получение веса ребра между двумя вершинами
    /// Сложность: O(1)
    /// </summary>
    /// <param name="from">1 вершина</param>
    /// <param name="to">2 вершина</param>
    /// <returns>вес ребра между двумя вершинами</returns>
    /// <exception cref="runtime_error - одна из вершин не существует или ребро не существует">
    /// </exception>
    W GetWeight(const T& from, const T& to) const {
        auto it1 = indexMap.find(from);
        auto it2 = indexMap.find(to);
        if (it1 == indexMap.end() || it2 == indexMap.end()) // если хотя бы одной из вершин не существует
            throw runtime_error("GetWeight: вершина не найдена");

        int u = it1->second;
        int v = it2->second;

        if (adj[u][v] == INF) // если ребра не существует
            throw runtime_error("GetWeight: ребро отсутствует");

        return adj[u][v];

    }
    
    /// <summary>
    /// Сохранение графа в файл формата GraphML
    /// </summary>
    /// <param name="filename">Имя файла для сохранения</param>
    /// <exception cref="runtime_error - невозможно открыть файл">
    /// </exception>
    void saveGraphML(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) // если файл не открывается
            throw runtime_error("Не удалось открыть файл для записи");

        file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        file << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\"\n";
        file << "         xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
        file << "         xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns\n";
        file << "         http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n";

        file << "  <graph id=\"G\" edgedefault=\"directed\">\n";

        // Узлы
        for (size_t i = 0; i < vertices.size(); ++i) {
            file << "    <node id=\"n" << i << "\"";
            file << " name = " << "\"" << vertices[i] << "\"";
            file << "</node>\n";
        }

        // Рёбра
        for (size_t u = 0; u < vertices.size(); ++u) {
            for (size_t v = 0; v < vertices.size(); ++v) {
                if (adj[u][v] != INF) {
                    file << "    <edge isDirected=\"true\" source=\"n" << u
                        << "\" target=\"n" << v << "\"";
                    file << " weight = " << "\"" << adj[u][v] << "\"";
                    file << "</edge>\n";
                }
            }
        }
        file << "  </graph>\n";
        file << "</graphml>\n";
        file.close();
    }

    /// <summary>
    /// Загрузка графа из файла формата GraphML
    /// </summary>
    /// <param name="filename">Имя файла для загрузки</param>
    /// <exception cref="runtime_error - невозможно открыть файл или некорректная запись узлов/рёбер (отсутствует weight у ребра или name у узла)">
    /// </exception>
    void loadGraphML(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) // если файл не открывается
            throw runtime_error("Не удалось открыть файл для чтения"); // Бросаем исключение ошибки времени выполнения

        vertices.clear();
        adj.clear();
        indexMap.clear();

        string line;
        unordered_map<string, int> xmlIdToIndex;

        while (getline(file, line)) {

            //  Узлы 
            if (line.find("<node") != string::npos) {

                // id="n0"
                size_t idPos = line.find("id=\"");
                if (idPos == string::npos)
                    continue;
                idPos += 4; // теперь указывает на первый символ значения id
                size_t idEnd = line.find("\"", idPos);
                string xmlId = line.substr(idPos, idEnd - idPos);

                // name="A"
                size_t namePos = line.find("name=\"");
                if (namePos == string::npos)
                    throw runtime_error("Ошибка: node без name");

                namePos += 6;
                size_t nameEnd = line.find("\"", namePos);
                string value = line.substr(namePos, nameEnd - namePos);

                // Добавляем вершину
                AddV(value);
                xmlIdToIndex[xmlId] = vertices.size() - 1;

                continue;
            }

            //  Рёбра 
            if (line.find("<edge") != string::npos) {

                // source="n0"
                size_t sPos = line.find("source=\"");
                if (sPos == string::npos)
                    continue;
                sPos += 8;
                size_t sEnd = line.find("\"", sPos);
                string xmlSource = line.substr(sPos, sEnd - sPos);

                // target="n1"
                size_t tPos = line.find("target=\"");
                if (tPos == string::npos)
                    continue;
                tPos += 8;
                size_t tEnd = line.find("\"", tPos);
                string xmlTarget = line.substr(tPos, tEnd - tPos);

                // weight="5.5"
                size_t wPos = line.find("weight=\"");
                if (wPos == string::npos)
                    throw runtime_error("Ошибка: edge без weight");

                wPos += 8;
                size_t wEnd = line.find("\"", wPos);
                string wStr = line.substr(wPos, wEnd - wPos);

                // Конвертация веса
                W weight{};
                stringstream ss(wStr);
                ss >> weight;

                // Индексы
                int u = xmlIdToIndex.at(xmlSource);
                int v = xmlIdToIndex.at(xmlTarget);

                AddE(vertices[u], vertices[v], weight);
            }
        }
        file.close();
    }
};

/// <summary>
/// Тестирование операций с вершинами (AddV, DeleteV, HasVert, size)
/// </summary>
void test_vertex_operations();

/// <summary>
/// Тестирование операций с рёбрами (AddE, DeleteE, HasEdge, GetEdge)
/// </summary>
void test_edge_operations();

/// <summary>
/// Тест BFS
/// </summary>
void test_bfs();

/// <summary>
/// Тест DFS
/// </summary>
void test_dfs();

/// <summary>
/// Тест BellmanFord
/// </summary>
void test_bellman_ford();

/// <summary>
/// Тест GetNeighbors
/// </summary>
void test_get_neighbors();

/// <summary>
/// Тест GetWeight
/// </summary>
void test_get_weight();

/// <summary>
/// Тест saveGraphML
/// </summary>
void test_saveGraphML();

/// <summary>
/// Тест loadGraphML
/// </summary>
void test_loadGraphML();