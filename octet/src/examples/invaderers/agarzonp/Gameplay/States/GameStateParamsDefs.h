#ifndef GAME_STATE_PARAMS_DEFS_H
#define GAME_STATE_PARAMS_DEFS_H

namespace agarzonp
{
	struct BattleStateParams : GameStateParams
	{
		BattleStateParams() : level(0) {}
		~BattleStateParams() {}

		int level;
	};
}

#endif // !GAME_STATE_PARAMS_DEFS_H

