#include "titlemenu.h"

#include <vector>
#include <string>
#include <nds.h>
#include "../../gfx/color.h"
#include "../../sound/sound.h"
#include "../world/generatemenu.h"
#include "instructionsmenu.h"
#include "aboutmenu.h"
#include "../world/loadmenu.h"

std::vector<std::string> options = {"Start game", "Load game", "How to play", "About"};

void TitleMenu::tick(Game &game)
{
  size_t optionCount = options.size();

  if (game.justTapped(KEY_UP))
  {
    if (selected == 0)
      selected = optionCount - 1;
    else
      selected--;
  }
  if (game.justTapped(KEY_DOWN))
    selected++;

  if (selected >= optionCount)
    selected = 0;

  if (game.justTapped(KEY_A))
  {
    if (selected == 0)
    {
      Sound::test.play();
      game.resetGame();
      game.setMenu(std::make_unique<GenerateMenu>());
    }
    if (selected == 1)
    {
      game.enterMenu(std::make_unique<LoadMenu>(game));
    }
    if (selected == 2)
    {
      game.enterMenu(std::make_unique<InstructionsMenu>());
    }
    else if (selected == 3)
    {
      game.enterMenu(std::make_unique<AboutMenu>());
    }
  }
}

void TitleMenu::render(Screen &screen, Screen &bottomScreen)
{
  if (!clearedSubScreen)
  {
    bottomScreen.clear(0);
    clearedSubScreen = true;
  }

  screen.clear(0);

  int h = 2;
  int w = 13;
  int titleColor = Color::get(0, 010, 131, 551);
  int xo = (screen.w - w * 8) / 2;
  int yo = 24;
  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      screen.renderTile(xo + x * 8, yo + y * 8, x + (y + 6) * 32, titleColor, 0);
    }
  }

  int subtitleColor = Color::get(0, 551, 551, 551);
  screen.renderTextCentered("DS Edition!", screen.w / 2, 6 * 8, subtitleColor);

  int optionColor = Color::get(0, 222, 222, 222);
  int highlightColor = Color::get(0, 555, 555, 555);

  for (size_t i = 0; i < options.size(); i++)
  {
    auto msg = options[i];

    int col = optionColor;
    if (i == selected)
    {
      msg = "> " + msg + " <";
      col = highlightColor;
    }
    screen.renderTextCentered(msg, screen.w / 2, (10 + i) * 8 + 4, col);
  }
}
