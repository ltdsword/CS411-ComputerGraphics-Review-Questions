#include <iostream>
#include <vector>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Core system
#include "class/renderer.h"
#include "class/benchmark_manager.h"
#include "class/accuracy_calculator.h"
#include "class/input_parser.h"
#include "class/object_factory.h"

// All algorithm headers needed for factory
#include "class/line_dda.h"
#include "class/line_bresenham.h"
#include "class/line_parametric.h"
#include "class/circle_midpoint.h"
#include "class/circle_parametric.h"
#include "class/ellipse_midpoint.h"
#include "class/ellipse_parametric.h"
#include "class/parabola_midpoint.h"
#include "class/parabola_parametric.h"
#include "class/hyperbola_midpoint.h"
#include "class/hyperbola_parametric.h"

using namespace std;

// Function to initialize GLFW window with OpenGL 2.1 compatibility
GLFWwindow* initWindow(int width, int height) {
    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }
    
    // Request OpenGL 4.6 Core Profile (modern OpenGL with shaders)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    GLFWwindow* window = glfwCreateWindow(width, height, "CS411 Lab 02 - 2D Drawing Algorithms", nullptr, nullptr);
    if (!window) {
        cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }
    
    glfwMakeContextCurrent(window);
    
    // Load OpenGL functions with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }
    
    return window;
}

// Factory function to create appropriate accuracy calculator
AccuracyCalculator* createAccuracyCalculator(int type, const vector<int>& params) {
    switch (type) {
        case 0: // LineDDA
        case 1: // LineBresenham
            if (params.size() == 4)
                return new LineAccuracyCalculator(params[0], params[1], params[2], params[3]);
            break;
        case 2: // CircleMidpoint
            if (params.size() == 3)
                return new CircleAccuracyCalculator(params[0], params[1], params[2]);
            break;
        case 3: // EllipseMidpoint
            if (params.size() == 4)
                return new EllipseAccuracyCalculator(params[0], params[1], params[2], params[3]);
            break;
        case 4: // ParabolaMidpoint
            if (params.size() == 3)
                return new ParabolaAccuracyCalculator(params[0], params[1], params[2], true);
            break;
        case 5: // HyperbolaMidpoint
            if (params.size() == 4)
                return new HyperbolaAccuracyCalculator(params[0], params[1], params[2], params[3], false);
            break;
    }
    return nullptr;
}

int main() {
    cout << "\n";
    cout << "╔══════════════════════════════════════════════════════════════════════╗\n";
    cout << "║         CS411 - COMPUTER GRAPHICS LAB 02                             ║\n";
    cout << "║         2D DRAWING ALGORITHMS BENCHMARK SYSTEM                       ║\n";
    cout << "╚══════════════════════════════════════════════════════════════════════╝\n";
    cout << "\n";
    
    // Step 1: Parse input file
    cout << "[1/6] Parsing input file...\n";
    InputParser parser;
    vector<InputParser::ParsedObject> parsedObjects = parser.parseFile("input/input.txt");
    
    if (parsedObjects.empty()) {
        cerr << "Error: No valid objects found in input file!\n";
        return 1;
    }
    
    // Step 2: Create object instances
    cout << "[2/6] Creating algorithm instances...\n";
    ObjectFactory factory;
    vector<Object2D*> customAlgorithms;
    vector<Object2D*> parametricAlgorithms;
    vector<AccuracyCalculator*> calculators;
    
    for (const auto& obj : parsedObjects) {
        Object2D* custom = factory.createCustomAlgorithm(obj.type, obj.params);
        Object2D* parametric = factory.createParametricAlgorithm(obj.type, obj.params);
        AccuracyCalculator* calc = createAccuracyCalculator(obj.type, obj.params);
        
        if (custom && parametric && calc) {
            customAlgorithms.push_back(custom);
            parametricAlgorithms.push_back(parametric);
            calculators.push_back(calc);
        } else {
            cerr << "Warning: Failed to create object: " << obj.getDescription() << "\n";
        }
    }
    
    cout << "Created " << customAlgorithms.size() << " algorithm pairs\n";
    
    // Step 3: Initialize OpenGL window
    cout << "[3/6] Initializing OpenGL window...\n";
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    GLFWwindow* window = initWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    if (!window) {
        cerr << "Failed to initialize window!\n";
        return 1;
    }
    
    cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    
    // Step 4: Draw all objects in single window
    cout << "[4/6] Rendering all objects to screen...\n";
    Renderer renderer;
    renderer.initOpenGL(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    renderer.setClearColor(0.05f, 0.05f, 0.1f); // Dark blue background
    renderer.setMode(Renderer::OPENGL_MODE);
    
    renderer.beginFrame();
    
    // Draw all custom algorithms in different colors
    for (size_t i = 0; i < customAlgorithms.size(); i++) {
        // Assign different colors to each object
        float r = 0.3f + (i * 0.15f);
        float g = 0.5f + (i * 0.1f);
        float b = 0.7f + (i * 0.05f);
        
        // Normalize colors
        while (r > 1.0f) r -= 1.0f;
        while (g > 1.0f) g -= 1.0f;
        while (b > 1.0f) b -= 1.0f;
        
        renderer.setPixelColor(r, g, b);
        customAlgorithms[i]->draw(renderer);
    }
    
    renderer.endFrame();
    renderer.present();
    
    cout << "All objects rendered successfully!\n";
    
    // Step 5: Benchmark each object
    cout << "[5/6] Benchmarking algorithms...\n";
    BenchmarkManager benchmarkManager;
    vector<BenchmarkManager::BenchmarkResult> results;
    
    // Switch to benchmark mode (no visual rendering)
    renderer.setMode(Renderer::BENCHMARK_MODE);
    
    for (size_t i = 0; i < customAlgorithms.size(); i++) {
        cout << "  Benchmarking object " << (i + 1) << "/" << customAlgorithms.size() << "...\n";
        
        auto result = benchmarkManager.benchmarkObject(
            customAlgorithms[i],
            parametricAlgorithms[i],
            renderer,
            *calculators[i],
            parsedObjects[i].getDescription(),
            10000  // iterations
        );
        
        results.push_back(result);
    }
    
    // Step 6: Output results
    cout << "[6/6] Generating output...\n";
    
    // Console output (execution time in milliseconds)
    benchmarkManager.printConsoleOutput(results);
    
    // File output (detailed comparison)
    benchmarkManager.writeOutputFile("output/output.txt", results);
    
    cout << "\n";
    cout << "==================================================\n";
    cout << "  BENCHMARK COMPLETE!\n";
    cout << "==================================================\n";
    cout << "  - Screen: All objects displayed in window\n";
    cout << "  - Console: Execution times shown above\n";
    cout << "  - File: Detailed results in output/output.txt\n";
    cout << "\n";
    cout << "Press ESC to close window, or wait...\n";
    
    // Wait for user to close window or press ESC
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            break;
        }
        glfwPollEvents();
    }
    
    // Cleanup
    for (auto* obj : customAlgorithms) delete obj;
    for (auto* obj : parametricAlgorithms) delete obj;
    for (auto* calc : calculators) delete calc;
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
