#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <glm/glm.hpp>



class Material
{
    public:
        Material(std::string const name, float r, float g, float b, float a = 0);
        Material(Material *mat);
        ~Material();
        void setColors(float r, float g, float b);
        void setAlpha(float a);
        const glm::vec4& getColors() { return mColor; }
        const glm::vec3& getDiffuse();
        std::string getMaterialName() { return mName; }
        int getMaterialTextureID() { return mTextureID; }
        void setMaterialTextureID(int id) { mTextureID = id; }
    protected:
    private:
        glm::vec4 mColor;
        std::string mName;
        int mTextureID;
};

#endif // MATERIAL_H
