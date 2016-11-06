#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H

#include "GameObjectDefs.h"

#include "Invaderer.h"

namespace agarzonp
{
	class GameObjectFactory
	{
		static GameObjectFactory* s_instance;

	protected:

		// Interface to get game objects from the pool
		GameObjectPoolInterface* pool;

		GameObjectFactory() 
			: pool(nullptr) 
		{
			assert(!s_instance);
			s_instance = this;
		}

	public:

		~GameObjectFactory()
		{
			s_instance = nullptr;
		}

		static GameObjectFactory* GetInstance()
		{
			if (!s_instance)
			{
				return new GameObjectFactory();
			}

			assert(s_instance->pool);
			return s_instance;
		}

		void SetPool(GameObjectPoolInterface* poolInterface)
		{
			pool = poolInterface;
		}

		GameObject* CreateBorder(octet::shaders::texture_shader* shader)
		{
			static GLuint texture = octet::resource_dict::get_texture_handle(GL_RGB, "#ffffff");

			GameObject* border = pool->GetBorder();
			border->GetSprite().init(texture, 0.0f, -3.0f, 6, 0.2f);
			border->AddShader(shader);

			return border;
		}

		GameObject* CreateMissile(GameObject* triggerer, octet::shaders::texture_shader* shader)
		{
			static GLuint texture = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/missile.gif");

			GameObject* missile = pool->GetMissile();
			missile->SetTriggerer(triggerer);
			missile->GetSprite().init(texture, 20, 0, 0.0625f, 0.25f);
			missile->AddShader(shader);

			return missile;
		}

		GameObject* CreateBomb(GameObject* triggerer, octet::shaders::texture_shader* shader)
		{
			static GLuint texture = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/bomb.gif");

			GameObject* bomb = pool->GetBomb();
			bomb->SetTriggerer(triggerer);
			bomb->GetSprite().init(texture, 20, 0, 0.0625f, 0.25f);
			bomb->AddShader(shader);

			return bomb;
		}

		GameObject* CreatePlayer(octet::shaders::texture_shader* shader)
		{
			static GLuint texture = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/ship.gif");

			GameObject* player = pool->GetPlayer();
			player->GetSprite().init(texture, 0, -2.75f, 0.25f, 0.25f);
			player->AddShader(shader);

			return player;
		}

		GameObject* CreateInvadererWave(octet::shaders::texture_shader* shader)
		{
			GameObject* wave = pool->GetInvadererWave();
			wave->Reset();
			wave->AddShader(shader);

			return wave;
		}

		GameObject* CreateInvaderer(octet::shaders::texture_shader* shader, const char* invadererType, float size, size_t row, size_t col)
		{
			// get invaderer type
			InvadererType type = GetInvadererType(invadererType, (int)size);

			// calculate invaderer position, width anf height
			float oneSizeWidth = 0.5f;
			float halfOneSizeWidth = oneSizeWidth * 0.5f;
			float width = size  * oneSizeWidth;
			float height = size * oneSizeWidth;
			float sizeOffset = float(size - 1) * halfOneSizeWidth;
			float x = (float)(col - GameObjectDefs::num_invaderer_cols * 0.5f) * 0.5f + sizeOffset;
			float y = 2.50f - ((float)row * 0.5f) - sizeOffset;

			// create the invaderer
			Invaderer* invaderer = static_cast<Invaderer*> (pool->GetInvaderer());
			invaderer->SetType(type);
			invaderer->GetSprite().init(type.texture, x, y, width, height);
			invaderer->AddShader(shader);

			return invaderer;
		}

		private:

			InvadererType GetInvadererType(const char* enemyType, int size)
			{
				InvadererType type;

				if (strcmp(enemyType, "Invader") == 0)
				{
					type.texture = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/invaderer.gif");
					type.killScore = size;
					type.lives = size * size;
				}
				else if (strcmp(enemyType, "InvaderHard") == 0)
				{
					type.texture = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/invadererHard.gif");
					type.killScore = 1 * size + 10;
					type.lives = size * size  * 2;
				}
				else
				{
					type.texture = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/invaderer.gif");
					type.killScore = size;
					type.lives = size * size;
				}

				return type;
			}
	};

	GameObjectFactory* GameObjectFactory::s_instance = nullptr;
}

#endif
