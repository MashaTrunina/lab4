#pragma once
#include <vector>
#include <string>

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
    BMPGenerator(int width, int height, const std::vector<Vertex>& vertices, const std::vector<Edge>& edges);

    void generate(const std::string& filename);

private:
    void writeHeader(std::ofstream& file);
    void writeImageData(std::ofstream& file);
    void drawText(std::vector<std::vector<bool>>& bitmap, const std::string& text, int x, int y);
    void drawCharacter(std::vector<std::vector<bool>>& bitmap, char character, int x, int y);
    void drawLine(std::vector<std::vector<bool>>& bitmap, int x0, int y0, int x1, int y1);
    void drawCircle(std::vector<std::vector<bool>>& bitmap, int xc, int yc);
    void writeInt(std::ofstream& file, int value);
    void writeShort(std::ofstream& file, short value);
private:
    int m_width;
    int m_height;
    std::vector<Vertex> m_vertices;
    std::vector<Edge> m_edges;
    void readInputFromFile(const std::string& filename, int& numVertices, int& width, int& height, std::vector<Edge>& edges);
};
