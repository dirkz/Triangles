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

static glm::mat4x4 GLMMatrixOperations()
{
    glm::mat4x4 m;

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

static XMMATRIX DirectXMatrixOperations()
{
    XMMATRIX m;

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

template <class F, class T> void Time(const string &name, F fn)
{
    auto t1 = high_resolution_clock::now();
    T t = fn();
    auto t2 = high_resolution_clock::now();
    duration<double> time = duration_cast<duration<double>>(t2 - t1);
    cout << name << ": " << time << "\n";
}

int main()
{
    auto directxFn1 = [] { return DirectXMatrixOperations(); };
    Time<decltype(directxFn1), XMMATRIX>("DirectX", directxFn1);

    auto glmFn1 = [] { return GLMMatrixOperations(); };
    Time<decltype(glmFn1), glm::mat4x4>("GLM", glmFn1);

    return 0;
}