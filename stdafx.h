#include <array>
#include <filesystem>
#include <format>
#include <iostream>
#include <numbers>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL.hpp>

#define GLM_FORCE_LEFT_HANDED
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale
#include <glm/ext/scalar_constants.hpp>  // glm::pi
#include <glm/mat4x4.hpp>                // glm::mat4
#include <glm/vec3.hpp>                  // glm::vec3
#include <glm/vec4.hpp>                  // glm::vec4
