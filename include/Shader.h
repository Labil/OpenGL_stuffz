#ifndef DEF_SHADER
#define DEF_SHADER

#ifdef WIN32
#include <GL/glew.h>

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif

#include <iostream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
    public:

        Shader();
        Shader(std::string vertexSource, std::string fragmentSource);
        ~Shader();
        Shader(Shader const &shaderToCopy);
        Shader& operator=(Shader const &shaderToCopy);

        void sendMatrix(std::string name, glm::mat4 matrix);
        GLuint getProgramID() const;
        bool compileShader(GLuint &shader, GLenum type, std::string const &sourcePath);
        bool load();

    private:
        GLuint mVertexID;
        GLuint mFragmentID;
        GLuint mProgramID;

        std::string mVertexSource;
        std::string mFragmentSource;


};

#endif