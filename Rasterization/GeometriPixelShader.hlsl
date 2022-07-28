Texture2D testTexture : register(t0);
Texture2D depthTexture : register(t1);

SamplerState testSampler : register(s0);
SamplerState DepthSampler : register(s1);

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
	float4 posLight : LIGHTPOS;
};

struct PixelShaderOutput
{
	float4 Texture : SV_Target0;
	float4 Normal : SV_Target1;
	float4 Position : SV_Target2;
};

PixelShaderOutput main(PixelShaderInput input)
{
	PixelShaderOutput output;
	
	
	
	input.posLight.xy /= input.posLight.w;

	float2 smTexcoord = float2(0.5f * input.posLight.x + 0.5f, -0.5f * input.posLight.y + 0.5f);
	float depth = input.posLight.z / input.posLight.w;
	float SHADOW_EPSILON = 0.0000125f;
	float dx = 1.f / 2048.0f;
	
	
	float d0 = (depthTexture.Sample(DepthSampler, smTexcoord + float2(0.0f, 0.0f)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float d1 = (depthTexture.Sample(DepthSampler, smTexcoord + float2(dx, 0.0f)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float d2 = (depthTexture.Sample(DepthSampler, smTexcoord + float2(0.0f, dx)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float d3 = (depthTexture.Sample(DepthSampler, smTexcoord + float2(dx, dx)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;

	float2 texelPos = smTexcoord * 2048.0f;
	float2 leps = frac(texelPos);
	float shadowco = (d0 + d1 + d2 + d3) / 4;
	if (shadowco <= 0.3)
		shadowco = 0.3f;
	
	
	
	float4 texColor = testTexture.Sample(testSampler, input.uv);
	
	
	texColor = texColor * shadowco;

	output.Texture = texColor;
	//output.Texture = testTexture.Sample(testSampler, input.uv);

	output.Position = input.position;
	output.Normal = float4(input.normal, 1.0f);

	return output;
}
