#include "SceneOpenGL.h"
#include "Shader.h"
#include "Cube.h"
#include "Texture.h"
#include "Crate.h"
#include "Camera.h"
#include "Square.h"
#include "Ground.h"
#include "Cabin.h"
#include "FrameBuffer.h"
#include "MeshObj.h"

#include <vector>
#include <ostream>

using namespace glm;

SceneOpenGL::SceneOpenGL(std::string windowTitle, int windowWidth, int windowHeight) :
    mWindowTitle(windowTitle), mWindowWidth(windowWidth), mWindowHeight(windowHeight),
    mWindow(0), mContextOpenGL(0), mInput()
{
    for(int i(0); i < 26; i++)
        mKeysClicked[i] = false;
}

SceneOpenGL::~SceneOpenGL()
{
    SDL_GL_DeleteContext(mContextOpenGL);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

bool SceneOpenGL::initGL()
{
    #ifdef WIN32

        GLenum initialisationGLEW(glewInit());


        if(initialisationGLEW != GLEW_OK)
        {
            std::cout << "Error initializing GLEW: " << glewGetErrorString(initialisationGLEW) << std::endl;

            SDL_GL_DeleteContext(mContextOpenGL);
            SDL_DestroyWindow(mWindow);
            SDL_Quit();

            return false;
        }
    #endif

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    glClearColor(0.4,0.4,0.4,1.0);

    return true;

}

bool SceneOpenGL::initializeWindow()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }

    //For version 3.3 of OpenGL. If we want 3.1, we put the 1 in the minor-attribute
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    //As opposed to SDL_GL_PROFILE_COMPATABILIY
    //CORE purges all the functionality that's no longer useful, but were kept for legacy reasons
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //1 true, 0 false
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    mWindow = SDL_CreateWindow(mWindowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowWidth, mWindowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if(mWindow == 0)
    {
        std::cout << "Error creating the window: " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }

    mContextOpenGL = SDL_GL_CreateContext(mWindow);

    if(mContextOpenGL == 0)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(mWindow);
        SDL_Quit();

        return false;
    }

    return true;
}

