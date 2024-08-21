
#pragma once

#include <SFML/Audio.hpp>

namespace engine {
	namespace audio {
		extern sf::SoundBuffer soundBuffer;
		extern sf::Sound sound;

		void initialize();
		void playSound(float pitch);
	}
}