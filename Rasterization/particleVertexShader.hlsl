cbuffer cBuf : register(b0)
{
	float4x4 worldMatrix;
}

struct VertexShaderInput
{
	float3 position : POSITION;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	
	output.position = mul(float4(input.position, 1.0f), worldMatrix);

	return output;
}