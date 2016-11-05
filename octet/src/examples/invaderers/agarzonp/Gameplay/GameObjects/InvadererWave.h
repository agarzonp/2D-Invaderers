#ifndef INVADERER_WAVE_H
#define INVADERER_WAVE_H

namespace agarzonp
{
	class InvadererWave : public GameObject
	{
		int bombs_disabled;
		octet::vec2 velocity;
		octet::containers::dynarray<Invaderer*> invaderers;

		class octet::random randomizer;

	public:

		InvadererWave()
			: bombs_disabled(50)
			, velocity(0.01f, 0.0f)
		{
		}

		~InvadererWave()
		{
			for (Invaderer* invaderer : invaderers)
			{
				invaderer->SetIsInUse(false);
			}
		}

		void AddInvaderer(Invaderer* invaderer)
		{
			invaderers.push_back(invaderer);
		}


		void Update() override
		{
			for (Invaderer* invaderer : invaderers)
			{
				invaderer->Update();
			}

			Move();
			Fire();

			
		}

		void Render(const octet::math::mat4t& cameraToWorld) override
		{
			for (Invaderer* invaderer : invaderers)
			{
				invaderer->Render(cameraToWorld);
			}
		}

	private:

		void Move()
		{
			int borderIndex = velocity.x() < 0 ? GameObjectDefs::left_border_object : GameObjectDefs::right_border_object;
			GameObject* border = World::GetInstance()->GetPool()->GetBorder(borderIndex);

			for (Invaderer* invaderer : invaderers)
			{
				if (invaderer->CollidesWith(border))
				{
					velocity.x() = -velocity.x();
					velocity.y() = -0.1f;
					break;
				}
			}

			for (Invaderer* invaderer : invaderers)
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
						GameObject* bomb = GameObjectFactory::GetInstance()->CreateBomb(shader);
						bomb->SetRelativeTo(invaderer, 0.0f, -0.25f);
						World::GetInstance()->AddGameObject(bomb);

						bombs_disabled = 30;

						//octet::ALuint source = get_sound_source();
						//octet::alSourcei(source, octet::AL_BUFFER, whoosh);
						//octet::alSourcePlay(source);

						return;
					}
				}
			}
		}
	};

}

#endif // !INVADERER_WAVE_H
