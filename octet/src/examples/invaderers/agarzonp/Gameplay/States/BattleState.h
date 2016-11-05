#ifndef BATTLE_STATE_H
#define BATTLE_STATE_H

#include "../Sprites/SpritePool.h"
#include "../Sprites/SpritesDefs.h"

#include "../GameObjects/GameObject.h"
#include "../World.h"
#include "../GameObjects/GameObjectFactory.h"
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

		// game object factory
		GameObjectFactory* gameObjectFactory;

		// World
		World* world;
		
		enum
		{
			num_sound_sources = 8,
		};

		// timers bombs
		int bombs_disabled;

		// accounting for bad guys
		int live_invaderers;
		int num_lives;

		// game state
		bool game_over;
		int score;

		// speed of enemy
		float invader_velocity;

		// sounds
		octet::ALuint whoosh;
		octet::ALuint bang;
		unsigned cur_source;
		octet::ALuint sources[num_sound_sources];

		// a texture for our text
		GLuint font_texture;

		// information for our text
		octet::bitmap_font font;

		octet::ALuint get_sound_source() { return sources[cur_source++ % num_sound_sources]; }

	public:
		BattleState(GameStateMachineInterface* gsmInterface) 
			: GameState(gsmInterface)
			, font(512, 256, "assets/big.fnt") 
			, world(nullptr)
		{
		}

		~BattleState() 
		{
			delete spritesPool;
			delete world;
			delete gameObjectFactory;
		}

		void Start(GameStateParams* params) override
		{
			GameState::Start(params);

			BattleStateParams* battleParams = static_cast<BattleStateParams*>(params);

			// init sprite pool
			spritesPool = SpritePool::GetInstance();
			spritesPool->Init(SpriteDefs::num_sprites);

			// init game object factory
			gameObjectFactory = GameObjectFactory::GetInstance();
			gameObjectFactory->SetPool(&gameObjectPool);

			// init world
			world = World::GetInstance();
			world->SetPool(&gameObjectPool);

			// set up the shader
			texture_shader_.init();

			// set up the matrices with a camera 5 units from the origin
			cameraToWorld.loadIdentity();
			cameraToWorld.translate(0, 0, 3);

			font_texture = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/big_0.gif");

			GLuint GameOver = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/GameOver.gif");
			(*spritesPool)[game_over_sprite].init(GameOver, 20, 0, 3, 1.5f);

			// create player
			GameObject* player = GameObjectFactory::GetInstance()->CreatePlayer(&texture_shader_);
			World::GetInstance()->AddGameObject(player);

			// create enemies
			LoadLevel(battleParams ? battleParams->level : 0);
			
			// create borders

			GameObject* border = GameObjectFactory::GetInstance()->CreateBorder(&texture_shader_);
			border->GetSprite().set(0.0f, -3.0f);
			World::GetInstance()->AddGameObject(border);

			border = GameObjectFactory::GetInstance()->CreateBorder(&texture_shader_);
			border->GetSprite().set(0.0f, 3.0f);
			World::GetInstance()->AddGameObject(border);

			border = GameObjectFactory::GetInstance()->CreateBorder(&texture_shader_);
			border->GetSprite().set(-3.0f, 0.0f, 0.2f, 6.0f);
			World::GetInstance()->AddGameObject(border);

			border = GameObjectFactory::GetInstance()->CreateBorder(&texture_shader_);
			border->GetSprite().set(3.0f, 0.0f, 0.2f, 6.0f);
			World::GetInstance()->AddGameObject(border);

			// sounds
			whoosh = octet::resource_dict::get_sound_handle(octet::AL_FORMAT_MONO16, "assets/invaderers/whoosh.wav");
			bang = octet::resource_dict::get_sound_handle(octet::AL_FORMAT_MONO16, "assets/invaderers/bang.wav");
			cur_source = 0;
			octet::alGenSources(num_sound_sources, sources);

			// sundry counters and game state.
			bombs_disabled = 50;
			invader_velocity = 0.01f;
			live_invaderers = num_invaderers;
			num_lives = 3;
			game_over = false;
			score = 0;
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

			if (game_over)
			{
				return;
			}

			if (Input::is_key_going_down(octet::key_esc))
			{
				gameStateMachineInterface->PushState(GameStateId::PAUSE);
				return;
			}

			World::GetInstance()->Update();
		}

		void Render() override 
		{
			GameState::Render();

			World::GetInstance()->Render(cameraToWorld);

			char score_text[32];
			sprintf(score_text, "score: %d   lives: %d\n", score, num_lives);
			draw_text(texture_shader_, -1.75f, 2, 1.0f / 256, score_text);

			// move the listener with the camera
			octet::vec4 &cpos = cameraToWorld.w();
			octet::alListener3f(octet::AL_POSITION, cpos.x(), cpos.y(), cpos.z());
		};

		
