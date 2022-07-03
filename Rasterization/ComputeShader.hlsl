RWTexture2D <unorm float4> backBuffer : register(u0);
Texture2D textureColor : register(t0);
Texture2D normal : register(t1);
Texture2D position : register(t2);

SamplerState Sampler;

//float4 texColor = testTexture.Sample(testSampler, input.uv);

[numthreads(32, 18, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	int3 location = int3(0, 0, 0);
	
	location.x = DTid.x;
	location.y = DTid.y;
	
	float4 color = textureColor[DTid.xy];
	
	float4 value = position.Load(location);
	backBuffer[DTid.xy] = float4(.0f, 1.0f, 1.0f, 1.0f) - color;
}