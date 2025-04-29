#pragma once

#include "stdafx.h"

#include "IndexedVertexBuffer.h"
#include "PositionColorTextureVertex.h"

namespace triangles
{

struct SphereUV
{
    using VertexBuffer = IndexedVertexBuffer<PositionColorTextureVertex>;

    SphereUV();

    const VertexBuffer &GetVertexBuffer() const
    {
        return m_vertexBuffer;
    }

  private:
    VertexBuffer m_vertexBuffer;
};

} // namespace triangles
