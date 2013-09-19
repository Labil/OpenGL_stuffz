#ifndef CUBE_H
#define CUBE_H

//Include OpenGL
#ifdef WIN32
#include <GL/glew.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif

//Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#ifndef BUFFER_OFFSET

    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

class Cube
{
    public:
        Cube(float size, std::string const vertexShader, std::string const fragmentShader);
        ~Cube();
        void display(glm::mat4 &projection, glm::mat4 &modelview);
        void load();
        void updateVBO(void *data, int byteSize, int offset); //data = a pointer to the data, for example an array. Type void because we don't necessarily know the type of data that we're sending
    protected:
        Shader mShader;
        float mVertices[108]; //3*36 (36 punkter i en kube)
        float mColors[108];
        GLuint mVBOid;
        int mSizeVerticeBytes;
        int mSizeColorBytes;
        GLuint mVAOid;
    private:
};

#endif // CUBE_H
