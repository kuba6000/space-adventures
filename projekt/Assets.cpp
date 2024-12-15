#include "Assets.h"
#include <Windows.h>

namespace GigaGra {

	void tryLoad(sf::Texture* txt, const std::string& path) {
		if (!txt->loadFromFile(path)) {
			MessageBox(NULL, L"Failed to load texture", L"Error", MB_ICONERROR | MB_OK);
			exit(1);
		}
	}

	Assets::Assets()
	{
		availableTiles[0] = new Grass{};
		availableTiles[1] = new Stone{};
		availableTiles[2] = new AluminiumFloor{};
		availableTiles[3] = new AluminiumWall{};
		availableTiles[4] = new AluminiumDoor{};
		availableTiles[5] = new ControlPanel{};
		availableTiles[6] = new Tree{};
		availableTiles[7] = new Wood{};
		availableTiles[8] = new Water{};
		availableTiles[9] = new Bed{};
		availableTiles[10] = new IronOre{};
		availableTiles[11] = new Gravel{};
		availableTiles[12] = new Sand{};
		availableTiles[13] = new Krzak{};
		availableTiles[14] = new Ice{};
		availableTiles[15] = new Snow{};

		tryLoad(&grassTexture, "assets\\grass.png");
		tryLoad(&stoneTexture, "assets\\stone.png");
		tryLoad(&playerTexture, "assets\\p.png");
		tryLoad(&aluminiumWallTexture, "assets\\aluminiumwall.png");
		tryLoad(&aluminiumFloorTexture, "assets\\aluminiumfloor.png");
		tryLoad(&aluminiumDoorTexture, "assets\\aluminiumdoor.png");
		tryLoad(&controlPanelTexture, "assets\\controlpanel.png");
		tryLoad(&hand, "assets\\hand.png");
		tryLoad(&handPistol, "assets\\handpistol.png");
		tryLoad(&planet1Texture, "assets\\test.png");
		tryLoad(&jackTexture, "assets\\jack.png");
		tryLoad(&lTexture, "assets\\l.png");
		tryLoad(&coinTexture, "assets\\coin.png");
		tryLoad(&shipPart1Texture, "assets\\shippart1.png");
		tryLoad(&shipPart2Texture, "assets\\shippart2.png");
		tryLoad(&shipPart3Texture, "assets\\shippart3.png");
		tryLoad(&shipPart4Texture, "assets\\shippart4.png");
		tryLoad(&gragTexture, "assets\\grag.png");
		tryLoad(&stoneBlobTexture, "assets\\stoneblob.png");
		tryLoad(&ironManTexture, "assets\\ironman.png");
		tryLoad(&krzakorTexture, "assets\\krzakor.png");
		tryLoad(&patyczakTexture, "assets\\patyczak.png");
		tryLoad(&treeTexture, "assets\\tree.png");
		tryLoad(&woodTexture, "assets\\wood.png");
		tryLoad(&waterTexture, "assets\\water.png");
		tryLoad(&bedTexture, "assets\\bed.png");
		tryLoad(&ironOreTexture, "assets\\ironore.png");
		tryLoad(&gravelTexture, "assets\\gravel.png");
		tryLoad(&sandTexture, "assets\\sand.png");
		tryLoad(&iceTexture, "assets\\ice.png");
		tryLoad(&snowTexture, "assets\\snow.png");
		tryLoad(&krzakTexture, "assets\\krzak.png");
	}

	Assets::~Assets()
	{
	}



	Assets* assets;
}