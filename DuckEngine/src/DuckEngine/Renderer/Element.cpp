#include "depch.h"
#include <DuckEngine/Renderer/Element.h>

namespace DuckEngine
{
    Element::Element(Material& material, Shader& shader) :
        Transformable(),
        Drawable(material, shader)
    {

    }

    Element::~Element()
    {

    }
}