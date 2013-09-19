#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#ifdef WIN32
#include <GL/glew.h>

//Linux
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif

#include <vector>
#include "Texture.h"


class FrameBuffer
{
    public:
        FrameBuffer();
        FrameBuffer(int width, int height, bool useStencilBuffer = false);
        ~FrameBuffer();
        FrameBuffer(const FrameBuffer &frameBufferToCopy);
        void createRenderBuffer(GLuint &id, GLenum internalFormat);
        bool load();


        GLuint getID() const { return mId; }
        GLuint getColorBufferID(unsigned int index) const { return mColorBuffers[index].getID(); }
        int getWidth() const { return mWidth; }
        int getHeight() const { return mHeight; }
    protected:
    private:
        GLuint mId;
        int mWidth;
        int mHeight;
        std::vector<Texture> mColorBuffers;
        GLuint mDepthBufferId;
        bool mbUseStencilBuffer;
};

#endif // FRAMEBUFFER_H
