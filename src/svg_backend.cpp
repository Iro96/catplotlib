#include "svg_backend.hpp"
#include <cmath>
#include <sstream>
#include <tuple>

namespace catplot {

static std::tuple<double, double, double, double> compute_data_bounds(
	const std::vector<std::pair<std::vector<double>, std::vector<double>>>& lineXY,
	const std::vector<std::pair<std::vector<double>, std::vector<double>>>& scatterXY) {
	bool has = false;
	double xmin=0, xmax=0, ymin=0, ymax=0;
	auto consider = [&](const std::vector<double>& xs, const std::vector<double>& ys) {
		for (size_t i = 0; i < xs.size() && i < ys.size(); ++i) {
			double x = xs[i], y = ys[i];
			if (!has) { xmin = xmax = x; ymin = ymax = y; has = true; }
			xmin = std::min(xmin, x); xmax = std::max(xmax, x);
			ymin = std::min(ymin, y); ymax = std::max(ymax, y);
		}
	};
	for (const auto& p : lineXY) consider(p.first, p.second);
	for (const auto& p : scatterXY) consider(p.first, p.second);
	if (!has) { xmin = 0; xmax = 1; ymin = 0; ymax = 1; }
	// expand by 5%
	auto expand = [](double& vmin, double& vmax) {
		double range = (vmax - vmin);
		if (range == 0.0) { vmin -= 0.5; vmax += 0.5; return; }
		double pad = range * 0.05;
		vmin -= pad; vmax += pad;
	};
	expand(xmin, xmax);
	expand(ymin, ymax);
	return {xmin, xmax, ymin, ymax};
}

static std::vector<double> nice_ticks(double vmin, double vmax, int target) {
	std::vector<double> ticks;
	if (vmax < vmin) std::swap(vmax, vmin);
	double range = vmax - vmin;
	if (range <= 0) range = std::abs(vmin) > 1e-12 ? std::abs(vmin) : 1.0;
	double rawStep = range / std::max(1, target);
	double mag = std::pow(10.0, std::floor(std::log10(rawStep)));
	double norm = rawStep / mag;
	double step;
	if (norm < 1.5) step = 1.0;
	else if (norm < 3.0) step = 2.0;
	else if (norm < 7.0) step = 5.0;
	else step = 10.0;
	step *= mag;
	double start = std::ceil(vmin / step) * step;
	for (double v = start; v <= vmax + 1e-12; v += step) ticks.push_back(v);
	return ticks;
}

static std::string fmt_num(double v) {
	std::ostringstream ss;
	ss.setf(std::ios::fixed); ss.precision(6);
	ss << v;
	std::string s = ss.str();
	// trim trailing zeros
	while (!s.empty() && s.back() == '0') s.pop_back();
	if (!s.empty() && s.back() == '.') s.pop_back();
	if (s.empty() || s == "-0") s = "0";
	return s;
}

static double map_x(double x, double xmin, double xmax, double left, double right) {
	if (xmax == xmin) return (left + right) * 0.5;
	double t = (x - xmin) / (xmax - xmin);
	return left + t * (right - left);
}

static double map_y(double y, double ymin, double ymax, double top, double bottom) {
	if (ymax == ymin) return (top + bottom) * 0.5;
	double t = (y - ymin) / (ymax - ymin);
	// SVG y grows downward
	return bottom - t * (bottom - top);
}

std::string SvgBackend::render(
	int widthPx, int heightPx,
	int marginLeft, int marginRight, int marginTop, int marginBottom,
	const std::vector<std::pair<std::vector<double>, std::vector<double>>>& lineXY,
	const std::vector<Rgba>& lineColors,
	const std::vector<double>& lineWidths,
	const std::vector<std::pair<std::vector<double>, std::vector<double>>>& scatterXY,
	const std::vector<Rgba>& scatterColors,
	const std::vector<double>& scatterRadius,
	const std::string& title,
	const std::string& xlabel,
	const std::string& ylabel) {
	SvgCanvas canvas(widthPx, heightPx);
	// delegate to the render_into overload
	SvgBackend::render_into(canvas, widthPx, heightPx, marginLeft, marginRight, marginTop, marginBottom,
		lineXY, lineColors, lineWidths, scatterXY, scatterColors, scatterRadius, title, xlabel, ylabel);
	return canvas.str();
}

void SvgBackend::render_into(SvgCanvas& canvas,
	int widthPx, int heightPx,
	int marginLeft, int marginRight, int marginTop, int marginBottom,
	const std::vector<std::pair<std::vector<double>, std::vector<double>>>& lineXY,
	const std::vector<Rgba>& lineColors,
	const std::vector<double>& lineWidths,
	const std::vector<std::pair<std::vector<double>, std::vector<double>>>& scatterXY,
	const std::vector<Rgba>& scatterColors,
	const std::vector<double>& scatterRadius,
	const std::string& title,
	const std::string& xlabel,
	const std::string& ylabel) {
	const double left = marginLeft;
	const double right = widthPx - marginRight;
	const double top = marginTop;
	const double bottom = heightPx - marginBottom;

	// Axis box
	canvas.rect(left, top, right - left, bottom - top, "black", 1.0);

	// Data bounds
	auto [xmin, xmax, ymin, ymax] = compute_data_bounds(lineXY, scatterXY);

	// Ticks
	auto xticks = nice_ticks(xmin, xmax, 6);
	auto yticks = nice_ticks(ymin, ymax, 6);

	for (double xv : xticks) {
		double x = map_x(xv, xmin, xmax, left, right);
		canvas.line(x, bottom, x, bottom + 6, "black", 1.0);
		canvas.text(x, bottom + 20, fmt_num(xv), "black", 12, "middle");
	}
	for (double yv : yticks) {
		double y = map_y(yv, ymin, ymax, top, bottom);
		canvas.line(left - 6, y, left, y, "black", 1.0);
		canvas.text(left - 10, y + 4, fmt_num(yv), "black", 12, "end");
	}

	// Title and labels
	if (!title.empty()) {
		canvas.text((left + right) * 0.5, marginTop * 0.7, title, "black", 14, "middle");
	}
	if (!xlabel.empty()) {
		canvas.text((left + right) * 0.5, bottom + 40, xlabel, "black", 13, "middle");
	}
	if (!ylabel.empty()) {
		canvas.text(20, (top + bottom) * 0.5, ylabel, "black", 13, "middle", -90.0);
	}

	// Lines
	for (size_t i = 0; i < lineXY.size(); ++i) {
		const auto& xs = lineXY[i].first;
		const auto& ys = lineXY[i].second;
		std::ostringstream pts;
		bool first = true;
		for (size_t k = 0; k < xs.size() && k < ys.size(); ++k) {
			double X = map_x(xs[k], xmin, xmax, left, right);
			double Y = map_y(ys[k], ymin, ymax, top, bottom);
			if (!first) pts << " ";
			pts << X << "," << Y;
			first = false;
		}
		canvas.polyline(pts.str(), rgba_to_css(lineColors[i]), lineWidths[i]);
	}

	// Scatter
	for (size_t i = 0; i < scatterXY.size(); ++i) {
		const auto& xs = scatterXY[i].first;
		const auto& ys = scatterXY[i].second;
		for (size_t k = 0; k < xs.size() && k < ys.size(); ++k) {
			double X = map_x(xs[k], xmin, xmax, left, right);
			double Y = map_y(ys[k], ymin, ymax, top, bottom);
			canvas.circle(X, Y, scatterRadius[i], rgba_to_css(scatterColors[i]));
		}
	}
}

} // namespace catplot


