#include "HW6.h"

const int width = 1000;
const int height = 1000;

glm::vec3 direction(0.0f, 0.0f, 0.0f);
int shadingRB = 1;
int lightRB = 3;
int viewRB = 5;

bool firstMouse = true;
double lastX = width / 2.0f;
double lastY = height / 2.0f;

Camera camera;

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  // switch
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
    shadingRB = 1;
  }
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
    shadingRB = 2;
  }

  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
    lightRB = 3;
  }
  if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
    lightRB = 4;
  }

  if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
    viewRB = 5;
  }
  if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
    viewRB = 6;
  }

  // move
  direction = glm::vec3(0.0f, 0.0f, 0.0f);
  if (viewRB == 5) {
    return;
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP)) {
    direction.z += 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN)) {
    direction.z -= 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT)) {
    direction.x -= 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT)) {
    direction.x += 1.0f;
  }
}

void mouse_callback(GLFWwindow* window, double posX, double posY) {
  if (firstMouse) {
    firstMouse = false;
    lastX = posX;
    lastY = posY;
  }

  double offsetX = posX - lastX;
  double offsetY = -(posY - lastY);

  lastX = posX;
  lastY = posY;

  camera.rotate(offsetX, offsetY);
}

int main() {
  // initialize window
  GLFWwindow* window;
  const char* title = "HW6";

  initWindow(window, width, height, title);
  glfwSetCursorPosCallback(window, mouse_callback);

  // initialize GLAD
  initGLAD();

  // intialize shaders
  Shader PhongShader = PhongShading();
  Shader GouraudShader = GouraudShading();

  // initialize ImGui
  initImGui(window);

  float orthoLeft = -5.0f;
  float orthoRight = 5.0f;
  float orthoBottom = -5.0f;
  float orthoTop = 5.0f;
  float orthoNear = 0.1f;
  float orthoFar = 20.0f;

  float persFov = 45.0f;
  float persNear = 0.1f;
  float persFar = 20.0f;

  // initialize texture
  unsigned int texture;

  initTexture(texture, "./container.jpg");

  PhongShader.use();
  PhongShader.setInt("texture", 0);
  GouraudShader.use();
  GouraudShader.setInt("texture", 0);

  // draw
  glEnable(GL_DEPTH_TEST);

  float vertices[] = {
    /*
    vec3                  vec2         vec3               vec3
    position              texture      color              normal
    */

    // behind
    -2.0f, -2.0f, -2.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
     2.0f, -2.0f, -2.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
     2.0f,  2.0f, -2.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
     2.0f,  2.0f, -2.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
    -2.0f,  2.0f, -2.0f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
    -2.0f, -2.0f, -2.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f,  0.0f, -1.0f,

    // front
    -2.0f, -2.0f,  2.0f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f,  0.0f,  1.0f,
     2.0f, -2.0f,  2.0f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f,  0.0f,  1.0f,
     2.0f,  2.0f,  2.0f,  1.0f, 1.0f,  1.0f, 1.0f, 0.0f,   0.0f,  0.0f,  1.0f,
     2.0f,  2.0f,  2.0f,  1.0f, 1.0f,  1.0f, 1.0f, 0.0f,   0.0f,  0.0f,  1.0f,
    -2.0f,  2.0f,  2.0f,  0.0f, 1.0f,  1.0f, 1.0f, 0.0f,   0.0f,  0.0f,  1.0f,
    -2.0f, -2.0f,  2.0f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f,  0.0f,  1.0f,

    // left
    -2.0f,  2.0f,  2.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -2.0f,  2.0f, -2.0f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -2.0f, -2.0f, -2.0f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -2.0f, -2.0f, -2.0f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -2.0f, -2.0f,  2.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -2.0f,  2.0f,  2.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,

    // right
     2.0f,  2.0f,  2.0f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     2.0f,  2.0f, -2.0f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     2.0f, -2.0f, -2.0f,  0.0f, 1.0f,  0.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     2.0f, -2.0f, -2.0f,  0.0f, 1.0f,  0.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     2.0f, -2.0f,  2.0f,  0.0f, 0.0f,  0.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     2.0f,  2.0f,  2.0f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,

     // bottom
    -2.0f, -2.0f, -2.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
     2.0f, -2.0f, -2.0f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
     2.0f, -2.0f,  2.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
     2.0f, -2.0f,  2.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
    -2.0f, -2.0f,  2.0f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
    -2.0f, -2.0f, -2.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,   0.0f, -1.0f,  0.0f,

    // top
    -2.0f,  2.0f, -2.0f,  0.0f, 1.0f,  1.0f, 0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
     2.0f,  2.0f, -2.0f,  1.0f, 1.0f,  1.0f, 0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
     2.0f,  2.0f,  2.0f,  1.0f, 0.0f,  1.0f, 0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
     2.0f,  2.0f,  2.0f,  1.0f, 0.0f,  1.0f, 0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
    -2.0f,  2.0f,  2.0f,  0.0f, 0.0f,  1.0f, 0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
    -2.0f,  2.0f, -2.0f,  0.0f, 1.0f,  1.0f, 0.0f, 1.0f,   0.0f,  1.0f,  0.0f
  };

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);

  // light
  glm::vec3 lightColor(1.0f); 
  glm::vec3 lightPos(-4.0f, 0.0f, 6.0f);
  float ambientStrength = 0.2f;
  float diffuseStrength = 1.0f;
  float specularStrength = 0.8f;
  int shininess = 64;

  PhongShader.use();
  PhongShader.setVec3("lightColor", lightColor);
  PhongShader.setVec3("lightPos", lightPos);
  PhongShader.setFloat("ambientStrength", ambientStrength);
  PhongShader.setFloat("diffuseStrength", diffuseStrength);
  PhongShader.setFloat("specularStrength", specularStrength);
  PhongShader.setInt("shininess", shininess);

  GouraudShader.use();
  GouraudShader.setVec3("lightColor", lightColor);
  GouraudShader.setVec3("lightPos", lightPos);
  GouraudShader.setFloat("ambientStrength", ambientStrength);
  GouraudShader.setFloat("diffuseStrength", diffuseStrength);
  GouraudShader.setFloat("specularStrength", specularStrength);
  GouraudShader.setInt("shininess", shininess);

  const char* lightVertexShaderSource =
    "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
    "  gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "}\0";
  const char* lightFragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "  FragColor = vec4(1.0);\n"
    "}\0";

  Shader lightShader(lightVertexShaderSource, lightFragmentShaderSource);
  lightShader.use();

  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // update
  glm::mat4 currentView = camera.getView();

  float deltaTime = 0.0f;
  float lastTime = (float)glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    float currTime = (float)glfwGetTime();
    deltaTime = currTime - lastTime;
    lastTime = currTime;

    processInput(window);
    camera.move(direction, deltaTime);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Light");

    ImGui::Text("Shading");
    ImGui::RadioButton("1.Phong", &shadingRB, 1); ImGui::SameLine();
    ImGui::RadioButton("2.Gouraud", &shadingRB, 2);

    ImGui::Text("Light");
    ImGui::RadioButton("3.Freeze", &lightRB, 3); ImGui::SameLine();
    ImGui::RadioButton("4.Move", &lightRB, 4);

    ImGui::Text("View");
    ImGui::RadioButton("5.Freeze", &viewRB, 5); ImGui::SameLine();
    ImGui::RadioButton("6.Move", &viewRB, 6);

    ImGui::Text("Shininess");
    ImGui::RadioButton("1", &shininess, 1); ImGui::SameLine();
    ImGui::RadioButton("2", &shininess, 2); ImGui::SameLine();
    ImGui::RadioButton("4", &shininess, 4); ImGui::SameLine();
    ImGui::RadioButton("8", &shininess, 8); ImGui::SameLine();
    ImGui::RadioButton("16", &shininess, 16); ImGui::SameLine();
    ImGui::RadioButton("32", &shininess, 32); ImGui::SameLine();
    ImGui::RadioButton("64", &shininess, 64); ImGui::SameLine();
    ImGui::RadioButton("128", &shininess, 128); ImGui::SameLine();
    ImGui::RadioButton("256", &shininess, 256); ImGui::SameLine();
    ImGui::RadioButton("512", &shininess, 512); ImGui::SameLine();
    ImGui::RadioButton("1024", &shininess, 1024);

    ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0, 1.0);
    ImGui::SliderFloat("Diffuse Strength", &diffuseStrength, 0.0, 1.0);
    ImGui::SliderFloat("Specular Strength", &specularStrength, 0.0, 1.0);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // mouse
    if (viewRB == 5) {
      firstMouse = true;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else if (viewRB == 6) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    // render
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    if (viewRB == 5) {
      view = currentView;
    }
    else if (viewRB == 6) {
      view = camera.getView();
      currentView = view;
    }

    PhongShader.use();
    PhongShader.setFloat("ambientStrength", ambientStrength);
    PhongShader.setFloat("diffuseStrength", diffuseStrength);
    PhongShader.setFloat("specularStrength", specularStrength);
    PhongShader.setInt("shininess", shininess);

    GouraudShader.use();
    GouraudShader.setFloat("ambientStrength", ambientStrength);
    GouraudShader.setFloat("diffuseStrength", diffuseStrength);
    GouraudShader.setFloat("specularStrength", specularStrength);
    GouraudShader.setInt("shininess", shininess);

    if (shadingRB == 1) {
      PhongShader.use();
      PhongShader.setMat4("model", model);
      PhongShader.setMat4("view", view);
      PhongShader.setMat4("projection", projection);
      PhongShader.setVec3("viewPos", camera.getViewPos());

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    else if (shadingRB == 2) {
      GouraudShader.use();
      GouraudShader.setMat4("model", model);
      GouraudShader.setMat4("view", view);
      GouraudShader.setMat4("projection", projection);
      GouraudShader.setVec3("viewPos", camera.getViewPos());

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // light
    glm::mat4 lightModel = glm::mat4(1.0f);

    if (lightRB == 4) {
      lightRotate(lightPos, deltaTime);
    }

    PhongShader.use();
    PhongShader.setVec3("lightPos", lightPos);
    GouraudShader.use();
    GouraudShader.setVec3("lightPos", lightPos);

    lightModel = glm::translate(lightModel, lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));

    lightShader.use();
    lightShader.setMat4("model", lightModel);
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", projection);

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // clear
  glDeleteBuffers(1, &VAO);
  glDeleteBuffers(1, &VBO);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

