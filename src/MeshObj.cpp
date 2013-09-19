#include "MeshObj.h"

#include <fstream>
#include <stdlib.h>
#include <iostream>

MeshObj::MeshObj(std::string const meshName, std::string const vertexShader, std::string const fragmentShader, int *firstFrame)
    : mNormals(0), mVertices(0), mTextures(0), mColors(0), mMaterials(0), mData(0), mTexture(0), mbIsQuadBased(false),
      mShader(vertexShader, fragmentShader), mVBOid(0), mVAOid(0), mSizeVerticeBytes(0), mSizeColorBytes(0), mSizeNormalBytes(0)
{
    mShader.load();
    loadObject(meshName, firstFrame);
}

MeshObj::~MeshObj()
{
    glDeleteBuffers(1, &mVBOid);
    glDeleteVertexArrays(1, &mVAOid);

    free(mVertices);
    free(mTextures);
    free(mColors);
    free(mNormals);

    for(unsigned int i(0); i < mMaterials.size(); i++)
        delete mMaterials[i];
    mMaterials.clear();
}

void MeshObj::loadObject(std::string const meshName, int *firstFrame)
{
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec4> colors;
    std::vector<int> indiceVert, indiceTex, indiceNorm;
    int numDrawCallsMultiplier(0);

    std::ifstream file(meshName.c_str(), std::ios::in);

    std::string line;
    std::string matName = "";

    if(file)
    {
        while(std::getline(file, line))
        {
            if(line[0] == 'v') //vertex, texture or normal
            {
                if(line[1] == ' ') //vertex
                {
                    float x(0), y(0), z(0);
                    sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
                    vertices.push_back(glm::vec3(x, y, z));
                }
                else if(line[1] == 't') //texture coord
                {
                    float x(0), y(0);
                    sscanf(line.c_str(), "vt %f %f", &x, &y);
                    texCoords.push_back(glm::vec2(x, y));
                }
                else if(line[1] == 'n') //normal
                {
                    float x(0), y(0), z(0);
                    sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
                    normals.push_back(glm::vec3(x, y, z));
                }

            }
            else if(line[0] == 'f') //Faces
            {
                line = replaceDoubleSlash(line); //Hvis ikke teksturcoordinater er oppgitt, erstattes // med /1/
                line = replaceSlash(line);

                std::vector<std::string> faceData = splitSpace(line.substr(2));
                int numFaceData=(int)faceData.size();
                //std::cout << numFaceData << std::endl;
                if(numFaceData == 12)
                {
                    numDrawCallsMultiplier = 4;
                    mbIsQuadBased = true;
                    for(int i=0;i< 4;i++)
                    {
                        indiceVert.push_back(atoi(faceData[i*3].c_str())-1); //-1 pga .obj er index 1-basert, mens c++ er 0-basert
                        indiceTex.push_back(atoi(faceData[i*3+1].c_str())-1);
                        indiceNorm.push_back(atoi(faceData[i*3+2].c_str())-1);
                    }
                }
                else if(numFaceData == 9)
                {
                    numDrawCallsMultiplier = 3;
                    mbIsQuadBased = false;
                    for(int i=0;i< 3;i++)
                    {
                        indiceVert.push_back(atoi(faceData[i*3].c_str())-1); //-1 pga .obj er index 1-basert, mens c++ er 0-basert
                        indiceTex.push_back(atoi(faceData[i*3+1].c_str())-1);
                        indiceNorm.push_back(atoi(faceData[i*3+2].c_str())-1);
                    }
                }
                else if(numFaceData == 4) //When there is no texture coord or normal, and there are 4 indices, that means it's a quad -> se under display, der tegner den enten triangler eller quads
                {
                    numDrawCallsMultiplier = numFaceData;
                    mbIsQuadBased = true;
                    for(int i=0;i< numFaceData;i++)
                    {
                        indiceVert.push_back(atoi(faceData[i].c_str())-1);
                    }
                }
                else if(numFaceData == 3)
                {
                    numDrawCallsMultiplier = numFaceData;
                    mbIsQuadBased = false;
                    for(int i=0;i< numFaceData;i++)
                    {
                        indiceVert.push_back(atoi(faceData[i].c_str())-1);
                    }
                }
                else
                {
                    std::cout << "Error: The .obj has got the wrong number of face indices." << std::endl;
                    std::cout << "Remember to export with normals, and as triangles for best result." << std::endl;
                    return;
                }

                for(unsigned int i(0); i < mMaterials.size(); i++)
                {
                    if(mMaterials[i]->getMaterialName() == matName)
                    {
                        glm::vec4 getcol = mMaterials[i]->getColors();
                        for(int j(0); j < 4; j++)
                            colors.push_back(getcol);
                    }
                }
            }
            else if(line[0] == 'm') //.mtl
            {
                loadMaterial(getDirectory(meshName) + line.substr(7));
            }
            else if(line[0] == 'u') //use material
                matName = line.substr(7);
        }
        file.close();

        std::vector<float> tv(0), tc(0), tn(0), tt(0);

        for(int i(0); i < indiceVert.size(); i++)
        {
            if(indiceVert[i] < vertices.size())
            {
                tv.push_back(vertices[indiceVert[i]].x);
                tv.push_back(vertices[indiceVert[i]].y);
                tv.push_back(vertices[indiceVert[i]].z);

                if(colors.size() > 0)
                {
                    tc.push_back(colors[i].x);
                    tc.push_back(colors[i].y);
                    tc.push_back(colors[i].z);
                   // tc.push_back(colors[i].w);
                }
                else
                    std::cout << "No colors" << std::endl;

            }
        }
        for(unsigned int i(0); i < indiceNorm.size(); i++)
        {
            if(indiceNorm[i] < normals.size())
            {
                tn.push_back(normals[indiceNorm[i]].x);
                tn.push_back(normals[indiceNorm[i]].y);
                tn.push_back(normals[indiceNorm[i]].z);
            }
        }
        for(unsigned int i(0); i < indiceTex.size(); i++)
        {
            if(indiceTex[i] < texCoords.size())
            {
                tt.push_back(texCoords[indiceTex[i]].x);
                tt.push_back(texCoords[indiceTex[i]].y);
            }
        }

        mNumPoints = indiceVert.size() * numDrawCallsMultiplier;
        mVertices = vectorToFloat(tv);
        mColors = vectorToFloat(tc);
        mTextures = vectorToFloat(tt);
        mNormals = vectorToFloat(tn);
        mSizeVerticeBytes = mNumPoints * sizeof(float);
        mSizeColorBytes = mNumPoints * sizeof(float);
        mSizeNormalBytes = mNumPoints * sizeof(float);
    }
    else
    {
        std::cout << "The .obj file was not found, or there was an error reading it." << std::endl;
        return;
    }
    load();

    vertices.clear();
    normals.clear();
    texCoords.clear();
    colors.clear();

    indiceVert.clear();
    indiceTex.clear();
    indiceNorm.clear();

}

