#pragma once
#include <SFML/Graphics.hpp>
namespace GigaGra {
	class Settings
	{
	public:
		Settings();
		~Settings();
		
		bool showFPS = true;
		int maxFPS = 300;
		bool VSync = true;
		bool fullscreen = false;
	};
	extern Settings* settings;
}