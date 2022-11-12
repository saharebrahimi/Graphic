#include "Camera.h"
#include "iostream"

Camera::Camera(short width, short height) :
	//we want this programe to be in prespective mode
	isPerspective(true),
	// to be realistic we usually set  this to 45 degrees
	fieldOfView(45.f),
	//between this to parameter we can se in eye of the camera
	nearDistance(0.1f),
	farDistance(100.f),
	 yaw_direction (1),
     pitch_direction (1)
{
	this->width = width;
	this->height = height;
	ComputeViewMatrix();
	ComputeProjectionMatrix();
}


void Camera::Update(GLFWwindow* window)
{

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	static double lastX = 0, lastY = 0;
	static float rotX = 0.f, rotY = 0.f;
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	double deltaX = x - lastX;
	double deltaY = y - lastY;
	lastX = x;
	lastY = y;

	auto targetY = (float)(deltaY / 50);
	auto targetX = (float)deltaX / 50;

	auto forward = GetForward();
	auto right = GetRight();
	auto up = GetUp();
	auto rotation = glm::degrees(glm::eulerAngles(GetQuaternion()));

	//std::cout << forward.x << " " << forward.y << " " << forward.z << std::endl;
	//std::cout << forward.x << " " << forward.y << " " << forward.z << std::endl;
	//std::cout << rotation.x << " " << rotation.y << " " << rotation.z << std::endl;

	
	forward.z = -forward.z;
	right.z = -right.z;
	up.z = -up.z;
	auto pos = GetPosition();

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	float cameraSpeed = 0.0001f * deltaTime;
	std::cout << "speed:";
    std::cout << cameraSpeed<<std::endl;
	std::cout << "deltatime:";
	std::cout << deltaTime << std::endl;
	std::cout << std::endl;
	glm::vec3 result = glm::vec3(0, 0, 0);
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		result = result + (forward*cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		result = result -(forward* cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		result = result + (right * cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		result = result - (right * cameraSpeed);
	}

	//same as the other we use up vector to go towards up and down direction
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		result = result +(up*cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		result = result -(up*cameraSpeed);
	}
	SetPosition(pos+result);
	
	
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		yaw_direction = -yaw_direction;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		pitch_direction = -pitch_direction;

	}

	//if  pitch direction<0 mouse move the way it supposed to
	if (pitch_direction > 0)
		rotY -= targetY;
	//if  pitch direction>0 mouse move reverse 
	if (pitch_direction < 0)
		rotY += targetY;
	//if  yaw direction>0 mouse move the way it supposed to
	if (yaw_direction > 0)
		rotX -= targetX;
	//if  yaw direction<0 mouse move reverse
	if (yaw_direction < 0)
		rotX += targetX;

	rotY = glm::clamp<float>(rotY, -85, 85);

	//std::cout << right.x << " " << right.y << " " << right.z << std::endl;
	auto first= glm::angleAxis(glm::radians(rotX), glm::vec3(0.f, 1.f, 0.f));
	auto second = glm::angleAxis(glm::radians(rotY), glm::vec3(1.f, 0.f, 0.f));
	auto mul = first * second;
	SetQuaternion(mul);
	
	

}

void Camera::Setup()
{
	//	SetFarDistance(100.f);
	//	SetFieldOfView(45.f);
	//	SetNearDistance(0.1f);
	//
}


void Camera::SetupMatrices()
{
	PositionedOrientedObject::SetupMatrices();
	ComputeViewMatrix();
}

void Camera::ComputeViewMatrix()
{
	viewMatrix = glm::mat4(RotateWithRadianAngles(glm::mat4(1.f), -glm::degrees(glm::eulerAngles(GetQuaternion()))));
	viewMatrix = glm::translate(viewMatrix, -GetPosition());
}

void Camera::ComputeProjectionMatrix()
{
	if (isPerspective)
	{
		projectionMatrix = glm::perspective(glm::radians(fieldOfView), 1.f * width / height, nearDistance, farDistance);
	}
	else
	{
		projectionMatrix = glm::ortho(0.f, (float)width, 0.f, (float)height, nearDistance, farDistance);
	}
}

glm::mat4x4 Camera::GetViewMatrix()
{
	return this->viewMatrix;
}

glm::mat4x4 Camera::GetProjectionMatrix()
{
	return this->projectionMatrix;
}

void Camera::SetViewPortSize(short width, short height, bool shouldCompute = true)
{
	this->width = width;
	this->height = height;
	if (shouldCompute)
		ComputeProjectionMatrix();
}

void Camera::SetPerspective(bool prs, bool shouldCompute = true)
{
	this->isPerspective = prs;
	if (shouldCompute)
		ComputeProjectionMatrix();
}

void Camera::SetFieldOfView(float fov, bool shouldCompute = true)
{
	this->fieldOfView = fov;
	if (shouldCompute)
		ComputeProjectionMatrix();
}

void Camera::SetNearDistance(float near, bool shouldCompute = true)
{
	this->nearDistance = near;
	if (shouldCompute)
		ComputeProjectionMatrix();
}

void Camera::SetFarDistance(float far, bool shouldCompute = true)
{
	this->farDistance = far;
	if (shouldCompute)
		ComputeProjectionMatrix();
}

bool Camera::IsPerspective()
{
	return isPerspective;
}

void Camera::GetViewPortSize(short* width, short* height)
{
	width = &this->width;
	height = &this->height;
}

float Camera::GetFieldOfView()
{
	return this->fieldOfView;
}

float Camera::GetNearDistance()
{
	return this->nearDistance;
}

float Camera::GetFarDistance()
{
	return this->farDistance;
}