void MeshObj::load()
{
    if(glIsBuffer(mVBOid) == GL_TRUE)
        glDeleteBuffers(1, &mVBOid);

    glGenBuffers(1, &mVBOid);

    glBindBuffer(GL_ARRAY_BUFFER, mVBOid);

        glBufferData(GL_ARRAY_BUFFER, mSizeVerticeBytes + mSizeColorBytes + mSizeNormalBytes, 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, mSizeVerticeBytes, mVertices);
        glBufferSubData(GL_ARRAY_BUFFER, mSizeVerticeBytes, mSizeColorBytes, mColors);
        glBufferSubData(GL_ARRAY_BUFFER, mSizeVerticeBytes + mSizeColorBytes, mSizeNormalBytes, mNormals);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if(glIsVertexArray(mVAOid) == GL_TRUE)
        glDeleteVertexArrays(1, &mVAOid);

    glGenVertexArrays(1, &mVAOid);

    glBindVertexArray(mVAOid);

        glBindBuffer(GL_ARRAY_BUFFER, mVBOid);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mSizeVerticeBytes));
            glEnableVertexAttribArray(1);

            //TODO: textures (2)

            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mSizeVerticeBytes + mSizeColorBytes));
            glEnableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void MeshObj::loadMaterial(std::string const fileName)
{
    std::ifstream file(fileName.c_str(), std::ios::in);
    std::string matName = "";

    if(file)
    {
        std::string line="";
        while(getline(file, line))
        {
            if(line[0] == 'n') //new mat
                matName = line.substr(7);
            else if(line[0] == 'K' && line[1] == 'd') //diffuse
            {
                float r(0.5f), g(0.5f), b(0.5f);
                sscanf(line.c_str(), "Kd %f %f %f", &r, &g, &b);
                mMaterials.push_back(new Material(matName, r, g, b));
            }
            else if(line[0] == 'm' && line[5] == 'd') //map_Kd (texture)
            {
                //std::string texturePath= getDirectory(fileName) + line.substr(7);
                std::cout << "Remember to implement textures in the parser!" << std::endl;
                //mTexture = new Texture(texturePath);
            }
            else if(line[0] == 'd') //Opacity
            {
                float alpha = atoi(line.substr(2).c_str());
                mMaterials[mMaterials.size()-1]->setAlpha(alpha);
            }
        }
        file.close();
    }
    else
    {
        std::cout << "Error reading .mat file" << std::endl;
        return;
    }
}


