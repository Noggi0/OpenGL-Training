//
// Created by vince on 29/07/2022.
//

#ifndef OPENGL_TRAINING_CAMERA_HPP
#define OPENGL_TRAINING_CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr float YAW         = -90.0f;
constexpr float PITCH       =  0.0f;
constexpr float SPEED       =  2.5f;
constexpr float SENSITIVITY =  0.1f;
constexpr float ZOOM        =  45.0f;

enum CameraDirection {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
            this->cameraPosition = position;
            this->cameraWorldUp = up;
            this->yaw = yaw;
            this->pitch = pitch;
            this->updateCameraVectors();
        };
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
            this->cameraPosition = glm::vec3(posX, posY, posZ);
            this->cameraWorldUp = glm::vec3(upX, upY, upZ);
            this->yaw = yaw;
            this->pitch = pitch;
            updateCameraVectors();
        };
        glm::mat4 getViewMatrix() const {
            return glm::lookAt(this->cameraPosition, this->cameraPosition + this->cameraFront, this->cameraUp);
        };
        void processKeyboard(CameraDirection direction, float deltaTime) {
            float velocity = this->movementSpeed * deltaTime;
            if (direction == FORWARD)
                this->cameraPosition += this->cameraFront * velocity;
            if (direction == BACKWARD)
                this->cameraPosition -= this->cameraFront * velocity;
            if (direction == LEFT)
                this->cameraPosition -= this->cameraRight * velocity;
            if (direction == RIGHT)
                this->cameraPosition += this->cameraRight * velocity;
        };
        void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true) {
            xOffset *= this->mouseSensitivity;
            yOffset *= this->mouseSensitivity;

            this->yaw += xOffset;
            this->pitch += yOffset;

            if (constrainPitch)
            {
                if (this->pitch > 89.0f)
                    this->pitch = 89.0f;
                if (this->pitch < -89.0f)
                    this->pitch = -89.0f;
            }
            updateCameraVectors();
        };
        void processMouseScroll(float yOffset) {
            this->zoom -= (float)yOffset;
            if (this->zoom < 1.0f)
                this->zoom = 1.0f;
            if (this->zoom > 90.0f)
                this->zoom = 90.0f;
        };

        glm::vec3 cameraPosition;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;
        glm::vec3 cameraRight;
        glm::vec3 cameraWorldUp;
        float yaw;
        float pitch;
        float movementSpeed;
        float mouseSensitivity;
        float zoom;
    private:
        void updateCameraVectors()
        {
            glm::vec3 front;
            front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
            front.y = sin(glm::radians(this->pitch));
            front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
            this->cameraFront = glm::normalize(front);
            this->cameraRight = glm::normalize(glm::cross(this->cameraFront, this->cameraWorldUp));
            this->cameraUp = glm::normalize(glm::cross(this->cameraRight, this->cameraFront));
        }
};

#endif //OPENGL_TRAINING_CAMERA_HPP
