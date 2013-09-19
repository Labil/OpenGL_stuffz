#include "Input.h"

Input::Input() : mX(0), mY(0), mXRel(0), mYRel(0), mTerminate(false)
{
    for(int i(0); i < SDL_NUM_SCANCODES; i++)
        mKeys[i] = false;

    for(int i(0); i < 8; i++)
        mMouseButtons[i] = false;
}

Input::~Input()
{
    //dtor
}

void Input::updateEvents()
{
    //For å forhindre fiktiv musbevegelse må disse reinitialiseres
    //Disse to lagrer forskjellen på den aktuelle musposisjonen og den forrige,
    //men hvis det ikke er noen events, så har de verdien til den forrige posisjonen,
    //noe som gjør at programmet tror at musa ennå beveger seg, selv om den ikke er aktiv
    mXRel = 0;
    mYRel = 0;

    //Event loop
    //SDL_PollEvent returnerer enten 1 eller 0, for å indikere om det ennå er events å håndtere
    //Gjør at vi kan håndtere alle typer events på én gang som har å gjøre med mus eller keyboard
    while(SDL_PollEvent(&mEvents))
    {
        switch(mEvents.type)
        {
            case SDL_KEYDOWN:
                mKeys[mEvents.key.keysym.scancode] = true;
                break;
            case SDL_KEYUP:
                mKeys[mEvents.key.keysym.scancode] = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mMouseButtons[mEvents.button.button] = true;
                break;
            case SDL_MOUSEBUTTONUP:
                mMouseButtons[mEvents.button.button] = false;
                break;
            case SDL_MOUSEMOTION:
                mX = mEvents.motion.x;
                mY = mEvents.motion.y;

                mXRel = mEvents.motion.xrel;
                mYRel = mEvents.motion.yrel;
                break;
            case SDL_WINDOWEVENT:
                if(mEvents.window.event == SDL_WINDOWEVENT_CLOSE)
                    mTerminate = true;
                break;

            default:
                break;
        }
    }
}
bool Input::exit() const
{
    return mTerminate;
}

bool Input::getKeyClicked(const SDL_Scancode key) const
{
    return mKeys[key];
}

bool Input::getMouseButtonClicked(const Uint8 button) const
{
    return mMouseButtons[button];
}

bool Input::mouseMovement() const
{
    if(mXRel == 0 && mYRel == 0)
        return false;
    else
        return true;
}

void Input::displayMousePointer(bool b) const
{
    if(b)
        SDL_ShowCursor(SDL_ENABLE);
    else
        SDL_ShowCursor(SDL_DISABLE);
}

void Input::captureMousePointer(bool b) const
{
    if(b)
        SDL_SetRelativeMouseMode(SDL_TRUE);
    else
        SDL_SetRelativeMouseMode(SDL_FALSE);
}




















