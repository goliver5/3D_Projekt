RWBuffer<float> particlePositions : register(u0);

cbuffer particleInformation : register(b0)
{
	float time;
	float pPerThread;
	float padding[2];
};


[numthreads(20, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	particlePositions[DTid.x * 3] = cos(time  + DTid.x);
	particlePositions[DTid.x * 3 + 1] = sin(time + DTid.x);
	
	//particlePositions[DTid.x * 3] = cos(time + DTid.x);
	//particlePositions[DTid.x * 3 + 1] = sqrt(time  + DTid.x);
	//particlePositions[DTid.x * 3 + 2] = sin(time + DTid.x);
}