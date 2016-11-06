#ifndef BATTLE_STATE_H
#define BATTLE_STATE_H

#include "../Sprites/SpritePool.h"
#include "../Sprites/SpritesDefs.h"

#include "../GameObjects/GameObject.h"
#include "../World.h"
#include "../GameObjectPool.h"

namespace agarzonp
{
	enum
	{
		num_levels = 2,
		win_time = 30
	};

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

		// level
		int currentLevel;

		// win timer
		int winTimer;

	public:
		BattleState(GameStateMachineInterface* gsmInterface) 
			: GameState(gsmInterface)
			, world(nullptr)
			, currentLevel(0)
			, winTimer(win_time)
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

			// win sprite
			GLuint winTexture = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/Win.gif");
			(*spritesPool)[win_sprite].init(winTexture, 0, 0, 3, 1.5f);
			(*spritesPool)[win_sprite].is_enabled() = false;

			// init world
			world = World::GetInstance();
			world->SetPool(&gameObjectPool);

			// load world
			BattleStateParams* battleParams = static_cast<BattleStateParams*>(params);
			LoadLevel(battleParams ? battleParams->level : 0);
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
					// enable game over sprite
					(*spritesPool)[game_over_sprite].is_enabled() = true;

					break;
				}
					
				case WorldState::WORLD_STATE_WIN:
				{
					// display win_sprite and load the new level when the timer expires
					if (winTimer)
					{
						winTimer--;
						(*spritesPool)[win_sprite].is_enabled() = true;
					}
					else
					{
						winTimer = win_time;

						(*spritesPool)[win_sprite].is_enabled() = false;

						// load next level
						LoadLevel(currentLevel + 1);
					}
					
					break;
				}

				case WorldState::WORLD_STATE_RUNNING:
				{
					if (Input::is_key_going_down(octet::key_esc))
					{
						//pause the game
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

			// render all game objects in the world
			World::GetInstance()->Render(cameraToWorld);

			// render any sprite
			for (int i = 0; i != SpriteDefs::num_sprites; ++i) 
			{
				(*spritesPool)[i].render(texture_shader_, cameraToWorld);
			}

			// move the listener with the camera
			octet::vec4 &cpos = cameraToWorld.w();
			octet::alListener3f(octet::AL_POSITION, cpos.x(), cpos.y(), cpos.z());
		};


	private:
			
		void LoadLevel(int level)
		{
			if (level < num_levels)
			{
				currentLevel = level;
				world->Load(currentLevel);
			}
			else
			{
				world->SetState(WorldState::WORLD_STATE_OVER);
			}
		}
	};
}

#endif



