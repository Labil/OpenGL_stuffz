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
        void loadVBO(std::vector<float>& v, std::vector<float>& c, std::vector<float>& t, std::vector<float>& n, int matIndex);
        void loadVAO(bool normals, bool texture);
        void loadMaterial(std::string const fileName);
  //      void display(glm::mat4 &projection, glm::mat4 &modelview, glm::mat3 &normalMatrix);
      //  void display(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, glm::mat3 &normalMatrix);
        void display(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection);
        void manageTextureLoading(std::string const path);
        void loadShader(std::string path); //0 = basic, 1= normals, 2 = texture

        std::string replaceDoubleSlash(std::string const s);
        std::string replaceSlash(std::string const s);
        std::vector<std::string> splitSpace(std::string const s);
        std::string getDirectory(std::string const s);
        float* vectorToFloat(std::vector<float>& vec);

  //      void sendLightInfoToShaders(glm::vec4 &v); //i.e LightPosition, Kd,

  void sendLightInfoToShaders(glm::vec3 &v);

        std::vector<GLuint>& getShaders();

    protected:
    private:

        float *mVertices;
        float *mNormals;
        float *mTextureCoords;
        float *mColors;
        std::vector<Texture*> mTextures;
        std::vector<Material*> mMaterials;

        std::vector<GLuint> mVBOids;
        std::vector<GLuint> mVAOids;
        std::vector<Shader*> mShaders;
        Shader mShader;
        std::vector<std::string> mPathsToShaders;
        //std::vector<std::string> mLoadedShaders;

        int mSizeVerticeBytes;
        int mSizeColorBytes;
        int mSizeNormalBytes;
        int mSizeTextureBytes;
        bool mbIsQuadBased;

        std::vector<int> mNumElementsPerMat; //Keeps track of how many elements to draw per vbo
        std::vector<GLuint> mOrderOfMaterials; //The order in wich the materials should be bound
        //std::vector<std::string> mLoadedTextures; //Keeps the names of already loaded textures, so as not to load a texture twice
        std::vector<int> mOrderOfShaders;
};


#endif // MESHOBJ_H
