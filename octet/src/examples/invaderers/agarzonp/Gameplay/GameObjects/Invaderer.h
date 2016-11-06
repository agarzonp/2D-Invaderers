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

		const InvadererType& GetType()
		{
			return invadererType;
		}

		void Update() override
		{
			
		}

		void OnCollisionWith(const GameObject* other) override
		{
			invadererType.lives -= 1;

			if (invadererType.lives <= 0)
			{
				SetIsInUse(false);

				Translate(20.0f, 0.0f);
			}
		}

	private:

	};
}

#endif // !INVADERER_H

