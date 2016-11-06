#ifndef BOMB_H
#define BOMB_H

namespace agarzonp
{
	class Bomb : public GameObject
	{
	public:

		void Update() override
		{
			Move();
			CheckCollisions();
		}

		void OnCollisionWith(const GameObject* other)
		{
			GameObject::OnCollisionWith(other);
			Translate(20, 0);
		}

	private:

		void Move()
		{
			const float bomb_speed = 0.2f;
			Translate(0, -bomb_speed);
		}

		void CheckCollisions()
		{
			GameObject* player = World::GetInstance()->GetPool()->GetPlayer(0);
			if (CollidesWith(player))
			{
				player->OnCollisionWith(this);
				OnCollisionWith(player);

				// tell the triggerer that you hit the player
				triggerer->OnGameObjectHit(player);

				SoundManager::GetInstance()->Play(SoundId::BANG);

				return;
			}

			GameObject* border = World::GetInstance()->GetPool()->GetBorder(bottom_border_object);
			if (CollidesWith(border))
			{
				OnCollisionWith(border);
			}
		}
	};
}

#endif // !BOMB
