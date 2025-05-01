#include "Icosahedron.h"

#include "Common.h"
#include "Noise.h"
#include "Triangle.h"

namespace triangles
{

using namespace DirectX;

const float G = (1.f + std::sqrt(5.f)) / 2.f;

static XMVECTOR XM_CALLCONV TextureCoordinates(FXMVECTOR point, float radius)
{
    XMFLOAT4 floats;
    XMStoreFloat4(&floats, point);

    XMVECTOR baseXZ = XMVectorSet(1, 0, 0, 1);
    XMVECTOR vXZ = XMVectorSet(floats.x, 0, floats.z, 1);
    XMVECTOR vAngleXZ = XMVector3AngleBetweenVectors(baseXZ, vXZ);
    float angleXZ = XMVectorGetX(vAngleXZ);
    float u = angleXZ / XM_PI;

    float v = (floats.y + radius) / 2.f * radius;

    return XMVectorSet(u, v, 0, 0);
}

Icosahedron::Icosahedron(DirectX::FXMVECTOR color1, DirectX::FXMVECTOR color2,
                         int numTriangulations, float radius)
{
    std::vector<Triangle> triangles{};

    XMVECTOR frontTop = XMVectorSet(0, 1, -G, 1);
    XMVECTOR frontBottom = XMVectorSet(0, -1, -G, 1);
    XMVECTOR frontLeft = XMVectorSet(-G, 0, -1, 1);
    XMVECTOR frontRight = XMVectorSet(G, 0, -1, 1);

    XMVECTOR midBottomLeft = XMVectorSet(-1, -G, 0, 1);
    XMVECTOR midBottomRight = XMVectorSet(1, -G, 0, 1);
    XMVECTOR midTopRight = XMVectorSet(1, G, 0, 1);
    XMVECTOR midTopLeft = XMVectorSet(-1, G, 0, 1);

    XMVECTOR backLeft = XMVectorSet(-G, 0, 1, 1);
    XMVECTOR backRight = XMVectorSet(G, 0, 1, 1);
    XMVECTOR backTop = XMVectorSet(0, 1, G, 1);
    XMVECTOR backBottom = XMVectorSet(0, -1, G, 1);

    triangles.push_back(Triangle{frontBottom, frontRight, frontTop});
    triangles.push_back(Triangle{frontBottom, frontTop, frontLeft});
    triangles.push_back(Triangle{frontTop, midTopRight, midTopLeft});
    triangles.push_back(Triangle{frontBottom, midBottomLeft, midBottomRight});
    triangles.push_back(Triangle{frontBottom, midBottomRight, frontRight});
    triangles.push_back(Triangle{frontBottom, frontLeft, midBottomLeft});
    triangles.push_back(Triangle{frontTop, frontRight, midTopRight});
    triangles.push_back(Triangle{frontTop, midTopLeft, frontLeft});
    triangles.push_back(Triangle{midBottomLeft, frontLeft, backLeft});
    triangles.push_back(Triangle{midBottomRight, backRight, frontRight});
    triangles.push_back(Triangle{frontLeft, midTopLeft, backLeft});
    triangles.push_back(Triangle{frontRight, backRight, midTopRight});
    triangles.push_back(Triangle{backLeft, midTopLeft, backTop});
    triangles.push_back(Triangle{backRight, backTop, midTopRight});
    triangles.push_back(Triangle{backTop, midTopLeft, midTopRight});
    triangles.push_back(Triangle{midBottomLeft, backLeft, backBottom});
    triangles.push_back(Triangle{midBottomRight, backBottom, backRight});
    triangles.push_back(Triangle{backBottom, backLeft, backTop});
    triangles.push_back(Triangle{backBottom, backTop, backRight});
    triangles.push_back(Triangle{backBottom, midBottomRight, midBottomLeft});

    triangles = Triangle::Triangulate(triangles, 5);

    Noise noise{};

    int counter = 0;
    for (Triangle &triangle : triangles)
    {
        triangle.Normalize(radius);

        for (const XMVECTOR point : triangle.Points())
        {
            XMVECTOR texture = TextureCoordinates(point, radius);
            XMFLOAT4 textureFloats;
            XMStoreFloat4(&textureFloats, texture);
            float u = textureFloats.x;
            float v = textureFloats.y;

            double noiseInput = static_cast<double>(counter) / 10.f;
            float colorMod = static_cast<float>(noise(noiseInput, 1.2, 1.2));
            XMVECTOR color = lerp(color1, color2, colorMod);

            PositionColorTextureVertex vertex{point, color, u, v};
            m_vertices.Add(vertex);
        }

        counter++;
    }
}

} // namespace triangles