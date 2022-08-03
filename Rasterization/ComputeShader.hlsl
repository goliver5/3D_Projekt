RWTexture2D <unorm float4> backBuffer : register(u0);
Texture2D textureColor : register(t0);
Texture2D normal : register(t1);
Texture2D position : register(t2);


[numthreads(32, 32, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	int3 location = int3(0, 0, 0);
	
	location.x = DTid.x;
	location.y = DTid.y;
	
	float4 color = textureColor[DTid.xy];
	
	//float4 value = textureColor.Load(location);
	backBuffer[DTid.xy] =  color;
	
	//int3 location = int3(0, 0, 0);
	
	//float shadow = textureColor.Load(location).w;
	
	//location.x = DTid.x;
	//location.y = DTid.y;
	
	//float3 temp = textureColor[DTid.xy] * shadow;
	//float4 color = float4(temp, 1.0f);
}