void MeshObj::display(glm::mat4 &projection, glm::mat4 &modelview, bool normal, bool texture)
{
    glm::mat4 modelviewProjection = projection * modelview;

    glUseProgram(mShader.getProgramID());

        glBindVertexArray(mVAOid);

            glUniformMatrix4fv(glGetUniformLocation(mShader.getProgramID(), "modelviewProjection"), 1, GL_FALSE, glm::value_ptr(modelviewProjection));
            if(mbIsQuadBased)
                glDrawArrays(GL_QUADS, 0, mNumPoints);
            else
                glDrawArrays(GL_TRIANGLES, 0, mNumPoints);

        glBindVertexArray(0);

    glUseProgram(0);
}

float* MeshObj::vectorToFloat(std::vector<float>& vec)
{
    float* t = NULL;
    t = (float*)malloc(vec.size()*sizeof(float));

    if(t == NULL || vec.empty())
    {
        float *t1 = (float*)malloc(sizeof(float) * 3);
        for(int i(0); i < 3; i++)
            t1[i] = 0;
        return t1;
    }

    for(int i(0); i < vec.size(); i++)
        t[i] = vec[i];
    return t;
}

std::string MeshObj::replaceDoubleSlash(std::string const s)
{
    //Replace "//" with "/1/"

    std::string newStr = "";
    for(int i=0; i < s.size(); i++)
    {
        if(i < s.size()-1 && s[i] == '/' && s[i+1] == '/')
        {
            newStr += "/1/";
            i++;
        }
        else
            newStr += s[i];
    }
    return newStr;
}

std::string MeshObj::replaceSlash(std::string const s)
{
    std::string ret="";
    for(unsigned int i=0;i<s.size();i++)
    {
        if(s[i]=='/')
            ret+=' ';
        else
            ret+=s[i];
    }
    return ret;
}
std::vector<std::string> MeshObj::splitSpace(std::string const s)
{
    std::vector<std::string> ret;
    std::string s1="";
    for(unsigned int i=0;i<s.size();i++)
    {
        if(s[i]==' '||i==s.size()-1)
        {
            if(i==s.size()-1)
                s1+=s[i];
            ret.push_back(s1);
            s1="";
        }
        else
            s1+=s[i];
    }
    return ret;
}

std::string MeshObj::getDirectory(std::string const s)
{
    std::string s1="", s2="";
    for(int i(0); i < s.size(); i++)
    {
        if(s[i] == '/' || s[i] == '\\')
        {
            s1 += s2+"/";
            s2="";
        }
        else
            s2 += s[i];
    }
    std::string temp = "The directory: ";
    temp += s1;
    std::cout << temp;
    return s1;
}


