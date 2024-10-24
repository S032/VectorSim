#include "engine2d.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

Engine::Engine(int newWinWidth, int newWinHeight, const char *title)
    : winWidth(newWinWidth), winHeight(newWinHeight) {
  initWin(title);
  initShaders();
  mainLoop();
}

void Engine::initWin(const char *title) {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to init GLFW\n");
    exit(EXIT_FAILURE);
  }

  window = glfwCreateWindow(winWidth, winHeight, title, NULL, NULL);
  if (window == NULL) {
    std::cerr << "Failed to init Window" << std::endl;
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glViewport(0, 0, winWidth, winHeight);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
}

void Engine::initShaders() {
  GLuint res = glewInit();
  if (GLEW_OK != res) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
    exit(EXIT_FAILURE);
  }

  const char *vertexShaderSource =
      "#version 330 core \n"
      "in vec2 vecPos; \n"
      "in vec4 vecColor; \n"
      "out vec4 Color; \n"
      "void main() {\n"
      "gl_Position = vec4(vecPos.x, vecPos.y, 0.0, 1.0);\n"
      "Color = vecColor;\n"
      "}";

  const char *fragmentShaderSource = "#version 330 core \n"
                                     "in vec4 Color; \n"
                                     "out vec4 fragColor; \n"
                                     "void main() {\n"
                                     "fragColor = Color;\n"
                                     "}";
  ///
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  checkStatus(vertexShader, GL_COMPILE_STATUS);

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  checkStatus(fragmentShader, GL_COMPILE_STATUS);

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "fragColor");
  glLinkProgram(shaderProgram);
  checkStatus(shaderProgram, GL_LINK_STATUS);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Engine::checkStatus(unsigned int shader, unsigned int checkStatus) {
  int success;
  char infoLog[512];
  glGetShaderiv(shader, checkStatus, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Engine::drawGrid(float step) {
  std::vector<float> Axis;
  // v
  for (float i = -1; i <= 1; i += 0.1) {
    float epsilon = 0.0001f;
    float aChannel;
    if (fabs(i) < epsilon)
      aChannel = 1.0f;
    else
      aChannel = 0.2f;
    //////////////////////////hLine
    // vertex
    Axis.push_back(i);
    Axis.push_back(1.0f);
    // color
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    Axis.push_back(aChannel);
    // vertex
    Axis.push_back(i);
    Axis.push_back(-1.0f);
    // color
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    Axis.push_back(aChannel);
    //////////////////////////hticks
    Axis.push_back(i);
    Axis.push_back(0.025f);
    // color
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    // vertex
    Axis.push_back(i);
    Axis.push_back(-0.025f);
    // color
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    //////////////////////////vLine
    // vertex
    Axis.push_back(1.0f);
    Axis.push_back(i);
    // color
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    Axis.push_back(aChannel);
    // vertex
    Axis.push_back(-1.0f);
    Axis.push_back(i);
    // color
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    Axis.push_back(aChannel);
    //////////////////////////vticks
    Axis.push_back(0.025f);
    Axis.push_back(i);
    // color
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    // vertex
    Axis.push_back(-0.025f);
    Axis.push_back(i);
    // color
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
    Axis.push_back(1.0f);
  }

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, Axis.size() * sizeof(float), Axis.data(),
               GL_STATIC_DRAW);

  GLuint pos = glGetAttribLocation(shaderProgram, "vecPos");
  glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)0);
  glEnableVertexAttribArray(pos);

  GLuint color = glGetAttribLocation(shaderProgram, "vecColor");
  glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(color);

  glUseProgram(shaderProgram);
  glDrawArrays(GL_LINES, 0, Axis.size());

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &VBO);
}

glm::vec2 Engine::RotVector(glm::vec2 vector, float theta) {
  float theta_rad = (theta * M_PI) / 180.0f;
  glm::mat2x2 IdentityMat = glm::mat2x2(0.0f);
  IdentityMat[0][0] = glm::cos(theta_rad);
  IdentityMat[1][0] = glm::sin(theta_rad);
  IdentityMat[0][1] = -glm::sin(theta_rad);
  IdentityMat[1][1] = glm::cos(theta_rad);

  glm::vec2 resVec = IdentityMat * vector;
  return resVec;
}

void Engine::drawVector(glm::vec2 vector, glm::vec3 _color) {
  std::vector<float> vertices = {
      0,        0,        _color.x,      _color.y,
      _color.z, 1,        vector.x / 10, vector.y / 10,
      _color.x, _color.y, _color.z,      1};

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  GLuint pos = glGetAttribLocation(shaderProgram, "vecPos");
  glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)0);
  glEnableVertexAttribArray(pos);

  GLuint color = glGetAttribLocation(shaderProgram, "vecColor");
  glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(color);

  glUseProgram(shaderProgram);
  glDrawArrays(GL_LINES, 0, vertices.size());

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &VBO);
}

void Engine::drawVector(glm::vec2 vector, glm::vec3 _color, glm::vec2 origin) {
  std::vector<float> vertices = {origin.x / 10,
                                 origin.y / 10,
                                 _color.x,
                                 _color.y,
                                 _color.z,
                                 1,
                                 origin.x / 10 + vector.x / 10,
                                 origin.y / 10 + vector.y / 10,
                                 _color.x,
                                 _color.y,
                                 _color.z,
                                 1};

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  GLuint pos = glGetAttribLocation(shaderProgram, "vecPos");
  glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)0);
  glEnableVertexAttribArray(pos);

  GLuint color = glGetAttribLocation(shaderProgram, "vecColor");
  glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(color);

  glUseProgram(shaderProgram);
  glDrawArrays(GL_LINES, 0, vertices.size());

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &VBO);
}

void Engine::mainLoop() {
  float theta = 0.0f;
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0196f, 0.1412f, 0.098f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawGrid(5.0f);

    // Creating vector and drawing it
    glm::vec2 vector = {2, 5};
    drawVector(vector, RED);

    glm::vec2 vec1 = {1, 0};
    vec1 *= 8;
    glm::vec rotvec1 = RotVector(vec1, theta);
    theta += 0.5f;
    drawVector(rotvec1, YELLOW);
    drawVector({rotvec1.x, 0}, RED);
    drawVector({0, rotvec1.y}, GREEN, {rotvec1.x, 0});

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
}
