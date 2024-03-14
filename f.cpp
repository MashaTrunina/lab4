#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <sstream>

struct Vertex {
    int x;
    int y;
    std::string label;
};

struct Edge {
    size_t vertex1;
    size_t vertex2;
};

class BMPGenerator {
public:
    BMPGenerator(int width, int height, const std::vector<Vertex>& vertices, const std::vector<Edge>& edges)
        : m_width(width), m_height(height), m_vertices(vertices), m_edges(edges) {}

    void generate(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);

        writeHeader(file);

        writeImageData(file);

        file.close();
    }

private:
    void writeHeader(std::ofstream& file) {
        int imageDataSize = m_width * m_height * 3;


        file.put('B').put('M');
        writeInt(file, 14 + 40 + imageDataSize);
        writeInt(file, 0);
        writeInt(file, 14 + 40);


        writeInt(file, 40);
        writeInt(file, m_width);
        writeInt(file, m_height);
        writeShort(file, 1);
        writeShort(file, 24);
        writeInt(file, 0);
        writeInt(file, imageDataSize);
        writeInt(file, 2835);
        writeInt(file, 2835);
        writeInt(file, 0);
        writeInt(file, 0);
    }

    void writeImageData(std::ofstream& file) {
        std::vector<std::vector<bool>> bitmap(m_height, std::vector<bool>(m_width, false));

        for (const auto& edge : m_edges) {
            drawLine(bitmap, m_vertices[edge.vertex1].x, m_vertices[edge.vertex1].y,
                m_vertices[edge.vertex2].x, m_vertices[edge.vertex2].y);
        }

        for (size_t i = 0; i < m_vertices.size(); ++i) {
            drawCircle(bitmap, m_vertices[i].x, m_vertices[i].y);
        }

        for (int y = m_height - 1; y >= 0; --y) {
            for (int x = 0; x < m_width; ++x) {

                file.put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255))
                    .put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255))
                    .put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255));
            }
        }
    }

    void drawLine(std::vector<std::vector<bool>>& bitmap, int x0, int y0, int x1, int y1) {
        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int sx = x0 < x1 ? 1 : -1;
        int sy = y0 < y1 ? 1 : -1;
        int err = dx - dy;

        while (x0 != x1 || y0 != y1) {
            if (x0 >= 0 && x0 < m_width && y0 >= 0 && y0 < m_height) {
                bitmap[y0][x0] = true;
            }
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y0 += sy;
            }
        }
    }

    void drawCircle(std::vector<std::vector<bool>>& bitmap, int xc, int yc) {
        for (int y = yc - 5; y <= yc + 5; ++y) {
            for (int x = xc - 5; x <= xc + 5; ++x) {
                if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
                    double distance = std::sqrt((x - xc) * (x - xc) + (y - yc) * (y - yc));
                    if (distance <= 5) {
                        bitmap[y][x] = true;
                    }
                }
            }
        }
    }

    void writeInt(std::ofstream& file, int value) {
        file.put(static_cast<char>(value & 0xFF))
            .put(static_cast<char>((value >> 8) & 0xFF))
            .put(static_cast<char>((value >> 16) & 0xFF))
            .put(static_cast<char>((value >> 24) & 0xFF));
    }

    void writeShort(std::ofstream& file, short value) {
        file.put(static_cast<char>(value & 0xFF))
            .put(static_cast<char>((value >> 8) & 0xFF));
    }

private:
    int m_width;
    int m_height;
    std::vector<Vertex> m_vertices;
    std::vector<Edge> m_edges;
};

template<typename T>
std::string to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

void readInputFromFile(const std::string& filename, int& numVertices, int& width, int& height) {
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> numVertices >> width >> height;
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

int main () {
    int numVertices, width, height;
    readInputFromFile("input.txt", numVertices, width, height);

    std::vector<Vertex> vertices;
    std::vector<Edge> edges;

    srand(time(NULL));
    for (int i = 0; i < numVertices; ++i) {
        vertices.push_back({ rand() % width, rand() % height, to_string(i) });
        srand(time(NULL));
        for (int i = 0; i < numVertices; ++i) {
            vertices.push_back({ rand() % width, rand() % height, to_string(i) });
        }
        for (int i = 0; i < numVertices; ++i) {
            for (int j = i + 1; j < numVertices; ++j) {
                if (rand() % 5 == 0) {
                    Edge edge;
                    edge.vertex1 = i;
                    edge.vertex2 = j;
                    edges.push_back(edge);
                }
            }
        }
        BMPGenerator bmpGenerator(width, height, vertices, edges);
        bmpGenerator.generate("graph.bmp");
        return 0;
    }
