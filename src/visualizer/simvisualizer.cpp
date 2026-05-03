#include "visualizer/simvisualizer.h"
#include "raylib.h"
#include <string>
#include <algorithm>

SimVisualizer::SimVisualizer(World& world)
    : _world(world)
    , _screenWidth(1200)
    , _screenHeight(800)
    , _paused(false)
    , _tickSpeed(16.0f) // ~60 FPS
{
    // Calculate scale to fit the world in the window with padding
    const WorldConfig& config = _world.getConfig();
    int padding = 200; // Space for UI on the right
    float scaleX = static_cast<float>(_screenWidth - padding) / config.width;
    float scaleY = static_cast<float>(_screenHeight) / config.height;
    _scale = std::min(scaleX, scaleY) * 0.85f; // 85% to leave some margin

    // Center the world
    _offsetX = (_screenWidth - padding - static_cast<int>(config.width * _scale)) / 2;
    _offsetY = (_screenHeight - static_cast<int>(config.height * _scale)) / 2;
}

SimVisualizer::~SimVisualizer() {
}

void SimVisualizer::worldToScreen(float worldX, float worldY, int& screenX, int& screenY) const {
    screenX = _offsetX + static_cast<int>(worldX * _scale);
    screenY = _offsetY + static_cast<int>(worldY * _scale);
}

void SimVisualizer::drawAgent(float x, float y, float radius, bool isPredator) {
    int sx, sy;
    worldToScreen(x, y, sx, sy);
    float sr = radius * _scale;

    if (isPredator) {
        // Predators: red triangles
        DrawCircle(sx, sy, sr, RED);
        DrawCircle(sx, sy, sr * 0.6f, ColorAlpha(RED, 0.3f));
    } else {
        // Prey: green circles
        DrawCircle(sx, sy, sr, GREEN);
        DrawCircle(sx, sy, sr * 0.6f, ColorAlpha(GREEN, 0.3f));
    }
}

void SimVisualizer::drawUI(float time, int generation) {
    const WorldConfig& config = _world.getConfig();
    int predators = static_cast<int>(_world.getPredators().size());
    int prey = static_cast<int>(_world.getPrey().size());

    // Background panel for UI
    int panelX = _screenWidth - 190;
    DrawRectangle(panelX, 0, 190, _screenHeight, ColorAlpha(BLACK, 0.7f));
    DrawLine(panelX, 0, panelX, _screenHeight, DARKGRAY);

    // Title
    DrawText("Predator-Prey", panelX + 10, 10, 18, WHITE);
    DrawText("Simulation", panelX + 10, 32, 18, WHITE);
    DrawLine(panelX + 10, 58, _screenWidth - 10, 58, DARKGRAY);

    // Stats
    int y = 75;
    DrawText("Generation:", panelX + 10, y, 14, LIGHTGRAY);
    DrawText(TextFormat("%d / %d", generation, config.numGenerations), panelX + 10, y + 18, 14, WHITE);

    y += 45;
    DrawText("Time:", panelX + 10, y, 14, LIGHTGRAY);
    DrawText(TextFormat("%.1f / %.0f", time, config.duration), panelX + 10, y + 18, 14, WHITE);

    y += 45;
    DrawText("Predators:", panelX + 10, y, 14, LIGHTGRAY);
    DrawText(TextFormat("%d", predators), panelX + 10, y + 18, 14, RED);

    y += 45;
    DrawText("Prey:", panelX + 10, y, 14, LIGHTGRAY);
    DrawText(TextFormat("%d", prey), panelX + 10, y + 18, 14, GREEN);

    // Controls help
    y = _screenHeight - 120;
    DrawLine(panelX + 10, y - 5, _screenWidth - 10, y - 5, DARKGRAY);
    DrawText("Controls:", panelX + 10, y, 14, LIGHTGRAY);
    y += 20;
    DrawText("SPACE - Pause/Resume", panelX + 10, y, 12, GRAY);
    y += 16;
    DrawText("R - Reset", panelX + 10, y, 12, GRAY);
    y += 16;
    DrawText("+/- Speed", panelX + 10, y, 12, GRAY);
    y += 16;
    DrawText("ESC - Exit", panelX + 10, y, 12, GRAY);

    // Pause indicator
    if (_paused) {
        DrawText("PAUSED", _screenWidth / 2 - 60, 10, 30, YELLOW);
    }

    // Generation transition notification
    if (time == 0.0f && generation > 1) {
        DrawText(TextFormat("Generation %d", generation), _screenWidth / 2 - 100, _screenHeight / 2 - 20, 40, WHITE);
    }
}

