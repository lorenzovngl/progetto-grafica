#include <stdio.h>
#include <math.h>
#include <chrono>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/gl.h>
#include <GL/glu.h>

#endif

#include <vector> // la classe vector di SDL

#include "headers/Controller.h"

// da invocare quando e' stato premuto/rilasciato il tasto numero "keycode"
void Controller::EatKey(int keycode, int *keymap, bool pressed_or_released) {
    for (int i = 0; i < NKEYS; i++)
        if (keycode == keymap[i]) key[i] = pressed_or_released;
}

// da invocare quando e' stato premuto/rilasciato jbutton"
void Controller::Joy(int keymap, bool pressed_or_released) {
    key[keymap] = pressed_or_released;
}

void drawAxis(); // anche questa


void Controller::Init() {
    for (int i = 0; i < NKEYS; i++) key[i] = false;
}