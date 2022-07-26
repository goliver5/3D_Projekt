#include "OBJParser.h"
#include<fstream>
#include <sstream>
#include<iostream>
#include <chrono>

bool ParseOBJFile(std::vector<float>& vertices, std::vector<float>& normals, std::vector<float>& uvs, std::vector<VertexData> &vertexForIndex,
	std::vector<int> &vertexSubMeshCounter, std::vector<int> &indices, string fileName)
{
	//plats i vertexForIndex där verticer är likadana
	//std::vector<int> indices;
	//vertexbuffern som används med indexbuffern
	//std::vector<VertexData> vertexForIndex;
	//
	//std::vector<int>vertexSubMeshCounter;

	std::string currentMTL;

	int indexCounter = 0;
	
	////test för att lägga till nya vertiser med default konstruktor
	//vertexForIndex.push_back(VertexData());
	//vertexForIndex[0].pos[0] = 1;

	std::ifstream file;
	std::string myString;
	std::string currentLine;
	file.open("models/" + fileName);

	if (file.is_open())
	{
		while (!(file.eof()))
		{
			std::getline(file, currentLine);

			std::stringstream currentStringStream(currentLine);
			std::getline(currentStringStream, myString, ' ');
			if (myString == "v")// reads vertices
			{
				std::getline(currentStringStream, myString, ' ');
				vertices.push_back(stof(myString));
				std::getline(currentStringStream, myString, ' ');
				vertices.push_back(stof(myString));
				std::getline(currentStringStream, myString, ' ');
				vertices.push_back(stof(myString));
			}
			else if (myString == "vt") //reads the UVs
			{
				std::getline(currentStringStream, myString, ' ');
				uvs.push_back(stof(myString));
				std::getline(currentStringStream, myString, ' ');
				uvs.push_back(stof(myString));
			}
			else if (myString == "vn")
			{
				std::getline(currentStringStream, myString, ' ');
				normals.push_back(stof(myString));
				std::getline(currentStringStream, myString, ' ');
				normals.push_back(stof(myString));
				std::getline(currentStringStream, myString, ' ');
				normals.push_back(stof(myString));
			}
			else if (myString == "mtllib")
			{
				//while (!(file.eof()))
				//{

				//}

			}
			else if (myString == "usemtl")
			{
				std::getline(currentStringStream, myString);
				currentMTL = myString;

				vertexSubMeshCounter.push_back(0);
			}
			else if(myString == "f")
			{

				vertexSubMeshCounter[vertexSubMeshCounter.size() - 1] = vertexSubMeshCounter[vertexSubMeshCounter.size() - 1] + 3;
				//ny array som kommer att bli vertexbuffern och en int array som är index buffern som har platsen vertisen är på
				//position/uv/normal
				
				//om vi lagt till ett värde i index arrayen
				bool addedindex = false;
				//om den är true så finns det ett värde som är likadant (antar först att det finns ett värde och om något blir fel så fortsätter loopen)
				bool variableExist = true;

				for (int i = 0; i < 3; i++)
				{
					std::vector<int> tempNr;
					addedindex = false;

					//första face 
					std::getline(currentStringStream, myString, '/');
					tempNr.push_back(stoi(myString) - 1);
					std::getline(currentStringStream, myString, '/');
					tempNr.push_back(stoi(myString) - 1);
					std::getline(currentStringStream, myString, ' ');
					tempNr.push_back(stoi(myString) - 1);

					std::vector<float> tempPos;
					tempPos.push_back(vertices[(tempNr[0] * 3) + 0]);
					tempPos.push_back(vertices[(tempNr[0] * 3) + 1]);
					tempPos.push_back(vertices[(tempNr[0] * 3) + 2]);
					std::vector<float> tempUv;
					tempUv.push_back(uvs[tempNr[1] * 2 + 0]);
					tempUv.push_back(uvs[tempNr[1] * 2 + 1]);
					std::vector<float>tempNorm;
					tempNorm.push_back(normals[(tempNr[0] * 3) + 0]);
					tempNorm.push_back(normals[(tempNr[0] * 3) + 1]);
					tempNorm.push_back(normals[(tempNr[0] * 3) + 2]);

					VertexData tempData(tempPos, tempNorm, tempUv);

					//kolla i arrayen om det finnns andra likadana indexer
					for (int j = 0; j < vertexForIndex.size(); j++)
					{

						for (int k = 0; k < 3; k++)
						{
							if (vertexForIndex[j].pos[k] != tempData.pos[k])
							{
								variableExist = false;
								//continue;
							}
							if (vertexForIndex[j].nrm[k] != tempData.nrm[k])
							{
								variableExist = false;
								//continue;
							}
						}
						for (int k = 0; k < 2; k++)
						{
							if (vertexForIndex[j].uv[k] != tempData.uv[k])
							{
								variableExist = false;
								//continue;
							}
						}

						//om detta är true så finns en variabel med samma värde så bryt loopen och skapa lägg till den
						if (variableExist)
						{
							indices.push_back(j);
							addedindex = true;
							break;
						}
						// sätter om den så att nästa loop fungerar på rätt sätt
						variableExist = true;
					}
					//variablen fanns redan i loopen så la bara till dens plats i vertexForIndex i arrayen indices
					if (!addedindex)
					{
						indices.push_back(indexCounter++);
						vertexForIndex.push_back(tempData);
						//VertexData(vertices[])
					}
				}

			}
			//vertices.push_back(stof(myString));
		}
		//file >> myString;
	}
	else
	{
		//std::cout << "couldnt Open file\n";
	}
	std::cout << myString;
	return true;
}