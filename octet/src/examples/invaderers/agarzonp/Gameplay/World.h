#ifndef WORLD_H
#define WORLD_H

#include "GameObjects/GameObject.h"
#include "GameObjectPoolInterface.h"

namespace agarzonp
{
	class World
	{
		static World* s_instance;

		octet::containers::dynarray<GameObject*> gameObjects;

		GameObjectPoolInterface* poolInterface;

	protected:

		World() : poolInterface(nullptr)
		{
			assert(!s_instance);
			s_instance = this;
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

		void SetPool(GameObjectPoolInterface* pool)
		{
			poolInterface = pool;
		}

		GameObjectPoolInterface* GetPool()
		{
			return poolInterface;
		}

		void AddGameObject(GameObject* gameObject)
		{
			gameObjects.push_back(gameObject);
		}

		void Update()
		{
			for (GameObject* gameObject : gameObjects)
			{
   				gameObject->Update();
			}

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
			for (GameObject* gameObject : gameObjects)
			{
				gameObject->Render(cameraToWorld);
			}
		}
	};

	World* World::s_instance = nullptr;
}
#endif // ! WORLD_H

