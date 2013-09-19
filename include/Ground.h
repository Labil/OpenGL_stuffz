#ifndef GROUND_H
#define GROUND_H

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

class Ground
{
    public:
        Ground(float size, std::string const texture, float const texRepX, float const texRepY, std::string const vertexShader, std::string const fragmentShader);
        virtual ~Ground();

        void display(glm::mat4 &projection, glm::mat4 &modelview);

    protected:
    private:
        float mVertices[18];
        float mTexCoords[12];
        float mTexRepX;
        float mTexRepY;
        Shader mShader;
        Texture mTexture;
};

#endif // GROUND_H
