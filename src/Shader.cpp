#include "Shader.h"

#include <iostream>

Shader::Shader()
    : mVertexID(0), mFragmentID(0), mProgramID(0), mVertexSource(), mFragmentSource()
{
}

Shader::Shader(std::string vertexSource, std::string fragmentSource)
    : mVertexID(0), mFragmentID(0), mProgramID(0), mVertexSource(vertexSource), mFragmentSource(fragmentSource)
{
}


Shader::~Shader()
{
    glDeleteShader(mVertexID);
    glDeleteShader(mFragmentID);
    glDeleteProgram(mProgramID);
}

Shader::Shader(Shader const &shaderToCopy)
{
    mVertexSource = shaderToCopy.mVertexSource;
    mFragmentSource = shaderToCopy.mFragmentSource;

    load();
}

GLuint Shader::getProgramID() const
{
    return mProgramID;
}

bool Shader::load()
{
    //This function compiles both the vertex shader and the fragment shader,
    //and unites them into a single program

    if(glIsShader(mVertexID) == GL_TRUE)
        glDeleteShader(mVertexID);
    if(glIsShader(mFragmentID) == GL_TRUE)
        glDeleteShader(mFragmentID);
    if(glIsProgram(mProgramID) == GL_TRUE)
        glDeleteProgram(mProgramID);

    if(!compileShader(mVertexID, GL_VERTEX_SHADER, mVertexSource))
        return false;

    if(!compileShader(mFragmentID, GL_FRAGMENT_SHADER, mFragmentSource))
        return false;

    mProgramID = glCreateProgram();

    glAttachShader(mProgramID, mVertexID);
    glAttachShader(mProgramID, mFragmentID);

    glBindAttribLocation(mProgramID, 0, "in_Vertex");
    glBindAttribLocation(mProgramID, 1, "in_Color");
    glBindAttribLocation(mProgramID, 2, "in_TexCoord0"); //First texture to be bound (when we are using multiple textures, it can be 1,2,3)
    glBindAttribLocation(mProgramID, 3, "in_Normals");
    glLinkProgram(mProgramID);

    GLint linkError(0);
    glGetProgramiv(mProgramID, GL_LINK_STATUS, &linkError);

    if(linkError != GL_TRUE)
    {
        GLint errorSize(0);
        glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &errorSize);

        char* errorString = new char[errorSize + 1];
        glGetProgramInfoLog(mProgramID, errorSize, &errorSize, errorString);
        errorString[errorSize] = '\0';

        std::cout << "Error linking the shader program: " << errorString << std::endl;

        delete[] errorString;
        glDeleteProgram(mProgramID);

        return false;
    }

    else
        return true;

}

bool Shader::compileShader(GLuint &shader, GLenum type, std::string const &sourcePath)
{
    //Generates shader ID, reads the source code, compiles it

    shader = glCreateShader(type);
    if(shader == 0)
    {
        std::cout << "Error, the type of shader ("<< type << ") does not exist" << std::endl;
        return false;
    }

    std::ifstream file(sourcePath.c_str());

    if(!file)
    {
        std::cout << "Error reading the file path " << sourcePath << ". Path doesn't exist" << std::endl;
        glDeleteShader(shader);
        return false;
    }

    std::string line;
    std::string sourceCode;

    while(getline(file, line))
        sourceCode += line + '\n';

    file.close();

    const GLchar* sourceCodeCString = sourceCode.c_str();

    //Sends the source code to the shader
    glShaderSource(shader, 1, &sourceCodeCString, 0);
    glCompileShader(shader);

    GLint compileError(0);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileError);

    if(compileError != GL_TRUE)
    {
        GLint sizeError(0);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &sizeError);

        //Allocating memory for the error string to be
        char* errorString2 = new char[sizeError + 1]; //+ 1 for the end of the character chain '\0'
        glGetShaderInfoLog(shader, sizeError, &sizeError, errorString2);
        errorString2[sizeError] = '\0';

        std::cout << errorString2 << std::endl;

        delete[] errorString2;
        glDeleteShader(shader);

        return false;
    }
    else
        return true;

}

void Shader::sendMatrix(std::string name, glm::mat4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(mProgramID, name.c_str()),1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform(const char *name, float x, float y, float z)
{
    int loc =  glGetUniformLocation(mProgramID, name);
    if(loc >= 0)
        glUniform3f(loc, x, y, z);
    else
        printf("Uniform %s not found.\n", name);
}

void Shader::setUniform(const char *name, const glm::vec3 &v)
{
    this->setUniform(name, v.x, v.y, v.z);
}

void Shader::setUniform(const char *name, const glm::vec4 &v)
{
    int loc = glGetUniformLocation(mProgramID, name);
    if(loc >= 0)
        glUniform4f(loc, v.x, v.y, v.z, v.w);
    else
        printf("Uniform %s not found.\n", name);
}

void Shader::setUniform(const char *name, const glm::vec2 &v)
{
    int loc = glGetUniformLocation(mProgramID, name);
    if(loc >= 0)
        glUniform2f(loc, v.x, v.y);
    else
        printf("Uniform %s not found.\n", name);
}

void Shader::setUniform(const char *name, const glm::mat4 &m)
{
    int loc = glGetUniformLocation(mProgramID, name);
    if(loc >= 0)
        glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
    else
        printf("Uniform %s not found.\n", name);
}

void Shader::setUniform(const char *name, const glm::mat3 &m)
{
    int loc = glGetUniformLocation(mProgramID, name);
    if(loc >= 0)
        glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
    else
        printf("Uniform %s not found\n", name);
}

void Shader::setUniform(const char *name, float val)
{
    int loc = glGetUniformLocation(mProgramID, name);
    if(loc >= 0)
        glUniform1f(loc, val);
    else
        printf("Uniform %s not found.\n", name);
}

void Shader::setUniform(const char *name, int val)
{
    int loc = glGetUniformLocation(mProgramID, name);
    if(loc >= 0)
        glUniform1i(loc, val);
    else
        printf("Uniform %s not found.\n", name);
}

void Shader::setUniform(const char *name, bool b)
{
    int loc = glGetUniformLocation(mProgramID, name);
    if(loc >= 0)
        glUniform1i(loc, b);
    else
        printf("Uniform %s not found.\n", name);
}

Shader& Shader::operator=(Shader const &shaderToCopy)
{
    mVertexSource = shaderToCopy.mVertexSource;
    mFragmentSource = shaderToCopy.mFragmentSource;

    load();

    return *this; //Returns the object
}


