struct HS_CONSTANT_DATA_OUTPUT
{
	float edgeTessFactor[3] : SV_TessFactor;
	float insideTessFactor : SV_InsideTessFactor;
};

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
	
	for (int i = 0; i < 3; i++)
		output.edgeTessFactor[i] = 10.0f;
	
	output.insideTessFactor = 10.0f;
	
	
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