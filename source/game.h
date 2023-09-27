#pragma once

#include "menu/menu.h"
#include "level/level.h"
#include "entity/player.h"
#include "gfx/glscreen.h"
#include "gfx/softwarescreen.h"
#include "gfx/lightmask.h"
#include "serialization/ContainerUUID.h"
#include "serialization/serializable.h"

#include <memory>

class Menu;
class Entity;

class Game : public IContainerUUID<std::shared_ptr<Entity>>
{
public:
  static const std::string NAME;
  GLScreen screen;
  SoftwareScreen bottomScreen;
  std::shared_ptr<Player> player;
  std::unique_ptr<Menu> menu;
  std::vector<Level> levels;
  int currentLevel = 3;
  bool frameSkipEnabled = true;

  Game();

  void tick();
  bool isHeld(int key);
  bool justTapped(int key);
  void render();
  void setMenu(std::unique_ptr<Menu> menu);
  void enterMenu(std::unique_ptr<Menu> menu);
  void scheduleLevelChange(int dir);
  void changeLevel(int dir);
  void win();
  void resetGame();

private:
  LightMask lightMask;
  static const int SCALE = 3;
  int tickCount = 0;
  int gameTime = 0;
  int playerDeadTime;
  int pendingLevelChange = 0;
  int wonTimer = 0;
  bool hasWon = false;
  void init();

  std::shared_ptr<Entity> getByUUID(UUID_Field &uuid) override;
};
