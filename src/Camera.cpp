#include "Camera.h"

Camera::Camera():
    mAngleX(0.0), mAngleY(0.0), mOrientation(),mVerticalAxis(0,0,1), mSidewaysMovement(),
    mPosition(), mTarget(), mTurnSensibility(), mMovementSpeed()
{

}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 verticalAxis, float sensibility, float speed) :
        mAngleX(0.0), mAngleY(0.0), mOrientation(), mVerticalAxis(verticalAxis), mSidewaysMovement(),
        mPosition(position), mTarget(target), mTurnSensibility(sensibility), mMovementSpeed(speed)
{
    setTarget(target);
}

Camera::~Camera()
{

}

void Camera::turn(int xRel, int yRel)
{
    mAngleY += -yRel * mTurnSensibility; //TODO: Add a way to change the turn-speed, as well as inverse the mouse
    mAngleX += -xRel * mTurnSensibility;

    //Because the orientation should never be parallel to the vertical axis (0,0,90),
    //it should be limited to being smaller than 89 and bigger than -89. If not, weird shit happens.
    if(mAngleY > 89.0)
        mAngleY = 89.0;
    else if(mAngleY < -89.0)
        mAngleY = -89.0;

    //Cos and sin takes radians as parameters
    float xAngleRad = mAngleX * M_PI / 180;
    float yAngleRad = mAngleY * M_PI / 180;

    //Depending on what sort of coordination system we are using, X up, Y up or Z up.
    if(mVerticalAxis.x == 1)
    {
        mOrientation.x = sin(yAngleRad);
        mOrientation.y = cos(yAngleRad) * cos(xAngleRad);
        mOrientation.z = cos(yAngleRad) * sin(xAngleRad);
    }
    else if(mVerticalAxis.y == 1)
    {
        mOrientation.x = cos(yAngleRad) * sin(xAngleRad);
        mOrientation.y = sin(yAngleRad);
        mOrientation.z = cos(yAngleRad) * cos(xAngleRad);

    }
    else if(mVerticalAxis.z == 1)
    {
        mOrientation.x = cos(yAngleRad) * cos(xAngleRad);
        mOrientation.y = cos(yAngleRad) * sin(xAngleRad);
        mOrientation.z = sin(yAngleRad);
    }

    mSidewaysMovement = glm::cross(mVerticalAxis, mOrientation);
    mSidewaysMovement = glm::normalize(mSidewaysMovement);

    mTarget = mPosition + mOrientation;
}

void Camera::move(Input const &input)
{
    if(input.mouseMovement())
        turn(input.getXRel(), input.getYRel());

    if(input.getKeyClicked(SDL_SCANCODE_UP))
    {
        mPosition = mPosition + mOrientation * mMovementSpeed;
        mTarget = mPosition + mOrientation;
    }
    if(input.getKeyClicked(SDL_SCANCODE_DOWN))
    {
        mPosition = mPosition - mOrientation * mMovementSpeed;
        mTarget = mPosition + mOrientation;
    }
    if(input.getKeyClicked(SDL_SCANCODE_RIGHT))
    {
        mPosition = mPosition - mSidewaysMovement * mMovementSpeed;
        mTarget = mPosition + mOrientation;
    }
    if(input.getKeyClicked(SDL_SCANCODE_LEFT))
    {
        mPosition = mPosition + mSidewaysMovement * mMovementSpeed;
        mTarget = mPosition + mOrientation;
    }
}

void Camera::lookAt(glm::mat4 &modelview)
{
    modelview = glm::lookAt(mPosition, mTarget, mVerticalAxis);
}

void Camera::setTarget(glm::vec3 target)
{
    mOrientation = target - mPosition;
    mOrientation = glm::normalize(mOrientation);

    if(mVerticalAxis.x == 1)
    {
        mAngleY = asin(mOrientation.x);
        mAngleX = acos(mOrientation.y / cos(mAngleY));

        if(mOrientation.y < 0)
            mAngleX *= -1;
    }
    else if(mVerticalAxis.y == 1)
    {
        mAngleY = asin(mOrientation.y);
        mAngleX = acos(mOrientation.z / cos(mAngleY));

        if(mOrientation.z < 0)
            mAngleX *= -1;
    }
    else if(mVerticalAxis.z == 1)
    {
        mAngleY = asin(mOrientation.x);
        mAngleX = acos(mOrientation.z / cos(mAngleY));

        if(mOrientation.z < 0)
            mAngleX *= -1;
    }

    mAngleX = mAngleX * 180 / M_PI; //to degrees
    mAngleY = mAngleY * 180 / M_PI;
}

void Camera::setPosition(glm::vec3 position)
{
    mPosition = position;
    mTarget = mPosition + mOrientation;
}

void Camera::addShader(const GLuint &shaderID)
{
    mShaders.push_back(shaderID);

    //glUseProgram(shaderID);
    //glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"))

}

void Camera::addShader(std::vector<GLuint> &shaders)
{
    for(int i(0); i < shaders.size(); i++)
    {
        mShaders.push_back(shaders[i]);
    }
}

void Camera::updateLightPosition(glm::vec3 lightPos)
{
    updateShaders("LightPosition_worldspace", lightPos);
}

void Camera::updateShaders(const char* locName, const float& value)
{
    for(int i=0; i< mShaders.size(); i++)
    {
        glUseProgram(mShaders[i]);
        glUniform1f(glGetUniformLocation(mShaders[i], locName ),value );
    }
    glUseProgram(0);
}
void Camera::updateShaders(const char* locName, const glm::mat4& matrix)
{
    for(int i=0; i< mShaders.size(); i++)
    {
        glUseProgram(mShaders[i]);
        glUniformMatrix4fv(glGetUniformLocation(mShaders[i], locName), 1,GL_FALSE,glm::value_ptr(matrix) );
    }
    glUseProgram(0);
}
void Camera::updateShaders(const char* locName, const glm::vec3& vec)
{
    for(int i=0; i< mShaders.size(); i++)
    {
        glUseProgram(mShaders[i]);
        glUniform3fv(glGetUniformLocation(mShaders[i], locName ), 1, glm::value_ptr(vec) );
    }
    glUseProgram(0);
}
