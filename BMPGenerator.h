#pragma once
#include <vector>
#include <string>

struct Vertex {
    int x;
    int y;
    std::string label;
    int tableNumber;
};

struct Edge {
    size_t vertex1;
    size_t vertex2;
};

class BMPGenerator {
public:
    BMPGenerator(int width, int height, const std::vector<Vertex>& vertices, const std::vector<Edge>& edges);
    void drawCharacter(std::vector<std::vector<bool>>& bitmap, char character, int x, int y);
    void generate(const std::string& filename);
    void drawCircle(std::vector<std::vector<bool>>& bitmap, int xc, int yc);
    void drawText(std::vector<std::vector<bool>>& bitmap, int tableNumber, int x, int y);
private:
    void writeHeader(std::ofstream& file);
    void writeImageData(std::ofstream& file);
    void drawLine(std::vector<std::vector<bool>>& bitmap, int x0, int y0, int x1, int y1);
    void writeInt(std::ofstream& file, int value);
    void writeShort(std::ofstream& file, short value);
    
private:
    int m_width;
    int m_height;
    std::vector<Vertex> m_vertices;
    std::vector<Edge> m_edges;
    std::vector<std::vector<std::vector<bool>>> alpha;
};