void SimVisualizer::handleInput() {
    if (IsKeyPressed(KEY_SPACE)) {
        _paused = !_paused;
    }

    if (IsKeyPressed(KEY_R)) {
        _world.initializeVisualSimulation();
    }

    if (IsKeyPressed(KEY_EQUAL) || IsKeyPressed(KEY_KP_ADD)) {
        _tickSpeed = std::max(1.0f, _tickSpeed - 5.0f);
    }

    if (IsKeyPressed(KEY_MINUS) || IsKeyPressed(KEY_KP_SUBTRACT)) {
        _tickSpeed = std::min(200.0f, _tickSpeed + 5.0f);
    }
}

void SimVisualizer::draw() {
    const WorldConfig& config = _world.getConfig();

    // Draw world background
    int wx, wy, ww, wh;
    worldToScreen(0, 0, wx, wy);
    ww = static_cast<int>(config.width * _scale);
    wh = static_cast<int>(config.height * _scale);
    DrawRectangle(wx, wy, ww, wh, ColorAlpha(DARKGREEN, 0.2f));
    DrawRectangleLines(wx, wy, ww, wh, DARKGRAY);

    // Draw grid lines
    int gridSpacing = 10;
    for (int x = 0; x <= config.width; x += gridSpacing) {
        int sx, sy, sy2;
        worldToScreen(static_cast<float>(x), 0.0f, sx, sy);
        worldToScreen(static_cast<float>(x), static_cast<float>(config.height), sx, sy2);
        DrawLine(sx, sy, sx, sy2, ColorAlpha(DARKGRAY, 0.15f));
    }
    for (int y = 0; y <= config.height; y += gridSpacing) {
        int sx, sy, sx2;
        worldToScreen(0.0f, static_cast<float>(y), sx, sy);
        worldToScreen(static_cast<float>(config.width), static_cast<float>(y), sx2, sy);
        DrawLine(sx, sy, sx2, sy, ColorAlpha(DARKGRAY, 0.15f));
    }

    // Draw prey
    for (const auto& prey : _world.getPrey()) {
        drawAgent(prey.getPosition().getX(), prey.getPosition().getY(), prey.getRadius(), false);
    }

    // Draw predators
    for (const auto& predator : _world.getPredators()) {
        drawAgent(predator.getPosition().getX(), predator.getPosition().getY(), predator.getRadius(), true);
    }
}

void SimVisualizer::run() {
    InitWindow(_screenWidth, _screenHeight, "Predator-Prey Simulation");
    SetTargetFPS(0); // We'll control timing manually

    _world.initializeVisualSimulation();

    float time = 0.0f;
    int generation = 1;
    float tickAccumulator = 0.0f;

    while (!WindowShouldClose()) {
        handleInput();

        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        }

        // Run simulation ticks
        if (!_paused && !_world.isSimulationComplete()) {
            tickAccumulator += GetFrameTime() * 1000.0f; // Convert to ms

            while (tickAccumulator >= _tickSpeed) {
                if (!_world.tickVisualSimulation(time, generation)) {
                    break;
                }
                tickAccumulator -= _tickSpeed;
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        draw();
        drawUI(time, generation);

        // Show completion message
        if (_world.isSimulationComplete()) {
            DrawText("SIMULATION COMPLETE", _screenWidth / 2 - 180, _screenHeight / 2 - 20, 40, GOLD);
            DrawText("Press ESC to exit", _screenWidth / 2 - 100, _screenHeight / 2 + 30, 20, LIGHTGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
}
