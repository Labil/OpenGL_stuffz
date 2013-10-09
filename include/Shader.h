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
        std::string getVertexShaderPath() { return mVertexSource; }
        bool compileShader(GLuint &shader, GLenum type, std::string const &sourcePath);
        bool load();
        void setUniform( const char *name, float x, float y, float z);
        void setUniform( const char *name, const glm::vec2 &v);
        void setUniform( const char *name, const glm::vec3 &v);
        void setUniform( const char *name, const glm::vec4 &v);
        void setUniform( const char *name, const glm::mat4 &m);
        void setUniform( const char *name, const glm::mat3 &m);
        void setUniform( const char *name, float val);
        void setUniform( const char *name, int val);
        void setUniform( const char *name, bool b);

    private:
        GLuint mVertexID;
        GLuint mFragmentID;
        GLuint mProgramID;

        std::string mVertexSource;
        std::string mFragmentSource;


};

#endif