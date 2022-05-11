#include "fonts.hpp"

namespace fonts {
jngl::Font& label() {
	static jngl::Font tmp("Inter-ExtraLight.ttf", 30);
	return tmp;
}
} // namespace fonts
