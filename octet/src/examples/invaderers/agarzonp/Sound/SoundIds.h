#ifndef SOUND_IDS_H
#define SOUND_IDS_H

namespace agarzonp
{
	enum class SoundId : unsigned
	{
		WHOOSH,
		BANG,

		NUM_SOUND_IDS
	};

	static const char* s_sounds[] =
	{
		"assets/invaderers/whoosh.wav",
		"assets/invaderers/bang.wav"
	};
};

#endif SOUND_IDS_H