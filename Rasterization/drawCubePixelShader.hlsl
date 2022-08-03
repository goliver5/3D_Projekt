Texture2D testTexture : register(t0);
TextureCube cubeTexture : register(t1);

SamplerState Sampler : register(s0);

cbuffer cBuf : register(b0)
{
	float3 cameraPos;
	float padd;
}

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float3 worldPos : WORLD_POS;
	float3 normal : NORMAL;
	float2 uv : UV;
	float4 posLight : LIGHTPOS;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 temp = input.worldPos - cameraPos;
	
	temp = normalize(temp);
	float3 norm = normalize(input.normal);
	float3 loc = reflect(temp, norm);
	loc = normalize(loc);
	
	float3 texColor = cubeTexture.Sample(Sampler, loc).xyz;
	
	return float4(texColor, 1.0f);
}