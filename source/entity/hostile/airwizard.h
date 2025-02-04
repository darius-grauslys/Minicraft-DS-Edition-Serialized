#pragma once

#include "../mob.h"

class AirWizard : public Mob
{
public:
  AirWizard();
  AirWizard(Serializer &serializer)
      : Mob(serializer)
  {}

  void tick(Game &game, Level &level, std::shared_ptr<Entity> self) override;
  void render(Screen &screen) override;

protected:
  void touchedBy(Level &level, Entity &entity) override;
  void doHurt(Level &level, int damage, int attackDir) override;
  void die(Game &game, Level &level) override;
};
