#ifndef BATTLE_STATE_H
#define BATTLE_STATE_H

#include "../Sprites/SpritePool.h"
#include "../Sprites/SpritesDefs.h"

#include "../GameObjects/GameObject.h"
#include "../World.h"
#include "../GameObjectPool.h"

namespace agarzonp
{
	class BattleState : public GameState
	{
		// Matrix to transform points in our camera space to the world.
		// This lets us move our camera
		octet::mat4t cameraToWorld;

		// shader to draw a textured triangle
		octet::texture_shader texture_shader_;

		// sprite pool
		SpritePool* spritesPool;

		// game object pool
		GameObjectPool gameObjectPool;

		// World
		World* world;

	public:
		BattleState(GameStateMachineInterface* gsmInterface) 
			: GameState(gsmInterface)
			, world(nullptr)
		{
		}

		~BattleState() 
		{
			delete spritesPool;
			delete world;
		}

		void Start(GameStateParams* params) override
		{
			GameState::Start(params);

			// set up the matrices with a camera 5 units from the origin
			cameraToWorld.loadIdentity();
			cameraToWorld.translate(0, 0, 3);

			// set up the shader
			texture_shader_.init();

			// init sprite pool
			spritesPool = SpritePool::GetInstance();
			spritesPool->Init(SpriteDefs::num_sprites);

			// game over sprite
			GLuint gameOverTexture = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/GameOver.gif");
			(*spritesPool)[game_over_sprite].init(gameOverTexture, 0, 0, 3, 1.5f);
			(*spritesPool)[game_over_sprite].is_enabled() = false;

			// init world
			world = World::GetInstance();
			world->SetPool(&gameObjectPool);

			// load world
			BattleStateParams* battleParams = static_cast<BattleStateParams*>(params);
			world->Load(battleParams ? battleParams->level : 0);
		}

		void Stop()	override 
		{
			GameState::Stop();
		}

		void Suspend() override 
		{
			GameState::Stop();
		}

		void Resume() override 
		{
			GameState::Resume();
		}

		void Update() override 
		{
			GameState::Update();

			switch (world->GetState())
			{
				case WorldState::WORLD_STATE_OVER:
				{
					if (!(*spritesPool)[game_over_sprite].is_enabled())
					{
						(*spritesPool)[game_over_sprite].is_enabled() = true;
					}
					break;
				}
					
				case WorldState::WORLD_STATE_WIN:
					// load next level
					break;
				case WorldState::WORLD_STATE_RUNNING:
				{
					if (Input::is_key_going_down(octet::key_esc))
					{
						gameStateMachineInterface->PushState(GameStateId::PAUSE);
						return;
					}

					World::GetInstance()->Update();
					break;
				}
			}			
		}

		void Render() override 
		{
			GameState::Render();

			World::GetInstance()->Render(cameraToWorld);

			for (int i = 0; i != SpriteDefs::num_sprites; ++i) 
			{
				(*spritesPool)[i].render(texture_shader_, cameraToWorld);
			}

			// move the listener with the camera
			octet::vec4 &cpos = cameraToWorld.w();
			octet::alListener3f(octet::AL_POSITION, cpos.x(), cpos.y(), cpos.z());
		};

		
private:
			
			
	};
}

#endif



