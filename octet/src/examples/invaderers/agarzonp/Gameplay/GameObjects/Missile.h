#ifndef MISSILE_H
#define MISSILE_H

namespace agarzonp
{
	class Missile : public GameObject
	{

	public:

		void Update() override
		{
			Move();
			CheckCollisions();
		}

		void OnCollisionWith(const GameObject* gameOject) override
		{
			GameObject::OnCollisionWith(gameOject);
			Translate(20.0f, 0.0f);
		}

	private:

		void Move()
		{
			const float missile_speed = 0.3f;
			Translate(0, missile_speed);
		}

		void CheckCollisions()
		{
			// check for collisions
			for (int index = 0; index != num_invaderers; ++index)
			{
				GameObject* invaderer = World::GetInstance()->GetPool()->GetInvaderer(index);
				if (invaderer->IsInUse() && CollidesWith(invaderer))
				{
					invaderer->OnCollisionWith(this);
					OnCollisionWith(invaderer);

					// tell the triggerer that you hit an invaderer
					triggerer->OnGameObjectHit(invaderer);

					SoundManager::GetInstance()->Play(SoundId::BANG);

					return;
				}
			}

			GameObject* border = World::GetInstance()->GetPool()->GetBorder(top_border_object);
			if (CollidesWith(border))
			{
				// No need to notify the border about the collision
				OnCollisionWith(border);
			}
		}

	};
}

#endif // !MISSILE
