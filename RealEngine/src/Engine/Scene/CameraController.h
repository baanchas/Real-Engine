#pragma once

#include "Entity.h"

namespace RealEngine {

    class CameraController : public ScriptableEntity
    {
    public:
        void OnCreate()
        {
        };

        void OnDestroy()
        {
        };

        void OnEvent(Event event)
        {
            /*if (event.Type == EventType::MouseScrolled)
            {
                std::cout << "zoomlevel is " << m_ZoomLevel <<  std::endl;

                if (event.MouseScrolled.yOffset == -1)
                {
                    m_ZoomLevel *= 0.95f;

                    auto& transform = GetComponent<TransformComponent>().Transform;

                    transform = glm::ortho(-m_ProjectionWidth / 2 * m_ZoomLevel, m_ProjectionWidth / 2 * m_ZoomLevel, -m_ProjectionHeight / 2 * m_ZoomLevel, m_ProjectionHeight / 2 * m_ZoomLevel, -1.0f, 1.0f);
                    
                }
                else if (event.MouseScrolled.yOffset == 1)
                {
                    m_ZoomLevel *= 1.05f;

                    auto& transform = GetComponent<TransformComponent>().Transform;

                    transform = glm::ortho(-m_ProjectionWidth / 2 * m_ZoomLevel, m_ProjectionWidth / 2 * m_ZoomLevel, -m_ProjectionHeight / 2 * m_ZoomLevel, m_ProjectionHeight / 2 * m_ZoomLevel, -1.0f, 1.0f);
                }
            }*/
        }

        void OnUpdate(float ts)
        {
            auto& transform = GetComponent<TransformComponent>();
            float speed = 5.0;


            if (Input::IsKeyPressed(KeyCodes::LEFT))
            {
                transform.Position.x -= speed * ts;
            }
            if (Input::IsKeyPressed(KeyCodes::RIGHT))
            {
                transform.Position.x += speed * ts;
            }
            if (Input::IsKeyPressed(KeyCodes::DOWN))
            {
                transform.Position.y -= speed * ts;
            }
            if (Input::IsKeyPressed(KeyCodes::UP))
            {
                transform.Position.y += speed * ts;
            }

            //m_ProjectionWidth = transform[3][0];
           //m_ProjectionHeight = transform[3][1];

        };

    private:
        float m_ZoomLevel = 1.0f;

        float m_ProjectionWidth = 10.0f;
        float m_ProjectionHeight = 10.0f;
    };

}

