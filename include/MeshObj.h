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
        void display(glm::mat4 &projection, glm::mat4 &modelview, bool normal=true, bool texture=false);

        std::string replaceDoubleSlash(std::string const s);
        std::string replaceSlash(std::string const s);
        std::vector<std::string> splitSpace(std::string const s);
        std::string getDirectory(std::string const s);
        float* vectorToFloat(std::vector<float>& vec);

    protected:
    private:
        //Texture mTexture;
        GLuint mTexture;
        int mData;
        float *mVertices;
        float *mNormals;
        float *mTextures;
        float *mColors;
        std::vector<Material*> mMaterials;

        GLuint mVBOid;
        GLuint mVAOid;
        Shader mShader;

        int mSizeVerticeBytes;
        int mSizeColorBytes;
        int mSizeNormalBytes;

        int mNumPoints;
        bool mbIsQuadBased;
};


#endif // MESHOBJ_H
