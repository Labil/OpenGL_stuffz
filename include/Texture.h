#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef WIN32
#include <GL/glew.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>


class Texture
{
    public:
        Texture(std::string imagePath);
        Texture(int width, int height, GLenum format, GLenum internalFormat, bool emptyTexture);
        Texture();
        Texture(Texture const &textureToCopy);
        ~Texture();
        bool load();
        void loadEmptyTexture();
        GLuint getID() const { return mId; }
        void setImagePath(const std::string &imagePath) { mImagePath = imagePath; }
        std::string getImagePath() { return mImagePath; }
        SDL_Surface* inversePixels(SDL_Surface *imageSource) const;

        Texture& operator=(Texture const &textureToCopy);

    protected:
    private:
        GLuint mId;
        std::string mImagePath;

        int mWidth;
        int mHeight;
        GLenum mFormat;
        GLenum mInternalFormat;
        bool mbEmptyTexture;
};

#endif // TEXTURE_H
