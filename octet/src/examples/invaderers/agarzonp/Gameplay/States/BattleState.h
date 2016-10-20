#ifndef BATTLE_STATE_H
#define BATTLE_STATE_H

namespace agarzonp
{
	class BattleState : public GameState
	{
		// Matrix to transform points in our camera space to the world.
		// This lets us move our camera
		octet::mat4t cameraToWorld;

		// shader to draw a textured triangle
		octet::texture_shader texture_shader_;

		enum {
			num_sound_sources = 8,
			num_rows = 5,
			num_cols = 10,
			num_missiles = 2,
			num_bombs = 2,
			num_borders = 4,
			num_invaderers = num_rows * num_cols,

			// sprite definitions
			first_border_sprite = 0,
			last_border_sprite = first_border_sprite + num_borders - 1,
			
			game_over_sprite,

			ship_sprite,
			
			first_missile_sprite,
			last_missile_sprite = first_missile_sprite + num_missiles - 1,

			first_bomb_sprite,
			last_bomb_sprite = first_bomb_sprite + num_bombs - 1,

			// FIXME: invaderers num sprites will be dynamic according to the level layout
			first_invaderer_sprite,
			last_invaderer_sprite = first_invaderer_sprite + num_invaderers - 1,

			num_sprites,

		};

		// timers for missiles and bombs
		int missiles_disabled;
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

		// big array of sprites
		octet::sprite sprites[num_sprites];

		// random number generator
		class octet::random randomizer;

		// a texture for our text
		GLuint font_texture;

		// information for our text
		octet::bitmap_font font;

		octet::ALuint get_sound_source() { return sources[cur_source++ % num_sound_sources]; }

	public:
		BattleState(GameStateMachineInterface* gsmInterface) 
			: GameState(gsmInterface)
			, font(512, 256, "assets/big.fnt") 
		{
		}

		~BattleState() {}

		void Start() override 
		{
			// set up the shader
			texture_shader_.init();

			// set up the matrices with a camera 5 units from the origin
			cameraToWorld.loadIdentity();
			cameraToWorld.translate(0, 0, 3);

			font_texture = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/big_0.gif");

			GLuint ship = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/ship.gif");
			sprites[ship_sprite].init(ship, 0, -2.75f, 0.25f, 0.25f);

			GLuint GameOver = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/GameOver.gif");
			sprites[game_over_sprite].init(GameOver, 20, 0, 3, 1.5f);

			LoadLevel(0);
			

			// set the border to white for clarity
			GLuint white = octet::resource_dict::get_texture_handle(GL_RGB, "#ffffff");
			sprites[first_border_sprite + 0].init(white, 0, -3, 6, 0.2f);
			sprites[first_border_sprite + 1].init(white, 0, 3, 6, 0.2f);
			sprites[first_border_sprite + 2].init(white, -3, 0, 0.2f, 6);
			sprites[first_border_sprite + 3].init(white, 3, 0, 0.2f, 6);

			// use the missile texture
			GLuint missile = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/missile.gif");
			for (int i = 0; i != num_missiles; ++i) {
				// create missiles off-screen
				sprites[first_missile_sprite + i].init(missile, 20, 0, 0.0625f, 0.25f);
				sprites[first_missile_sprite + i].is_enabled() = false;
			}

			// use the bomb texture
			GLuint bomb = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/bomb.gif");
			for (int i = 0; i != num_bombs; ++i) {
				// create bombs off-screen
				sprites[first_bomb_sprite + i].init(bomb, 20, 0, 0.0625f, 0.25f);
				sprites[first_bomb_sprite + i].is_enabled() = false;
			}

			// sounds
			whoosh = octet::resource_dict::get_sound_handle(octet::AL_FORMAT_MONO16, "assets/invaderers/whoosh.wav");
			bang = octet::resource_dict::get_sound_handle(octet::AL_FORMAT_MONO16, "assets/invaderers/bang.wav");
			cur_source = 0;
			octet::alGenSources(num_sound_sources, sources);

			// sundry counters and game state.
			missiles_disabled = 0;
			bombs_disabled = 50;
			invader_velocity = 0.01f;
			live_invaderers = num_invaderers;
			num_lives = 3;
			game_over = false;
			score = 0;
		}

