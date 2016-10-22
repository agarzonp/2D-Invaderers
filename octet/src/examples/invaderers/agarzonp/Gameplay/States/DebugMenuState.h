#ifndef PAUSE_STATE_H
#define DEBUG_MENU_STATE_H

namespace agarzonp
{
	enum class DebugCommand : uint8_t
	{
		STATE,		// state battle 2 ; state menu;
		SETTING,	// setting sound on
		HELP,		// help
		EXIT,		// exit

		NUM_VALID_COMMANDS,

		UNKNOWN
	};

	static char* s_debugCommandNames[] =
	{
		"state",
		"setting",
		"help",
		"exit"
	};

	class DebugMenuState : public GameState
	{

	public:
		DebugMenuState(GameStateMachineInterface* gsmInterface)
			: GameState(gsmInterface)
		{
		}

		~DebugMenuState() {}

		void Start(GameStateParams* params) override 
		{
			GameState::Start(params);
		}

		void Stop() override 
		{
			GameState::Stop();
		}

		void Suspend() override 
		{
			GameState::Suspend();
		}

		void Resume() override 
		{
			GameState::Resume();
		}

		void Update() override
		{
			GameState::Update();

			static std::string command; 
			
			printf("Enter debug command (type help for available commands): ");
			std::getline(std::cin, command);

			if (!ExecuteCommand(command.c_str()))
			{
				printf("\nCommand not valid. Make sure you typed correctly.\n");
			}
		}

		void Render() override 
		{
			GameState::Render();
		}

	private:

		bool ExecuteCommand(const char* command)
		{
			CSVParser parser;
			parser.ParseCSVLine(command, ' ');

			const char* commandId = parser[0][0];
			switch (GetCommand(commandId))
			{
			case DebugCommand::STATE:
			{
				return ExecuteStateCommand(parser[0][1], parser[0].NumTokens() > 2 ? parser[0][2] : "");
			}
			case DebugCommand::SETTING:
			{
				return ExecuteSettingCommand(parser[0][1], parser[0].NumTokens() > 2 ? parser[0][2] : "");
			}
			case DebugCommand::HELP:
			{
				return ExecuteHelpCommand();
			}
			case DebugCommand::EXIT:
			{
				gameStateMachineInterface->PopState();
				return true;
			}
			case DebugCommand::UNKNOWN:
				break;
			default:
				assert(false); // command not handled!
				break;
			}

			return false;
		}

		DebugCommand GetCommand(const char* commandName)
		{
			for (int i = 0; i < (int)DebugCommand::NUM_VALID_COMMANDS; i++)
			{
				if (strcmp(commandName, s_debugCommandNames[i]) == 0)
				{
					return DebugCommand(i);
				}
			}

			return DebugCommand::UNKNOWN;
		}

		bool ExecuteStateCommand(const char* state, const char* arg)
		{
			for (int i = 0; i < (int)GameStateId::NUM_GAME_STATE_IDS; i++)
			{
				if (strcmp(state, s_gameStateIdNames[i]) == 0)
				{
					BattleStateParams* params = new BattleStateParams();

					// check that the argument is the level to load
					if (arg)
					{
						bool isNumber = true;
						
						const char* p = arg;
						while (*p)
						{
							if (!std::isdigit(*p))
							{
								isNumber = false;
								
								break;
							}

							p++;
						}
					
						assert(isNumber);
						if (isNumber)
						{
							int level = atoi(arg);
							params->level = level;
						}
					}

					gameStateMachineInterface->SetState(GameStateId(i), params); 
					return true;
				}
			}

			return false;
		}

		bool ExecuteSettingCommand(const char* setting, const char* arg)
		{
			printf("Setting Command not implemented");
			return true;
		}

		bool ExecuteHelpCommand()
		{
			printf("Available commands:\n");
			printf("state [GameStateId] [param] -> Set the next GameState\n");
			printf("setting [SettingId] [0, 1] -> Set a setting off (0) or on (1)\n");
			printf("exit -> Exit from the debug menu\n");

			return true;
		}
	};
}

#endif // !PAUSE_STATE_H



