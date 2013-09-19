#include "Texture.h"

Texture::Texture(std::string imagePath)
    : mId(0), mImagePath(imagePath), mWidth(0), mHeight(0), mFormat(0), mInternalFormat(0), mbEmptyTexture(false)
{

}

Texture::Texture() : mId(0), mImagePath(""), mWidth(0), mHeight(0), mFormat(0), mInternalFormat(0), mbEmptyTexture(false)
{

}

Texture::Texture(int width, int height, GLenum format, GLenum internalFormat, bool emptyTexture)
    : mId(0), mImagePath(""), mWidth(width), mHeight(height), mFormat(format), mInternalFormat(internalFormat), mbEmptyTexture(emptyTexture)
{

}

Texture::Texture(Texture const &textureToCopy)
{
    mImagePath = textureToCopy.mImagePath;
    mWidth = textureToCopy.mWidth;
    mHeight = textureToCopy.mHeight;
    mFormat = textureToCopy.mFormat;
    mInternalFormat = textureToCopy.mInternalFormat;
    mbEmptyTexture = textureToCopy.mbEmptyTexture;

    if(mbEmptyTexture && glIsTexture(textureToCopy.mId) == GL_TRUE)
        loadEmptyTexture();
    else if(glIsTexture(textureToCopy.mId) == GL_TRUE)
        load();
}

Texture::~Texture()
{
    glDeleteTextures(1, &mId);
}

Texture& Texture::operator=(Texture const &textureToCopy)
{
    mImagePath = textureToCopy.mImagePath;
    mWidth = textureToCopy.mWidth;
    mHeight = textureToCopy.mHeight;
    mFormat = textureToCopy.mFormat;
    mInternalFormat = textureToCopy.mInternalFormat;
    mbEmptyTexture = textureToCopy.mbEmptyTexture;

    if(mbEmptyTexture && glIsTexture(textureToCopy.mId) == GL_TRUE)
        loadEmptyTexture();
    else if(glIsTexture(textureToCopy.mId) == GL_TRUE)
        load();

    //Return pointer *this

    return *this;
}

void Texture::loadEmptyTexture()
{
    if(glIsTexture(mId) == GL_TRUE)
        glDeleteTextures(1, &mId);

    glGenTextures(1, &mId);

    glBindTexture(GL_TEXTURE_2D, mId);

        glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, mWidth, mHeight, 0, mFormat, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::load()
{
    SDL_Surface *imageSDL = IMG_Load(mImagePath.c_str());

    if(imageSDL == 0)
    {
        std::cout << "Error :" << SDL_GetError() << std::endl;
        return false;
    }

    SDL_Surface* inversedImage = inversePixels(imageSDL);
    SDL_FreeSurface(imageSDL);

    if(glIsTexture(mId) == GL_TRUE)
        glDeleteTextures(1, &mId);

    //Genererer id
    glGenTextures(1, &mId);
    //Binder texturen så opengl kan jobbe med den
    glBindTexture(GL_TEXTURE_2D, mId);

    GLenum internalFormat(0); //RGB eller RGBA
    GLenum format(0);

    if(inversedImage->format->BytesPerPixel == 3) //Har ikke alpha
    {
        internalFormat = GL_RGB;

        //Finn ut hvilket format fargene er i, RGB eller BGR (.BMP har fargene i denne rekkefølgen feks)
        if(inversedImage->format->Rmask == 0xff)
            format = GL_RGB;
        else
            format = GL_BGR;
    }
    else if(inversedImage->format->BytesPerPixel == 4) //Bildetypen har alphakanal
    {
        internalFormat = GL_RGBA;

        if(inversedImage->format->Rmask == 0xff)
            format = GL_RGBA;
        else
            format = GL_BGRA;
    }
    else
    {
        std::cout << "Error, image format is unknown" << std::endl;
        SDL_FreeSurface(inversedImage);
        return false;
    }
    //Kopierer pixler
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imageSDL->w, inversedImage->h, 0, format, GL_UNSIGNED_BYTE, inversedImage->pixels);

    //Adder filtere for å bestemme oppløsning på teksturene
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Filter for teksturer som skal være smoothe pga de er nær skjermen
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //Teksturer som er lengre unna og som kan være mer pixlete
    //Unbinder teksturen
    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_FreeSurface(inversedImage); //Trenger ikke denne lengre nå, vi har lagret pixlene fra bildet i texturen vår
    return true;
}

SDL_Surface* Texture::inversePixels(SDL_Surface* imageSource) const
{
    SDL_Surface* inversedImage;

    inversedImage = SDL_CreateRGBSurface(0, imageSource->w, imageSource->h,imageSource->format->BitsPerPixel,
                                         imageSource->format->Rmask, imageSource->format->Gmask,
                                         imageSource->format->Bmask, imageSource->format->Amask);

    unsigned char* pixelsSource = (unsigned char*)imageSource->pixels;
    unsigned char* pixelsInversed = (unsigned char*)inversedImage->pixels;

    for(int i(0); i < imageSource->h; i++)
    {
        for(int j(0); j < imageSource->w * imageSource->format->BytesPerPixel; j++)
        {
            pixelsInversed[(imageSource->w * imageSource->format->BytesPerPixel * (imageSource->h - 1 - i)) + j] = pixelsSource[(imageSource->w * imageSource->format->BytesPerPixel * i) + j];
            //inversedImage->pixels[inversedImage->h - i][j] = imageSource->pixels[i][j];
        }
    }

    return inversedImage;
}



























