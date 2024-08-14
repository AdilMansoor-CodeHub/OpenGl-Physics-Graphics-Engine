#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include <iostream>
#include <vector>
#include "Rigidbody.h"

// function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void RenderImGui();
bool checkCollision(glm::vec3 cubePosition, glm::vec3 scale, glm::vec3 cubePos, glm::vec3 scale3);
// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool controlCamera;
bool collisionDetectionEnabled = true;
bool forwardForceEnabled = true;
bool gravityEnabled = false;

glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cubeRotation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);


glm::vec3 cubePosition2 = glm::vec3(2.0f, 0.0f, -2.0f);
glm::vec3 cubePosition3 = glm::vec3(-2.0f, 0.0f, -3.0f);
glm::vec3 planePosition = glm::vec3(0.0f, -0.6f, 0.0f);
glm::vec3 cubeVelocity = glm::vec3(0.0f); // Initial velocity

// Define initial rotation angles and speeds
static float rotationAngleX = 0.0f;
static float rotationAngleY = 0.0f;
float rotationSpeedX = 50.0f; // Degrees per second for X-axis rotation
float rotationSpeedY = 30.0f; // Degrees per second for Y-axis rotation

const float gravity = -9.81f; // Gravity acceleration
float cubeMass = 1.0f; // Mass of the cube



int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("default.vert", "default.frag");

    // Cube vertex data
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // Plane vertex data
    float planeVertices[] = {
        -1.0f, -0.5f,  1.0f,  0.0f, 1.0f,
         1.0f, -0.5f,  1.0f,  1.0f, 1.0f,
         1.0f, -0.5f, -1.0f,  1.0f, 0.0f,
         1.0f, -0.5f, -1.0f,  1.0f, 0.0f,
        -1.0f, -0.5f, -1.0f,  0.0f, 0.0f,
        -1.0f, -0.5f,  1.0f,  0.0f, 1.0f
    };

    unsigned int cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int planeVBO, planeVAO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);

    glBindVertexArray(planeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("media/wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ourShader.use();
    ourShader.setInt("texture1", 0);

    // Initialize ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
 

    while (!glfwWindowShouldClose(window))
    {
        // Assuming you have a GLFW window created named 'window'
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Tell OpenGL a new frame is about to begin
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        ourShader.use();

        // Drawing the cube
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePosition);
        if (gravityEnabled) {
            cubeVelocity.y -= 9.8f * deltaTime;  // Simple gravity effect
        }
        cubePosition += cubeVelocity * deltaTime;
        
        if (cubePosition.y < 0.0f) {  // Example ground collision check
            cubePosition.y = 0.0f;
            cubeVelocity.y = 0.0f;
        }
        model = glm::scale(model, scale);
        model = glm::rotate(model, glm::radians(cubeRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(cubeRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(cubeRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));


        
   


        // Set model, view, and projection matrices
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setBool("isPlane", false);
        ourShader.setMat4("projection", projection);

        // Draw the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);



        // Draw the second cube
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, cubePosition2);
        model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.0f));
        ourShader.setMat4("model", model2);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw the third cube
        glm::mat4 model3 = glm::mat4(1.0f);
        model3 = glm::translate(model3, cubePosition3);
        model3 = glm::scale(model3, glm::vec3(1.0f, 1.0f, 1.0f));
        ourShader.setMat4("model", model3);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glm::mat4 planeTransform = glm::mat4(1.0f);
        planeTransform = glm::translate(planeTransform, planePosition);
        planeTransform = glm::scale(planeTransform, glm::vec3(10.0f, 0.1f, 10.0f)); // Adjust plane size

        ourShader.setMat4("model", planeTransform);
        ourShader.setBool("isPlane", true);
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        // Cube positions and scales
        glm::vec3 cubePosition1 = cubePosition;


        glm::vec3 scale1 = scale;
        glm::vec3 scale2 = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 scale3 = glm::vec3(1.0f, 1.0f, 1.0f);

        // Check collisions
        if (checkCollision(cubePosition1, scale1, cubePosition2, scale2)) {
            std::cout << "Cube 1 is colliding with Cube 2!" << std::endl;
        }

        if (checkCollision(cubePosition1, scale1, cubePosition3, scale3)) {
            std::cout << "Cube 1 is colliding with Cube 3!" << std::endl;
        }

        if (checkCollision(cubePosition2, scale2, cubePosition3, scale3)) {
            std::cout << "Cube 2 is colliding with Cube 3!" << std::endl;
        }




        RenderImGui();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deletes all ImGUI instances
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);

    glfwTerminate();
    return 0;
}

