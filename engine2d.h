#ifndef ENGINE2D_H
#define ENGINE2D_H

#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#define RED                                                                    \
  { 1.0f, 0.0f, 0.0f }
#define GREEN                                                                  \
  { 0.0f, 1.0f, 0.0f }
#define BLUE                                                                   \
  { 0.0f, 0.0f, 1.0f }
#define YELLOW                                                                 \
  { 1.0f, 1.0f, 0.0f }
#define PURPLE                                                                 \
  { 0.5f, 0.0f, 0.5f }

class Engine {
private:
  GLFWwindow *window;
  int winWidth;
  int winHeight;
  unsigned int shaderProgram;

public:
  Engine(int newWinWidth, int newWinHeight, const char *title);

private:
  void initWin(const char *title);
  void initShaders();
  void checkStatus(unsigned int shader, unsigned int checkStatus);
  void drawGrid(float step);
  glm::vec2 RotVector(glm::vec2 vector, float theta);
  void drawVector(glm::vec2 vector, glm::vec3 color);
  void drawVector(glm::vec2 vector, glm::vec3 color, glm::vec2 origin);
  void mainLoop();
};

#endif