		void Stop()	override {};
		void Suspend() override {};
		void Resume() override {};

		void Update() override 
		{
			if (game_over) {
				return;
			}

			if (Input::is_key_going_down(octet::key_esc))
			{
				gameStateMachineInterface->PushState(GameStateId::PAUSE);
				return;
			}

			move_ship();

			fire_missiles();

			fire_bombs();

			move_missiles();

			move_bombs();

			move_invaders(invader_velocity, 0);

			octet::sprite &border = sprites[first_border_sprite + (invader_velocity < 0 ? 2 : 3)];
			if (invaders_collide(border)) {
				invader_velocity = -invader_velocity;
				move_invaders(invader_velocity, -0.1f);
			}
		}
		void Render() override 
		{
			// draw all the sprites
			for (int i = 0; i != num_sprites; ++i) {
				sprites[i].render(texture_shader_, cameraToWorld);
			}

			char score_text[32];
			sprintf(score_text, "score: %d   lives: %d\n", score, num_lives);
			draw_text(texture_shader_, -1.75f, 2, 1.0f / 256, score_text);

			// move the listener with the camera
			octet::vec4 &cpos = cameraToWorld.w();
			octet::alListener3f(octet::AL_POSITION, cpos.x(), cpos.y(), cpos.z());
		};

		
private:
			// called when we hit an enemy
			void on_hit_invaderer() {
				octet::ALuint source = get_sound_source();
				octet::alSourcei(source, octet::AL_BUFFER, bang);
				octet::alSourcePlay(source);

				live_invaderers--;
				score++;
				if (live_invaderers == 4) {
					invader_velocity *= 4;
				}
				else if (live_invaderers == 0) {
					game_over = true;
					sprites[game_over_sprite].translate(-20, 0);
				}
			}

			// called when we are hit
			void on_hit_ship() {
				octet::ALuint source = get_sound_source();
				octet::alSourcei(source, octet::AL_BUFFER, bang);
				octet::alSourcePlay(source);

				if (--num_lives == 0) {
					game_over = true;
					sprites[game_over_sprite].translate(-20, 0);
				}
			}

			// use the keyboard to move the ship
			void move_ship() {
				const float ship_speed = 0.05f;
				// left and right arrows
				if (Input::is_key_down(octet::key_left)) {
					sprites[ship_sprite].translate(-ship_speed, 0);
					if (sprites[ship_sprite].collides_with(sprites[first_border_sprite + 2])) {
						sprites[ship_sprite].translate(+ship_speed, 0);
					}
				}
				else if (Input::is_key_down(octet::key_right)) {
					sprites[ship_sprite].translate(+ship_speed, 0);
					if (sprites[ship_sprite].collides_with(sprites[first_border_sprite + 3])) {
						sprites[ship_sprite].translate(-ship_speed, 0);
					}
				}
			}

			// fire button (space)
			void fire_missiles() {
				if (missiles_disabled) {
					--missiles_disabled;
				}
				else if (Input::is_key_going_down(' ')) {
					// find a missile
					for (int i = 0; i != num_missiles; ++i) {
						if (!sprites[first_missile_sprite + i].is_enabled()) {
							sprites[first_missile_sprite + i].set_relative(sprites[ship_sprite], 0, 0.5f);
							sprites[first_missile_sprite + i].is_enabled() = true;
							missiles_disabled = 5;
							octet::ALuint source = get_sound_source();
							octet::alSourcei(source, octet::AL_BUFFER, whoosh);
							octet::alSourcePlay(source);
							break;
						}
					}
				}
			}

			// pick and invader and fire a bomb
			void fire_bombs() {
				if (bombs_disabled) {
					--bombs_disabled;
				}
				else {
					// find an invaderer
					octet::sprite &ship = sprites[ship_sprite];
					for (int j = randomizer.get(0, num_invaderers); j < num_invaderers; ++j) {
						octet::sprite &invaderer = sprites[first_invaderer_sprite + j];
						if (invaderer.is_enabled() && invaderer.is_above(ship, 0.3f)) {
							// find a bomb
							for (int i = 0; i != num_bombs; ++i) {
								if (!sprites[first_bomb_sprite + i].is_enabled()) {
									sprites[first_bomb_sprite + i].set_relative(invaderer, 0, -0.25f);
									sprites[first_bomb_sprite + i].is_enabled() = true;
									bombs_disabled = 30;
									octet::ALuint source = get_sound_source();
									octet::alSourcei(source, octet::AL_BUFFER, whoosh);
									octet::alSourcePlay(source);
									return;
								}
							}
							return;
						}
					}
				}
			}

