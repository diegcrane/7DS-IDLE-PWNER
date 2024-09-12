
#include "screen.hpp"

auto screen::capture() -> void
{
	auto context = GetDC(nullptr);

	auto compatible_context = CreateCompatibleDC(context);

	screen::size_x = GetDeviceCaps(context, HORZRES);
	screen::size_y = GetDeviceCaps(context, VERTRES);

	screen::bit_map = CreateCompatibleBitmap(context, screen::size_x, screen::size_y);

	auto old_bit_map = (HBITMAP__*)SelectObject(compatible_context, screen::bit_map);

	BitBlt(compatible_context, 0, 0, screen::size_x, screen::size_y, context, 0, 0, SRCCOPY);

	screen::bit_map_information.biSize = sizeof(tagBITMAPINFOHEADER);
	screen::bit_map_information.biWidth = screen::size_x;
	screen::bit_map_information.biHeight = -screen::size_y;
	screen::bit_map_information.biPlanes = 1;
	screen::bit_map_information.biBitCount = 24;
	screen::bit_map_information.biCompression = BI_RGB;
	screen::bit_map_information.biSizeImage = 0;
	screen::bit_map_information.biClrUsed = 0;
	screen::bit_map_information.biClrImportant = 0;

	auto temporal_pixels = std::vector<unsigned __int8>(screen::size_x * screen::size_y * 3);

	GetDIBits(compatible_context, screen::bit_map, 0, screen::size_y, temporal_pixels.data(), (tagBITMAPINFO*)&screen::bit_map_information, DIB_RGB_COLORS);

	screen::pixels.resize(screen::size_x * screen::size_y);

	for (auto y = 0; y < screen::size_y; ++y)
	{
		for (auto x = 0; x < screen::size_x; ++x)
		{
			auto pixel_index = (y * screen::size_x + x) * 3;

			screen::pixels[y * screen::size_x + x] =
			{
				(__int32)temporal_pixels[pixel_index + 2],
				(__int32)temporal_pixels[pixel_index + 1],
				(__int32)temporal_pixels[pixel_index]
			};
		}
	}

	SelectObject(compatible_context, old_bit_map);

	DeleteDC(compatible_context);

	ReleaseDC(nullptr, context);

	DeleteObject(screen::bit_map);
}

auto screen::release() -> void
{
	screen::pixels.clear();
}

auto screen::clear_console() -> void
{
	auto console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	auto console_buffer_information = _CONSOLE_SCREEN_BUFFER_INFO();

	GetConsoleScreenBufferInfo(console_handle, &console_buffer_information);

	auto min_coordinates = _COORD(0, 0);

	auto console_size = console_buffer_information.dwSize.X * console_buffer_information.dwSize.Y;

	unsigned long cells_written = 0;

	FillConsoleOutputCharacterA(console_handle, ' ', console_size, min_coordinates, &cells_written);

	FillConsoleOutputAttribute(console_handle, console_buffer_information.wAttributes, console_size, min_coordinates, &cells_written);

	SetConsoleCursorPosition(console_handle, min_coordinates);
}

auto screen::get_pixel(__int32 x, __int32 y) -> screen::__color
{
	return screen::pixels[y * screen::size_x + x];
}