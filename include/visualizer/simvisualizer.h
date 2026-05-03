#pragma once

#include "engine/world.h"

class SimVisualizer {
public:
    SimVisualizer(World& world);
    ~SimVisualizer();

    void run();

private:
    World& _world;
    int _screenWidth;
    int _screenHeight;
    float _scale; // How many pixels per world unit
    int _offsetX; // Offset to center the world in the window
    int _offsetY;
    bool _paused;
    float _tickSpeed; // Milliseconds between ticks

    void draw();
    void drawAgent(float x, float y, float radius, bool isPredator);
    void drawUI(float time, int generation);
    void handleInput();
    void worldToScreen(float worldX, float worldY, int& screenX, int& screenY) const;
};
