//Дан невзвешенный неориентированный граф. Определить, является ли он планарным.
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Edge
{
    int from;
    int to;
};

class ListGraph
{
public:
    ListGraph(int num_of_vert) : adjacencies(num_of_vert) { };

    void AddEdge(int from, int to)
    {
        if(from != to && find(adjacencies[from].begin(), adjacencies[from].end(), to) == adjacencies[from].end()) {
            adjacencies[from].push_back(to);
            adjacencies[to].push_back(from);
        }
    }

    void DeleteEdge(int from, int to)
    {
        auto it1 = find(adjacencies[from].begin(), adjacencies[from].end(), to);
        auto it2 = find(adjacencies[to].begin(), adjacencies[to].end(), from);
        if(it1 != adjacencies[from].end())
        {
            adjacencies[from].erase(it1);
            adjacencies[to].erase(it2);
        }
    }

    int VerticesCount() const
    {
        return static_cast<int>(adjacencies.size());
    }

    void GetNextVertices(int vertex, vector<int> &vertices) const
    {
        vertices.clear();
        for(int i : adjacencies[vertex]) {
            vertices.push_back(i);
        }
    }
private:
    vector<vector<int>> adjacencies;
};

//поиск компонент связности
void find_component(const ListGraph &graph, vector<char> &visited, vector<int> &component, int vert)
{
    visited[vert] = 1;
    component.push_back(vert);
    vector<int> next_verticies;
    graph.GetNextVertices(vert, next_verticies);
    for(int i : next_verticies)
    {
        if(!visited[i]) {
            find_component(graph, visited, component, i);
        }
    }
}

bool cycle_dfs(const ListGraph &graph, vector<char> &visited, vector<int> &cycle,
               int vert, vector<int> &prev, bool is_find = false) //обход в глубину для проверки на ацикличность
{
    visited[vert] = 1;
    vector<int> next_verticies;
    graph.GetNextVertices(vert, next_verticies);
    for(int next : next_verticies)
    {
        if(visited[next] && next != prev[vert]) //нашли цикл
        {
            if(is_find)
            {
                cycle.push_back(next);
                while(vert != next)
                {
                    cycle.push_back(vert);
                    vert = prev[vert];
                }
            }
            return true;
        }
        if(!visited[next])
        {
            prev[next] = vert;
            if(cycle_dfs(graph, visited, cycle, next, prev, is_find)) {
                return true;
            }
        }
    }

    return false;
}

bool is_cycle(const ListGraph &graph, const vector<int> &component) //проверка компоненты на ацикличность
{
    vector<char> visited(graph.VerticesCount());
    vector<int> prev(graph.VerticesCount()), cycle;
    prev[component[0]] = -1;
    return cycle_dfs(graph, visited, cycle, component[0], prev);
}

void find_cycle(const ListGraph &graph, const vector<int> &component, vector<int> &cycle) //поиск цикла в компоненте
{
    vector<char> visited(graph.VerticesCount());
    vector<int> prev(graph.VerticesCount());
    prev[component[0]] = -1;
    cycle_dfs(graph, visited, cycle, component[0], prev, true);
}

void brige_dfs(const ListGraph &graph, vector<char> &visited, vector<int> &time_in, vector<int> &min_time,
               vector<Edge> &briges, int time, int vert, int prev) //обход в глубину для поиска мостов
{
    visited[vert] = 1;
    time_in[vert] = min_time[vert] = time;
    vector<int> next_vertecies;
    graph.GetNextVertices(vert, next_vertecies);
    for(int next : next_vertecies)
    {
        if(next == prev) {
            continue;
        }
        if(visited[next]) {
            min_time[vert] = min(min_time[vert], time_in[next]);
        }
        else
        {
            brige_dfs(graph, visited, time_in, min_time, briges, time + 1, next, vert);
            min_time[vert] = min(min_time[vert], min_time[next]);
            if(min_time[next] > time_in[vert]) {
                briges.push_back({vert, next});
            }
        }
    }
}

