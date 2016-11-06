#ifndef DRAW_TEXT_H
#define DRAW_TEXT_H

namespace agarzonp
{
	class TextDrawer
	{
		static TextDrawer* s_instance;

		octet::bitmap_font font;
		GLuint font_texture;

	protected:

		TextDrawer()
			: font(512, 256, "assets/big.fnt")
			, font_texture(octet::resource_dict::get_texture_handle(GL_RGBA, "assets/big_0.gif"))
		{
			assert(!s_instance);
			s_instance = this;
		}

	public:

		static TextDrawer* GetInstance()
		{
			if (!s_instance)
			{
				return new TextDrawer();
			}

			return s_instance;
		}

		void Draw(octet::texture_shader &shader, const octet::mat4t& cameraToWorld, float x, float y, float scale, const char *text)
		{
			octet::draw_text(shader, cameraToWorld, font, font_texture, x, y, scale, text);
		}
	};

	TextDrawer* TextDrawer::s_instance = nullptr;
}

#endif // !DRAW_TEXT_H

