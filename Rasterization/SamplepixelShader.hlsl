Texture2D testTexture : register(t0);
Texture2D cubeSrv : register(t1);

SamplerState testSampler : register(s0);


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
	float4 texColor = testTexture.Sample(testSampler, input.uv);
	return float4(texColor);

}