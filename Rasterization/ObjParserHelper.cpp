#include "ObjParserHelper.h"

void ObjParserHelper(std::vector<objectStruct>& output)
{


	static const int nrOfMeshes = 5;
	std::string fileName[nrOfMeshes]{};
	fileName[0] = "cube.obj";
	fileName[1] = "Sphere.obj";
	fileName[2] = "BusterSword.obj";
	fileName[3] = "torus.obj";
	fileName[4] = "ground.obj";

	for (int i = 0; i < nrOfMeshes; i++)
	{
		output.push_back(objectStruct());
		ParseOBJFile(output[i].vertices, output[i].normals, output[i].uvs, output[i].vertexForIndex, output[i].vertexSubMeshCounter, output[i].indices, fileName[i]);
	}

}
