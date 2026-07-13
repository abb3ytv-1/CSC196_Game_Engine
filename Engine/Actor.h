#pragma once

#include "Transform.h"


namespace nu {
    class Actor {
        public:
            Actor() = default;
            Actor(const Transform& transform) : a_transform{ transform } {}

            void Update(float dt);

            void Draw(const class Renderer& renderer);

            const Transform& GetTransform() { return a_transform; }
            void SetPosition(const Vector2& position) { a_transform.position = position; }
            void SetRotation(float rotation) { a_transform.rotation = rotation; }
            void SetScale(float scale) { a_transform.scale = scale; }

            const Vector2& GetVelocity() const { return a_velocity; }
            void SetVelocity(const Vector2& velocity) { a_velocity = velocity; }


        protected:
            Transform a_transform;
            Vector2 a_velocity{ 0,0 };
        };
 }