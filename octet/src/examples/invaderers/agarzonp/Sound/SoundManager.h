#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "SoundIds.h"

namespace agarzonp
{
	enum
	{
		num_sound_sources = 8
	};

	class SoundManager
	{
		static SoundManager* s_instance;

		unsigned cur_source;
		octet::ALuint sources[num_sound_sources];

	protected:

		SoundManager()
		{
			assert(!s_instance);
			s_instance = this;

			Init();
		}

		octet::ALuint get_sound_source() { return sources[cur_source++ % num_sound_sources]; }

	public:

		static SoundManager* GetInstance()
		{
			if (!s_instance)
			{
				return new SoundManager;
			}

			return s_instance;
		}

		~SoundManager()
		{
			s_instance = nullptr;
		}

		void Init()
		{
			cur_source = 0;
			octet::alGenSources(num_sound_sources, sources);
		}

		void Play(SoundId soundId)
		{
			octet::ALuint sound = octet::resource_dict::get_sound_handle(octet::AL_FORMAT_MONO16, s_sounds[(int)soundId]);
			octet::ALuint source = get_sound_source();
			octet::alSourcei(source, octet::AL_BUFFER, sound);
			octet::alSourcePlay(source);
		}
	};

	SoundManager* SoundManager::s_instance = nullptr;
}

#endif // !SOUND_MANAGER_H

