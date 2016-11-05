#ifndef INVADERER_H
#define INVADERER_H

namespace agarzonp
{
	class Invaderer : public GameObject
	{

	public:

		Invaderer()
		{
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

