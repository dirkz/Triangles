cbuffer UBO : register(b0, space1)
{
    float4x4 ViewProjection;
};

Texture2D<float4> Texture : register(t0, space2);
SamplerState Sampler : register(s0, space2);

struct Vertex
{
    float3 Position : TEXCOORD0;
    float4 Color : TEXCOORD1;
    float2 Texture : TEXCOORD2;
};

struct Pixel
{
    float4 Color : TEXCOORD0;
    float4 Position : SV_Position;
    float2 Texture : TEXCOORD1;
};

Pixel VS(Vertex v)
{
    Pixel p;

    p.Position = mul(ViewProjection, float4(v.Position, 1));
    p.Color = v.Color;
    p.Texture = v.Texture;
    
    return p;
}

float4 PS(Pixel p) : SV_Target
{
    return p.Color * Texture.Sample(Sampler, p.Texture);
}