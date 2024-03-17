#include "BMPGenerator.h"

BMPGenerator::BMPGenerator(int width, int height, const std::vector<Vertex>& vertices, const std::vector<Edge>& edges)
    : m_width(width), m_height(height), m_vertices(vertices), m_edges(edges) {}

void BMPGenerator::generate(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    writeHeader(file);
    writeImageData(file);
    file.close();
}

void BMPGenerator::writeHeader(std::ofstream& file) {
     int imageDataSize = m_width * m_height * 3; // Вычисление размера данных изображения

    // Запись заголовка BMP
    file.put('B').put('M'); // Числа для определения формата файла
    writeInt(file, 14 + 40 + imageDataSize); // Размер файла
    writeInt(file, 0); // Зарезервированное поле
    writeInt(file, 14 + 40); // Смещение до начала данных изображения

    // Запись информации о заголовке изображения
    writeInt(file, 40); // Размер информационного заголовка
    writeInt(file, m_width); // Ширина изображения
    writeInt(file, m_height); // Высота изображения
    writeShort(file, 1); // Число плоскостей
    writeShort(file, 24); // Глубина цвета (24 бита на пиксель)
    writeInt(file, 0); // Тип сжатия 
    writeInt(file, imageDataSize); // Размер данных изображения
    writeInt(file, 2835); // Горизонтальное разрешение (пикселей на метр)
    writeInt(file, 2835); // Вертикальное разрешение (пикселей на метр)
    writeInt(file, 0); // Количество используемых цветов (0 - все цвета)
    writeInt(file, 0); // Количество основных цветов (0 - все цвета)
}

void BMPGenerator::writeImageData(std::ofstream& file) {
    std::vector<std::vector<bool>> bitmap(m_height, std::vector<bool>(m_width, false)); // Создание битовой карты

    // Отрисовка ребер графа на изображении
    for (const auto& edge : m_edges) {
        drawLine(bitmap, m_vertices[edge.vertex1].x, m_vertices[edge.vertex1].y,
            m_vertices[edge.vertex2].x, m_vertices[edge.vertex2].y);
    }

    // Отрисовка вершин графа на изображении
    for (size_t i = 0; i < m_vertices.size(); ++i) {
        drawCircle(bitmap, m_vertices[i].x, m_vertices[i].y); // Отрисовка вершины графа

        int labelX = m_vertices[i].x + 7;
        int labelY = m_vertices[i].y + 7;

        drawText(bitmap, m_vertices[i].label, labelX, labelY); // Отрисовка метки вершины
    }

    // Запись данных изображения в файл
    for (int y = m_height - 1; y >= 0; --y) {
        for (int x = 0; x < m_width; ++x) {
            file.put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255)) // Запись цвета пикселя
                .put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255)) // Запись цвета пикселя
                .put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255)); // Запись цвета пикселя
        }
    }
}

void BMPGenerator::drawText(std::vector<std::vector<bool>>& bitmap, const std::string& text, int x, int y) {
    int labelWidth = text.length() * 10;
    int labelHeight = 20;
    int labelX = x - labelWidth / 10;
    int labelY = y - labelHeight / 10;

    // Проверка на выход за границы изображения
    if (labelX < 0 || labelX + labelWidth >= m_width || labelY < 0 || labelY + labelHeight >= m_height) {
        return;
    }
    // Отрисовка рамки вокруг текста
    for (int i = 0; i < labelWidth; ++i) {
        bitmap[labelY][labelX + i] = true; // Верхняя граница
        bitmap[labelY + labelHeight][labelX + i] = true; // Нижняя граница
    }
    for (int i = 0; i < labelHeight; ++i) {
        bitmap[labelY + i][labelX] = true; // Левая граница
        bitmap[labelY + i][labelX + labelWidth] = true; // Правая граница
    }
    // Отрисовка символов текста
    for (size_t i = 0; i < text.length(); ++i) {
        drawCharacter(bitmap, text[i], labelX + i * 6, labelY + labelHeight / 2 - 3); // Отрисовка отдельного символа
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

    // Проверка на то, что символ является цифрой
    if (character >= '0' && character <= '9') {
        int index = character - '0'; // Индекс символа
        // Отрисовка символа по матрице
        for (size_t i = 0; i < charTemplates[index].size(); ++i) {
            for (size_t j = 0; j < charTemplates[index][i].size(); ++j) {
                if (charTemplates[index][i][j]) { // Если в матрице на данной позиции стоит единица
                    if (x + j >= 0 && x + j < bitmap[0].size() && y + i >= 0 && y + i < bitmap.size()) {
                        bitmap[y + i][x + j] = true; // Отрисовка пикселя
                    }
                }
            }
        }
    }
}

void BMPGenerator::drawLine(std::vector<std::vector<bool>>& bitmap, int x0, int y0, int x1, int y1) {
    int dx = std::abs(x1 - x0); // Приращение по X
    int dy = std::abs(y1 - y0); // Приращение по Y
    int sx = x0 < x1 ? 1 : -1; // Направление по X
    int sy = y0 < y1 ? 1 : -1; // Направление по Y
    int err = dx - dy; // Ошибка

    // Отрисовка линии методом Брезенхэма
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
    // Отрисовка круга с радиусом 5 пикселей
    for (int y = yc - 5; y <= yc + 5; ++y) {
        for (int x = xc - 5; x <= xc + 5; ++x) {
            if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
                double distance = std::sqrt((x - xc) * (x - xc) + (y - yc) * (y - yc)); // Вычисление расстояния от центра круга
                if (distance <= 5) { // Если расстояние меньше или равно радиусу круга
                    bitmap[y][x] = true;
                }
            }
        }
    }
}

void BMPGenerator::writeInt(std::ofstream& file, int value) {
    file.put(static_cast<char>(value & 0xFF)) // Младший байт
        .put(static_cast<char>((value >> 8) & 0xFF)) // Второй байт
        .put(static_cast<char>((value >> 16) & 0xFF)) // Третий байт
        .put(static_cast<char>((value >> 24) & 0xFF)); // Старший байт
}

void BMPGenerator::writeShort(std::ofstream& file, short value) {
    file.put(static_cast<char>(value & 0xFF)) // Младший байт
        .put(static_cast<char>((value >> 8) & 0xFF)); // Старший байт
}
void BMPGenerator::writeShort(std::ofstream& file, short value) {
    file.put(static_cast<char>(value & 0xFF))
        .put(static_cast<char>((value >> 8) & 0xFF));
}
