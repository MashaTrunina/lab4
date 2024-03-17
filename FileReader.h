#pragma once
#include <string>
#include <vector>
#include "Edge.h"

void readInputFromFile(const std::string& filename, int& numVertices, int& width, int& height);
void readEdgesFromFile(const std::string& filename, std::vector<Edge>& edges);
