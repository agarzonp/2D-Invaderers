#ifndef PLAYER_H
#define PLAYER_H

namespace agarzonp
{
	class Player : public GameObject
	{
		int missiles_disabled;
		int num_lives;

	public:

		Player()
			: GameObject()
			, missiles_disabled(0)
			, num_lives(3)
		{
		}

		void Update() override
		{
			Move();
			Fire();
		}

		void OnCollisionWith(const GameObject* other) override
		{
			--num_lives;

			if (num_lives <= 0)
			{
				// World code
				//game_over = true;
			}
		}

	private:

		void Move()
		{
			const float ship_speed = 0.05f;
			// left and right arrows
			
			if (Input::is_key_down(octet::key_left))
			{
				Translate(-ship_speed, 0);
				if (CollidesWith(World::GetInstance()->GetPool()->GetBorder(2)))
				{
					Translate(+ship_speed, 0);
				}
			}
			else if (Input::is_key_down(octet::key_right))
			{
				Translate(+ship_speed, 0);

				if (CollidesWith(World::GetInstance()->GetPool()->GetBorder(3)))
				{
					Translate(-ship_speed, 0);
				}
			}
			
		}

		void Fire()
		{
			if (missiles_disabled)
			{
				--missiles_disabled;
			}
			else if (Input::is_key_going_down(' '))
			{
				// find a missile
				
				GameObject* missile = GameObjectFactory::GetInstance()->CreateMissile(shader);
				missile->SetRelativeTo(this, 0, 0.5f);
				World::GetInstance()->AddGameObject(missile);

				missiles_disabled = 5;

				SoundManager::GetInstance()->Play(SoundId::WHOOSH);
			}
		}

	};
}


#endif // !PLAYER_H

