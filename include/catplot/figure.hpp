#pragma once

#include <memory>
#include <string>
#include <vector>

namespace catplot {

class Axes;

class Figure {
public:
	Figure(int width_px = 800, int height_px = 600);

	Axes& axes();
	const Axes& axes() const;
	// Create or access subplots (1-based index like matplotlib)
	Axes& subplot(int nrows, int ncols, int index1based);
	int rows() const;
	int cols() const;

	int width() const;
	int height() const;

	// Save the figure to an SVG file
	void save(const std::string& filepath) const;
	// PNG export stub (to be implemented with a raster backend)
	void save_png(const std::string& filepath) const;

private:
	int widthPx;
	int heightPx;
	int gridRows{1};
	int gridCols{1};
	std::vector<std::unique_ptr<Axes>> axesGrid; // size = gridRows*gridCols
	void ensure_grid(int r, int c);
};

} // namespace catplot


