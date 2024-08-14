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

class gameObject {
public:
	glm::vec3 position = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 rotation = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);
	 
	gameObject(float vertices[], float indices[]) {

	}
};