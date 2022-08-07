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
	//particlePositions[DTid.x * 3] = cos(time  + DTid.x);
	//particlePositions[DTid.x * 3 + 1] = sin(time + DTid.x);
	
	//particlePositions[DTid.x * 3] = sin(time + DTid.x);
	//particlePositions[DTid.x * 3 + 1] = sqrt(time  + DTid.x);
	//particlePositions[DTid.x * 3 + 2] = cos(time + DTid.x);
	
	//partiklarna går upp och ner
	particlePositions[DTid.x * 3 + 1] = sin(time + DTid.x) * 5;
	
	//particlePositions[DTid.x * 3 + 1] = (1000 % int((particlePositions[DTid.x * 3 + 1] + time) * 100)) * 0.01f - 0.01f;
	
	//particlePositions[DTid.x * 3 + 1] = sqrt(particlePositions[DTid.x * 3 + 1] - time);
	
	//if (int(particlePositions[DTid.x * 3 + 1]) % 10.f)
	//{
	//	particlePositions[DTid.x * 3 + 1] = 5.f;
	//}
	
}