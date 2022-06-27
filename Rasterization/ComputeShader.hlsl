RWTexture2D <unorm float4> backBuffer : register(u0);

[numthreads(32, 18, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	backBuffer[DTid.xy] = float4(1.0f, 1.0f, 1.0f, 1.0f) - backBuffer[DTid.xy];
}