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
	float3 worldPos : WORLD_POS;
	float3 normal : NORMAL;
	float2 uv : UV;
};

struct GeometryShaderOutput
{
	float4 pos : SV_POSITION;
};

[maxvertexcount(3)]
void main(point VertexShaderOutput input[1], inout TriangleStream<GeometryShaderOutput> outputStream)
{
	
	float3 cameraForwardVector = input[0].position - float4(cameraPosition, 0.0f);

	const static float SIZE = 0.25f;
	float3 up = float3(0.0f, -1.0f, 0.0f);
	float3 right = cross(cameraForwardVector, up);
	
	float3 topLeft = input[0].position - right * SIZE + up * SIZE;
	float3 topRight = input[0].position + right * SIZE + up * SIZE;
	float3 bottomRight = input[0].position + right * SIZE - up * SIZE;
	
	GeometryShaderOutput outputs[3] =
	{
		mul(viewProjectionMatrix, float4(topLeft, 1.0f))
		, mul(viewProjectionMatrix, float4(topRight, 1.0f))
		, mul(viewProjectionMatrix, float4(bottomRight, 1.0f))
	};
	
	outputStream.Append(outputs[0]);
	outputStream.Append(outputs[1]);
	outputStream.Append(outputs[2]);
}