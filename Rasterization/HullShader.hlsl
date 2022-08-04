struct HS_CONSTANT_DATA_OUTPUT
{
	float edgeTessFactor[3] : SV_TessFactor;
	float insideTessFactor : SV_InsideTessFactor;
};

cbuffer cameraBuffer : register(b0)
{
	float3 pos;
	float padding;
}

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float4 worldPos : WORLD_POS;
	float3 normal : NORMAL;
	float2 uv : UV;
	float4 posLight : LIGHTPOS;
};

HS_CONSTANT_DATA_OUTPUT ConstantPatchFunction(InputPatch<VertexShaderOutput, 3> inputPatch, uint patchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT output;
	
	float distance = sqrt((pos.x - inputPatch[0].worldPos.x) * (pos.x - inputPatch[0].worldPos.x) 
	+ (pos.y - inputPatch[0].worldPos.y) * (pos.y - inputPatch[0].worldPos.y)
	+ (pos.z - inputPatch[0].worldPos.z) * (pos.z - inputPatch[0].worldPos.z));
	
	if (distance <= 5.0f)
	{
		for (int i = 0; i < 3; i++)
			output.edgeTessFactor[i] = 10.0f;
		
		output.insideTessFactor = 10.0f;
	}
	else
	{
		for (int i = 0; i < 3; i++)
			output.edgeTessFactor[i] = 1.0f;
		
		output.insideTessFactor = 1.0f;
	}
	
		return output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantPatchFunction")]
VertexShaderOutput PerPatchFunction(InputPatch<VertexShaderOutput, 3> inputPatch, uint i : SV_OutputControlPointID,
	uint patchID : SV_PrimitiveID)
{
	return inputPatch[i];
}