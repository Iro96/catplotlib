#pragma once

#include <string>
#include <utility>
#include <vector>
#include <stdexcept>

namespace numbits {
	template<typename T> class ndarray;
}

namespace catplot {

struct Rgba {
	double r{0};
	double g{0};
	double b{0};
	double a{1};
	static Rgba Black();
	static Rgba White();
	static Rgba Red();
	static Rgba Green();
	static Rgba Blue();
};

class SvgCanvas; // fwd

class Axes {
public:
	Axes(int figureWidthPx, int figureHeightPx);

	// Plot a line series
	void plot(const std::vector<double>& x, const std::vector<double>& y, const Rgba& color = Rgba::Blue(), double lineWidthPx = 2.0, const std::string& label = "");

	// NumBits array overloads for plot()
	template<typename T>
	void plot(const numbits::ndarray<T>& x, const numbits::ndarray<T>& y, const Rgba& color = Rgba::Blue(), double lineWidthPx = 2.0, const std::string& label = "") {
		// Validate inputs
		if (x.ndim() != 1) {
			throw std::invalid_argument("NumBits array 'x' must be 1D for plotting, got " + std::to_string(x.ndim()) + " dimensions");
		}
		if (y.ndim() != 1) {
			throw std::invalid_argument("NumBits array 'y' must be 1D for plotting, got " + std::to_string(y.ndim()) + " dimensions");
		}
		if (x.size() != y.size()) {
			throw std::invalid_argument("X and Y arrays must have the same size: X has " + std::to_string(x.size()) + " elements, Y has " + std::to_string(y.size()) + " elements");
		}
		if (x.size() == 0) {
			throw std::invalid_argument("Cannot plot empty arrays");
		}
		
		// Extract data using data() and size() methods
		const T* x_data = x.data();
		const T* y_data = y.data();
		size_t n = x.size();
		
		// Convert NumBits arrays to std::vector for internal processing
		std::vector<double> x_vec;
		std::vector<double> y_vec;
		x_vec.reserve(n);
		y_vec.reserve(n);
		
		// Use direct pointer access for better performance
		for (size_t i = 0; i < n; ++i) {
			x_vec.push_back(static_cast<double>(x_data[i]));
			y_vec.push_back(static_cast<double>(y_data[i]));
		}
		
		// Call existing method that works with std::vector<double>
		plot(x_vec, y_vec, color, lineWidthPx, label);
	}

	// Scatter points with circular markers
	void scatter(const std::vector<double>& x, const std::vector<double>& y, double radiusPx = 3.0, const Rgba& color = Rgba::Red(), const std::string& label = "");

	// NumBits array overloads for scatter()
	template<typename T>
	void scatter(const numbits::ndarray<T>& x, const numbits::ndarray<T>& y, double radiusPx = 3.0, const Rgba& color = Rgba::Red(), const std::string& label = "") {
		// Validate inputs
		if (x.ndim() != 1) {
			throw std::invalid_argument("NumBits array 'x' must be 1D for scatter plotting, got " + std::to_string(x.ndim()) + " dimensions");
		}
		if (y.ndim() != 1) {
			throw std::invalid_argument("NumBits array 'y' must be 1D for scatter plotting, got " + std::to_string(y.ndim()) + " dimensions");
		}
		if (x.size() != y.size()) {
			throw std::invalid_argument("X and Y arrays must have the same size: X has " + std::to_string(x.size()) + " elements, Y has " + std::to_string(y.size()) + " elements");
		}
		if (x.size() == 0) {
			throw std::invalid_argument("Cannot plot empty arrays");
		}
		
		// Extract data using data() and size() methods
		const T* x_data = x.data();
		const T* y_data = y.data();
		size_t n = x.size();
		
		// Convert NumBits arrays to std::vector for internal processing
		std::vector<double> x_vec;
		std::vector<double> y_vec;
		x_vec.reserve(n);
		y_vec.reserve(n);
		
		// Use direct pointer access for better performance
		for (size_t i = 0; i < n; ++i) {
			x_vec.push_back(static_cast<double>(x_data[i]));
			y_vec.push_back(static_cast<double>(y_data[i]));
		}
		
		// Call existing method that works with std::vector<double>
		scatter(x_vec, y_vec, radiusPx, color, label);
	}

	// Labels
	void set_title(const std::string& titleText);
	void set_xlabel(const std::string& labelText);
	void set_ylabel(const std::string& labelText);

	// Decorations
	void grid(bool enabled);
	void legend(bool enabled);

	// Save as SVG (called by Figure)
	std::string render_svg() const;

	// Render into an existing SVG canvas region
	void render_to(SvgCanvas& canvas, double x, double y, double w, double h) const;

private:
	struct LineSeries {
		std::vector<double> x;
		std::vector<double> y;
		Rgba color;
		double widthPx;
		std::string label;
	};
	struct ScatterSeries {
		std::vector<double> x;
		std::vector<double> y;
		double radiusPx;
		Rgba color;
		std::string label;
	};

	int widthPx;
	int heightPx;

	// plot content margins
	int marginLeft{70};
	int marginRight{20};
	int marginTop{30};
	int marginBottom{60};

	std::vector<LineSeries> lines;
	std::vector<ScatterSeries> scatters;
	std::string title;
	std::string xlabel;
	std::string ylabel;
	bool showGrid{false};
	bool showLegend{false};

	// helpers
	static std::pair<double, double> minmax(const std::vector<double>& v);
	static void expand_range(double& vmin, double& vmax, double expandFrac);
};

} // namespace catplot


