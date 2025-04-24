cbuffer UBO : register(b0, space1)
{
    float4x4 ViewProjection;
};


struct Vertex
{
    float3 Position : TEXCOORD0;
    float4 Color : TEXCOORD1;
};

struct Pixel
{
    float4 Color : TEXCOORD0;
    float4 Position : SV_Position;
};

Pixel VS(Vertex v)
{
    Pixel p;

    p.Position = mul(ViewProjection, float4(v.Position, 1));
    p.Color = v.Color;
    
    return p;
}

float4 PS(Pixel p) : SV_Target
{
    return p.Color;
}