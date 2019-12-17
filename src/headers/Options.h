#ifndef PROGETTOGRAFICA_OPTIONS_H
#define PROGETTOGRAFICA_OPTIONS_H

class Options {
private:
    bool wireframesEnabled;
    bool shadowsEnabled;
    bool fogEnabled;
    bool shadersEnabled;
public:
    Options();
    bool areWireframesEnabled();
    bool areShadowsEnabled();
    bool isFogEnabled();
    bool areShadersEnabled();
    void toggleWireframes();
    void toggleShadows();
    void toggleFog();
    void toggleShaders();
    void printMenu();
};


#endif
