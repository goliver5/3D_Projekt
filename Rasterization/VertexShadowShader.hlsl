cbuffer cBuf : register(b0)
{
	float4x4 worldMatrix;
}

cbuffer viewProjection : register(b1)
{
	float4x4 VPMatrix;
}

cbuffer lightViewProjection : register(b2)
{
	float4x4 lightVPMatrix;
};

struct VertexShaderInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float3 worldPos : WORLD_POS;
	float3 normal : NORMAL;
	float2 uv : UV;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	
	output.position = mul(float4(input.position, 1.0f), mul(worldMatrix, lightVPMatrix));
	output.worldPos = mul(float4(input.position, 1.0f), worldMatrix);

	output.normal = mul(input.normal, worldMatrix);
	output.uv = input.uv;
	return output;
}