void SceneOpenGL::mainLoop()
{
    unsigned int framerate (1000/60);
    Uint32 startLoopTime(0), endLoopTime(0), timePassed(0);

    FrameBuffer frameBuffer(512, 512);
    frameBuffer.load();

    //mat4 projection; //Displays a 3D world on a 2D screen
   // mat4 modelview; //Koordinasjonssystemet vi bruker til å plassere, rotere, skalere etc alle modellene i scenen

   /* mat4 projectionFBO, modelviewFBO;
    projectionFBO = perspective(70.0, (double)frameBuffer.getWidth() / frameBuffer.getHeight(), 1.0, 100.0);
    modelviewFBO = mat4(1.0);*/

    float angle = 0.0;

    Camera cam(vec3(3,3,3), vec3(0,0,0), vec3(0,1,0), 0.1, 0.1);

    MeshObj meshTest("Objects/Ironman/ironman2.obj", "Shaders/normal.vert", "Shaders/normal.frag");

    //cam.addShader(meshTest.getShaders());

    mInput.displayMousePointer(false);
    mInput.captureMousePointer(true);



   // cam.updateLightPosition(sunPos);

  /*  char *test = "12.11";
    double temp = strtod(test, NULL);
    float ftemp = atof(test);
    printf("price: %f, %f", temp, ftemp);*/

   /* Ground ground(10.0, "Textures/grass.jpg", 15.0, 15.0, "Shaders/texture.vert", "Shaders/texture.frag");

    Cabin cabin(5.0, "Textures/Wallpaper.jpg", 15.0, 15.0, "Shaders/texture.vert", "Shaders/texture.frag");
*/

    mModel = glm::mat4(1.0f);
   // mModel *= glm::rotate(-35.0f, vec3(1.0f,0.0f,0.0f));
   // mModel *= glm::rotate(35.0f, vec3(0.0f,1.0f,0.0f));

    mView = glm::lookAt(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
   // mProjection = MakeOrtho(0, mWindowWidth, 0, mWindowHeight, 1.0f, 100.0f);

    mProjection = perspective(70.0, (double)mWindowWidth/mWindowHeight, 0.3, 100.0);


    glm::vec3 sunPos = glm::vec3(5.0f, 5.0f, 2.0f);
  //  glm::vec4 vec = mModel * glm::vec4(5.0f, 20.0f, 2.0f, 1.0f);
     meshTest.sendLightInfoToShaders(sunPos);


    mat4 modelSaved = mModel;


   // glm::mat3 normalMatrix;


    while(!mInput.exit())
    {
        startLoopTime = SDL_GetTicks();

        mInput.updateEvents();

        if(mInput.getKeyClicked(SDL_SCANCODE_ESCAPE))
            break;

        cam.move(mInput);
        //modelview = lookAt(vec3(0, 0, -4), vec3(0, 0, 0), vec3(0, 1, 0)); //Trenger ikke sette modelview til identity igjen, pga det å sette den til lookAt gjør det på en måte for oss

   //     glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.getID());

  //          glClearColor(0.5, 0.5, 0.5, 1.0);
  //          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 //           glViewport(0, 0, frameBuffer.getWidth(), frameBuffer.getHeight());

           // modelviewFBO = lookAt(vec3(3,0,3), vec3(0,0,0), vec3(0,1,0));


            angle += 0.1f;
            if(angle > 40)
                angle = 0;

           // mat4 savedModelviewFBO = modelviewFBO;
          //      modelviewFBO = rotate(modelviewFBO, angle, vec3(0,1,0));

//                crate.display(projectionFBO, modelviewFBO);
       //     modelviewFBO = savedModelviewFBO;

   //     glBindFramebuffer(GL_FRAMEBUFFER, 0);


        glClearColor(0.1, 0.1, 0.6, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0,0,mWindowWidth, mWindowHeight);

      /*  glm::vec4 vec = mModel * glm::vec4(5.0f, 20.0f, 2.0f, 1.0f);
        glm::vec3 sunPos = vec3(vec.x, vec.y, vec.z);
        meshTest.sendLightInfoToShaders(sunPos);*/

  //  glm::vec4 vec = mModel * glm::vec4(5.0f, 20.0f, 2.0f, 1.0f);

     /*sunPos.x = sin(angle);
        sunPos.z = 2;
        sunPos.y = 4.0f;

        meshTest.sendLightInfoToShaders(sunPos);*/


      //  mat4 savedModelview = mModelview;
      //          mModelview = translate(mModelview, vec3(5,0,0));
                //modelview = rotate(modelview, angle, vec3(0,1,0));
                //meshTest.display(projection, modelview, false, false);
    //    mModelview = savedModelview;


     //   cam.lookAt(mModel);
       cam.lookAt(mView);
      //  cam.lookAt(mModel);







        //mModelview = mView * mModel;



        modelSaved = mView;
       // mView = rotate(mView, angle, vec3(0,1,0));

     //  glm::mat4 modelview = mView * mModel;
      //  normalMatrix = glm::mat3(vec3(modelview[0]), vec3(modelview[1]), vec3(modelview[2]));

       // meshTest.display(mProjection, mModelview, normalMatrix);
   //      meshTest.display(mModel, mView, mProjection, normalMatrix);
        meshTest.display(mModel, mView, mProjection);

        mView = modelSaved;





    /*        glUseProgram(shaderText.getProgramID());
                glBindVertexArray(vao);
                    shaderText.sendMatrix("modelviewProjection", projection * modelview);
                    glBindTexture(GL_TEXTURE_2D, frameBuffer.getColorBufferID(0));
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                    glBindTexture(GL_TEXTURE_2D, 0);
                glBindVertexArray(0);
            glUseProgram(0);*/


        //crate.display(projection, modelview);

        /*ground.display(projection, modelview);

        savedModelview = modelview;

        modelview = translate(modelview, vec3(0.0, 1.25, 0.0));
            cabin.display(projection, modelview);
        modelview = savedModelview;

        savedModelview = modelview;
        modelview = translate(modelview, vec3(1.0, 0.5, 1.0));
            crate.display(projection, modelview);
        modelview = savedModelview;*/


        SDL_GL_SwapWindow(mWindow);

        endLoopTime = SDL_GetTicks();
        timePassed = endLoopTime - startLoopTime;

        //Hvis programmet har kjørt raskere enn den gitte fps, pause programmet til den har tatt igjen, ellers må CPUen jobbe så veldig hardt og kjøre altfor mange loops enn nødvendig
        if(timePassed < framerate)
            SDL_Delay(framerate - timePassed);

    }
}

