#pragma once

#include "stdafx.h"

#include "IndexedVertexBuffer.h"
#include "PositionColorTextureVertex.h"

namespace triangles
{

struct Icosahedron
{
    Icosahedron(DirectX::FXMVECTOR color1, DirectX::FXMVECTOR color2, int numTriangulations = 5,
                float radius = 1.f);

    inline const IndexedVertexBuffer<PositionColorTextureVertex, Uint32> &Vertices()
    {
        return m_vertices;
    }

  private:
    IndexedVertexBuffer<PositionColorTextureVertex, Uint32> m_vertices;
};

} // namespace triangles
