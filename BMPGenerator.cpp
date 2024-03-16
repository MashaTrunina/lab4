#include "BMPGenerator.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <algorithm>

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

    // Apply Fruchterman-Reingold layout algorithm
    applyFruchtermanReingoldLayout(bitmap);

    // Draw lines between vertices
    for (const auto& edge : m_edges) {
        drawLine(bitmap, m_vertices[edge.vertex1].x, m_vertices[edge.vertex1].y,
            m_vertices[edge.vertex2].x, m_vertices[edge.vertex2].y);
    }

    // Draw vertices
    for (size_t i = 0; i < m_vertices.size(); ++i) {
        drawCircle(bitmap, m_vertices[i].x, m_vertices[i].y);

        std::string vertexNumberString = std::to_string(i);

        int offsetX = 0;
        int offsetY = 0;

        if (vertexNumberString.size() == 1) {
            offsetX = 4;
            offsetY = 4;
        }
        else if (vertexNumberString.size() == 2) {
            offsetX = 4;
            offsetY = 4;
        }
        else if (vertexNumberString.size() == 3) {
            offsetX = 4;
            offsetY = 4;
        }

        for (size_t j = 0; j < vertexNumberString.size(); ++j) {
            drawCharacter(bitmap, vertexNumberString[j], m_vertices[i].x - offsetX, m_vertices[i].y - offsetY);
            offsetX -= 3;
        }
    }

    // Write bitmap data to file
    for (int y = m_height - 1; y >= 0; --y) {
        for (int x = 0; x < m_width; ++x) {
            file.put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255))
                .put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255))
                .put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255));
        }
    }
}

void BMPGenerator::applyFruchtermanReingoldLayout(std::vector<std::vector<bool>>& bitmap) {
    // Constants for Fruchterman-Reingold algorithm
    const double k = sqrt(static_cast<double>(m_width * m_height) / m_vertices.size()); // Ideal edge length
    const double initialTemperature = m_width / 10.0; // Initial "temperature" for the simulation
    const double coolingFactor = 0.99; // Cooling factor to reduce temperature over iterations
    const int maxIterations = 1000; // Maximum number of iterations

    // Initialize random positions for vertices
    std::vector<std::pair<double, double>> positions(m_vertices.size());
    for (size_t i = 0; i < m_vertices.size(); ++i) {
        positions[i].first = static_cast<double>(rand() % m_width);
        positions[i].second = static_cast<double>(rand() % m_height);
    }

    double temperature = initialTemperature;

    // Perform Fruchterman-Reingold algorithm iterations
    for (int iter = 0; iter < maxIterations; ++iter) {
        // Calculate repulsive forces between vertices
        std::vector<std::pair<double, double>> forces(m_vertices.size(), { 0.0, 0.0 });
        for (size_t i = 0; i < m_vertices.size(); ++i) {
            for (size_t j = 0; j < m_vertices.size(); ++j) {
                if (i != j) {
                    double dx = positions[i].first - positions[j].first;
                    double dy = positions[i].second - positions[j].second;
                    double distanceSquared = dx * dx + dy * dy;
                    if (distanceSquared > 0) {
                        double force = k * k / distanceSquared;
                        forces[i].first += force * dx / sqrt(distanceSquared);
                        forces[i].second += force * dy / sqrt(distanceSquared);
                    }
                }
            }
        }

        // Update vertex positions
        for (size_t i = 0; i < m_vertices.size(); ++i) {
            double displacement = sqrt(forces[i].first * forces[i].first + forces[i].second * forces[i].second);
            double dx = forces[i].first / displacement * std::min(displacement, temperature);
            double dy = forces[i].second / displacement * std::min(displacement, temperature);
            positions[i].first += dx;
            positions[i].second += dy;

            // Keep vertices within image bounds
            positions[i].first = std::max(0.0, std::min(static_cast<double>(m_width - 1), positions[i].first));
            positions[i].second = std::max(0.0, std::min(static_cast<double>(m_height - 1), positions[i].second));
        }

        // Cool down temperature
        temperature *= coolingFactor;
    }

    // Update vertex positions after layout calculation
    for (size_t i = 0; i < m_vertices.size(); ++i) {
        m_vertices[i].x = static_cast<int>(positions[i].first);
        m_vertices[i].y = static_cast<int>(positions[i].second);
    }
}

void BMPGenerator::drawText(std::vector<std::vector<bool>>& bitmap, int tableNumber, int x, int y) {
    std::string text = std::to_string(tableNumber);

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
                        bitmap[y + i][x + j] = false;
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