void find_briges(const ListGraph &graph, const vector<int> &component, vector<Edge> &briges) //поиск мостов
{
    int sz = graph.VerticesCount();
    vector<int> time_in(sz), min_time(sz);
    vector<char> visited(sz);
    brige_dfs(graph, visited, time_in, min_time, briges, 0, component[0], -1);
}

void find_path(const ListGraph &graph, vector<char> &visited, vector<char> &is_contact,
                 vector<int> &path, vector<int> &segment, int vert, int prev) //поиск цепи в сегменте
{
    if(!is_contact[vert]) {
        visited[vert] = 1;
    }
    vector<int> next_verticies;
    graph.GetNextVertices(vert, next_verticies);
    for(int next : next_verticies)
    {
        if(next != prev && !visited[next] && find(segment.begin(), segment.end(), next) != segment.end())
        {
            if(is_contact[next])
            {
                path.push_back(next);
                break;
            }
            find_path(graph, visited, is_contact, path, segment, next, vert);
            if(!path.empty())
            {
                path.push_back(next);
                break;
            }
        }
    }
}

void find_segment(const ListGraph &graph, vector<char> &visited, vector<char> &is_contact,
                  vector<int> &segment, int vert)
{
    if(find(segment.begin(), segment.end(), vert) == segment.end()) {
        segment.push_back(vert);
    }
    if(is_contact[vert]) {
        return;
    }
    visited[vert] = 1;
    vector<int> next_verticies;
    graph.GetNextVertices(vert, next_verticies);
    for(int next : next_verticies)
    {
        if(!visited[next]) {
            find_segment(graph, visited, is_contact, segment, next);
        }
    }
}

void find_segments(const ListGraph &graph, vector<int> &component, vector<char> &is_contact,
                   vector<vector<int>> &segments) //поиск всех сегментов
{
    vector<char> visited(graph.VerticesCount());
    int num_of_seg = static_cast<int>(segments.size());
    for(int vert : component)
    {
        if(!is_contact[vert] && !visited[vert])
        {
            segments.push_back(vector<int> ());
            ++num_of_seg;
            find_segment(graph, visited, is_contact, segments[num_of_seg - 1], vert);
        }
        else if(is_contact[vert])
        {
            vector<int> next_verticies;
            graph.GetNextVertices(vert, next_verticies);
            for(int next : next_verticies)
            {
                if(is_contact[next] && next > vert) {
                    segments.push_back({vert, next});
                    ++num_of_seg;
                }
            }
        }
    }
}

