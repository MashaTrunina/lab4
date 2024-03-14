#include "BMPGenerator.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <sstream>

BMPGenerator::BMPGenerator(int width, int height, const std::vector<Vertex>& vertices, const std::vector<Edge>& edges)
    : m_width(width), m_height(height), m_vertices(vertices), m_edges(edges) {}

void BMPGenerator::generate(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    writeHeader(file);
    writeImageData(file);

    file.close();
}

void BMPGenerator::writeHeader(std::ofstream& file) {
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

void BMPGenerator::writeImageData(std::ofstream& file) {
    std::vector<std::vector<bool>> bitmap(m_height, std::vector<bool>(m_width, false));


    for (const auto& edge : m_edges) {
        drawLine(bitmap, m_vertices[edge.vertex1].x, m_vertices[edge.vertex1].y,
            m_vertices[edge.vertex2].x, m_vertices[edge.vertex2].y);
    }

    for (size_t i = 0; i < m_vertices.size(); ++i) {
        drawCircle(bitmap, m_vertices[i].x, m_vertices[i].y);

        int labelX = m_vertices[i].x + 7;
        int labelY = m_vertices[i].y + 7;

        drawText(bitmap, m_vertices[i].label, labelX, labelY);
    }

    for (int y = m_height - 1; y >= 0; --y) {
        for (int x = 0; x < m_width; ++x) {
            file.put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255))
                .put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255))
                .put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255));
        }
    }
}

void BMPGenerator::drawText(std::vector<std::vector<bool>>& bitmap, const std::string& text, int x, int y) {
    int textWidth = text.length() * 6;
    int textHeight = 8;

    drawCircle(bitmap, x + textWidth / 2, y + textHeight / 2);

    for (size_t i = 0; i < text.length(); ++i) {
        drawCharacter(bitmap, text[i], x + i * 6, y + 2);
    }
}

void BMPGenerator::drawCharacter(std::vector<std::vector<bool>>& bitmap, char character, int x, int y) {
    const std::vector<std::vector<std::vector<bool>>> charTemplates = {
            {
                {0, 1, 1, 1, 0},
                {1, 0, 0, 0, 1},
                {1, 0, 0, 0, 1},
                {1, 0, 0, 0, 1},
                {0, 1, 1, 1, 0}
            },
            {
                {0, 0, 1, 0, 0},
                {0, 1, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 1, 1, 0}
            },
            {
                {1, 1, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 0, 0, 0},
                {1, 0, 0, 0, 0},
                {1, 1, 1, 1, 0}
            },
            {
                {0, 1, 1, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 1, 1, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 1, 1, 1, 0}
            },
            {
                {0, 1, 0, 1, 0},
                {0, 1, 0, 1, 0},
                {0, 1, 1, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 1, 0}
            },
            {
                {0, 0, 1, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 1, 1, 0}
            },
            {
                {0, 1, 1, 1, 0},
                {0, 1, 0, 0, 0},
                {0, 1, 1, 1, 0},
                {0, 1, 0, 1, 0},
                {0, 1, 1, 1, 0}
            },
            {
                {0, 1, 1, 1, 0},
                {0, 1, 0, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 1, 0}
            },
            {
                {0, 1, 1, 1, 0},
                {0, 1, 0, 1, 0},
                {0, 1, 1, 1, 0},
                {0, 1, 0, 1, 0},
                {0, 1, 1, 1, 0}
            },
            {
                {0, 1, 1, 1, 0},
                {0, 1, 0, 1, 0},
                {0, 1, 1, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 1, 1, 1, 0}
            },
    };

    if (character >= '0' && character <= '9') {
        int index = character - '0';
        for (size_t i = 0; i < charTemplates[index].size(); ++i) {
            for (size_t j = 0; j < charTemplates[index][i].size(); ++j) {
                if (charTemplates[index][i][j]) {
                    if (x + j >= 0 && x + j < bitmap[0].size() && y + i >= 0 && y + i < bitmap.size()) {
                        if (bitmap[y + i][x + j]) {
                            bitmap[y + i][x + j] = false;
                        }
                    }
                }
            }
        }
    }
}

void BMPGenerator::drawLine(std::vector<std::vector<bool>>& bitmap, int x0, int y0, int x1, int y1) {
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

void BMPGenerator::drawCircle(std::vector<std::vector<bool>>& bitmap, int xc, int yc) {
    int radius = 8;

    for (int y = yc - radius; y <= yc + radius; ++y) {
        for (int x = xc - radius; x <= xc + radius; ++x) {
            if (x >= 0 && x < bitmap[0].size() && y >= 0 && y < bitmap.size()) {
                double distance = std::sqrt((x - xc) * (x - xc) + (y - yc) * (y - yc));
                if (distance <= radius) {
                    bitmap[y][x] = true;
                }
            }
        }
    }
}

void BMPGenerator::writeInt(std::ofstream& file, int value) {
    file.put(static_cast<char>(value & 0xFF))
        .put(static_cast<char>((value >> 8) & 0xFF))
        .put(static_cast<char>((value >> 16) & 0xFF))
        .put(static_cast<char>((value >> 24) & 0xFF));
}

void BMPGenerator::writeShort(std::ofstream& file, short value) {
    file.put(static_cast<char>(value & 0xFF))
        .put(static_cast<char>((value >> 8) & 0xFF));
}

void BMPGenerator::readInputFromFile(const std::string& filename, int& numVertices, int& width, int& height, std::vector<Edge>& edges) {
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
