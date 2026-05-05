#pragma once

#include <set>
#include <utility>
#include <vector>

// Forward declaration for GLFW window
struct GLFWwindow;

class Renderer {
public:
    enum Mode {
        BENCHMARK_MODE,  // Only record pixels (no visual output)
        OPENGL_MODE      // Render pixels using OpenGL
    };

    Renderer();
    ~Renderer();

    // Initialize OpenGL rendering (call after GLFW/GLAD setup)
    void initOpenGL(GLFWwindow* window, int width, int height);
    
    // Set rendering mode
    void setMode(Mode mode);
    Mode getMode() const;

    void beginFrame();
    void endFrame();

    void drawPixel(int x, int y);
    void drawPixel(int x, int y, float r, float g, float b); // Colored pixel

    void clearRecordedPixels();
    const std::set<std::pair<int,int>>& getRecordedPixels() const;
    
    // OpenGL specific
    void setClearColor(float r, float g, float b);
    void setPixelColor(float r, float g, float b);
    void present(); // Display the frame (swap buffers)

private:
    std::set<std::pair<int,int>> recordedPixels;
    
    // OpenGL state
    GLFWwindow* window;
    Mode currentMode;
    int windowWidth;
    int windowHeight;
    float clearR, clearG, clearB;
    float pixelR, pixelG, pixelB;
    
    // Modern OpenGL 4.6 resources
    unsigned int shaderProgram;
    
    // Pixel buffer for OpenGL rendering
    std::vector<std::pair<int, int>> currentFramePixels;
    
    void renderPixelsToOpenGL();
};