bool is_comp_planar(const ListGraph &source_graph, const vector<int> &component) //проверка компоненты на планарность
{
    ListGraph graph = source_graph;

    if(!is_cycle(graph, component)) { //если компонента дерево, то она планарна
        return true;
    }

    vector<Edge> briges;
    find_briges(graph, component, briges); //поиск мостов
    for(Edge e : briges) {
        graph.DeleteEdge(e.from, e.to);
    }

    //поиск компонент в графе без мостов
    vector<char> visited(graph.VerticesCount());
    for(int i : component)
    {
        if(visited[i]) {
            continue;
        }

        vector<int> new_component;
        find_component(graph, visited, new_component, i);

        if(!is_cycle(graph, new_component)) { //если компонента дерево, то она планарна
            continue;
        }

        vector<int> cycle;
        find_cycle(graph, new_component, cycle);

        vector<char> is_contact(graph.VerticesCount()); //помечаем контактные вершины
        for(int j : cycle) {
            is_contact[j] = 1;
        }

        for(size_t j = 1; j < cycle.size(); ++j) { //удвляем рёбра, находящиеся в цикле
            graph.DeleteEdge(cycle[j - 1], cycle[j]);
        }
        graph.DeleteEdge(cycle[0], cycle[cycle.size() - 1]);

        vector<vector<int>> segments;
        find_segments(graph, new_component, is_contact, segments);

        vector<vector<int>> faces; //множество граней
        faces.push_back(cycle);
        faces.push_back(cycle);

        while(!segments.empty())
        {
            vector<vector<int>> seg_faces(segments.size());
            for(size_t j = 0; j < segments.size(); ++j) //для каждого сегмента ищем все грани, вмещающие этот сегмент
            {
                vector<int> contact_verts;
                for(int vert : segments[j])
                {
                    if(is_contact[vert]) {
                        contact_verts.push_back(vert);
                    }
                }

                for(size_t k = 0; k < faces.size(); ++k)
                {
                    vector<int> &face = faces[k];
                    bool is_in_face = true;
                    for(int vert : contact_verts)
                    {
                        if(find(face.begin(), face.end(), vert) == face.end())
                        {
                            is_in_face = false;
                            break;
                        }
                    }
                    if(is_in_face) {
                        seg_faces[j].push_back(k);
                    }
                }

                if(seg_faces[j].size() == 0) {
                    return false;
                }
            }

            size_t min_seg = 0;
            for(size_t j = 1; j < seg_faces.size(); ++j) //ищем сегмент с минимальным числом вмещающих граней
            {
                if(seg_faces[j].size() < seg_faces[min_seg].size()) {
                    min_seg = j;
                }
            }
            int split_face = seg_faces[min_seg][0];

            //находим цепь между контактными вершинами в сегменте
            int first_contact = 0;
            while(!is_contact[segments[min_seg][first_contact]]) {
                ++first_contact;
            }
            vector<char> used(graph.VerticesCount());
            vector<int> path;
            find_path(graph, used, is_contact, path, segments[min_seg], segments[min_seg][first_contact], -1);
            path.push_back(segments[min_seg][first_contact]);

            //делим грань на две части
            vector<int> new_face_1, new_face_2;
            auto iter_1 = find(faces[split_face].begin(), faces[split_face].end(), path.front());
            auto iter_2 = find(faces[split_face].begin(), faces[split_face].end(), path.back());
            if(static_cast<int>(distance(iter_1, iter_2)) < 0) {
                swap(iter_1, iter_2);
            }

            new_face_1.insert(new_face_1.begin(), iter_1, iter_2 + 1);
            if(path.front() != new_face_1.back()) {
                reverse(path.begin(), path.end());
            }
            new_face_1.insert(new_face_1.end(), path.begin() + 1, path.end() - 1);
            reverse(path.begin(), path.end());
            new_face_2.insert(new_face_2.begin(), faces[split_face].begin(), iter_1 + 1);
            new_face_2.insert(new_face_2.end(), path.begin() + 1, path.end() - 1);
            new_face_2.insert(new_face_2.end(), iter_2, faces[split_face].end());

            faces.push_back(new_face_1);
            faces.push_back(new_face_2);
            faces.erase(faces.begin() + split_face);
            for(int j : path) {
                is_contact[j] = 1;
            }

            //пересчитываем множество сегментов
            for(size_t k = 1; k < path.size(); ++k) {
                graph.DeleteEdge(path[k - 1], path[k]);
            }
            segments.clear();
            find_segments(graph, new_component, is_contact, segments);
        }
    }

    return true;
}

bool is_planar(const ListGraph &graph) //проверка на планарность
{
    int sz = graph.VerticesCount();
    vector<vector<int> > components;
    vector<char> visited(sz);
    int num_of_comps = 0;
    for(int i = 0; i < sz; ++i) //разбиение на компоненты связности
    {
        if(!visited[i]) {
            components.push_back(vector<int> ());
            ++num_of_comps;
            find_component(graph, visited, components[num_of_comps - 1], i);
        }
    }

    for(int i = 0; i < num_of_comps; ++i) //проверка каждой компоненты на планарность
    {
        if(!is_comp_planar(graph, components[i])) {
            return false;
        }
    }

    return true;
}

int main()
{
    int n, m;
    cin >> n >> m;

    ListGraph graph(n);
    for(int i = 0; i < m; ++i)
    {
        int vert1, vert2;
        cin >> vert1 >> vert2;
        graph.AddEdge(vert1, vert2);
    }

    if(is_planar(graph)) {
        cout << "YES";
    }
    else {
        cout  << "NO";
    }

    return 0;
}
