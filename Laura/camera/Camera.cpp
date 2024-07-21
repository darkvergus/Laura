#include "camera/Camera.h"

namespace Laura {

    FlyThroughCamera::FlyThroughCamera(float fov, float aspectRatio)
    {
        m_Fov = fov;
        m_AspectRatio = aspectRatio;
        m_Position = glm::vec3(0);
        m_Rotation = glm::quat(0, 0, 1, 0);
    }

    void FlyThroughCamera::Rotate(float pitch, float yaw, float roll) {
        glm::quat pitchQuat = glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0));
        glm::quat yawQuat = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0));
        glm::quat rollQuat = glm::angleAxis(glm::radians(roll), glm::vec3(0, 0, 1));

        // Apply the roll, then yaw, then pitch
        m_Rotation = yawQuat * pitchQuat * rollQuat * m_Rotation;
        m_Rotation = glm::normalize(m_Rotation);
    }

}



