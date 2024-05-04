#ifndef ENGINE2D_H
#define ENGINE2D_H

#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <ft2build.h>
//#include FT_FREETYPE_H

#define RED {1.0f, 0.0f, 0.0f}
#define GREEN {0.0f, 1.0f, 0.0f}
#define BLUE {0.0f, 0.0f, 1.0f}
#define YELLOW {1.0f, 1.0f, 0.0f}
#define PURPLE {0.5f, 0.0f, 0.5f}

class Engine {
private:
    //FT_Library ft;
    //FT_Face face;
    GLFWwindow *window;
    int winWidth;
    int winHeight;
    unsigned int shaderProgram;
public:
    Engine(int newWinWidth, int newWinHeight, const char* title);
private:
    //void initFreeType();
    //FT_Face loadFont(const char* fontPath, int fontSize);
    //void renderText(FT_Face face, const char* text, glm::vec3 posScale);
    void initWin(const char* title);
    void initShaders();
    void checkStatus(unsigned int shader, unsigned int checkStatus);
    void drawGrid(float step);
    void RotVector(float theta);
    void drawVector(glm::vec2 vector, glm::vec3 color);
    void drawVector(glm::vec2 vector, glm::vec3 color, glm::vec2 origin);
    void mainLoop();
};

#endif