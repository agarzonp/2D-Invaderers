#ifndef GAME_OBJECT_POOL_INTERFACE_H
#define GAME_OBJECT_POOL_INTERFACE_H

#include "GameObjects/GameObject.h"

namespace agarzonp
{
	class GameObjectPoolInterface
	{

	public:
		virtual ~GameObjectPoolInterface() {}

		virtual GameObject* GetMissile() = 0;
		virtual GameObject* GetMissile(unsigned index) = 0;

		virtual GameObject* GetBomb() = 0;
		virtual GameObject* GetBomb(unsigned index) = 0;

		virtual GameObject* GetBorder() = 0;
		virtual GameObject* GetBorder(unsigned index) = 0;

		virtual GameObject* GetInvaderer() = 0;
		virtual GameObject* GetInvaderer(unsigned index) = 0;

		virtual GameObject* GetInvadererWave() = 0;
		virtual GameObject* GetInvadererWave(unsigned index) = 0;

		virtual GameObject* GetPlayer() = 0;
		virtual GameObject* GetPlayer(unsigned index) = 0;
	};
}

#endif
