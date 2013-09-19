#include "Cube.h"


Cube::Cube(float size, std::string const vertexShader, std::string const fragmentShader)
    : mShader(vertexShader, fragmentShader), mVBOid(0),
      mSizeVerticeBytes(108 * sizeof(float)), mSizeColorBytes(108 * sizeof(float)), mVAOid(0)
{
    //Load the shader
    mShader.load();


    // Del størrelsen på to, ellers blir den dobbelt så stor

    size /= 2;


    float verticesTmp[] = {-size, -size, -size,   size, -size, -size,   size, size, -size,     // Face 1
                           -size, -size, -size,   -size, size, -size,   size, size, -size,     // Face 1

                           size, -size, size,   size, -size, -size,   size, size, -size,       // Face 2
                           size, -size, size,   size, size, size,   size, size, -size,         // Face 2

                           -size, -size, size,   size, -size, size,   size, -size, -size,      // Face 3
                           -size, -size, size,   -size, -size, -size,   size, -size, -size,    // Face 3

                           -size, -size, size,   size, -size, size,   size, size, size,        // Face 4
                           -size, -size, size,   -size, size, size,   size, size, size,        // Face 4

                           -size, -size, -size,   -size, -size, size,   -size, size, size,     // Face 5
                           -size, -size, -size,   -size, size, -size,   -size, size, size,     // Face 5

                           -size, size, size,   size, size, size,   size, size, -size,         // Face 6
                           -size, size, size,   -size, size, -size,   size, size, -size};      // Face 6

    float colorsTmp[] =   {1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 1
                           1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 1

                           0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 2
                           0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 2

                           0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,           // Face 3
                           0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,           // Face 3

                           1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 4
                           1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 4

                           0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 5
                           0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 5

                           0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,           // Face 6
                           0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0};          // Face 6



    for(int i(0); i < 108; i++)
    {
        mVertices[i] = verticesTmp[i];
        mColors[i] = colorsTmp[i];
    }
}

Cube::~Cube()
{
    //Shaderen vil bli desallokert automatisk ettersom den er et eget objekt, så vi trenger ikke gjøre det her
    glDeleteBuffers(1, &mVBOid);
    glDeleteVertexArrays(1, &mVAOid);
}

void Cube::display(glm::mat4 &projection, glm::mat4 &modelview)
{
    glm::mat4 modelviewProjection = projection * modelview;

    glUseProgram(mShader.getProgramID());

        glBindVertexArray(mVAOid);

            //Sends the matrices that we got from the calling code
            glUniformMatrix4fv(glGetUniformLocation(mShader.getProgramID(), "modelviewProjection"), 1, GL_FALSE, glm::value_ptr(modelviewProjection));
           // glUniformMatrix4fv(glGetUniformLocation(mShader.getProgramID(), "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));

            //Display
            glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

    //Deaktiverer shader
    glUseProgram(0);

}

void Cube::load()
{
    //Primitives like float don't necessarily take up the same amount of space in
    //the VRAM (GPU memory) as in the RAM (CPU memory), so to transfer the variables
    //from one to the other, you have to allocate the right amount of space in bytes on
    //the VRAM.
    //Calculating how much space is needed for the two arrays we have in RAM:
    //int sizeVerticeBytes = 108 * sizeof(float); //sizeof gets us the size of the variable type
    //int sizeColorBytes = 108 * sizeof(float); (Se i konstruktøren i stedet, flyttet dit)

    //Makes sure to delete the buffer first if it is already created, or this one is lost in the VRAM
    if(glIsBuffer(mVBOid) == GL_TRUE)
        glDeleteBuffers(1, &mVBOid);

    glGenBuffers(1, &mVBOid); //Generates 1 id, and saves it in mVBOid

    glBindBuffer(GL_ARRAY_BUFFER, mVBOid); //To use the VBO, it must be bound, so that the graphics card knows where to store it in memory

        //Allocating space in VRAM
        //Type, size, data (we are not sending anything at the moment, because it's impossible to send
        //several arrays of data at once, and we have two), and usage (Will this be a static object that
        //only needs to be updated once, or a dynamic character that needs to be updated every frame?
        //GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW (the last one is updated every frame)
        glBufferData(GL_ARRAY_BUFFER, mSizeVerticeBytes + mSizeColorBytes, 0, GL_STATIC_DRAW);

        //Actual transfer! This function is slightly faster than the BindBuffer one.
        //Type, offset (where in memory to start the transfer), size, and data)
        glBufferSubData(GL_ARRAY_BUFFER, 0, mSizeVerticeBytes, mVertices);
        //Starts at the end of the vertices that was tranfered, so as not to overwrite the already transfered data
        glBufferSubData(GL_ARRAY_BUFFER, mSizeVerticeBytes, mSizeColorBytes, mColors);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if(glIsVertexArray(mVAOid) == GL_TRUE)
        glDeleteVertexArrays(1, &mVAOid);

    glGenVertexArrays(1, &mVAOid);

    glBindVertexArray(mVAOid);

        glBindBuffer(GL_ARRAY_BUFFER, mVBOid);
            //Sends the vertices to the shader
            //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, mVertices);

            //With VBO, the last parameter is the offset in VRAM
            //Since the vertices I uploaded to the VRAM in the load method
            //was at the beginning of the memory space, the offset is 0 for the vertices
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);

            //Sends the colors to the shader (w/o VBO)
            //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, mColors);

            //With VBO:
            //Gives the GPU the offset to the color array, here the size of the vertices that was placed before them
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mSizeVerticeBytes));
            glEnableVertexAttribArray(1);

            //Deactivate the arrays - don't need to, gets done when the VAO is unbound
            //glDisableVertexAttribArray(1); //Må disables i motsatt rekkefølge som de ble enabla
            //glDisableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Cube::updateVBO(void *data, int byteSize, int offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, mVBOid);


        //Getting the address of the VBO in the VRAM
        void* VBOaddress = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

        if(VBOaddress == NULL)
        {
            std::cout << "Error getting the VBO address" << std::endl;
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            return;
        }

        //Updating data
        memcpy((char*)VBOaddress + offset, data, byteSize);

        glUnmapBuffer(GL_ARRAY_BUFFER);
        VBOaddress = 0;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


















