#pragma once
#define _CRT_SECURE_NO_WARNINGS

// ---------- STD -----------------

#include <iostream>
#include <memory>
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <stack>
#include <array>
#include <fstream>

//----------- OPENGL --------------

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//----------- LOG -----------------

#include "Engine/Log.h"

//----------- Input ---------------

#include "Engine/Input/Input.h"
#include "Engine/Input/KeyCodes.h"

//----------- ImGui ---------------

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//----------- GLM -----------------

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//----------- Timestep ------------

#include "Engine/Timestep.h"

//----------- Event ------------

#include "Engine/Events/Event.h"

//----------- ENTITY ----------------

#include "entt/include/entt.hpp"
#include "Scene/Entity.h"