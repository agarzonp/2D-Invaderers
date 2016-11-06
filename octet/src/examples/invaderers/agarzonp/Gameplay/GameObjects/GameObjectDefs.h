#ifndef GAME_OBJECT_DEFS
#define GAME_OBJECT_DEFS

namespace agarzonp
{
	enum GameObjectDefs
	{
		// invaderers
		num_invaderer_rows = 6,
		num_invaderer_cols = 10,
		
		num_invaderers = num_invaderer_rows * num_invaderer_cols,

		num_invaderer_waves = 1,

		// missiles and bombs
		num_missiles = 20,
		num_bombs = 20,

		// borders
		bottom_border_object = 0,
		top_border_object,
		left_border_object,
		right_border_object,

		num_borders,

		// players
		player_1_object = 0,

		num_players
	};
}

#endif // !GAME_OBJECT_DEFS

