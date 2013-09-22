#include "MeshObj.h"

#include <fstream>
#include <stdlib.h>
#include <iostream>

MeshObj::MeshObj(std::string const meshName, std::string const vertexShader, std::string const fragmentShader, int *firstFrame)
    : mNormals(0), mVertices(0), mTextureCoords(0), mColors(0), mMaterials(0), mTextures(0), mData(0), mTexture(), mbIsQuadBased(false), mbHasNormals(false), mbHasTextureCoords(false),
      mShader(vertexShader, fragmentShader), mVBOid(0), mVAOid(0), mSizeVerticeBytes(0), mSizeColorBytes(0), mSizeNormalBytes(0), mNumElementsPerMat(0), mOrderOfMaterials(0)
{
    mShader.load();
    loadObject(meshName, firstFrame);
}

MeshObj::~MeshObj()
{
    glDeleteBuffers(1, &mVBOid);
    glDeleteVertexArrays(1, &mVAOid);

    free(mVertices);
    free(mTextureCoords);
    free(mColors);
    free(mNormals);

    for(unsigned int i(0); i < mMaterials.size(); i++)
        delete mMaterials[i];
    mMaterials.clear();

    for(unsigned int i(0); i < mTextures.size(); i++)
        delete mTextures[i];
    mTextures.clear();
}

