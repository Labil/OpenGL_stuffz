#include "Square.h"

Square::Square(float size, std::string const vertexShader, std::string const fragmentShader, float r, float g, float b) : mShader(vertexShader, fragmentShader)
{
    mShader.load();
    mbInvisible = false;

    size /= 2;

    float verticesTemp[] = { -size,size,-size,  -size,-size,-size,  size,size,-size,
                             size,size,-size,   -size,-size,-size,  size,-size,-size };
    //Setting a default color for the square
    float colorTemp[] = { r,g,b,    r,g,b,  r,g,b,
                          r,g,b,    r,g,b,  r,g,b };

    for(int i(0); i < 18; i++)
    {
        mVertices[i] = verticesTemp[i];
        mColors[i] = colorTemp[i];
    }
}
Square::Square(): mShader("Shaders/couleur3D.vert", "Shaders/couleur3D.frag")
{

}

Square::~Square()
{

}

void Square::setColor(float r, float g, float b)
{
    float clr[] = { r,g,b,    r,g,b,    r,g,b,
                    r,g,b,    r,g,b,    r,g,b };

    for(int i(0); i < 18; i++)
        mColors[i] = clr[i];

}

void Square::display(glm::mat4 &projection, glm::mat4 &modelview)
{
    if(mbInvisible)
        return;
    glUseProgram(mShader.getProgramID());

        glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0, mVertices);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 0, mColors);
        glEnableVertexAttribArray(1);

        glUniformMatrix4fv(glGetUniformLocation(mShader.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(mShader.getProgramID(), "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

    glUseProgram(0);

}
