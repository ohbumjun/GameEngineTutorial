﻿#pragma once

#include "hzpch.h"

#include "Hazel/Core/Core.h"
#include "Hazel/Event/Event.h"

// Platform Dependent
namespace Hazel
{
struct WindowProps
{
    std::string Title;
    uint32_t Width;
    uint32_t Height;
    HINSTANCE m_Inst;

    WindowProps(const std::string &title = "Hazel Engine",
                uint32_t width = 1200,
                uint32_t height = 720)
        : Title(title), Width(width), Height(height)
    {
    }
};

// Interface representing a desktop system based window
class HAZEL_API Window
{
public:
    using EventCallbackFn = std::function<void(Event &)>;
    virtual ~Window()
    {
    }
    virtual void OnUpdate() = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    // Window Attributes
    virtual void SetEventCallback(const EventCallbackFn &callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    // ex) OpenGL -> return GLFW Window
    virtual void *GetNativeWindow() const = 0;

    static Window *Create(const WindowProps &props = WindowProps());
};

} // namespace Hazel