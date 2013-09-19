#include "Cabin.h"

Cabin::Cabin(float size, std::string const texture, float const texRepX, float const texRepY, std::string const vertexShader, std::string const fragmentShader)
    : mTexRepX(texRepX), mTexRepY(texRepY), mShader(vertexShader, fragmentShader), mTexture(texture)
{
    mShader.load();
    mTexture.load();

    size /= 2;

    float verts[] = { -size, size/2, -size,   -size, - size/2, -size,   -size, size/2, size,  //Left wall
                      -size, size/2, size,    -size, -size/2, -size,    -size, -size/2, size, //Left wall

                      -size, size/2, size,    -size, -size/2, size,     size, size/2, size, //Back wall
                      size, size/2, size,     -size, -size/2, size,     size, -size/2, size, //back wall

                      size, size/2, size,     size, -size/2, size,      size, -size/2, -size, //Right wall
                      size, -size/2, -size,   size, size/2, size,       size, size/2, -size,  //Right wall

                      size, size/2, -size,    0, size, -size,       -size, size/2, -size, //Front triangle

                      size, size/2, size,    0, size, size,       -size, size/2, size, //Back triangle

                      0, size, -size,    size, size/2, -size,     0, size, size,  //Roof right
                      0, size, size,    size, size/2, -size,      size, size/2, size, //roof right

                      0, size, size,    -size, size/2, size,      0, size, -size, //roof left
                      0, size, -size,   -size, size/2, size,      -size, size/2, -size //roof left
                      };



    float tex[] = { 0, texRepY,    0, 0,    texRepX, texRepY,
                    texRepX, texRepY,   0, 0,   texRepX, 0,

                    0, texRepY,     0,0,    texRepY, texRepY,
                    texRepX, texRepY,   0, 0,   texRepX, 0,

                    0, texRepY,    0, 0,    texRepX, 0,
                    texRepX, 0,     0, texRepY,     texRepY, texRepY
                     };

    for(int i(0); i < 108; i++)
        mVertices[i] = verts[i];

    for(int i(0); i < 36; i++)
        mTexCoords[i] = tex[i];
}

Cabin::~Cabin()
{

}

void Cabin::display(glm::mat4 &projection, glm::mat4 &modelview)
{
    glUseProgram(mShader.getProgramID());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, mVertices);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, mTexCoords);
        glEnableVertexAttribArray(2);

        glUniformMatrix4fv(glGetUniformLocation(mShader.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(mShader.getProgramID(), "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));

        glBindTexture(GL_TEXTURE_2D, mTexture.getID());

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindTexture(GL_TEXTURE_2D, 0);

        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(0);

    glUseProgram(0);
}
