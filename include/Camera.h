#ifndef CAMERA_H
#define CAMERA_H


//Include OpenGL
#ifdef WIN32
#include <GL/glew.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif

//GLM Math lib
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Input.h"
#include <vector>

class Camera
{
    public:
        Camera();
        Camera(glm::vec3 position, glm::vec3 target, glm::vec3 verticalAxis, float sensibility, float speed);
        ~Camera();

        void turn(int xRel, int yRel);
        void move(Input const &input);
        void lookAt(glm::mat4 &modelview);
        void setTarget(glm::vec3 target);
        void setPosition(glm::vec3 position);
        void addShader(GLuint const &shaderID);
        void addShader(std::vector<GLuint> &shaders);
        void updateLightPosition(glm::vec3 lightPos);

        float getSensibility() const { return mTurnSensibility; }
        float getSpeed() const { return mMovementSpeed; }

        void setSensibility(float sens) { mTurnSensibility = sens; }
        void setSpeed(float speed) { mMovementSpeed = speed; }


    protected:
    private:
        //Mouse angles:
        float mAngleY;
        float mAngleX;
        //The direction the camera is facing:
        glm::vec3 mOrientation;
        //To help calculate the sidewaysMovement,
        //which is mOrientation x mVerticalAxe to get the normal
        //(the perpendicular) on the plane which mOrientation is in:
        glm::vec3 mVerticalAxis;
        //A vector perpendicular to the direction the camera is facing:
        glm::vec3 mSidewaysMovement;
        //The current position of the camera:
        glm::vec3 mPosition;
        //The target to look towards
        glm::vec3 mTarget;

        float mTurnSensibility;
        float mMovementSpeed;

        std::vector<GLuint> mShaders;
        void updateShaders(const char* locName, const glm::mat4 &matrix);
        void updateShaders(const char* locName, const glm::vec3 &vec);
        void updateShaders(const char* locName, const float &value);
};

#endif // CAMERA_H
