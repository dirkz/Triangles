#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale
#include <glm/mat4x4.hpp>                // glm::mat4
#include <glm/vec4.hpp>                  // glm::vec4

#include <DirectXMath.h>

#include <chrono>
#include <iostream>

using namespace DirectX;
using namespace std::chrono;
using namespace std;

uint64_t NumIterations = 1'000'000;

static glm::mat4x4 GLMMatrix()
{
    glm::mat4x4 m{1.f};

    for (uint64_t i = 0; i < NumIterations; ++i)
    {
        glm::mat4x4 m1 = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 10.f);
        glm::mat4x4 m2 = glm::rotate(m1, glm::radians(45.f), glm::vec3{1.f, 0.f, 0.f});
        glm::mat4x4 m3 = glm::inverse(m2);
        glm::mat4x4 m4 = m3 * m2;
        m = m4;
    }

    return m;
}

static glm::vec4 GLMMatrixVector()
{
    glm::mat4x4 m = glm::rotate(glm::mat4x4{1.f}, glm::radians(45.f), glm::vec3{1.f, 0.f, 0.f});
    glm::vec4 v{0.f, 0.f, 0.f, 0.f};

    for (uint64_t i = 0; i < NumIterations; ++i)
    {
        v = m * v;
    }

    return v;
}

static XMMATRIX DirectXMatrix()
{
    XMMATRIX m = XMMatrixIdentity();

    for (uint64_t i = 0; i < NumIterations; ++i)
    {
        XMMATRIX m1 = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.f), 1.f, 0.1f, 10.f);
        XMMATRIX m2 =
            XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(45.f));
        XMMATRIX m3 = XMMatrixInverse(nullptr, m2);
        XMMATRIX m4 = XMMatrixMultiply(m3, m2);
        m = m4;
    }

    return m;
}

static XMVECTOR DirectXMatrixVector()
{
    XMMATRIX m = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(45.f));
    XMVECTOR v = XMVectorSet(0.f, 0.f, 0.f, 0.f);

    for (uint64_t i = 0; i < NumIterations; ++i)
    {
        v = XMVector4Transform(v, m);
    }

    return v;
}

template <class T, class F> void Time(const string &name, F fn)
{
    auto t1 = high_resolution_clock::now();
    T t = fn();
    auto t2 = high_resolution_clock::now();
    duration<double> time = duration_cast<duration<double>>(t2 - t1);
    cout << name << ":\t" << time << "\n";
}

int main()
{
    cout << "Matrix multiplication\n";
    cout << "=====================\n\n";

    auto directxFn1 = [] { return DirectXMatrix(); };
    Time<XMMATRIX, decltype(directxFn1)>("DXMath", directxFn1);

    auto glmFn1 = [] { return GLMMatrix(); };
    Time<glm::mat4x4, decltype(glmFn1)>("GLM", glmFn1);

    cout << "\nVector transformation\n";
    cout << "=====================\n\n";

    auto directxFn2 = [] { return DirectXMatrixVector(); };
    Time<XMVECTOR, decltype(directxFn2)>("DXMath", directxFn2);

    auto glmFn2 = [] { return GLMMatrixVector(); };
    Time<glm::vec4, decltype(glmFn2)>("GLM", glmFn2);

    return 0;
}