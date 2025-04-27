#pragma once

#include "stdafx.h"

namespace triangles
{

template <class V, class I = Uint16, class Compare = std::less<V>> struct IndexedVertexBuffer
{
    void Add(const V &vertex)
    {
        if (m_mapVertexIndex.contains(vertex))
        {
            I index = m_mapVertexIndex[vertex];
            m_indices.push_back(index);
        }
        else
        {
            I index = static_cast<I>(m_vertices.size());
            m_vertices.push_back(vertex);
            m_indices.push_back(index);
        }
    }

    const std::vector<I> &Indices() const
    {
        return m_indices;
    }

    const std::vector<V> &Vertices() const
    {
        return m_vertices;
    }

  private:
    std::map<V, I, Compare> m_mapVertexIndex;
    std::vector<V> m_vertices;
    std::vector<I> m_indices;
};

} // namespace triangles
