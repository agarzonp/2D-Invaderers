#ifndef GAME_OBJECT_DEFS
#define GAME_OBJECT_DEFS

namespace agarzonp
{
	enum GameObjectDefs
	{
		num_invaderer_rows = 6,
		num_invaderer_cols = 10,
		num_missiles = 20,
		num_bombs = 20,
		num_invaderers = num_invaderer_rows * num_invaderer_cols,

		// game object definitions
		bottom_border_object = 0,
		top_border_object,
		left_border_object,
		right_border_object,

		num_borders,

		player_1_object = 0,

		num_players
	};
}

#endif // !GAME_OBJECT_DEFS