			// animate the missiles
			void move_missiles() {
				const float missile_speed = 0.3f;
				for (int i = 0; i != num_missiles; ++i) {
					octet::sprite &missile = sprites[first_missile_sprite + i];
					if (missile.is_enabled()) {
						missile.translate(0, missile_speed);
						for (int j = 0; j != num_invaderers; ++j) {
							octet::sprite &invaderer = sprites[first_invaderer_sprite + j];
							if (invaderer.is_enabled() && missile.collides_with(invaderer)) {
								invaderer.is_enabled() = false;
								invaderer.translate(20, 0);
								missile.is_enabled() = false;
								missile.translate(20, 0);
								on_hit_invaderer();

								goto next_missile;
							}
						}
						if (missile.collides_with(sprites[first_border_sprite + 1])) {
							missile.is_enabled() = false;
							missile.translate(20, 0);
						}
					}
				next_missile:;
				}
			}

			// animate the bombs
			void move_bombs() {
				const float bomb_speed = 0.2f;
				for (int i = 0; i != num_bombs; ++i) {
					octet::sprite &bomb = sprites[first_bomb_sprite + i];
					if (bomb.is_enabled()) {
						bomb.translate(0, -bomb_speed);
						if (bomb.collides_with(sprites[ship_sprite])) {
							bomb.is_enabled() = false;
							bomb.translate(20, 0);
							bombs_disabled = 50;
							on_hit_ship();
							goto next_bomb;
						}
						if (bomb.collides_with(sprites[first_border_sprite + 0])) {
							bomb.is_enabled() = false;
							bomb.translate(20, 0);
						}
					}
				next_bomb:;
				}
			}

			// move the array of enemies
			void move_invaders(float dx, float dy) {
				for (int j = 0; j != num_invaderers; ++j) {
					octet::sprite &invaderer = sprites[first_invaderer_sprite + j];
					if (invaderer.is_enabled()) {
						invaderer.translate(dx, dy);
					}
				}
			}

			// check if any invaders hit the sides.
			bool invaders_collide(octet::sprite &border) {
				for (int j = 0; j != num_invaderers; ++j) {
					octet::sprite &invaderer = sprites[first_invaderer_sprite + j];
					if (invaderer.is_enabled() && invaderer.collides_with(border)) {
						return true;
					}
				}
				return false;
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
					if (maxNumEnemies > num_sprites)
					{
						assert(false);
						// FIXME: The sprite pool needs to be resized!
					}

					if (numRows > 0 && numCols > 0)
					{
						bool success = true;

						unsigned spriteIndex = first_invaderer_sprite;

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
								CreateEnemy(spriteIndex, enemyType.c_str(), float(enemySize), row, col);

								spriteIndex++;
							}
						}
							
						return success ? LevelLoaderResult::LOADED : LevelLoaderResult::LOADED_BUT_ERRORS;
					}

					return LevelLoaderResult::NOT_LOADED_LEVEL_INVALID_LAYOUT;
				}

				return LevelLoaderResult::NOT_LOADED_LEVEL_NOT_FOUND;
			}

			void CreateEnemy(unsigned spriteIndex, const char* enemyType, float size, size_t row, size_t col)
			{
				static GLuint invaderer = octet::resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/invaderer.gif");
			
				static float oneSizeWidth = 0.5f;
				static float halfOneSizeWidth = oneSizeWidth * 0.5f;
				float width = size  * oneSizeWidth;
				float height = size * oneSizeWidth;
				float sizeOffset = float(size - 1) * halfOneSizeWidth;
				float x = (float)(col - num_cols * 0.5f) * 0.5f + sizeOffset;
				float y = 2.50f - ((float)row * 0.5f) - sizeOffset;

				assert(spriteIndex < num_sprites);
				if (spriteIndex < num_sprites)
				{
					sprites[spriteIndex].init
					(
						invaderer, x, y, width, height
					);
				}
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



