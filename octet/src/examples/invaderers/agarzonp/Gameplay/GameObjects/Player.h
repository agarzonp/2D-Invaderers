#ifndef PLAYER_H
#define PLAYER_H

namespace agarzonp
{
	class Player : public GameObject
	{
		int missiles_disabled;
		int lives;
		int score;

	public:

		Player()
			: GameObject()
			, missiles_disabled(0)
			, lives(3)
			, score(0)
		{
		}

		int GetLives()
		{
			return lives;
		}

		int GetScore()
		{
			return score;
		}

		void Update() override
		{
			Move();
			Fire();
		}

		void Render(const octet::math::mat4t& cameraToWorld) override
		{
			if (inUse)
			{
				GameObject::Render(cameraToWorld);

				// Definitely not the best play to draw this!
				char score_text[32];
				sprintf(score_text, "score: %d   lives: %d\n", score, lives);
				TextDrawer::GetInstance()->Draw(*shader, cameraToWorld, -1.75f, 2, 1.0f / 256, score_text);
			}
		}

		void OnCollisionWith(const GameObject* other) override
		{
			--lives;

			if (lives <= 0)
			{
				World::GetInstance()->SetState(WorldState::WORLD_STATE_OVER);
			}
		}

		void OnGameObjectHit(GameObject* other) override
		{
			Invaderer* invaderer = static_cast<Invaderer*> (other);
			if (invaderer->GetType().lives > 0)
			{
				score += 1;
			}
			else
			{
				score += invaderer->GetType().killScore;
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
				
				GameObject* missile = GameObjectFactory::GetInstance()->CreateMissile(this, shader);
				missile->SetRelativeTo(this, 0, 0.5f);
				World::GetInstance()->AddGameObject(missile);

				missiles_disabled = 5;

				SoundManager::GetInstance()->Play(SoundId::WHOOSH);
			}
		}

	};
}


#endif // !PLAYER_H

