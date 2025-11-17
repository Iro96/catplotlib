#include "catplot/axes.hpp"
#include "svg_backend.hpp"
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <tuple>

namespace catplot {

Rgba Rgba::Black() { return {0,0,0,1}; }
Rgba Rgba::White() { return {1,1,1,1}; }
Rgba Rgba::Red() { return {1,0,0,1}; }
Rgba Rgba::Green() { return {0,1,0,1}; }
Rgba Rgba::Blue() { return {0,0,1,1}; }

Axes::Axes(int figureWidthPx, int figureHeightPx)
	: widthPx(figureWidthPx), heightPx(figureHeightPx) {}

void Axes::plot(const std::vector<double>& x, const std::vector<double>& y, const Rgba& color, double lineWidthPx, const std::string& label) {
	if (x.size() != y.size()) throw std::invalid_argument("x and y must be same length");
	lines.push_back(LineSeries{ x, y, color, lineWidthPx, label });
}

void Axes::scatter(const std::vector<double>& x, const std::vector<double>& y, double radiusPx, const Rgba& color, const std::string& label) {
	if (x.size() != y.size()) throw std::invalid_argument("x and y must be same length");
	scatters.push_back(ScatterSeries{ x, y, radiusPx, color, label });
}

void Axes::set_title(const std::string& titleText) { title = titleText; }
void Axes::set_xlabel(const std::string& labelText) { xlabel = labelText; }
void Axes::set_ylabel(const std::string& labelText) { ylabel = labelText; }
void Axes::grid(bool enabled) { showGrid = enabled; }
void Axes::legend(bool enabled) { showLegend = enabled; }

std::pair<double, double> Axes::minmax(const std::vector<double>& v) {
	if (v.empty()) return {0.0, 1.0};
	auto [mnIt, mxIt] = std::minmax_element(v.begin(), v.end());
	return {*mnIt, *mxIt};
}

void Axes::expand_range(double& vmin, double& vmax, double expandFrac) {
	double range = vmax - vmin;
	if (range == 0.0) { vmin -= 0.5; vmax += 0.5; return; }
	double pad = range * expandFrac;
	vmin -= pad; vmax += pad;
}

std::string Axes::render_svg() const {
	std::vector<std::pair<std::vector<double>, std::vector<double>>> lineXY;
	std::vector<Rgba> lineColors;
	std::vector<double> lineWidths;
	lineXY.reserve(lines.size());
	lineColors.reserve(lines.size());
	lineWidths.reserve(lines.size());
	for (const auto& s : lines) {
		lineXY.emplace_back(s.x, s.y);
		lineColors.push_back(s.color);
		lineWidths.push_back(s.widthPx);
	}

	std::vector<std::pair<std::vector<double>, std::vector<double>>> scatterXY;
	std::vector<Rgba> scatterColors;
	std::vector<double> scatterRadius;
	scatterXY.reserve(scatters.size());
	scatterColors.reserve(scatters.size());
	scatterRadius.reserve(scatters.size());
	for (const auto& s : scatters) {
		scatterXY.emplace_back(s.x, s.y);
		scatterColors.push_back(s.color);
		scatterRadius.push_back(s.radiusPx);
	}

	return SvgBackend::render(
		widthPx, heightPx,
		marginLeft, marginRight, marginTop, marginBottom,
		lineXY, lineColors, lineWidths,
		scatterXY, scatterColors, scatterRadius,
		title, xlabel, ylabel
	);
}

void Axes::render_to(SvgCanvas& canvas, double x, double y, double w, double h) const {
	// Use a translated group for this axes viewport
	canvas.begin_group_translate(x, y);
	std::vector<std::pair<std::vector<double>, std::vector<double>>> lineXY;
	std::vector<Rgba> lineColors;
	std::vector<double> lineWidths;
	for (const auto& s : lines) {
		lineXY.emplace_back(s.x, s.y);
		lineColors.push_back(s.color);
		lineWidths.push_back(s.widthPx);
	}
	std::vector<std::pair<std::vector<double>, std::vector<double>>> scatterXY;
	std::vector<Rgba> scatterColors;
	std::vector<double> scatterRadius;
	for (const auto& s : scatters) {
		scatterXY.emplace_back(s.x, s.y);
		scatterColors.push_back(s.color);
		scatterRadius.push_back(s.radiusPx);
	}
	SvgBackend::render_into(
		canvas,
		static_cast<int>(w), static_cast<int>(h),
		marginLeft, marginRight, marginTop, marginBottom,
		lineXY, lineColors, lineWidths,
		scatterXY, scatterColors, scatterRadius,
		title, xlabel, ylabel
	);
	// Legend (simple, top-right inside plot area)
	if (showLegend) {
		const double plotLeft = marginLeft;
		const double plotTop = marginTop;
		const double plotRight = w - marginRight;
		double lx = plotRight - 140.0;
		double ly = plotTop + 10.0;
		double entryH = 18.0;
		double boxW = 130.0;
		std::vector<std::pair<std::string, Rgba>> entries;
		for (const auto& s : lines) if (!s.label.empty()) entries.push_back({s.label, s.color});
		for (const auto& s : scatters) if (!s.label.empty()) entries.push_back({s.label, s.color});
		if (!entries.empty()) {
			double boxH = 10.0 + entries.size() * entryH + 6.0;
			canvas.rect(lx, ly, boxW, boxH, "black", 1.0, "rgba(255,255,255,0.8)");
			double ycur = ly + 18.0;
			for (const auto& e : entries) {
				// marker/line
				canvas.line(lx + 10.0, ycur - 6.0, lx + 30.0, ycur - 6.0, rgba_to_css(e.second), 3.0);
				canvas.circle(lx + 20.0, ycur - 6.0, 3.0, rgba_to_css(e.second));
				canvas.text(lx + 40.0, ycur - 2.0, e.first, "black", 12, "start");
				ycur += entryH;
			}
		}
	}
	// Grid: overlay after ticks. Simple vertical and horizontal lines at tick positions
	if (showGrid) {
		// Recompute bounds and ticks like SvgBackend does
		std::vector<std::pair<std::vector<double>, std::vector<double>>> lineXYLocal;
		for (const auto& s : lines) lineXYLocal.emplace_back(s.x, s.y);
		std::vector<std::pair<std::vector<double>, std::vector<double>>> scatterXYLocal;
		for (const auto& s : scatters) scatterXYLocal.emplace_back(s.x, s.y);
		auto bounds = [&]() {
			bool has=false; double xmin=0,xmax=0,ymin=0,ymax=0;
			auto consider=[&](const std::vector<double>& xs,const std::vector<double>& ys){
				for(size_t i=0;i<xs.size()&&i<ys.size();++i){double X=xs[i],Y=ys[i]; if(!has){xmin=xmax=X;ymin=ymax=Y;has=true;} xmin=std::min(xmin,X);xmax=std::max(xmax,X);ymin=std::min(ymin,Y);ymax=std::max(ymax,Y);} };
			for (const auto& p : lineXYLocal) consider(p.first,p.second);
			for (const auto& p : scatterXYLocal) consider(p.first,p.second);
			if (!has){xmin=0;xmax=1;ymin=0;ymax=1;}
			double padX=(xmax-xmin); if (padX==0) { xmin-=0.5; xmax+=0.5; } else { double p=padX*0.05; xmin-=p; xmax+=p; }
			double padY=(ymax-ymin); if (padY==0) { ymin-=0.5; ymax+=0.5; } else { double p=padY*0.05; ymin-=p; ymax+=p; }
			return std::tuple<double,double,double,double>{xmin,xmax,ymin,ymax};
		}();
		double xmin, xmax, ymin, ymax;
		std::tie(xmin, xmax, ymin, ymax) = bounds;
		auto xticks = [&](){
			// reuse simple nice ticks logic
			std::vector<double> ticks;
			double range = xmax - xmin; if (range<=0) range = 1.0;
			double rawStep = range / 6.0;
			double mag = std::pow(10.0, std::floor(std::log10(rawStep)));
			double norm = rawStep / mag;
			double step = norm<1.5?1.0:norm<3.0?2.0:norm<7.0?5.0:10.0; step*=mag;
			double start = std::ceil(xmin/step)*step;
			for (double v=start; v<=xmax+1e-12; v+=step) ticks.push_back(v);
			return ticks;
		}();
		auto yticks = [&](){
			std::vector<double> ticks;
			double range = ymax - ymin; if (range<=0) range = 1.0;
			double rawStep = range / 6.0;
			double mag = std::pow(10.0, std::floor(std::log10(rawStep)));
			double norm = rawStep / mag;
			double step = norm<1.5?1.0:norm<3.0?2.0:norm<7.0?5.0:10.0; step*=mag;
			double start = std::ceil(ymin/step)*step;
			for (double v=start; v<=ymax+1e-12; v+=step) ticks.push_back(v);
			return ticks;
		}();
		auto mapx = [&](double xv) {
			double t = (xv - xmin) / (xmax - xmin);
			double left = marginLeft;
			double right = w - marginRight;
			return left + t * (right - left);
		};
		auto mapy = [&](double yv) {
			double t = (yv - ymin) / (ymax - ymin);
			double top = marginTop;
			double bottom = h - marginBottom;
			return bottom - t * (bottom - top);
		};
		std::string gridColor = "rgba(0,0,0,0.1)";
		for (double xv : xticks) {
			double X = mapx(xv);
			canvas.line(X, marginTop, X, h - marginBottom, gridColor, 1.0, "butt");
		}
		for (double yv : yticks) {
			double Y = mapy(yv);
			canvas.line(marginLeft, Y, w - marginRight, Y, gridColor, 1.0, "butt");
		}
	}
	canvas.end_group();
}

} // namespace catplot


