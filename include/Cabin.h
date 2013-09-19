#ifndef CABIN_H
#define CABIN_H

//Include OpenGL
#ifdef WIN32
#include <GL/glew.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif

//Include GLM math lib
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"

class Cabin
{
    public:
        Cabin(float size, std::string const texture, float const texRepX, float const texRepY, std::string const vertexShader, std::string const fragmentShader);
        virtual ~Cabin();
        void display(glm::mat4 &projection, glm::mat4 &modelview);

    protected:
    private:
        float mVertices[108];
        float mTexCoords[36];
        float mTexRepX;
        float mTexRepY;
        Shader mShader;
        Texture mTexture;
};

#endif // CABIN_H
