
#include "../screen/screen.hpp"

namespace game
{
	inline HWND__* window = nullptr;
	inline tagRECT window_position = {};

	extern auto get_window_data() -> bool;

	struct __position
	{
		float x = 0, y = 0;
	};

	struct __slot
	{
		__slot(game::__position position, __int32 size_x, __int32 size_y);

		game::__position position = {};	
		__int32 size_x = 0;
		__int32 size_y = 0;
		screen::__color original_color = {};
		screen::__color filtered_color = {};

		auto is_empty() -> bool;
		auto contains_same_item(__slot slot) -> bool;
	};

	extern auto get_slot_from_grid(game::__position position) -> game::__slot;
	extern auto run_scan() -> void;
}