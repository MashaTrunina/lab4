#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <set>
#include "Vertex.h"
#include "Edge.h"

class BMPGenerator {
public:
    BMPGenerator(int width, int height, const std::vector<Vertex>& vertices, const std::vector<Edge>& edges);
    bool isGraphPlanar() const;
    void generate(const std::string& filename);
    bool hasEdgeBetween(size_t v1, size_t v2) const;
    bool containsK5() const;
    bool containsK33() const;
    void modifyForK5();
    void modifyForK33();

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
    std::vector<size_t> findK5Vertices() const;
    std::vector<std::pair<size_t, size_t>> findK33Edges() const;
};
