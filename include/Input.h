#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

class Input
{
    public:
        Input();
        ~Input();
    void updateEvents();
    bool exit() const;
    bool getKeyClicked(const SDL_Scancode key) const;
    bool getMouseButtonClicked(const Uint8 button) const;
    bool mouseMovement() const;

    int getX() const { return mX; }
    int getY() const { return mY; }
    int getXRel() const { return mXRel; }
    int getYRel() const { return mYRel; }

    void displayMousePointer(bool b) const;
    void captureMousePointer(bool b) const;


    protected:
    private:

        SDL_Event mEvents;
        bool mKeys[SDL_NUM_SCANCODES]; //SDL tar seg av hvor mange keyboardknapper som er tilgjengelige
        bool mMouseButtons[8]; //7 er maks antall musknapper SDL kan håndtere, og indicen starter på 1, ikke på 0.

        int mX;
        int mY;
        int mXRel;
        int mYRel;

        bool mTerminate;
};

#endif // INPUT_H
