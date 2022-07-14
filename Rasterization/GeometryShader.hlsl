cbuffer viewProjection : register(b0)
{
	float4x4 viewProjectionMatrix;
}

cbuffer cameraBuf : register(b1)
{
	float3 cameraPosition;
	float padding;
}

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
};

struct GeometryShaderOutput
{
	float4 pos : SV_POSITION;
};

[maxvertexcount(10)]
void main(point VertexShaderOutput input[1], inout TriangleStream<GeometryShaderOutput> outputStream)
{
	
	float3 cameraForwardVector = input[0].position - float4(cameraPosition, 0.0f);

	cameraForwardVector = float3(0.0f, 0.0f, 1.0f);
	
	const static float SIZE = 0.12f;
	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 right = cross(cameraForwardVector, up);
	
	//static const float PI = 3.14159265f;
	
	//int n = 5; //trianglar i cirklen
	//float deltaTheta = 2 * PI / n;
	
	//int theta = 0;
	//int index = 0;
	
	//for (int i = 0; i < n; i++)
	//{
	//	theta = i * deltaTheta;//theta är angle för triangeln
	//	index = 3 * i;
		
	//}
	
	
	
	float3 topLeft = input[0].position - right * SIZE + up * SIZE;
	float3 topRight = input[0].position + right * SIZE + up * SIZE;
	float3 bottomRight = input[0].position + right * SIZE - up * SIZE;
	float3 bottomLeft = input[0].position - right * SIZE - up * SIZE;
	
	GeometryShaderOutput outputs[4] =
	{
		mul(float4(bottomLeft, 1.0f), viewProjectionMatrix),
	mul(float4(bottomRight, 1.0f), viewProjectionMatrix),
	mul(float4(topLeft, 1.0f), viewProjectionMatrix),
	mul(float4(topRight, 1.0f), viewProjectionMatrix)
	};
	
	outputStream.Append(outputs[0]);
	outputStream.Append(outputs[1]);
	outputStream.Append(outputs[2]);
	outputStream.Append(outputs[3]);
	}