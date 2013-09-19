#ifndef SCENEOPENGL_H
#define SCENEOPENGL_H

#ifdef WIN32
#include <GL/glew.h>

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif
//GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include "Input.h"
#include <vector>
#include "Square.h"
#include <iostream>

class SceneOpenGL
{
    public:
        SceneOpenGL(std::string windowTitle, int windowWidth, int windowHeight);
        ~SceneOpenGL();

        bool initializeWindow();
        bool initGL();
        void mainLoop();
        glm::mat4 MakeOrtho(float left, float right, float top, float bottom, float far, float near);
        bool typeLetter(std::string const letter);
        void handleKeyClicks();


    protected:
    private:

        SDL_Window *mWindow;
        SDL_GLContext mContextOpenGL;
        SDL_Event mEvents;

        std::string mWindowTitle;
        int mWindowWidth;
        int mWindowHeight;

        Input mInput;
        //bool mKeyClicked;
        bool mKeysClicked[29];

        std::vector<Square> mSquares;

};

#endif // SCENEOPENGL_H
