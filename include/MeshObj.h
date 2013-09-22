#ifndef MESHOBJ_H
#define MESHOBJ_H

#ifdef WIN32
#include <GL/glew.h>

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Material.h"
#include "Texture.h"

#ifndef BUFFER_OFFSET

    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

class MeshObj
{
    public:
        MeshObj(std::string const meshName, std::string const vertexShader, std::string fragmentShader, int *firstFrame=NULL);
        ~MeshObj();

        void loadObject(std::string const meshName, int *firstFrame=NULL);
        void load();
        void loadMaterial(std::string const fileName);
        void display(glm::mat4 &projection, glm::mat4 &modelview);

        std::string replaceDoubleSlash(std::string const s);
        std::string replaceSlash(std::string const s);
        std::vector<std::string> splitSpace(std::string const s);
        std::string getDirectory(std::string const s);
        float* vectorToFloat(std::vector<float>& vec);

    protected:
    private:
        Texture mTexture;
        int mData;
        float *mVertices;
        float *mNormals;
        float *mTextureCoords;
        float *mColors;
        std::vector<Texture*> mTextures;
        std::vector<Material*> mMaterials;

        GLuint mVBOid;
        GLuint mVAOid;
        Shader mShader;

        int mSizeVerticeBytes;
        int mSizeColorBytes;
        int mSizeNormalBytes;
        int mSizeTextureBytes;

        int mNumPoints;
        bool mbIsQuadBased;
        bool mbHasNormals;
        bool mbHasTextureCoords;

        std::vector<int> mNumElementsPerMat;
        std::vector<GLuint> mOrderOfMaterials;

};


#endif // MESHOBJ_H
