#pragma once

#include <DuckEngine/Renderer/Drawable.h>
#include <DuckEngine/Renderer/Transformable.h>

namespace DuckEngine
{
    class Element : public Transformable, public Drawable
    {
    private:

    public:
        Element(Material& material, Shader& shader);
        virtual ~Element();
    };
}