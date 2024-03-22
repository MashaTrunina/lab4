#include "BMPGenerator.h"

//Конструктор класса BMPGenerator, который инициализирует объект генератора изображения BMP с заданными шириной и высотой, а также векторами вершин и рёбер.
BMPGenerator::BMPGenerator(int width, int height, const std::vector<Vertex>& vertices, const std::vector<Edge>& edges)  
    : m_width(width), m_height(height), m_vertices(vertices), m_edges(edges) {} 

//Функция которая создаёт и записывает изображение в файл
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
                .put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255)) 
                .put(bitmap[y][x] ? static_cast<char>(0) : static_cast<char>(255));
        }
    }
}

void BMPGenerator::drawText(std::vector<std::vector<bool>>& bitmap, const std::string& text, int x, int y) {
    int labelWidth = text.length() * 10;
    int labelHeight = 12;
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
    // Отрисовка круга с радиусом 7 пикселей
    int radius = 7;
    for (int y = yc - radius; y <= yc + radius; ++y) {
        for (int x = xc - radius; x <= xc + radius; ++x) {
            if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
                double distance = std::sqrt((x - xc) * (x - xc) + (y - yc) * (y - yc)); // Вычисление расстояния от центра круга
                if (distance <= radius) { // Если расстояние меньше или равно радиусу круга
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

bool BMPGenerator::isGraphPlanar() const {
    // Проверка наличия подграфа K5
    if (containsK5())
        return false;

    // Проверка наличия подграфа K3,3
    if (containsK33())
        return false;

    return true; // Если не найдены K5 и K3,3, граф планарен
}

bool BMPGenerator::containsK5() const {
    // Перебираем все возможные наборы 5 вершин
    for (size_t v1 = 0; v1 < m_vertices.size(); ++v1) {
        for (size_t v2 = v1 + 1; v2 < m_vertices.size(); ++v2) {
            for (size_t v3 = v2 + 1; v3 < m_vertices.size(); ++v3) {
                for (size_t v4 = v3 + 1; v4 < m_vertices.size(); ++v4) {
                    for (size_t v5 = v4 + 1; v5 < m_vertices.size(); ++v5) {
                        // Проверяем, существует ли хотя бы одно ребро между вершинами данного набора
                        if (!hasEdgeBetween(v1, v2) && !hasEdgeBetween(v1, v3) && !hasEdgeBetween(v1, v4) && !hasEdgeBetween(v1, v5) &&
                            !hasEdgeBetween(v2, v3) && !hasEdgeBetween(v2, v4) && !hasEdgeBetween(v2, v5) &&
                            !hasEdgeBetween(v3, v4) && !hasEdgeBetween(v3, v5) &&
                            !hasEdgeBetween(v4, v5)) {
                            // Если нет ни одного ребра, то найден K5
                            return true;
                        }
                    }
                }
            }
        }
    }
    // Если не найдено K5
    return false;
}

bool BMPGenerator::hasEdgeBetween(size_t v1, size_t v2) const {
    // Проверяем, есть ли ребро между вершинами v1 и v2
   for (const auto& edge : m_edges) {
        if ((edge.vertex1 == v1 && edge.vertex2 == v2) || (edge.vertex1 == v2 && edge.vertex2 == v1)) {
            return true;
        }
    }
    return false;
}

bool BMPGenerator::containsK33() const {
    // Перебираем все возможные наборы 6 вершин
    for (size_t v1 = 0; v1 < m_vertices.size(); ++v1) {
        for (size_t v2 = v1 + 1; v2 < m_vertices.size(); ++v2) {
            for (size_t v3 = v2 + 1; v3 < m_vertices.size(); ++v3) {
                // Проверяем, являются ли вершины v1, v2, v3 независимыми
                if (hasEdgeBetween(v1, v2) || hasEdgeBetween(v1, v3) || hasEdgeBetween(v2, v3))
                    continue; // Если не являются, пропускаем текущую тройку

                for (size_t v4 = 0; v4 < m_vertices.size(); ++v4) {
                    if (v4 == v1 || v4 == v2 || v4 == v3) continue; // Убеждаемся, что v4 не является частью первой тройки

                    // Проверяем, являются ли вершины v1, v2, v3 и v4 независимыми
                    if (hasEdgeBetween(v1, v4) || hasEdgeBetween(v2, v4) || hasEdgeBetween(v3, v4))
                        continue; // Если не являются, пропускаем текущую четвёрку

                    for (size_t v5 = v4 + 1; v5 < m_vertices.size(); ++v5) {
                        if (v5 == v1 || v5 == v2 || v5 == v3) continue; // Убеждаемся, что v5 не является частью первой тройки

                        // Проверяем, являются ли вершины v1, v2, v3, v4 и v5 независимыми
                        if (hasEdgeBetween(v1, v5) || hasEdgeBetween(v2, v5) || hasEdgeBetween(v3, v5) || hasEdgeBetween(v4, v5))
                            continue; // Если не являются, пропускаем текущую пятерку

                        for (size_t v6 = v5 + 1; v6 < m_vertices.size(); ++v6) {
                            if (v6 == v1 || v6 == v2 || v6 == v3) continue; // Убеждаемся, что v6 не является частью первой тройки

                            // Проверяем, являются ли вершины v1, v2, v3, v4, v5 и v6 независимыми
                            if (!hasEdgeBetween(v4, v6) && !hasEdgeBetween(v5, v6)) {
                                // Если все условия выполнены, это K3,3
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    // Если не найдено K3,3
    return false;
}

void BMPGenerator::modifyForK5() {
    // Находим набор вершин, образующих K5
    std::vector<size_t> k5Vertices = findK5Vertices();

    // Проверяем, найден ли K5
    if (k5Vertices.empty()) {
        std::cout << "No K5 found." << std::endl;
        return;
    }

    // Выводим индексы вершин K5
    std::cout << "Vertices forming K5: ";
    for (auto v : k5Vertices) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    // Меняем координаты вершин
    for (auto v : k5Vertices) {
        // Здесь можно изменить координаты вершин, например:
        m_vertices[v].x += 10; // Увеличиваем координату X на 10
        m_vertices[v].y += 10; // Увеличиваем координату Y на 10
    }
}

void BMPGenerator::modifyForK33() {
    // Находим набор вершин, образующих K33
    std::vector<std::pair<size_t, size_t>> k33Edges = findK33Edges();

    // Модификация структуры рёбер
    for (const auto& edge : k33Edges) {
        // Проверяем, существует ли уже такое ребро в нашей структуре
        bool edgeExists = false;
        for (const auto& existingEdge : m_edges) {
            if ((existingEdge.vertex1 == edge.first && existingEdge.vertex2 == edge.second) ||
                (existingEdge.vertex1 == edge.second && existingEdge.vertex2 == edge.first)) {
                edgeExists = true;
                break;
            }
        }

        // Если такого ребра ещё нет, то добавляем его
        if (!edgeExists) {
            m_edges.push_back({ edge.first, edge.second });
        }
    }

    // Вывод рёбер K33 (для демонстрации)
    std::cout << "Edges forming K33: ";
    for (const auto& edge : k33Edges) {
        std::cout << "(" << edge.first << ", " << edge.second << ") ";
    }
    std::cout << std::endl;
}


std::vector<size_t> BMPGenerator::findK5Vertices() const {
    std::vector<size_t> k5Vertices; // Вектор для хранения индексов вершин, образующих K5
    // Перебираем все возможные наборы 5 вершин
    for (size_t v1 = 0; v1 < m_vertices.size(); ++v1) {
        for (size_t v2 = v1 + 1; v2 < m_vertices.size(); ++v2) {
            for (size_t v3 = v2 + 1; v3 < m_vertices.size(); ++v3) {
                for (size_t v4 = v3 + 1; v4 < m_vertices.size(); ++v4) {
                    for (size_t v5 = v4 + 1; v5 < m_vertices.size(); ++v5) {
                        // Проверяем, существует ли хотя бы одно ребро между вершинами данного набора
                        if (!hasEdgeBetween(v1, v2) && !hasEdgeBetween(v1, v3) && !hasEdgeBetween(v1, v4) && !hasEdgeBetween(v1, v5) &&
                            !hasEdgeBetween(v2, v3) && !hasEdgeBetween(v2, v4) && !hasEdgeBetween(v2, v5) &&
                            !hasEdgeBetween(v3, v4) && !hasEdgeBetween(v3, v5) &&
                            !hasEdgeBetween(v4, v5)) {
                            // Если нет ни одного ребра, то найден K5, добавляем вершины в вектор
                            k5Vertices.push_back(v1);
                            k5Vertices.push_back(v2);
                            k5Vertices.push_back(v3);
                            k5Vertices.push_back(v4);
                            k5Vertices.push_back(v5);
                            return k5Vertices; // Найден один набор вершин образующих K5, завершаем поиск
                        }
                    }
                }
            }
        }
    }

    // Если не найдено K5, возвращаем пустой вектор
    return k5Vertices;
}

std::vector<std::pair<size_t, size_t>> BMPGenerator::findK33Edges() const {
    std::vector<std::pair<size_t, size_t>> k33Edges;

    // Перебираем все возможные комбинации вершин
    for (size_t v1 = 0; v1 < m_vertices.size(); ++v1) {
        for (size_t v2 = v1 + 1; v2 < m_vertices.size(); ++v2) {
            for (size_t v3 = v2 + 1; v3 < m_vertices.size(); ++v3) {
                // Формируем множества вершин
                std::set<size_t> set1 = { v1, v2, v3 };
                std::set<size_t> set2;
                for (size_t i = 0; i < m_vertices.size(); ++i) {
                    if (i != v1 && i != v2 && i != v3) {
                        set2.insert(i);
                    }
                }

                // Проверяем, являются ли два множества независимыми
                bool isIndependent = true;
                for (const auto& edge : m_edges) {
                    size_t v1 = edge.vertex1;
                    size_t v2 = edge.vertex2;
                    if ((set1.count(v1) && set1.count(v2)) || (set2.count(v1) && set2.count(v2))) {
                        isIndependent = false;
                        break;
                    }
                }

                // Если два множества независимы, то добавляем соответствующие рёбра в результат
                if (isIndependent) {
                    for (size_t v1 : set1) {
                        for (size_t v2 : set2) {
                            k33Edges.emplace_back(std::min(v1, v2), std::max(v1, v2));
                        }
                    }
                }
            }
        }
    }

    return k33Edges;
}
