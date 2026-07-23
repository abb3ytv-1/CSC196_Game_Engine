#pragma once

#include "Transform.h"
#include "Model.h"


namespace nu {
    class Actor {

        /*
        * Actor description:
        * - name
        * - model
        * - transform
        * - speed
        * - damping
        * - velocity
        * - tag
        * - lifespan
        */ 
        

        public:
            virtual ~Actor() = default;
            Actor(const Transform& transform) : a_transform{ transform } {}
            Actor(const Transform& transform, const Model& model) : a_transform{transform}, a_model{model} {}

            virtual void Update(float dt);
            virtual void Draw(const class Renderer& renderer) const;

            const Transform& GetTransform() const { return a_transform; }
            void SetPosition(const Vector2& position) { a_transform.position = position; }
            void SetRotation(float rotation) { a_transform.rotation = rotation; }
            void SetScale(float scale) { a_transform.scale = scale; }

            const Vector2& GetVelocity() const { return a_velocity; }
            void SetVelocity(const Vector2& velocity) { a_velocity = velocity; }


        protected:
            Transform a_transform;
            Vector2 a_velocity{ 0,0 };
            
            Model a_model;
        };
 }