#ifndef CAMERA_H
#define CAMERA_H

//GLM Math lib
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Input.h"

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
};

#endif // CAMERA_H
