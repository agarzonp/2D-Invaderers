#ifndef SPRITE_POOL_H
#define SPRITE_POOL_H

namespace agarzonp
{
	class SpritePool
	{
		// big array of sprites
		octet::sprite* sprites;
		unsigned numSprites;

		static SpritePool* s_instance;

	protected:

		SpritePool()
			: sprites(nullptr)
			, numSprites(0)
		{
			assert(!s_instance);
			s_instance = this;
		}

	public:

		static SpritePool* GetInstance()
		{
			if (!s_instance)
			{
				return new SpritePool();
			}

			return s_instance;
		}
		

		~SpritePool()
		{
			delete[] sprites;
			s_instance = nullptr;
		}

		void Init(int size)
		{
			sprites = new octet::sprite[size];
			numSprites = size;
		}

		octet::sprite& operator[](unsigned index)
		{
			assert(index < numSprites);
			return sprites[index];
		}
		
		octet::sprite& GetSprite(unsigned index)
		{
			assert(index < numSprites);
			return sprites[index];
		}
	};

	SpritePool* SpritePool::s_instance = nullptr;
}


#endif // ! SPRITE_POOL_H
