#ifndef INVADERER_WAVE_H
#define INVADERER_WAVE_H

namespace agarzonp
{
	class InvadererWave : public GameObject
	{
		int bombs_disabled;
		octet::vec2 velocity;
		octet::containers::dynarray<GameObject*> invaderers;

		class octet::random randomizer;

	public:

		InvadererWave()
		{
			Reset();
		}

		~InvadererWave()
		{
			SetIsInUse(false);
			Reset();
		}

		void Reset() override
		{
			bombs_disabled = 50,
			velocity = octet::vec2(0.01f, 0.0f);

			for (GameObject* invaderer : invaderers)
			{
				invaderer->SetIsInUse(false);
			}
			invaderers.resize(0);
		}

		void OnGameObjectHit(GameObject* gameObject)
		{
			bombs_disabled = 50;
		}

		void AddChild(GameObject* child) override
		{
			invaderers.push_back(child);
		}


		void Update() override
		{
			int aliveInvaderes = 0;

			for (GameObject* invaderer : invaderers)
			{
				invaderer->IsInUse() ? aliveInvaderes += 1 : 0;

				invaderer->Update();
			}

			Move();
			Fire();

			UpdateVelocity(aliveInvaderes);

			if (aliveInvaderes == 0)
			{
				World::GetInstance()->SetState(WorldState::WORLD_STATE_WIN);
			}
		}

		void Render(const octet::math::mat4t& cameraToWorld) override
		{
			for (GameObject* invaderer : invaderers)
			{
				invaderer->Render(cameraToWorld);
			}
		}

	private:

		void UpdateVelocity(int aliveInvaderes)
		{
			// FIXME: velocity bug
			//if (aliveInvaderes <= 16)
			//{
			//	velocity.x() *= 4;
			//}
		}

		void Move()
		{
			int borderIndex = velocity.x() < 0 ? GameObjectDefs::left_border_object : GameObjectDefs::right_border_object;
			GameObject* border = World::GetInstance()->GetPool()->GetBorder(borderIndex);

			for (GameObject* invaderer : invaderers)
			{
				if (invaderer->CollidesWith(border))
				{
					velocity.x() = -velocity.x();
					velocity.y() = -0.1f;
					break;
				}
			}

			for (GameObject* invaderer : invaderers)
			{
				invaderer->Translate(velocity.x(), velocity.y());
			}

			velocity.y() = 0;
		}

		void Fire()
		{
			if (bombs_disabled)
			{
				--bombs_disabled;
			}
			else
			{
				// find an invaderer
				GameObject* player = World::GetInstance()->GetPool()->GetPlayer(GameObjectDefs::player_1_object);
				for (unsigned j = randomizer.get(0, invaderers.size()); j < invaderers.size(); ++j)
				{
					GameObject* invaderer = invaderers[j];
					if (invaderer->IsInUse() && invaderer->IsAbove(player, 0.3f))
					{
						GameObject* bomb = GameObjectFactory::GetInstance()->CreateBomb(this, shader);
						bomb->SetRelativeTo(invaderer, 0.0f, -0.25f);
						World::GetInstance()->AddGameObject(bomb);

						bombs_disabled = 30;

						SoundManager::GetInstance()->Play(SoundId::WHOOSH);

						return;
					}
				}
			}
		}
	};

}

#endif // !INVADERER_WAVE_H
