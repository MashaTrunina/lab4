#include <iostream>
#include "BMPGenerator.h"
#include "FileReader.h"


// Шаблонная функция для преобразования значения в строку
template<typename T>
std::string to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

int main() {
    int numVertices, width, height;

    readInputFromFile("graphs_data.txt", numVertices, width, height);

    std::vector<Vertex> vertices; // Вектор вершин графа
    std::vector<Edge> edges; // Вектор ребер графа

    srand(time(NULL)); // Инициализация генератора случайных чисел 

    // Генерация случайных координат для вершин графа и их меток
    for (int i = 0; i < numVertices; ++i) {
        vertices.push_back({ rand() % width, rand() % height, to_string(i) });
    }

    readEdgesFromFile("test1.txt", edges);

    int numEdges = edges.size(); // Количество ребер графа
    int numFaces = 2 - numVertices + numEdges; // Количество граней в графе

    if (numFaces == 2) {
        std::cout << "Graph is planar." << std::endl;

        // Проверка наличия подграфов K5 и K33
        BMPGenerator bmpGenerator(width, height, vertices, edges);
        if (!bmpGenerator.containsK33()) {
            bmpGenerator.modifyForK5(); // Изменение графа для удаления K5
            std::cout << "Graph contains K33. It is not planar." << std::endl;
            //bmpGenerator.generate("graph.bmp");
        }
        else if (!bmpGenerator.containsK5()) {
            bmpGenerator.modifyForK5(); // Изменение графа для удаления K33
            std::cout << "Graph contains K5. It is not planar." << std::endl;
           // bmpGenerator.generate("graph.bmp");
        }
        else {
            std::cout << "And without k5 and k33." << std::endl;
            bmpGenerator.generate("graph.bmp"); // Генерация изображения графа
        }

        return 0;
    }
}
