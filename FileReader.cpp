#include "FileReader.h"

#include <iostream>
#include <fstream>

void readInputFromFile(const std::string& filename, int& numVertices, int& width, int& height) {
    std::ifstream file(filename); // Открытие файла для чтения
    if (file.is_open()) {
        file >> numVertices >> width >> height; // Чтение данных из файла
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

void readEdgesFromFile(const std::string& filename, std::vector<Edge>& edges) {
    std::ifstream file(filename);
    if (file.is_open()) {
        int vertex1, vertex2;
        while (file >> vertex1 >> vertex2) { // Чтение данных из файла
            Edge edge;
            edge.vertex1 = vertex1; // Заполнение первой вершины ребра
            edge.vertex2 = vertex2; // Заполнение второй вершины ребра
            edges.push_back(edge); // Добавление ребра в вектор
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}