private:
			
			enum class LevelLoaderResult : uint8_t
			{
				LOADED,
				LOADED_BUT_ERRORS,
				NOT_LOADED_LEVEL_NOT_FOUND,
				NOT_LOADED_LEVEL_INVALID_LAYOUT
			};

			void LoadLevel(int level)
			{
				octet::string levelPath;
				levelPath.printf("Level_%d.csv", level);

				LevelLoaderResult result = LoadLevel(levelPath.c_str());
				switch (result)
				{
				case LevelLoaderResult::LOADED:
					break;
				case LevelLoaderResult::LOADED_BUT_ERRORS:
					printf("Level %s was loaded with some errors. Check output.", levelPath.c_str());
					break;
				case LevelLoaderResult::NOT_LOADED_LEVEL_NOT_FOUND:
					printf("Level %s couldn´t be loaded. Not such file or directory.", levelPath.c_str());
					break;
				case LevelLoaderResult::NOT_LOADED_LEVEL_INVALID_LAYOUT:
					printf("Level %s couldn´t be loaded. Level layout is not setup properly.", levelPath.c_str());
					break;
				}
			}

			LevelLoaderResult LoadLevel(const char* levelPath)
			{
				CSVParser csvParser(levelPath);

				if (csvParser.IsValid())
				{
					size_t numRows = csvParser.NumRows();
					size_t numCols = csvParser.NumCols();

					int maxNumEnemies = int(numRows * numCols);
					if (maxNumEnemies > GameObjectDefs::num_invaderers)
					{
						// FIXME: Increase the invaderers pool
						assert(false);	
					}

					if (numRows > 0 && numCols > 0)
					{
						InvadererWave* wave = new InvadererWave(); // This is never freed!
						wave->SetIsInUse(true);
						wave->AddShader(&texture_shader_);
						World::GetInstance()->AddGameObject(wave);

						bool success = true;

						for (size_t row = 0; row < numRows; row++)
						{
							for (size_t col = 0; col < numCols; col++)
							{
								// Parse the enemy "[enemyType]_[enemySize]". For example, Invader_2 
								const char* enemyStart = csvParser[row][col];
								const char* p = enemyStart;

								if (*p == '\0')
								{
									// Empty means that the enemy occupies more than one column, so skip to the next column
									continue;
								}

								// Increment until we get the underscore
								while (*p++ != '_')
								{
								}

								if (*p == '\0')
								{
									printf("Error while parsing enemy %s. Level %s row %d col %d", enemyStart, levelPath, int(row), int(col));
									success = false;
									continue;
								}
								p--;

								// get the enemy
								octet::string enemyType(enemyStart, int(p - enemyStart));

								// get the size of the enemy
								p++;
								int enemySize = atoi(p);

								GameObject* invaderer = GameObjectFactory::GetInstance()->CreateInvaderer(&texture_shader_, enemyType.c_str(), float(enemySize), row, col);
								wave->AddInvaderer(static_cast<Invaderer*>(invaderer));
							}
						}
							
						return success ? LevelLoaderResult::LOADED : LevelLoaderResult::LOADED_BUT_ERRORS;
					}

					return LevelLoaderResult::NOT_LOADED_LEVEL_INVALID_LAYOUT;
				}

				return LevelLoaderResult::NOT_LOADED_LEVEL_NOT_FOUND;
			}

			void draw_text(octet::texture_shader &shader, float x, float y, float scale, const char *text) {
				octet::mat4t modelToWorld;
				modelToWorld.loadIdentity();
				modelToWorld.translate(x, y, 0);
				modelToWorld.scale(scale, scale, 1);
				octet::mat4t modelToProjection = octet::mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

				/*mat4t tmp;
				glLoadIdentity();
				glTranslatef(x, y, 0);
				glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&tmp);
				glScalef(scale, scale, 1);
				glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&tmp);*/

				enum { max_quads = 32 };
				octet::bitmap_font::vertex vertices[max_quads * 4];
				uint32_t indices[max_quads * 6];
				octet::aabb bb(octet::vec3(0, 0, 0), octet::vec3(256, 256, 0));

				unsigned num_quads = font.build_mesh(bb, vertices, indices, max_quads, text, 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, font_texture);

				shader.render(modelToProjection, 0);

				glVertexAttribPointer(octet::attribute_pos, 3, GL_FLOAT, GL_FALSE, sizeof(octet::bitmap_font::vertex), (void*)&vertices[0].x);
				glEnableVertexAttribArray(octet::attribute_pos);
				glVertexAttribPointer(octet::attribute_uv, 3, GL_FLOAT, GL_FALSE, sizeof(octet::bitmap_font::vertex), (void*)&vertices[0].u);
				glEnableVertexAttribArray(octet::attribute_uv);

				glDrawElements(GL_TRIANGLES, num_quads * 6, GL_UNSIGNED_INT, indices);
			}
	};
}

#endif



