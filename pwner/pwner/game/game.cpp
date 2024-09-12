
#include "game.hpp"

auto game::get_window_data() -> bool
{
	if (game::window = FindWindowA(nullptr, "BlueStacks App Player"); game::window == nullptr)
		return false;

	if (game::window != GetForegroundWindow())
		return false;

	if (GetWindowRect(game::window, &game::window_position) == 0)
		return false;

	return true;
}

game::__slot::__slot(game::__position position, __int32 size_x, __int32 size_y) : position(position), size_x(size_x), size_y(size_y)
{
	auto original_color_iterations = 0, filtered_color_iterations = 0;

	for (auto x = -this->size_x * .25f; x < this->size_x * .25f; ++x)
	{
		for (auto y = -this->size_y * .25f; y < this->size_y * .25f; ++y)
		{
			auto pixel = screen::get_pixel((__int32)this->position.x + (__int32)x, (__int32)this->position.y + (__int32)y);

			this->original_color.r += pixel.r;
			this->original_color.g += pixel.g;
			this->original_color.b += pixel.b;

			++original_color_iterations;

			if (abs(pixel.r - 228) < 5 && abs(pixel.g - 216) < 5 && abs(pixel.b - 198) < 5)
				continue;

			if (abs(pixel.r - 247) < 5 && abs(pixel.g - 241) < 5 && abs(pixel.b - 214) < 5)
				continue;

			this->filtered_color.r += pixel.r;
			this->filtered_color.g += pixel.g;
			this->filtered_color.b += pixel.b;

			++filtered_color_iterations;
		}
	}

	this->original_color.r /= original_color_iterations;
	this->original_color.g /= original_color_iterations;
	this->original_color.b /= original_color_iterations;

	if (filtered_color_iterations != 0)
	{
		this->filtered_color.r /= filtered_color_iterations;
		this->filtered_color.g /= filtered_color_iterations;
		this->filtered_color.b /= filtered_color_iterations;
	}
}

auto game::__slot::is_empty() -> bool
{
	if (abs(this->original_color.r - 228) < 5 && abs(this->original_color.g - 216) < 5 && abs(this->original_color.b - 198) < 5)
		return true;

	if (abs(this->original_color.r - 247) < 5 && abs(this->original_color.g - 241) < 5 && abs(this->original_color.b - 214) < 5)
		return true;

	return false;
}

auto game::__slot::contains_same_item(game::__slot slot) -> bool
{
	return abs(this->filtered_color.r - slot.filtered_color.r) < 10 && abs(this->filtered_color.g - slot.filtered_color.g) < 10 && abs(this->filtered_color.b - slot.filtered_color.b) < 10;
}

auto game::get_slot_from_grid(game::__position position) -> game::__slot	//900x1600 320DPI
{
	auto game_virtual_window_size_x = (float)game::window_position.right - 1.f - (float)game::window_position.left - 1.f;
	auto game_virtual_window_size_y = (float)game::window_position.bottom - 1.f - (float)game::window_position.top - 33.f;

	auto slot_size_x = game_virtual_window_size_x * .9553f * .125f;
	auto slot_size_y = game_virtual_window_size_y * .5763f * .1f;

	return game::__slot
	({
		(float)game::window_position.left + 1.f + game_virtual_window_size_x * .0239f + position.x * slot_size_x + slot_size_x * .5f,
		(float)game::window_position.top + 33.f + game_virtual_window_size_y * .3258f + position.y * slot_size_y + slot_size_y * .5f
	},
	slot_size_x, slot_size_y);
}

