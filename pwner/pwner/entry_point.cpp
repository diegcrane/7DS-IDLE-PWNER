
#include "game/game.hpp"

auto main() -> __int32
{
	for (;; game::run_scan(), Sleep(100));
}