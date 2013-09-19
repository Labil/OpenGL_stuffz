#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
    :mId(0), mWidth(0), mHeight(0), mColorBuffers(0), mDepthBufferId(0), mbUseStencilBuffer(false)
{

}
FrameBuffer::FrameBuffer(int width, int height, bool useStencilBuffer)
    :mId(0), mWidth(width), mHeight(height), mColorBuffers(0), mDepthBufferId(0), mbUseStencilBuffer(useStencilBuffer)
{

}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &mId);
    glDeleteRenderbuffers(1, &mDepthBufferId);
    mColorBuffers.clear();
}

FrameBuffer::FrameBuffer(const FrameBuffer &frameBufferToCopy)
{
    mWidth = frameBufferToCopy.mWidth;
    mHeight = frameBufferToCopy.mHeight;
    mbUseStencilBuffer = frameBufferToCopy.mbUseStencilBuffer;

    load();
}

void FrameBuffer::createRenderBuffer(GLuint &id, GLenum internalFormat)
{
    if(glIsRenderbuffer(id) == GL_TRUE)
        glDeleteRenderbuffers(1, &id);
    glGenRenderbuffers(1, &id);

    glBindBuffer(GL_RENDERBUFFER, id);

        //Configurating the render buffer
        //the internal format is different for depth buffer and stencil buffer
        glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, mWidth, mHeight);

    glBindBuffer(GL_RENDERBUFFER, 0);

}

bool FrameBuffer::load()
{
    if(glIsFramebuffer(mId) == GL_TRUE)
    {
        glDeleteFramebuffers(1, &mId);
        mColorBuffers.clear();
    }


    glGenFramebuffers(1, &mId);

    glBindFramebuffer(GL_FRAMEBUFFER, mId);

        Texture colorBuffer(mWidth, mHeight, GL_RGBA, GL_RGBA, true);
        colorBuffer.loadEmptyTexture();
        mColorBuffers.push_back(colorBuffer);

        if(mbUseStencilBuffer)
            createRenderBuffer(mDepthBufferId, GL_DEPTH24_STENCIL8);
        else
            createRenderBuffer(mDepthBufferId, GL_DEPTH_COMPONENT24);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorBuffers[0].getID(), 0);

        if(mbUseStencilBuffer)
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT ,GL_RENDERBUFFER, mDepthBufferId);
        else
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBufferId);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            glDeleteFramebuffers(1, &mId);
            glDeleteRenderbuffers(1, &mDepthBufferId);
            mColorBuffers.clear();

            std::cout << "Error: FBO is not working" << std::endl;
            return false;
        }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;



}
