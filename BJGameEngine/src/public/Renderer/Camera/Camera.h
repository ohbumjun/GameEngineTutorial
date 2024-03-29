﻿#pragma once

#include "Hazel/Core/Serialization/SerializeTarget.h"
#include <glm/glm.hpp>

class Serializer;

namespace Hazel
{
class HAZEL_API Camera : public SerializeTarget
{
public:
    Camera() = default;
    Camera(const glm::mat4 &projection) : m_ProjectionMatrix(projection)
    {
    }
    virtual ~Camera() = default;

    virtual void Serialize(Serializer *serializer);
    virtual void Deserialize(Serializer *serializer);

    glm::mat4 &GetProjection()
    {
        return m_ProjectionMatrix;
    }
    const glm::mat4 &GetProjection() const
    {
        return m_ProjectionMatrix;
    }

protected:
    virtual void serializeData(Serializer *serializer);
    virtual void deserializeData(Serializer *serializer);

    glm::mat4 m_ProjectionMatrix = glm::mat4(1.f);
};
} // namespace Hazel
