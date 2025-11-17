#pragma once

#include "svg_canvas.hpp"
#include "catplot/axes.hpp"
#include <string>
#include <utility>
#include <vector>

namespace catplot {

inline std::string rgba_to_css(const Rgba& c) {
	std::ostringstream ss;
	ss << "rgba(" << int(c.r * 255.0 + 0.5) << "," << int(c.g * 255.0 + 0.5) << "," << int(c.b * 255.0 + 0.5) << "," << c.a << ")";
	return ss.str();
}

class SvgBackend {
public:
	static std::string render(int widthPx, int heightPx,
		int marginLeft, int marginRight, int marginTop, int marginBottom,
		const std::vector<std::pair<std::vector<double>, std::vector<double>>>& lineXY,
		const std::vector<Rgba>& lineColors,
		const std::vector<double>& lineWidths,
		const std::vector<std::pair<std::vector<double>, std::vector<double>>>& scatterXY,
		const std::vector<Rgba>& scatterColors,
		const std::vector<double>& scatterRadius,
		const std::string& title,
		const std::string& xlabel,
		const std::string& ylabel);

	// Overload: render into an existing canvas at 0,0 with width/height; margins still apply inside
	static void render_into(SvgCanvas& canvas, int widthPx, int heightPx,
		int marginLeft, int marginRight, int marginTop, int marginBottom,
		const std::vector<std::pair<std::vector<double>, std::vector<double>>>& lineXY,
		const std::vector<Rgba>& lineColors,
		const std::vector<double>& lineWidths,
		const std::vector<std::pair<std::vector<double>, std::vector<double>>>& scatterXY,
		const std::vector<Rgba>& scatterColors,
		const std::vector<double>& scatterRadius,
		const std::string& title,
		const std::string& xlabel,
		const std::string& ylabel);
};

} // namespace catplot


