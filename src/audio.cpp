#include "../include/audio.h"
#include "../include/ui.h"

namespace engine {
	namespace audio {
		sf::SoundBuffer buffer{};
		sf::Sound sound{};

		void initialize()
		{
			buffer.loadFromFile("../resources/beep.wav");
			sound.setBuffer(buffer);
		}

		void playSound(float pitch) {
			sound.setPitch(pitch);
			sound.setVolume(engine::ui::audioSlider->getValue());
			sound.play();
		}
	}
}