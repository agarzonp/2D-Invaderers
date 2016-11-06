////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// invaderer example: simple game with sprites and sounds
//
// Level: 1
//
// Demonstrates:
//   Basic framework app
//   Shaders
//   Basic Matrices
//   Simple game mechanics
//   Texture loaded from GIF file
//   Audio
//

namespace octet {

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class sprite {
		// where is our sprite (overkill for a 2D game!)
		mat4t modelToWorld;

		// half the width of the sprite
		float halfWidth;

		// half the height of the sprite
		float halfHeight;

		// what texture is on our sprite
		int texture;

		// true if this sprite is enabled.
		bool enabled;
	public:
		sprite() {
			texture = 0;
			enabled = true;
		}

		void init(int _texture, float x, float y, float w, float h) {
			modelToWorld.loadIdentity();
			modelToWorld.translate(x, y, 0);
			halfWidth = w * 0.5f;
			halfHeight = h * 0.5f;
			texture = _texture;
			enabled = true;
		}

		void set(float x, float y, float w, float h) {
			modelToWorld.loadIdentity();
			modelToWorld.translate(x, y, 0);
			halfWidth = w * 0.5f;
			halfHeight = h * 0.5f;
		}

		void set(float x, float y) {
			modelToWorld.loadIdentity();
			modelToWorld.translate(x, y, 0);
		}

		void render(texture_shader &shader, const mat4t &cameraToWorld) {
			// invisible sprite... used for gameplay.
			if (!texture || !enabled) return;

			// build a projection matrix: model -> world -> camera -> projection
			// the projection space is the cube -1 <= x/w, y/w, z/w <= 1
			mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

			// set up opengl to draw textured triangles using sampler 0 (GL_TEXTURE0)
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);

			// use "old skool" rendering
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			shader.render(modelToProjection, 0);

			// this is an array of the positions of the corners of the sprite in 3D
			// a straight "float" here means this array is being generated here at runtime.
			float vertices[] = {
			  -halfWidth, -halfHeight, 0,
			   halfWidth, -halfHeight, 0,
			   halfWidth,  halfHeight, 0,
			  -halfWidth,  halfHeight, 0,
			};

			// attribute_pos (=0) is position of each corner
			// each corner has 3 floats (x, y, z)
			// there is no gap between the 3 floats and hence the stride is 3*sizeof(float)
			glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)vertices);
			glEnableVertexAttribArray(attribute_pos);

			// this is an array of the positions of the corners of the texture in 2D
			static const float uvs[] = {
			   0,  0,
			   1,  0,
			   1,  1,
			   0,  1,
			};

			// attribute_uv is position in the texture of each corner
			// each corner (vertex) has 2 floats (x, y)
			// there is no gap between the 2 floats and hence the stride is 2*sizeof(float)
			glVertexAttribPointer(attribute_uv, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)uvs);
			glEnableVertexAttribArray(attribute_uv);

			// finally, draw the sprite (4 vertices)
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}

		// move the object
		void translate(float x, float y) {
			modelToWorld.translate(x, y, 0);
		}

		// local rotate to calculate the new halfWidth hand halfHeight
		/*
		void local_rotate(float angle)
		{
			// rotate local coordinates...

			octet::vec2 points[4];
			points[0] = octet::vec2(-halfWidth, -halfHeight);
			points[1] = octet::vec2(halfWidth, -halfHeight);
			points[2] = octet::vec2(halfWidth, halfHeight);
			points[3] = octet::vec2(-halfWidth, halfHeight);

			angle = (angle * 3.14159265f) / 180.0f;

			for (int i = 0; i < 4; i++)
			{
				octet::vec2& p = points[i];

				float x = cosf(angle) * p.x() - sinf(angle) * p.y();
				float y = sinf(angle) * p.x() + cosf(angle) * p.y();

				p.x() = x;
				p.y() = y;
			}

			// ...so we can adjust halfWidth and halfHeight

			float minX = FLT_MAX;
			float minY = FLT_MAX;
			float maxX = FLT_MIN;
			float maxY = FLT_MIN;
			for (int i = 0; i < 4; i++)
			{
				octet::vec2& p = points[i];

				minX = fminf(minX, p.x());
				minY = fminf(minY, p.y());

				maxX = fmaxf(maxX, p.x());
				maxY = fmaxf(maxY, p.y());
			}

			halfWidth  = fabsf(maxX - minX) * 0.5f;
			halfHeight = fabsf(maxY - minY) * 0.5f;
			
		}
		*/

		// position the object relative to another.
		void set_relative(const sprite &rhs, float x, float y) {
			modelToWorld = rhs.modelToWorld;
			modelToWorld.translate(x, y, 0);
		}

		// return true if this sprite collides with another.
		// note the "const"s which say we do not modify either sprite
		bool collides_with(const sprite &rhs) const {
			float dx = rhs.modelToWorld[3][0] - modelToWorld[3][0];
			float dy = rhs.modelToWorld[3][1] - modelToWorld[3][1];

			// both distances have to be under the sum of the halfwidths
			// for a collision
			return
				(fabsf(dx) < halfWidth + rhs.halfWidth) &&
				(fabsf(dy) < halfHeight + rhs.halfHeight)
				;
		}

		bool is_above(const sprite &rhs, float margin) const {
			float dx = rhs.modelToWorld[3][0] - modelToWorld[3][0];

			return
				(fabsf(dx) < halfWidth + margin)
				;
		}

		bool &is_enabled() {
			return enabled;
		}
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

namespace octet
{
	void draw_text(octet::texture_shader &shader, const octet::mat4t& cameraToWorld, octet::bitmap_font& font, GLuint font_texture, float x, float y, float scale, const char *text) {
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
}

#include "agarzonp/agarzonp.h"

namespace octet
{
  class invaderers_app : public octet::app 
  {
	  agarzonp::Gameplay gameplay;

  public:

    // this is called when we construct the class
    invaderers_app(int argc, char **argv) 
		: app(argc, argv)
	{
    }

	~invaderers_app()
	{
	}

    // this is called once OpenGL is initialized
	void app_init()
	{
		gameplay.Init(*this);
	}

	// this is called to simulate the world
	void simulate_world() 
	{
		// update the gameplay
		gameplay.Update();
	}

    // this is called to draw the world
    void draw_world(int x, int y, int w, int h) {

      // set a viewport - includes whole window area
      glViewport(x, y, w, h);

      // clear the background to black
      glClearColor(1, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // don't allow Z buffer depth testing (closer objects are always drawn in front of far ones)
      glDisable(GL_DEPTH_TEST);

      // allow alpha blend (transparency when alpha channel is 0)
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	 
	  // render the gameplay
	  gameplay.Render();
    }
  };
}
