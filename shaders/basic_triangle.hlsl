struct Vertex
{
    float3 Position : POSITION;
    float4 Color : COLOR;
};

struct Pixel
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
};

Pixel VS(Vertex v)
{
    Pixel p;

    p.Position = float4(v.Position, 1);
    p.Color = v.Color;
    
    return p;
}

float4 PS(Pixel p) : SV_Target
{
    return p.Color;
}