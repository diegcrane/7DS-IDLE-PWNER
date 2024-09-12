
#include <windows.h>
#include <iostream>
#include <vector>

namespace screen
{
	inline __int32 size_x = 0, size_y = 0;
	inline HBITMAP__* bit_map = nullptr;
	inline tagBITMAPINFOHEADER bit_map_information = {};

	struct __color
	{
		__int32 r = 0, g = 0, b = 0;
	};

	inline std::vector<__color> pixels;

	extern auto capture() -> void;
	extern auto release() -> void;
	extern auto clear_console() -> void;
	extern auto get_pixel(__int32 x, __int32 y) -> screen::__color;
}