void MeshObj::loadObject(std::string const meshName, int *firstFrame)
{
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec4> colors;
    std::vector<int> indiceVert, indiceTex, indiceNorm;

    std::ifstream file(meshName.c_str(), std::ios::in);

    std::string line;
    std::string matName = "";
    int curMatIndex(-1);

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
                int colPerFaceData(0);
                //std::cout << numFaceData << std::endl;

                if(numFaceData == 12)
                {
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
                    mbIsQuadBased = false;
                    for(int i=0;i< 3;i++)
                    {
                        indiceVert.push_back(atoi(faceData[i*3].c_str())-1); //-1 pga .obj er index 1-basert, mens c++ er 0-basert
                        indiceTex.push_back(atoi(faceData[i*3+1].c_str())-1);
                        indiceNorm.push_back(atoi(faceData[i*3+2].c_str())-1);
                    }
                }
                else if(numFaceData == 8)
                {
                    mbIsQuadBased = true;
                    for(int i=0;i< 4;i++)
                    {
                        indiceVert.push_back(atoi(faceData[i*2].c_str())-1); //-1 pga .obj er index 1-basert, mens c++ er 0-basert
                        indiceTex.push_back(atoi(faceData[i*2+1].c_str())-1);
                        //indiceNorm.push_back(atoi(faceData[i*3+2].c_str())-1);
                    }
                }
                else if(numFaceData == 6)
                {
                    mbIsQuadBased = false;
                    for(int i=0;i< 3;i++)
                    {
                        indiceVert.push_back(atoi(faceData[i*2].c_str())-1);
                        indiceTex.push_back(atoi(faceData[i*2+1].c_str())-1);
                        //indiceNorm.push_back(atoi(faceData[i*3+2].c_str())-1);
                    }
                }
                else if(numFaceData == 4) //When there is no texture coord or normal, and there are 4 indices, that means it's a quad -> se under display, der tegner den enten triangler eller quads
                {
                    mbIsQuadBased = true;
                    for(int i=0;i< numFaceData;i++)
                    {
                        indiceVert.push_back(atoi(faceData[i].c_str())-1);
                    }
                }
                else if(numFaceData == 3)
                {
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

                if(curMatIndex != -1) //As long as there is a material associated with this obj
                {
                    mbIsQuadBased == true ? colPerFaceData = 4 : colPerFaceData = 3;
                    glm::vec4 currentColor = mMaterials[curMatIndex]->getColors();
                    for(int i(0); i < colPerFaceData; i++)
                        colors.push_back(currentColor);
                }

            }
            else if(line[0] == 'm') //.mtl -> contains all the materials needed for this obj
            {
                loadMaterial(getDirectory(meshName) + line.substr(7));
            }
            else if(line[0] == 'u') //The material int the .mtl for next section of faces
            {
                matName = line.substr(7);
                mNumElementsPerMat.push_back(indiceVert.size());

                for(unsigned int i(0); i < mMaterials.size(); i++)
                {
                    if(mMaterials[i]->getMaterialName() == matName)
                    {
                        curMatIndex = i; //Sets the current index so that we don't have to loop through to find colors (see above)

                        if(mMaterials[i]->getMaterialTextureID() != 0)
                            mOrderOfMaterials.push_back(mMaterials[i]->getMaterialTextureID());
                        else
                            mOrderOfMaterials.push_back(0); //No texture for this mat
                    }
                }
            }
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
                    tc.push_back(colors[i].w);
                }
                else //If the obj didn't specify a material / color
                {
                    tc.push_back(0.5f);
                    tc.push_back(0.5f);
                    tc.push_back(0.5f);
                    tc.push_back(1.0f);
                }

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

        mNumPoints = indiceVert.size();
        mVertices = vectorToFloat(tv);
        mColors = vectorToFloat(tc);
        mTextureCoords = vectorToFloat(tt);
        mNormals = vectorToFloat(tn);

        if(normals.size() > 0)
            mbHasNormals = true;
        if(texCoords.size() > 0)
            mbHasTextureCoords = true;

        mSizeVerticeBytes = tv.size() * sizeof(float);
        mSizeColorBytes = tc.size() * sizeof(float);
        mSizeNormalBytes = tn.size() * sizeof(float);
        mSizeTextureBytes = tt.size() * sizeof(float);

        std::cout << "Size vertex array: " << tv.size() << std::endl;
        std::cout << "Size color array: " << tc.size() << std::endl;
        std::cout << "Size texture array: " << tt.size() << std::endl;
        std::cout << "Size normal array: " << tn.size() << std::endl;

        tt.clear();
        tc.clear();
        tn.clear();
        tv.clear();
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

        if(mbHasNormals && mbHasTextureCoords && mTextures.size() > 0) //Assumes that all models are sent in with vertex and color info defined, and that textures are supplied
        {
            glBufferData(GL_ARRAY_BUFFER, mSizeVerticeBytes + mSizeColorBytes + mSizeNormalBytes + mSizeTextureBytes, 0, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, mSizeVerticeBytes, mVertices);
            glBufferSubData(GL_ARRAY_BUFFER, mSizeVerticeBytes, mSizeColorBytes, mColors);
            glBufferSubData(GL_ARRAY_BUFFER, mSizeVerticeBytes + mSizeColorBytes, mSizeTextureBytes, mTextureCoords);
            glBufferSubData(GL_ARRAY_BUFFER, mSizeVerticeBytes + mSizeColorBytes + mSizeTextureBytes, mSizeNormalBytes, mNormals);
        }
        else if(mbHasNormals)
        {
            glBufferData(GL_ARRAY_BUFFER, mSizeVerticeBytes + mSizeColorBytes + mSizeNormalBytes, 0, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, mSizeVerticeBytes, mVertices);
            glBufferSubData(GL_ARRAY_BUFFER, mSizeVerticeBytes, mSizeColorBytes, mColors);
            glBufferSubData(GL_ARRAY_BUFFER, mSizeVerticeBytes + mSizeColorBytes, mSizeNormalBytes, mNormals);
        }
        else if(mbHasTextureCoords && mTextures.size() > 0)
        {
            glBufferData(GL_ARRAY_BUFFER, mSizeVerticeBytes + mSizeColorBytes + mSizeTextureBytes, 0, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, mSizeVerticeBytes, mVertices);
            glBufferSubData(GL_ARRAY_BUFFER, mSizeVerticeBytes, mSizeColorBytes, mColors);
            glBufferSubData(GL_ARRAY_BUFFER, mSizeVerticeBytes + mSizeColorBytes, mSizeTextureBytes, mTextureCoords);
        }
        else //If neither textures or normals are included in the .obj
        {
            glBufferData(GL_ARRAY_BUFFER, mSizeVerticeBytes + mSizeColorBytes, 0, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, mSizeVerticeBytes, mVertices);
            glBufferSubData(GL_ARRAY_BUFFER, mSizeVerticeBytes, mSizeColorBytes, mColors);
        }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if(glIsVertexArray(mVAOid) == GL_TRUE)
        glDeleteVertexArrays(1, &mVAOid);

    glGenVertexArrays(1, &mVAOid);

    glBindVertexArray(mVAOid);

        glBindBuffer(GL_ARRAY_BUFFER, mVBOid);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mSizeVerticeBytes));
            glEnableVertexAttribArray(1);

            if(mbHasNormals && mbHasTextureCoords && mTextures.size() > 0)
            {
                std::cout << "This has both tex and normals!" << std::endl;
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mSizeVerticeBytes + mSizeColorBytes));
                glEnableVertexAttribArray(2);

                glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mSizeVerticeBytes + mSizeColorBytes + mSizeTextureBytes));
                glEnableVertexAttribArray(3);
            }
            else if(mbHasTextureCoords && mTextures.size() > 0)
            {
                std::cout << "This only has texture coords!" << std::endl;
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mSizeVerticeBytes + mSizeColorBytes));
                glEnableVertexAttribArray(2);
            }
            else if(mbHasNormals)
            {
                std::cout << "This only has normals!" << std::endl;
                glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mSizeVerticeBytes + mSizeColorBytes));
                glEnableVertexAttribArray(3);
            }

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
            {
                matName = line.substr(7);
            }
            else if(line[0] == 'K' && line[1] == 'd') //diffuse
            {
                float r(0.5f), g(0.5f), b(0.5f);
                sscanf(line.c_str(), "Kd %f %f %f", &r, &g, &b);
                mMaterials.push_back(new Material(matName, r, g, b)); //alpha a is optional, initializes to 1
            }
            else if(line[0] == 'm' && line[5] == 'd') //map_Kd (texture)
            {
                if(line[7] != ' ')
                {
                    std::string texturePath= getDirectory(fileName) + line.substr(7);
                    mTextures.push_back(new Texture(texturePath));
                    if(mTextures[mTextures.size()-1]->load() != true)
                    {
                        std::cout << "Error loading the texture. Check that a texture image has been supplied with the .obj" << std::endl;
                        return;
                    }
                    mMaterials[mMaterials.size()-1]->setMaterialTextureID(mTextures[mTextures.size()-1]->getID());
                }
                else
                {
                    std::cout << "The name of the texture to use is not specified in the .mtl" << std::endl;
                    mMaterials[mMaterials.size()-1]->setMaterialTextureID(0);
                }


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
        std::cout << "Could not open .mat file" << std::endl;
        return;
    }
}


