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
cbuffer lightViewProjection : register(b3)
{
	float4x4 lightVPMatrix2;
};
cbuffer lightViewProjection : register(b4)
{
	float4x4 lightVPMatrix3;
};
cbuffer lightViewProjection : register(b5)
{
	float4x4 lightVPMatrix4;
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
	float4 worldPos : WORLD_POS;
	float3 normal : NORMAL;
	float2 uv : UV;
	float4 posLight : LIGHTPOS;
	float4 posLight2 : LIGHTPOS2;
	float4 posLight3 : LIGHTPOS3;
	float4 posLight4 : LIGHTPOS4;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	//float4x4 temp = mul(worldMatrix, viewProjectionMatrix);
	//output.position = mul(float4(input.position, 1.0f), viewProjectionMatrix);
	//output.position = mul(float4(input.position, 1.0f), worldMatrix);
	
	
	output.position = mul(float4(input.position, 1.0f), mul(worldMatrix, VPMatrix));
	output.worldPos = mul(float4(input.position, 1.0f), worldMatrix);
	
	output.posLight = mul(float4(input.position, 1.0f), mul(worldMatrix, lightVPMatrix));
	output.posLight2 = mul(float4(input.position, 1.0f), mul(worldMatrix, lightVPMatrix2));
	output.posLight3 = mul(float4(input.position, 1.0f), mul(worldMatrix, lightVPMatrix3));
	output.posLight4 = mul(float4(input.position, 1.0f), mul(worldMatrix, lightVPMatrix4));

	//output.position = float4(input.position, 1.0f);
	//output.normal = mul(float4(input.normal,0.0f), worldMatrix);
	output.normal = mul(input.normal, worldMatrix);
	output.uv = input.uv;
	return output;
}