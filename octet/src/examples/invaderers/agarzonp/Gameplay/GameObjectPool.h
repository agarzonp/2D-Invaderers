#ifndef	GAME_OBJECT_POOL_H
#define GAME_OBJECT_POOL_H

#include "GameObjects/GameObjectDefs.h"
#include "GameObjectPoolInterface.h"
#include "GameObjects.h"

namespace agarzonp
{
	class GameObjectPool : public GameObjectPoolInterface
	{
		// The pools
		octet::containers::dynarray<Missile> missiles;
		octet::containers::dynarray<Bomb> bombs;
		octet::containers::dynarray<Border> borders;
		octet::containers::dynarray<Invaderer> invaderers;
		octet::containers::dynarray<InvadererWave> invadererWaves;
		octet::containers::dynarray<Player> players;

	public:

		GameObjectPool()
		{
			missiles.resize(GameObjectDefs::num_missiles);
			bombs.resize(GameObjectDefs::num_bombs);
			borders.resize(GameObjectDefs::num_borders);
			invadererWaves.resize(GameObjectDefs::num_invaderer_waves);
			invaderers.resize(GameObjectDefs::num_invaderers);
			players.resize(GameObjectDefs::num_players);
		}

		GameObject* GetMissile() final
		{
			return  GetFreeObject(missiles);
		}

		GameObject* GetMissile(unsigned index) final
		{
			return GetGameObject(index, missiles);
		}

		GameObject* GetBomb() final
		{
			return GetFreeObject(bombs);
		}

		GameObject* GetBomb(unsigned index) final
		{
			return GetGameObject(index, bombs);
		}

		GameObject* GetBorder() final
		{
			return GetFreeObject(borders);
		}

		GameObject* GetBorder(unsigned index) final
		{
			return GetGameObject(index, borders);
		}

		GameObject* GetInvaderer() final
		{
			return GetFreeObject(invaderers);
		}

		GameObject* GetInvaderer(unsigned index) final
		{
			return GetGameObject(index, invaderers);
		}

		GameObject* GetInvadererWave() final
		{
			return GetFreeObject(invadererWaves);
		}

		GameObject* GetInvadererWave(unsigned index) final
		{
			return GetGameObject(index, invadererWaves);
		}
		
		GameObject* GetPlayer() final
		{
			return GetFreeObject(players);
		}

		GameObject* GetPlayer(unsigned index) final
		{
			return GetGameObject(index, players);
		}
		

	private:
		
		template <class TGameObject>
		GameObject* GetFreeObject(octet::dynarray<TGameObject>& dynarray)
		{
			GameObject* freeObject = nullptr;
			for (TGameObject& object : dynarray)
			{
				if (!object.IsInUse())
				{
					freeObject = &object;
					break;
				}
			}

			assert(freeObject); // increase the pool!

			if (freeObject)
			{
				freeObject->SetIsInUse(true);
			}

			return freeObject;
		}

		template<class T>
		GameObject* GetGameObject(unsigned index, octet::dynarray<T>& dynarray)
		{
			GameObject* object = nullptr;

			assert(index < dynarray.size());

			if (index < dynarray.size())
			{
				object = &dynarray[index];
			}

			return object;
		}
	};
}

#endif // !GAME_OBJECT_POOL_H

