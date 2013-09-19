#include "Crate.h"

Crate::Crate(float size, std::string const vertexShader, std::string const fragmentShader, std::string const texture)
    : Cube(size, vertexShader, fragmentShader), mTexture(texture), mSizeTextureBytes(72 * sizeof(float))
{
    mTexture.load();

    float textureCoords[] = { 0,0,  1,0,   1,1,
                              0,0,  0,1,   1,1,

                              0,0,  1,0,   1,1,
                              0,0,  0,1,   1,1,

                              0,0,  1,0,   1,1,
                              0,0,  0,1,   1,1,

                              0,0,  1,0,   1,1,
                              0,0,  0,1,   1,1,

                              0,0,  1,0,   1,1,
                              0,0,  0,1,   1,1,

                              0,0,  1,0,   1,1,
                              0,0,  0,1,   1,1 };

        for(int i(0); i < 72; i++)
        {
            mTexCoords[i] = textureCoords[i];
        }
}

Crate::~Crate()
{
    //dtor
}

void Crate::display(glm::mat4 &projection, glm::mat4 &modelview)
{
    //Mer effektivt å regne det ut her enn i shaderen
    glm::mat4 modelviewProjection = projection * modelview;

    glUseProgram(mShader.getProgramID());

        glBindVertexArray(mVAOid);

            glUniformMatrix4fv(glGetUniformLocation(mShader.getProgramID(), "modelviewProjection"), 1, GL_FALSE, glm::value_ptr(modelviewProjection));
            //glUniformMatrix4fv(glGetUniformLocation(mShader.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            //glUniformMatrix4fv(glGetUniformLocation(mShader.getProgramID(), "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));

            glBindTexture(GL_TEXTURE_2D, mTexture.getID());

            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);


    glUseProgram(0);

}

void Crate::load()
{
    if(glIsBuffer(mVBOid) == GL_TRUE)
        glDeleteBuffers(1, &mVBOid);

    glGenBuffers(1, &mVBOid);

    glBindBuffer(GL_ARRAY_BUFFER, mVBOid);

        glBufferData(GL_ARRAY_BUFFER, mSizeVerticeBytes + mSizeTextureBytes, 0, GL_STATIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, mSizeVerticeBytes, mVertices);
        glBufferSubData(GL_ARRAY_BUFFER, mSizeVerticeBytes, mSizeTextureBytes, mTexCoords);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if(glIsVertexArray(mVAOid) == GL_TRUE)
        glDeleteVertexArrays(1, &mVAOid);

    glGenVertexArrays(1, &mVAOid);

    glBindVertexArray(mVAOid);

        glBindBuffer(GL_ARRAY_BUFFER, mVBOid);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mSizeVerticeBytes));
            glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

}








