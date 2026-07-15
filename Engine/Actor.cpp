#include "pch.h"
#include "Actor.h"
#include "Renderer.h"
#include "MathUtils.h"


using namespace nu;

namespace nu {

        void Actor::Update(float dt) {
            a_transform.position += (a_velocity * dt);
            a_velocity *= 0.997f;

            a_transform.position.x = Wrap(0.0f, 1920.0f, a_transform.position.x);
            a_transform.position.y = Wrap(0.0f, 1080.0f, a_transform.position.y);
        }

        void Actor::Draw(const Renderer& renderer) {
            renderer.DrawModel(a_model, a_transform);

        //    renderer.SetColor(a_model.GetColor().r, a_model.GetColor().g, a_model.GetColor().b);

        //    // Character
        //    renderer.SetColor(0.0f, 255.0f, 255.0f, 255.0f);
        //    renderer.DrawFillRect(a_transform.position.x - (a_transform.scale * 0.5f),
        //                          a_transform.position.y - (a_transform.scale * 0.5f),
        //                          a_transform.scale, a_transform.scale);
        }
}