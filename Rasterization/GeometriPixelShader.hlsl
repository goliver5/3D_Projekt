Texture2D Texture_KA : register(t0);
Texture2D Texture_KD : register(t1);
Texture2D Texture_KS : register(t2);
Texture2DArray depthTexture : register(t3);

SamplerState testSampler : register(s0);
SamplerState DepthSampler : register(s1);

cbuffer nsBuf : register(b0)
{
	float NS;
	float3 padding;
}


struct PixelShaderInput
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

struct PixelShaderOutput
{
	float4 Texture : SV_Target0;
	float4 Normal : SV_Target1;
	float4 Position : SV_Target2;
	float4 DiffuseMap : SV_Target3;
	float4 AmbientMap : SV_Target4;
	float4 SpecularMap : SV_Target5;
};

PixelShaderOutput main(PixelShaderInput input)
{
	PixelShaderOutput output;
	
	
	float3 norm = normalize(input.normal);
	
	input.posLight.xy /= input.posLight.w;
	input.posLight2.xy /= input.posLight2.w;
	input.posLight3.xy /= input.posLight3.w;
	input.posLight4.xy /= input.posLight4.w;

	float3 smTexcoord = float3(0.5f * input.posLight.x + 0.5f, -0.5f * input.posLight.y + 0.5f,0.0f);
	float3 smTexcoord2 = float3(0.5f * input.posLight2.x + 0.5f, -0.5f * input.posLight2.y + 0.5f,1.0f);
	float3 smTexcoord3 = float3(0.5f * input.posLight3.x + 0.5f, -0.5f * input.posLight3.y + 0.5f,2.0f);
	float3 smTexcoord4 = float3(0.5f * input.posLight4.x + 0.5f, -0.5f * input.posLight4.y + 0.5f,3.0f);
	float depth = input.posLight.z / input.posLight.w;
	float depth2 = input.posLight2.z / input.posLight2.w;
	float depth3 = input.posLight3.z / input.posLight3.w;
	float depth4 = input.posLight4.z / input.posLight4.w;
	float SHADOW_EPSILON = 0.000125f;
	
	
	float d0 = (depthTexture.Sample(DepthSampler, smTexcoord).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float d1 = (depthTexture.Sample(DepthSampler, smTexcoord2).r + SHADOW_EPSILON < depth2) ? 0.0f : 1.0f;
	float d2 = (depthTexture.Sample(DepthSampler, smTexcoord3).r + SHADOW_EPSILON < depth3) ? 0.0f : 1.0f;
	float d3 = (depthTexture.Sample(DepthSampler, smTexcoord4).r + SHADOW_EPSILON < depth4) ? 0.0f : 1.0f;
	
	float finalSpotLight = d0 * d1 * d2;
	//float d0 = (depthTexture.Sample(DepthSampler, smTexcoord + float2(0.0f, 0.0f)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	
	//if (d1 <= 0.3)
	//	d1 = 0.3f;
	
	float3 texColor = Texture_KA.Sample(testSampler, input.uv);
	
	//float4 color = float4((texColor * finalSpotLight), 1.0f);
	
	
	
	
	float4 color = float4((texColor), NS);

	output.Texture = color;
	output.Position = float4(input.worldPos.xyz, d0);
	output.Normal = float4(input.normal, d1);
	output.AmbientMap = float4(Texture_KA.Sample(testSampler, input.uv).xyz, d2);
	output.DiffuseMap = float4(Texture_KD.Sample(testSampler, input.uv).xyz, d3);
	output.SpecularMap = Texture_KS.Sample(testSampler, input.uv);

	return output;
}
