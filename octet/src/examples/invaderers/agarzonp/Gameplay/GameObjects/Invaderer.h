#ifndef INVADERER_H
#define INVADERER_H

namespace agarzonp
{
	struct InvadererType
	{
		int lives;
		int killScore;
		GLuint texture;
	};

	class Invaderer : public GameObject
	{
		InvadererType invadererType;

	public:

		Invaderer()
		{
		}

		void SetType(InvadererType& type)
		{
			invadererType = type;
		}

		void Update() override
		{
		}

		void OnCollisionWith(const GameObject* other) override
		{
			GameObject::OnCollisionWith(other);

			Translate(20.0f, 0.0f);
		}

	private:

	};
}

#endif // !INVADERER_H

