#include "ObjParserHelper.h"

void ObjParserHelper(std::vector<objectStruct>& output, ID3D11Device* device)
{


	static const int nrOfMeshes = 3;
	std::string fileName[nrOfMeshes]{};
	fileName[0] = "Soapbox_Car";
	fileName[1] = "Sphere";
	fileName[2] = "cubereal";
	//fileName[3] = "torus";
	//fileName[4] = "ground";
	//fileName[5] = "BusterSword";

	for (int i = 0; i < nrOfMeshes; i++)
	{
		output.push_back(objectStruct());
		ParseOBJFile(output[i].vertices, output[i].normals, output[i].uvs, output[i].vertexForIndex, output[i].vertexSubMeshCounter, output[i].indices, fileName[i], device,
			output[i].srvs_ka, output[i].srvs_kd, output[i].srvs_ks, output[i].topLeft, output[i].bottomLeft);
	}

}
