#include "class/renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

// Simple shader source code for modern OpenGL 4.6
const char* vertexShaderSource = R"(
#version 460 core
layout (location = 0) in vec2 aPos;
uniform vec3 uColor;
out vec3 vertexColor;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    gl_PointSize = 2.0;
    vertexColor = uColor;
}
)";

const char* fragmentShaderSource = R"(
#version 460 core
in vec3 vertexColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
)";

// Shader compilation helper
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
    }
    return shader;
}

Renderer::Renderer() 
    : window(nullptr), 
      currentMode(BENCHMARK_MODE),
      windowWidth(800), 
      windowHeight(600),
      clearR(0.0f), clearG(0.0f), clearB(0.0f),
      pixelR(1.0f), pixelG(1.0f), pixelB(1.0f),
      shaderProgram(0) {}

Renderer::~Renderer() {}

void Renderer::initOpenGL(GLFWwindow* win, int width, int height) {
    window = win;
    windowWidth = width;
    windowHeight = height;
    currentMode = OPENGL_MODE;
    
    // Setup OpenGL viewport
    glViewport(0, 0, width, height);
    glEnable(GL_PROGRAM_POINT_SIZE);
    
    // Compile shaders (modern OpenGL 4.6)
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    
    // Link shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader linking error: " << infoLog << std::endl;
    }
    
    glUseProgram(shaderProgram);
    
    // Clean up shaders (no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    std::cout << "OpenGL 4.6 renderer initialized with shaders\n";
}

void Renderer::setMode(Mode mode) {
    currentMode = mode;
}

Renderer::Mode Renderer::getMode() const {
    return currentMode;
}

void Renderer::beginFrame() {
    recordedPixels.clear();
    currentFramePixels.clear();
    
    if (currentMode == OPENGL_MODE && window) {
        glClearColor(clearR, clearG, clearB, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void Renderer::endFrame() {
    if (currentMode == OPENGL_MODE && window) {
        renderPixelsToOpenGL();
    }
}

void Renderer::drawPixel(int x, int y) {
    // Always record for benchmarking
    recordedPixels.insert({x, y});
    
    // Also buffer for OpenGL rendering
    if (currentMode == OPENGL_MODE) {
        currentFramePixels.push_back({x, y});
    }
}

void Renderer::drawPixel(int x, int y, float r, float g, float b) {
    recordedPixels.insert({x, y});
    
    if (currentMode == OPENGL_MODE) {
        currentFramePixels.push_back({x, y});
        // Note: color per pixel would need additional data structure
    }
}

void Renderer::clearRecordedPixels() {
    recordedPixels.clear();
    currentFramePixels.clear();
}

const std::set<std::pair<int,int>>& Renderer::getRecordedPixels() const {
    return recordedPixels;
}

void Renderer::setClearColor(float r, float g, float b) {
    clearR = r;
    clearG = g;
    clearB = b;
}

void Renderer::setPixelColor(float r, float g, float b) {
    pixelR = r;
    pixelG = g;
    pixelB = b;
}

void Renderer::present() {
    if (currentMode == OPENGL_MODE && window) {
        glfwSwapBuffers(window);
    }
}

void Renderer::renderPixelsToOpenGL() {
    if (currentFramePixels.empty()) return;
    
    // Convert pixel coordinates to normalized device coordinates (NDC)
    // NDC: (-1,-1) bottom-left to (1,1) top-right
    std::vector<float> vertices;
    vertices.reserve(currentFramePixels.size() * 2);
    
    for (const auto& pixel : currentFramePixels) {
        float x = (2.0f * pixel.first / windowWidth) - 1.0f;
        float y = 1.0f - (2.0f * pixel.second / windowHeight); // Flip Y
        vertices.push_back(x);
        vertices.push_back(y);
    }
    
    // Create and bind VAO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // Configure vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Set color uniform
    GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    glUniform3f(colorLoc, pixelR, pixelG, pixelB);
    
    // Draw points
    glDrawArrays(GL_POINTS, 0, currentFramePixels.size());
    
    // Cleanup
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}
