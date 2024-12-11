#include "Assets.h"
#include <Windows.h>

namespace GigaGra {
	Assets::Assets()
	{
		if (!grassTexture.loadFromFile("assets\\grass.png")) {
			MessageBox(NULL, L"Failed to load grass.png", L"Error", MB_ICONERROR || MB_OK);
			exit(1);
		}
		if (!stoneTexture.loadFromFile("assets\\stone.png")) {
			MessageBox(NULL, L"Failed to load stone.png", L"Error", MB_ICONERROR || MB_OK);
			exit(1);
		}
		if (!ziomalTexture.loadFromFile("assets\\ziomal.png")) {
			MessageBox(NULL, L"Failed to load ziomal.png", L"Error", MB_ICONERROR || MB_OK);
			exit(1);
		}
		if (!aluminiumWallTexture.loadFromFile("assets\\aluminiumwall.png")) {
			MessageBox(NULL, L"Failed to load aluminiumwall.png", L"Error", MB_ICONERROR || MB_OK);
			exit(1);
		}
		if (!aluminiumFloorTexture.loadFromFile("assets\\aluminiumfloor.png")) {
			MessageBox(NULL, L"Failed to load aluminiumfloor.png", L"Error", MB_ICONERROR || MB_OK);
			exit(1);
		}
	}

	Assets::~Assets()
	{
	}



	Assets* assets;
}