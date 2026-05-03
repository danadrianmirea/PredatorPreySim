#include <iostream>
#include <string>
#include "engine/world.h"
#include "visualizer/simvisualizer.h"

using namespace std;

int main(int argc, char* argv[])
{    
    WorldConfig defaultConfig = WorldConfig();
    
    // Check command line arguments
    bool visualMode = true;
    if (argc > 1) {
        string arg = argv[1];
        if (arg == "--visual" || arg == "-v") {
            visualMode = true;
        } else if (arg == "--help" || arg == "-h") {
            cout << "Predator-Prey Simulation" << endl;
            cout << "Usage: " << argv[0] << " [options]" << endl;
            cout << "Options:" << endl;
            cout << "  --visual, -v    Run with visual rendering (raylib)" << endl;
            cout << "  --help, -h      Show this help message" << endl;
            cout << "  (no args)       Run in text mode (CSV output)" << endl;
            return 0;
        }
    }
    
    if (visualMode) {
        cout << "Starting visual simulation..." << endl;
        World world(defaultConfig);
        SimVisualizer visualizer(world);
        visualizer.run();
        cout << "Visual simulation complete." << endl;
    } else {
        cout << "Starting text-mode simulation..." << endl;
        World world(defaultConfig);
        world.runSimulation();
    }
    
    return 0;
}