void RenderImGui()
{
    ImGui::Begin("Transformations");

    // Position controls
    ImGui::Text("Position");
    ImGui::SliderFloat("X", &cubePosition.x, -10.0f, 10.0f);
    ImGui::SliderFloat("Y", &cubePosition.y, -10.0f, 10.0f);
    ImGui::SliderFloat("Z", &cubePosition.z, -10.0f, 10.0f);

    // Rotation controls
    ImGui::Text("Rotation");
    ImGui::SliderFloat("Pitch", &cubeRotation.x, -180.0f, 180.0f);
    ImGui::SliderFloat("Yaw", &cubeRotation.y, -180.0f, 180.0f);
    ImGui::SliderFloat("Roll", &cubeRotation.z, -180.0f, 180.0f);

    // Scale controls
    ImGui::Text("Scale");
    ImGui::SliderFloat("X Scale", &scale.x, 0.1f, 10.0f);
    ImGui::SliderFloat("Y Scale", &scale.y, 0.1f, 10.0f);
    ImGui::SliderFloat("Z Scale", &scale.z, 0.1f, 10.0f);

    ImGui::End();


    // Inspector window
    ImGui::Begin("Inspector");
    ImGui::Checkbox("Enable Collision Detection", &collisionDetectionEnabled);
    ImGui::Checkbox("Enable Forward Force", &forwardForceEnabled);
    ImGui::Checkbox("Enable Gravity", &gravityEnabled);

    // Check if the inspector window is hovered and right mouse button is released
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup("InspectorMenu");
    }

    //  In your UI render loop

    ImGui::Begin("Scene Panel");

    // Button to create a cube
    if (ImGui::Button("Create Cube")) {
        std::cout << "Cube created: ";
        ImGui::Text("Cube");
    }

    // Button to create a light
    if (ImGui::Button("Create Light")) {

    }

    // Dropdown to load a 3D model
    if (ImGui::Button("Load 3D Model")) {

    }

    // Hierarchy display
    ImGui::Text("Scene Hierarchy");


    ImGui::End();

    ImGui::End();



}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    float moveSpeed = 2.0f * deltaTime;
    glm::vec3 oldPosition = cubePosition;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cubePosition.z -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cubePosition.z += moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cubePosition.x -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cubePosition.x += moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        cubePosition.y += moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        cubePosition.y -= moveSpeed;

    if (collisionDetectionEnabled) {
        glm::vec3 cubeScale = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 planeScale = glm::vec3(10.0f, 0.6f, 10.0f);

        if (checkCollision(cubePosition, cubeScale, cubePosition2, cubeScale) ||
            checkCollision(cubePosition, cubeScale, cubePosition3, cubeScale) ||
            checkCollision(cubePosition, planeScale, planePosition, planeScale)) {
            cubePosition = oldPosition;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        rotationAngleY += rotationSpeedY * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        rotationAngleY -= rotationSpeedY * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        rotationAngleX += rotationSpeedX * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        rotationAngleX -= rotationSpeedX * deltaTime;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}



bool checkCollision(glm::vec3 pos1, glm::vec3 scale1, glm::vec3 pos2, glm::vec3 scale2) {
    // Check if the cubes are colliding along the x, y, and z axes
    return (abs(pos1.x - pos2.x) < (scale1.x + scale2.x) / 2.0f &&
        abs(pos1.y - pos2.y) < (scale1.y + scale2.y) / 2.0f &&
        abs(pos1.z - pos2.z) < (scale1.z + scale2.z) / 2.0f);
}
