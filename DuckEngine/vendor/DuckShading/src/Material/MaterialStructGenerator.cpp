#include <ShaderGenerator/Materials/MaterialStructGenerator.hpp>

namespace sg
{
    BPhongMaterial MaterialStructGenerator::generateBPhongMaterial(const Material& material)
    {
        BPhongMaterial bphongMaterial;

        if(material.hasTexture("ambient"))
            bphongMaterial.ambient = MaterialProperty(material.getTexture("ambient"));
        else
            bphongMaterial.ambient = MaterialProperty(material.getVec3("ambient"));

        if (material.hasTexture("diffuse"))
            bphongMaterial.diffuse = MaterialProperty(material.getTexture("diffuse"));
        else
            bphongMaterial.diffuse = MaterialProperty(material.getVec3("diffuse"));

        if (material.hasTexture("specular"))
            bphongMaterial.specular = MaterialProperty(material.getTexture("specular"));
        else
            bphongMaterial.specular = MaterialProperty(material.getVec3("specular"));

        bphongMaterial.shininess = material.getFloat("shininess");

        bphongMaterial.hasAnyTexture = (material.hasTexture("ambient") || material.hasTexture("diffuse") || material.hasTexture("specular"));

        return bphongMaterial;
    }
}
