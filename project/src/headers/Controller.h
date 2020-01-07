#ifndef PROGETTOGRAFICA_CONTROLLER_H
#define PROGETTOGRAFICA_CONTROLLER_H

class Controller {
public:
    enum {
        LEFT = 0, RIGHT = 1, ACC = 2, DEC = 3, NKEYS = 4
    };
    bool key[NKEYS];

    void Init();

    void EatKey(int keycode, int *keymap, bool pressed_or_released);

    void Joy(int keymap, bool pressed_or_released);

    Controller() { Init(); } // costruttore
};

#endif
