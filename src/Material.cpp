#include "Material.h"

Material::Material(std::string const name, float r, float g, float b, float a)
    : mName(name), mTextureID(0)
{
    mColor.x = r;
    mColor.y = g;
    mColor.z = b;
    mColor.w = a;
}

Material::Material(Material *mat)
{
    mColor = mat->mColor;
    mName = mat->mName;
}

void Material::setColors(float r, float g, float b)
{
    mColor.x = r;
    mColor.y = g;
    mColor.z = b;
}
void Material::setAlpha(float a)
{
    mColor[3] = a;
}

const glm::vec3& Material::getDiffuse()
{
    glm::vec3 col(mColor.x, mColor.y, mColor.z);
    return col;
}

Material::~Material()
{
    //dtor
}
