#include "airwizard.h"

#include "../projectile/spark.h"
#include "../../sound/sound.h"
#include "../../level/level.h"

AirWizard::AirWizard()
: Mob(MK_AIRWIZARD)
{
  x = random.nextInt(64 * 16);
  y = random.nextInt(64 * 16);
  health = maxHealth = 2000;
}

void AirWizard::tick(Game &game, Level &level, std::shared_ptr<Entity> self)
{
  Mob::tick(game, level, self);

  if (attackDelay > 0)
  {
    dir = (attackDelay - 45) / 4 % 4;
    dir = (dir * 2 % 4) + (dir / 2);
    if (attackDelay < 45)
    {
      dir = 0;
    }
    attackDelay--;
    if (attackDelay == 0)
    {
      attackType = 0;
      if (health < 1000)
        attackType = 1;
      if (health < 200)
        attackType = 2;
      attackTime = 60 * 2;
    }
    return;
  }

  if (attackTime > 0)
  {
    attackTime--;
    double dir = attackTime * 0.25 * (attackTime % 2 * 2 - 1);
    double speed = (0.7) + attackType * 0.2;
    level.add(std::make_shared<Spark>(self, cos(dir) * speed, sin(dir) * speed));
    return;
  }

  if (level.player != NULL && randomWalkTime == 0)
  {
    int xd = level.player->x - x;
    int yd = level.player->y - y;
    if (xd * xd + yd * yd < 32 * 32)
    {
      xa = 0;
      ya = 0;
      if (xd < 0)
        xa = +1;
      if (xd > 0)
        xa = -1;
      if (yd < 0)
        ya = +1;
      if (yd > 0)
        ya = -1;
    }
    else if (xd * xd + yd * yd > 80 * 80)
    {
      xa = 0;
      ya = 0;
      if (xd < 0)
        xa = -1;
      if (xd > 0)
        xa = +1;
      if (yd < 0)
        ya = -1;
      if (yd > 0)
        ya = +1;
    }
  }

  int speed = (tickTime % 4) == 0 ? 0 : 1;
  if (!move(level, xa * speed, ya * speed) || random.nextInt(100) == 0)
  {
    randomWalkTime = 30;
    xa = (random.nextInt(3) - 1);
    ya = (random.nextInt(3) - 1);
  }
  if (randomWalkTime > 0)
  {
    randomWalkTime--;
    if (level.player != NULL && randomWalkTime == 0)
    {
      int xd = level.player->x - x;
      int yd = level.player->y - y;
      if (random.nextInt(4) == 0 && xd * xd + yd * yd < 50 * 50)
      {
        if (attackDelay == 0 && attackTime == 0)
        {
          attackDelay = 60 * 2;
        }
      }
    }
  }
}

void AirWizard::render(Screen &screen)
{
  int xt = 8;
  int yt = 14;

  int flip1 = (walkDist >> 3) & 1;
  int flip2 = (walkDist >> 3) & 1;

  if (dir == 1)
  {
    xt += 2;
  }
  if (dir > 1)
  {

    flip1 = 0;
    flip2 = ((walkDist >> 4) & 1);
    if (dir == 2)
    {
      flip1 = 1;
    }
    xt += 4 + ((walkDist >> 3) & 1) * 2;
  }

  int xo = x - 8;
  int yo = y - 11;

  int col1 = Color::get(-1, 100, 500, 555);
  int col2 = Color::get(-1, 100, 500, 532);
  if (health < 200)
  {
    if (tickTime / 3 % 2 == 0)
    {
      col1 = Color::get(-1, 500, 100, 555);
      col2 = Color::get(-1, 500, 100, 532);
    }
  }
  else if (health < 1000)
  {
    if (tickTime / 5 % 4 == 0)
    {
      col1 = Color::get(-1, 500, 100, 555);
      col2 = Color::get(-1, 500, 100, 532);
    }
  }
  if (hurtTime > 0)
  {
    col1 = Color::get(-1, 555, 555, 555);
    col2 = Color::get(-1, 555, 555, 555);
  }

  screen.renderTile(xo + 8 * flip1, yo + 0, xt + yt * 32, col1, flip1);
  screen.renderTile(xo + 8 - 8 * flip1, yo + 0, xt + 1 + yt * 32, col1, flip1);
  screen.renderTile(xo + 8 * flip2, yo + 8, xt + (yt + 1) * 32, col2, flip2);
  screen.renderTile(xo + 8 - 8 * flip2, yo + 8, xt + 1 + (yt + 1) * 32, col2, flip2);
}

void AirWizard::touchedBy(Level &level, Entity &entity)
{
  if (dynamic_cast<Player *>(&entity))
  {
    entity.hurt(level, *this, 3, dir);
  }
}

void AirWizard::doHurt(Level &level, int damage, int attackDir)
{
  Mob::doHurt(level, damage, attackDir);
  if (attackDelay == 0 && attackTime == 0)
  {
    attackDelay = 60 * 2;
  }
}

void AirWizard::die(Game &game, Level &level)
{
  Mob::die(game, level);

  if (game.player != NULL)
  {
    game.player->score += 1000;
    game.player->invulnerableTime = 60 * 5;
    game.win();
  }

  Sound::bossdeath.play();
}
