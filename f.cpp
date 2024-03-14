#include "BMPGenerator.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <sstream>

template<typename T>
std::string to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

void readInputFromFile(const std::string& filename, int& numVertices, int& width, int& height, std::vector<Edge>& edges) {
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> numVertices >> width >> height;

        int vertex1, vertex2;
        while (file >> vertex1 >> vertex2) {
            edges.push_back({ static_cast<size_t>(vertex1), static_cast<size_t>(vertex2) });
        }

        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

int main() {
    int numVertices, width, height;
    std::vector<Edge> edges;
    readInputFromFile("input.txt", numVertices, width, height, edges);

    std::vector<Vertex> vertices;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = 0; i < numVertices; ++i) {
        vertices.push_back({ std::rand() % width, std::rand() % height, to_string(i), i }); 
    }

    BMPGenerator bmpGenerator(width, height, vertices, edges);
    bmpGenerator.generate("graph.bmp");

    return 0;
}
