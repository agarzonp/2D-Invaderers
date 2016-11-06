#ifndef WORLD_H
#define WORLD_H

#include "GameObjects/GameObject.h"
#include "GameObjectPoolInterface.h"
#include "GameObjects/GameObjectFactory.h"

namespace agarzonp
{
	enum class WorldState
	{
		WORLD_STATE_OVER,
		WORLD_STATE_WIN,
		WORLD_STATE_RUNNING,

		NUM_WORLD_STATES
	};

	class World
	{
		static World* s_instance;

		octet::containers::dynarray<GameObject*> gameObjects;

		GameObjectPoolInterface* poolInterface;

		WorldState worldState;

		// shader to draw a textured triangle
		octet::texture_shader texture_shader_;

		// game object factory
		GameObjectFactory* gameObjectFactory;

	protected:

		World() 
			: poolInterface(nullptr)
			, gameObjectFactory(GameObjectFactory::GetInstance())
			, worldState(WorldState::WORLD_STATE_RUNNING)
		{
			assert(!s_instance);
			s_instance = this;

			// set up the shader
			texture_shader_.init();
		}

	public:

		static World* GetInstance()
		{
			if (!s_instance)
			{
				return new World();
			}

			return s_instance;
		}

		~World()
		{
			delete gameObjectFactory;
			s_instance = nullptr;
		}

		void SetPool(GameObjectPoolInterface* pool)
		{
			poolInterface = pool;

			gameObjectFactory->SetPool(poolInterface);
		}

		GameObjectPoolInterface* GetPool()
		{
			return poolInterface;
		}

		void SetState(WorldState state)
		{
			worldState = state;
		}

		WorldState GetState()
		{
			return worldState;
		}

		void AddGameObject(GameObject* gameObject)
		{
			gameObjects.push_back(gameObject);
		}

		void Update()
		{
			// update game objects
			for (GameObject* gameObject : gameObjects)
			{
   				gameObject->Update();
			}

			// erase game objects that are not anymore in use
			for (auto itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
			{
				GameObject* gameObject = *itr;
				if (!gameObject->IsInUse())
				{
					itr = gameObjects.erase(itr);
					itr--;
				}
			}
		}

		void Render(const octet::mat4t& cameraToWorld)
		{
			// render game objects
			for (GameObject* gameObject : gameObjects)
			{
				gameObject->Render(cameraToWorld);
			}
		}

		void Load(int level)
		{
			Reset();

			// create player
			GameObject* player = GameObjectFactory::GetInstance()->CreatePlayer(&texture_shader_);
			AddGameObject(player);

			// create enemies
			LoadLevel(level);

			// create borders

			GameObject* border = GameObjectFactory::GetInstance()->CreateBorder(&texture_shader_);
			border->GetSprite().set(0.0f, -3.0f);
			AddGameObject(border);

			border = GameObjectFactory::GetInstance()->CreateBorder(&texture_shader_);
			border->GetSprite().set(0.0f, 3.0f);
			AddGameObject(border);

			border = GameObjectFactory::GetInstance()->CreateBorder(&texture_shader_);
			border->GetSprite().set(-3.0f, 0.0f, 0.2f, 6.0f);
			AddGameObject(border);

			border = GameObjectFactory::GetInstance()->CreateBorder(&texture_shader_);
			border->GetSprite().set(3.0f, 0.0f, 0.2f, 6.0f);
			AddGameObject(border);
		}

	private:

		void Reset()
		{
			SetState(WorldState::WORLD_STATE_RUNNING);

			// make all game objects available again
			for (GameObject* gameObject : gameObjects)
			{
				gameObject->SetIsInUse(false);
			}

			gameObjects.resize(0);
		}

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
					GameObject* wave = GameObjectFactory::GetInstance()->CreateInvadererWave(&texture_shader_);
					AddGameObject(wave);

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
							wave->AddChild(invaderer);
						}
					}

					return success ? LevelLoaderResult::LOADED : LevelLoaderResult::LOADED_BUT_ERRORS;
				}

				return LevelLoaderResult::NOT_LOADED_LEVEL_INVALID_LAYOUT;
			}

			return LevelLoaderResult::NOT_LOADED_LEVEL_NOT_FOUND;
		}
	};

	World* World::s_instance = nullptr;
}
#endif // ! WORLD_H

