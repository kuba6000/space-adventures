#pragma once
#include <SFML/Graphics.hpp>

namespace GigaGra{
	class Editor
	{
	public:
		Editor();
		~Editor();
		void processEvents(sf::Event event);
		void render(float frame_delta);
	private:
		sf::View editorView;
	};

	extern Editor* editor;
}

