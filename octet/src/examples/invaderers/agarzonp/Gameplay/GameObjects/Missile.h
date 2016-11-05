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

			//octet::ALuint source = get_sound_source();
			//octet::alSourcei(source, octet::AL_BUFFER, bang);
			//octet::alSourcePlay(source);

			// InvadererWave code
			/*
			live_invaderers--;
			score++;
			if (live_invaderers == 4) {
				invader_velocity *= 4;
			}
			else if (live_invaderers == 0) {
				game_over = true;
				(*spritesPool)[game_over_sprite].translate(-20, 0);
			}
			*/
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
