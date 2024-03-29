#include "Renderer/Buffer/Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "hzpch.h"

namespace Hazel
{
Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        HZ_CORE_ASSERT(false, "no api set");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLVertexBuffer>(size);
    }

    HZ_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}
Ref<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t size)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        HZ_CORE_ASSERT(false, "no api set");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLVertexBuffer>(vertices, size);
    }

    HZ_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}
Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t count)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        HZ_CORE_ASSERT(false, "no api set");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLIndexBuffer>(indices, count);
    }

    HZ_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}
} // namespace Hazel
