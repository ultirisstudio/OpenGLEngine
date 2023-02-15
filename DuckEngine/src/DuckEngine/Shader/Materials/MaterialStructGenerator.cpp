////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "depch.h"

#include <DuckEngine/Shader/Materials/MaterialStructGenerator.h>

namespace DuckEngine
{
    BPhongMaterial MaterialStructGenerator::generateBPhongMaterial(const Material& material)
    {
        BPhongMaterial bphongMaterial;

        if (material.hasTexture("ambient"))
            bphongMaterial.ambient = MaterialProperty(material.getTexture("ambient").get());
        else
            bphongMaterial.ambient = MaterialProperty(material.getVec3("ambient").get());

        if (*material.getBoolean("diffuse"))
        {
            if (material.hasTexture("diffuse"))
                bphongMaterial.diffuse = MaterialProperty(material.getTexture("diffuse").get());
        }
        else
        {
            if (material.hasVec3("diffuse"))
                bphongMaterial.diffuse = MaterialProperty(material.getVec3("diffuse").get());
        }

        if (*material.getBoolean("specular"))
        {
            if (material.hasTexture("specular"))
                bphongMaterial.specular = MaterialProperty(material.getTexture("specular").get());
        }
        else
        {
            if (material.hasVec3("specular"))
                bphongMaterial.specular = MaterialProperty(material.getVec3("specular").get());
        }

        bphongMaterial.shininess = material.getFloat("shininess").get();

        bphongMaterial.hasAnyTexture = (material.hasTexture("ambient") || material.hasTexture("diffuse") || material.hasTexture("specular"));

        return bphongMaterial;
    }
}
