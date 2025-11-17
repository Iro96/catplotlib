#include "catplot/figure.hpp"
#include "catplot/axes.hpp"
#include "svg_canvas.hpp"
#include <fstream>
#include <stdexcept>

namespace catplot {

Figure::Figure(int width_px, int height_px)
	: widthPx(width_px), heightPx(height_px) {
		ensure_grid(1, 1);
}

Axes& Figure::axes() {
	return *axesGrid[0];
}

const Axes& Figure::axes() const {
	return *axesGrid[0];
}

Axes& Figure::subplot(int nrows, int ncols, int index1based) {
	ensure_grid(nrows, ncols);
	if (index1based < 1 || index1based > nrows * ncols) throw std::out_of_range("subplot index out of range");
	return *axesGrid[static_cast<size_t>(index1based - 1)];
}

int Figure::rows() const { return gridRows; }
int Figure::cols() const { return gridCols; }

int Figure::width() const {
	return widthPx;
}

int Figure::height() const {
	return heightPx;
}

void Figure::save(const std::string& filepath) const {
	SvgCanvas canvas(widthPx, heightPx);
	// layout cells
	double cellW = static_cast<double>(widthPx) / gridRows; // deliberate typical row/col orientation correction below
	double cellH = static_cast<double>(heightPx) / gridCols;
	// Correct: width divided by cols, height by rows
	cellW = static_cast<double>(widthPx) / gridCols;
	cellH = static_cast<double>(heightPx) / gridRows;
	for (int r = 0; r < gridRows; ++r) {
		for (int c = 0; c < gridCols; ++c) {
			size_t idx = static_cast<size_t>(r * gridCols + c);
			double x = c * cellW;
			double y = r * cellH;
			axesGrid[idx]->render_to(canvas, x, y, cellW, cellH);
		}
	}
	std::ofstream ofs(filepath, std::ios::binary);
	ofs << canvas.str();
}

void Figure::save_png(const std::string& filepath) const {
	throw std::runtime_error("PNG export not implemented yet. Planned: raster backend integration.");
}

void Figure::ensure_grid(int r, int c) {
	if (r == gridRows && c == gridCols && axesGrid.size() == static_cast<size_t>(r * c)) return;
	gridRows = r;
	gridCols = c;
	axesGrid.clear();
	axesGrid.resize(static_cast<size_t>(gridRows * gridCols));
	for (int i = 0; i < gridRows * gridCols; ++i) {
		axesGrid[static_cast<size_t>(i)] = std::make_unique<Axes>(widthPx / gridCols, heightPx / gridRows);
	}
}

} // namespace catplot


