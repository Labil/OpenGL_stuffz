#ifndef SQUARE_H
#define SQUARE_H

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

class Square
{
    public:
        Square(float size, std::string const vertexShader, std::string const fragmentShader, float r, float g, float b);
        Square();
        virtual ~Square();

        void display(glm::mat4 &projection, glm::mat4 &modelview);
        void setColor(float r, float g, float b);
        void setInvisible(bool b) { mbInvisible = b; }
    protected:
        float mVertices[18];
        float mColors[18];
        Shader mShader;
        bool mbInvisible;

    private:
};

#endif // SQUARE_H
