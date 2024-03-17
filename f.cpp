#include <cstdlib>
#include "BMPGenerator.h"
#include "FileReader.h"

template<typename T>
std::string to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

int main() {
    int numVertices, width, height;
    readInputFromFile("graphs_data.txt", numVertices, width, height);

    std::vector<Vertex> vertices;
    std::vector<Edge> edges;

    srand(time(NULL));

    for (int i = 0; i < numVertices; ++i) {
        vertices.push_back({ rand() % width, rand() % height, to_string(i) });
    }

    readEdgesFromFile("edge_data.txt", edges);

    BMPGenerator bmpGenerator(width, height, vertices, edges);
    bmpGenerator.generate("graph.bmp");

    return 0;
}
