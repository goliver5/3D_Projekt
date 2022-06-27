Texture2D testTexture : register(t0);
SamplerState testSampler;

cbuffer cBuf : register(b0)
{
	float3 lightPos;
	float ambienceStrength;
	float3 cameraPos;
	float padd;
}

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float3 worldPos : WORLD_POS;
	float3 normal : NORMAL;
	float2 uv : UV;
};

struct PixelShaderOutput
{
	float3 Normal : SV_Target0;
	float4 Position : SV_Target1;
};

PixelShaderOutput main(PixelShaderInput input)
{
	PixelShaderOutput output;

	output.Position = input.position;
	output.Normal = input.normal;

	return output;
}
