#ifndef PROGETTOGRAFICA_OPTIONS_H
#define PROGETTOGRAFICA_OPTIONS_H

class Options {
private:
    bool wireframesEnable;
    bool shadowsEnable;
    bool shadersEnable;
public:
    Options();
    bool areWireframesEnabled();
    bool areShadowsEnabled();
    bool areShadersEnabled();
    void toggleWireframes();
    void toggleShadows();
    void toggleShaders();
    void printMenu();
};


#endif
