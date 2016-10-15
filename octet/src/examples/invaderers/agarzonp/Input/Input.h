#ifndef AGARZONP_INPUT_H
#define AGARZONP_INPUT_H

namespace agarzonp
{
	// This is basically a workaround/hack to get the input through different states
	class Input
	{
		static const octet::app* app;

	public:

		static void SetInput(const octet::app& theApp)
		{
			app = &theApp;
		};

		static bool is_key_down(unsigned int key)
		{
			assert(app); // SetInput has not been called
			return app ? app->is_key_down(key) : false;
		}

		static bool is_key_going_down(unsigned int key)
		{
			assert(app); // SetInput has not been called
			return app ? app->is_key_going_down(key) : false;
		}

		static bool is_key_going_up(unsigned int key)
		{
			assert(app); // SetInput has not been called
			return app ? app->is_key_going_up(key) : false;
		}
	};

	const octet::app* Input::app = nullptr;
}

#endif // !INPUT_H
