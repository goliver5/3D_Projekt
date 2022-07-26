#pragma once
#include <vector>
#include <string>
#include "objectStruct.h"

using namespace std;

bool ParseOBJFile(std::vector<float>& vertices, std::vector<float>& normals, std::vector<float>& uvs, string fileName);