#include "Material.h"

Material::Material(std::string const name, float r, float g, float b, float a)
    : mName(name)
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

Material::~Material()
{
    //dtor
}