void MeshObj::display(glm::mat4 &projection, glm::mat4 &modelview)
{
    glm::mat4 modelviewProjection = projection * modelview;

    glUseProgram(mShader.getProgramID());

        glBindVertexArray(mVAOid);

            glUniformMatrix4fv(glGetUniformLocation(mShader.getProgramID(), "modelviewProjection"), 1, GL_FALSE, glm::value_ptr(modelviewProjection));

            if(mNumElementsPerMat.size() > 1)
            {
                for(unsigned int i(0); i < mNumElementsPerMat.size(); i++)
                {
                    if(mOrderOfMaterials[i] != 0)
                        glBindTexture(GL_TEXTURE_2D, mOrderOfMaterials[i]);

                    if(mbIsQuadBased)
                        glDrawArrays(GL_QUADS, mNumElementsPerMat[i], mNumElementsPerMat[i+1] - mNumElementsPerMat[i]);
                    else
                        glDrawArrays(GL_TRIANGLES, mNumElementsPerMat[i], mNumElementsPerMat[i+1] - mNumElementsPerMat[i]);

                    glBindTexture(GL_TEXTURE_2D, 0);
                }
            }
            else
            {
                if(mTextures.size() > 0)
                    glBindTexture(GL_TEXTURE_2D, mTextures[mTextures.size()-1]->getID());

                if(mbIsQuadBased)
                    glDrawArrays(GL_QUADS, 0, mNumPoints);
                else
                    glDrawArrays(GL_TRIANGLES, 0, mNumPoints);

                glBindTexture(GL_TEXTURE_2D, 0);
            }


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
    /*std::string temp = "The directory: ";
    temp += s1;
    std::cout << temp << std::endl;*/
    return s1;
}


