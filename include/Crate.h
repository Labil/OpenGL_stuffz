#ifndef CRATE_H
#define CRATE_H

#include "Cube.h"
#include "Texture.h"
#include <string>


class Crate : public Cube
{
    public:
        Crate(float size, std::string const vertexShader, std::string const fragmentShader, std::string const texture);
        ~Crate();
        void display(glm::mat4 &projection, glm::mat4 &modelview);
        void load();
    protected:
    private:
        Texture mTexture;
        float mTexCoords[72];
        int mSizeTextureBytes;
};

#endif // CRATE_H
