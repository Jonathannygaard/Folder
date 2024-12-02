#include "Camera.h"
#include <glad/glad.h>
#include "../../Shader/Shader.h"

void Camera::init() {
    viewLoc = glGetUniformLocation(Shader::Program, "view");
    projectionLoc = glGetUniformLocation(Shader::Program, "projection");
}

glm::mat4 Camera::getView() {
    glm::mat4 view = glm::mat4(1.f);
    view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
    return view;
}
 
glm::mat4 Camera::getProjection(float width, float height) {
    glm::mat4 proj = glm::mat4(1.f);
    proj = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 10000.0f);
    //proj = glm::ortho(-width/height, width/height, -1.f, 1.f);
    return proj;
}

void Camera::setPlayerPos(glm::vec3 position)
{
    PlayerPos = glm::vec3(position.x + 0.375f, position.y + 0.65f, position.z - 0.375f);
}

glm::vec3 Camera::OrbitCamera()
{
    cameraPos = glm::vec3(PlayerPos.x,PlayerPos.y,PlayerPos.z) + cameraFront * -10.f;
    return cameraPos;
}

void Camera::updatePos(glm::vec3 newpos)
{
    cameraPos += newpos;
    cameraFront += newpos;
}