auto game::run_scan() -> void
{
	if (!game::get_window_data())
		return;

	screen::capture();

	screen::clear_console();

	for (auto y = 0.f; y < 10.f; ++y)
	{
		for (auto x = 0.f; x < 8.f; ++x)
			std::cout << "[" << (game::get_slot_from_grid({ x, y }).is_empty() ? " " : "X") << "]";

		std::cout << std::endl;
	}

	auto stop_search_complete = false;

	for (auto x = 0.f; x < 8.f && !stop_search_complete; ++x)
	{
		for (auto y = 0.f; y < 10.f && !stop_search_complete; ++y)
		{
			auto slot = game::get_slot_from_grid({ x, y });

			if (!slot.is_empty())
				continue;

			static auto should_click_slot_simple = [&]() -> bool
			{
				auto stop_search_simple_0 = false;

				for (auto _x = x - 1.f; _x >= 0.f && !stop_search_simple_0; --_x)
				{
					if (auto temporal_slot_0 = game::get_slot_from_grid({ _x, y }); !temporal_slot_0.is_empty())
					{
						for (auto x_ = x + 1.f; x_ < 8.f && !stop_search_simple_0; ++x_)
						{
							if (auto temporal_slot_1 = game::get_slot_from_grid({ x_, y }); !temporal_slot_1.is_empty())
							{
								if (temporal_slot_0.contains_same_item(temporal_slot_1))
									return true;

								stop_search_simple_0 = true;
							}
						}

						stop_search_simple_0 = true;
					}
				}

				auto stop_search_simple_1 = false;

				for (auto _y = y - 1.f; _y >= 0.f && !stop_search_simple_1; --_y)
				{
					if (auto temporal_slot_0 = game::get_slot_from_grid({ x, _y }); !temporal_slot_0.is_empty())
					{
						for (auto y_ = y + 1.f; y_ < 10.f && !stop_search_simple_1; ++y_)
						{
							if (auto temporal_slot_1 = game::get_slot_from_grid({ x, y_ }); !temporal_slot_1.is_empty())
							{
								if (temporal_slot_0.contains_same_item(temporal_slot_1))
									return true;

								stop_search_simple_1 = true;
							}
						}

						stop_search_simple_1 = true;
					}
				}

				return false;
			};

			static auto should_click_slot_advanced = [&]() -> bool
			{
				auto stop_search_advanced_0 = false;

				for (auto _x = x - 1.f; _x >= 0.f && !stop_search_advanced_0; --_x)
				{
					if (auto temporal_slot_0 = game::get_slot_from_grid({ _x, y }); !temporal_slot_0.is_empty())
					{
						for (auto y_ = y + 1.f; y_ < 10.f && !stop_search_advanced_0; ++y_)
						{
							if (auto temporal_slot_1 = game::get_slot_from_grid({ x, y_ }); !temporal_slot_1.is_empty())
							{
								if (temporal_slot_0.contains_same_item(temporal_slot_1))
									return true;

								stop_search_advanced_0 = true;
							}
						}

						stop_search_advanced_0 = true;
					}
				}

				auto stop_search_advanced_1 = false;

				for (auto x_ = x + 1.f; x_ < 8.f && !stop_search_advanced_1; ++x_)
				{
					if (auto temporal_slot_0 = game::get_slot_from_grid({ x_, y }); !temporal_slot_0.is_empty())
					{
						for (auto _y = y - 1.f; _y >= 0.f && !stop_search_advanced_1; --_y)
						{
							if (auto temporal_slot_1 = game::get_slot_from_grid({ x, _y }); !temporal_slot_1.is_empty())
							{
								if (temporal_slot_0.contains_same_item(temporal_slot_1))
									return true;

								stop_search_advanced_1 = true;
							}
						}

						stop_search_advanced_1 = true;
					}
				}

				auto stop_search_advanced_2 = false;

				for (auto _x = x - 1.f; _x >= 0.f && !stop_search_advanced_2; --_x)
				{
					if (auto temporal_slot_0 = game::get_slot_from_grid({ _x, y }); !temporal_slot_0.is_empty())
					{
						for (auto _y = y - 1.f; _y >= 0.f && !stop_search_advanced_2; --_y)
						{
							if (auto temporal_slot_1 = game::get_slot_from_grid({ x, _y }); !temporal_slot_1.is_empty())
							{
								if (temporal_slot_0.contains_same_item(temporal_slot_1))
									return true;

								stop_search_advanced_2 = true;
							}
						}

						stop_search_advanced_2 = true;
					}
				}

				auto stop_search_advanced_3 = false;

				for (auto x_ = x + 1.f; x_ < 8.f && !stop_search_advanced_3; ++x_)
				{
					if (auto temporal_slot_0 = game::get_slot_from_grid({ x_, y }); !temporal_slot_0.is_empty())
					{
						for (auto y_ = y + 1.f; y_ < 10.f && !stop_search_advanced_3; ++y_)
						{
							if (auto temporal_slot_1 = game::get_slot_from_grid({ x, y_ }); !temporal_slot_1.is_empty())
							{
								if (temporal_slot_0.contains_same_item(temporal_slot_1))
									return true;

								stop_search_advanced_3 = true;
							}
						}

						stop_search_advanced_3 = true;
					}
				}

				return false;
			};

			if (should_click_slot_simple() || should_click_slot_advanced())
			{
				auto cursor_pos = tagPOINT();

				GetCursorPos(&cursor_pos);

				SetCursorPos(slot.position.x, slot.position.y);

				mouse_event(0x0002, slot.position.x, slot.position.y, 0, 0);
				mouse_event(0x0004, slot.position.x, slot.position.y, 0, 0);

				SetCursorPos(cursor_pos.x, cursor_pos.y);

				stop_search_complete = true;
			}
		}
	}

	if (stop_search_complete)
		Sleep(500);

	screen::release();
}