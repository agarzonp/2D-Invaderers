#ifndef GAME_OJECT_H
#define GAME_OJECT_H

namespace agarzonp
{
	class GameObject
	{
		octet::sprite sprite;

	protected:

		octet::shaders::texture_shader* shader;

		bool inUse;

		// The GameObject that triggers the current GameObject
		GameObject* triggerer;

	public:

		GameObject()
			: shader(nullptr)
			, inUse (false)
		{
		}

		~GameObject()
		{
		}

		const octet::sprite& GetSprite() const { return sprite; }
		octet::sprite& GetSprite() { return sprite; }

		bool IsInUse()
		{
			return inUse;
		}

		void SetIsInUse(bool isInUse)
		{
			inUse = isInUse;
		}
		
		virtual void Reset()
		{

		}

		virtual void Update() = 0;

		virtual void Render(const octet::math::mat4t& cameraToWorld)
		{
			if (inUse)
			{
				sprite.render(*shader, cameraToWorld);
			}
		}

		virtual void AddChild(GameObject* child)
		{
		}


		void SetTriggerer(GameObject* triggerer_)
		{
			triggerer = triggerer_;
		}

		void AddShader(octet::shaders::texture_shader* _shader)
		{
			assert(_shader);

			shader = _shader;
		}

		void Translate(float x, float y)
		{
			sprite.translate(x, y);
		}

		virtual bool CollidesWith(const GameObject* other)
		{
			return sprite.collides_with(other->GetSprite());
		}

		virtual void OnCollisionWith(const GameObject* other)
		{
			SetIsInUse(false);
		}

		virtual void OnGameObjectHit(GameObject* other)
		{

		}

		void SetRelativeTo(const GameObject* other, float x, float y)
		{
			sprite.set_relative(other->GetSprite(), x, y);
		}

		bool IsAbove(const GameObject* other, float margin)
		{
			return sprite.is_above(other->GetSprite(), margin);
		}

	};
}

#endif // !GAME_OJECT_H

