#include "OBJParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <d3d11.h>
#include <DirectXCollision.h>
#include"CreateTextures.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

bool readMTL(ID3D11Device* device, std::string mtlFileName, ID3D11ShaderResourceView*& temp_ka, ID3D11ShaderResourceView*& temp_kd, ID3D11ShaderResourceView*& temp_ks, float &NS,
	std::string mtlName)
{
	std::ifstream file;
	std::string myString;
	std::string currentLine;
	file.open(mtlFileName);

	bool doneReadingFile = false;

	if (file.is_open())
	{
		while (!(file.eof()))
		{
			if (doneReadingFile)
			{
				break;
			}

			std::getline(file, currentLine);

			std::stringstream currentStringStream(currentLine);
			std::getline(currentStringStream, myString, ' ');
			if (myString == "newmtl")// reads mtl
			{
				std::getline(currentStringStream, myString, ' ');

				if(myString == mtlName)
				{
					while (!(file.eof()))
					{

						std::getline(file, currentLine);

						std::stringstream mtlStringStream(currentLine);
						std::getline(mtlStringStream, myString, ' ');

						if (myString == "map_Ka")
						{
							std::getline(mtlStringStream, myString, ' ');
							createTextures(device, temp_ka, "models/" + myString);
						}
						if (myString == "map_Kd")
						{
							std::getline(mtlStringStream, myString, ' ');
							createTextures(device, temp_kd, "models/" + myString);
						}
						if (myString == "map_Ks")
						{
							std::getline(mtlStringStream, myString, ' ');
							createTextures(device, temp_ks, "models/" + myString);
						}
						if (myString == "Ns")
						{
							std::getline(mtlStringStream, myString, ' ');
							NS = stof(myString);
						}
						if (myString == "")
						{
							doneReadingFile = true;
							break;
						}
					}
				}
			}
		}
	}

	file.close();

	return true;
}


bool ParseOBJFile(std::vector<float>& vertices, std::vector<float>& normals, std::vector<float>& uvs, std::vector<VertexData> &vertexForIndex,
	std::vector<int> &vertexSubMeshCounter, std::vector<int> &indices, string fileName, ID3D11Device* device, std::vector<ID3D11ShaderResourceView*>& srvs_ka,
	std::vector<ID3D11ShaderResourceView*>& srvs_kd, std::vector<ID3D11ShaderResourceView*>& srvs_ks, DirectX::XMFLOAT3& topLeft, DirectX::XMFLOAT3& bottomLeft, std::vector<float>& NS)
{
	//plats i vertexForIndex där verticer är likadana
	//std::vector<int> indices;
	//vertexbuffern som används med indexbuffern
	//std::vector<VertexData> vertexForIndex;
	//
	//std::vector<int>vertexSubMeshCounter;

	std::string currentMTL;

	int indexCounter = 0;

	//DirectX::BoundingBox::CreateFromPoints()
	DirectX::XMFLOAT3 lowestBoundingBoxPoint;
	DirectX::XMFLOAT3 highestBoundingBoxPoint;
	
	////test för att lägga till nya vertiser med default konstruktor
	//vertexForIndex.push_back(VertexData());
	//vertexForIndex[0].pos[0] = 1;

	std::ifstream file;
	std::string myString;
	std::string currentLine;
	file.open("models/" + fileName + ".obj");

	if (file.is_open())
	{
		while (!(file.eof()))
		{
			std::getline(file, currentLine);

			std::stringstream currentStringStream(currentLine);
			std::getline(currentStringStream, myString, ' ');
			if (myString == "v")// reads vertices
			{
				DirectX::XMFLOAT3 saved;

				std::getline(currentStringStream, myString, ' ');
				saved.x = stof(myString);
				vertices.push_back(stof(myString));
				std::getline(currentStringStream, myString, ' ');
				saved.y = stof(myString);
				vertices.push_back(stof(myString));
				std::getline(currentStringStream, myString, ' ');
				saved.z = stof(myString);
				vertices.push_back(stof(myString));
				
				

				if (vertices.size() == 3 )//starta saken
				{
					topLeft = saved;
					bottomLeft = saved;
				}
				else
				{
					if (topLeft.x >= saved.x && topLeft.y >= saved.y && topLeft.z >= saved.z) topLeft = saved;
					else if (bottomLeft.x <= saved.x && bottomLeft.x <= saved.y && bottomLeft.z <= saved.z) bottomLeft = saved;
				}

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
				std::getline(currentStringStream, myString, ' ');

				ID3D11ShaderResourceView* temp_ka;
				ID3D11ShaderResourceView* temp_kd;
				ID3D11ShaderResourceView* temp_ks;
				float tempNS;
				readMTL(device,"models/" + fileName + ".mtl", temp_ka, temp_kd, temp_ks, tempNS, myString);

				srvs_ka.push_back(temp_ka);
				srvs_kd.push_back(temp_kd);
				srvs_ks.push_back(temp_ks);
				NS.push_back(tempNS);

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
					tempNorm.push_back(normals[(tempNr[2] * 3) + 0]);
					tempNorm.push_back(normals[(tempNr[2] * 3) + 1]);
					tempNorm.push_back(normals[(tempNr[2] * 3) + 2]);

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
	file.close();
	std::cout << myString;
	return true;
}