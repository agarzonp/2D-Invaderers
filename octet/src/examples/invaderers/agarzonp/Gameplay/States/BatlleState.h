#ifndef BATTLE_STATE_H
#define BATTLE_STATE_H

namespace agarzonp
{
	class BattleState : public GameState
	{
	public:
		BattleState() {};
		~BattleState() {};

		void Start() override {};
		void Stop()	override {};
		void Suspend() override {};
		void Resume() override {};
		void Update() override {};
		void Render() override {};

	};
}

#endif



