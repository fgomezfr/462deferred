#include "camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() : eye_pos( glm::vec3( 0.0f, 0.0f, 0.0f ) ),
				   view_dir( glm::vec3( 0.0f, 0.0f, -1.0f ) ),
				   up_dir( glm::vec3( 0.0f, 1.0f, 0.0f ) ),
				   proj_mat( glm::perspective( 45.0f, 1.25f, 0.1f, 1000.0f ) )
{
}

Camera::Camera( float fovy, float aspect, float near, float far )
	: eye_pos( glm::vec3( 0.0f, 0.0f, 0.0f ) ),
	  view_dir( glm::vec3( 0.0f, 0.0f, -1.0f ) ),
	  up_dir( glm::vec3( 0.0f, 1.0f, 0.0f ) ),
	  proj_mat( glm::perspective( fovy, aspect, near, far ) )
{
}

Camera::~Camera()
{
}

void Camera::handleInput( float deltaTime )
{
	// adjust the camera position and orientation to account for movement over deltaTime seconds
	// use sf::IsKeyPressed( sf::Keyboard::A ) to check if 'a' is currently pressed, etc
}

// get a read-only handle to the projection matrix
const glm::mat4& Camera::getProjectionMatrix() const
{
	return proj_mat;
}

glm::mat4 Camera::getViewMatrix() const
{
	// construct and return a view matrix from your position representation
	return glm::mat4();
}