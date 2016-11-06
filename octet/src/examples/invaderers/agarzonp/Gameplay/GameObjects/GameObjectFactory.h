#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H

#include "GameObjectDefs.h"

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

		GameObject* CreateInvaderer(octet::shaders::texture_shader* shader, const char* enemyType, float size, size_t row, size_t col)
		{
			// TO-DO: Create different types of invaderers according to enemyType

			static GLuint texture = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/invaderer.gif");

			static float oneSizeWidth = 0.5f;
			static float halfOneSizeWidth = oneSizeWidth * 0.5f;
			float width = size  * oneSizeWidth;
			float height = size * oneSizeWidth;
			float sizeOffset = float(size - 1) * halfOneSizeWidth;
			float x = (float)(col - GameObjectDefs::num_invaderer_cols * 0.5f) * 0.5f + sizeOffset;
			float y = 2.50f - ((float)row * 0.5f) - sizeOffset;

			GameObject* invaderer = pool->GetInvaderer();
			invaderer->GetSprite().init(texture, x, y, width, height);
			invaderer->AddShader(shader);

			return invaderer;
		}
	};

	GameObjectFactory* GameObjectFactory::s_instance = nullptr;
}

#endif
