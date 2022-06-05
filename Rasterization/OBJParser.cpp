#include "OBJParser.h"
#include<fstream>
#include<iostream>
#include <chrono>

bool ParseOBJFile(std::vector<float>& vertices, std::vector<float>& normals, std::vector<float>& uvs, string fileName)
{
	std::ifstream file;
	std::string temp;
	file.open("models/" + fileName);
	if (file.is_open())
	{
		std::getline(file, temp);
		std::getline(file, temp);
		temp = "wow keep the loop going\n";
		while (!(file.eof()))
		{
			file >> temp;
			if (temp == "v")// reads vertices
			{
				file >> temp;
				vertices.push_back(stof(temp));
				file >> temp;
				vertices.push_back(stof(temp));
				file >> temp;
				vertices.push_back(stof(temp));
			}
			else if (temp == "vt") //reads the UVs
			{
				file >> temp;
				uvs.push_back(stof(temp));
				file >> temp;
				uvs.push_back(stof(temp));
			}
			else if (temp == "vn")
			{
				file >> temp;
				normals.push_back(stof(temp));
				file >> temp;
				normals.push_back(stof(temp));
				file >> temp;
				normals.push_back(stof(temp));
			}
			//vertices.push_back(stof(temp));
		}
		//file >> temp;
	}
	else
	{
		//std::cout << "couldnt Open file\n";
	}
	std::cout << temp;
	return true;
}