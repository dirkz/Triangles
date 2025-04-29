#pragma once

#include "stdafx.h"

namespace triangles
{

template <class V, class I = Uint16, class Hash = std::hash<V>, class KeyEqual = std::equal_to<V>>
struct IndexedVertexBuffer
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
            m_mapVertexIndex.insert(std::make_pair(vertex, index));
            m_vertices.push_back(vertex);
            m_indices.push_back(index);
        }
    }

    void Quad(const V &bottomLeft, const V &bottomRight, const V &topRight, const V &topLeft)
    {
        Add(bottomLeft);
        Add(bottomRight);
        Add(topRight);
        Add(bottomLeft);
        Add(topRight);
        Add(topLeft);
    }

    I Add(const IndexedVertexBuffer<V, I, Hash, KeyEqual> &vertexBuffer)
    {
        I currentIndex = static_cast<I>(m_indices.size());
        I vertexIndex = static_cast<I>(m_vertices.size());

        for (const V &vertex : vertexBuffer.m_vertices)
        {
            m_vertices.push_back(vertex);
        }

        for (const I index : vertexBuffer.m_indices)
        {
            m_indices.push_back(index + currentIndex);
        }

        return currentIndex;
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
    std::unordered_map<V, I, Hash, KeyEqual> m_mapVertexIndex;
    std::vector<V> m_vertices;
    std::vector<I> m_indices;
};

} // namespace triangles
