#pragma once

#include <sstream>
#include <string>

namespace catplot {

class SvgCanvas {
public:
	SvgCanvas(int widthPx, int heightPx)
		: width(widthPx), height(heightPx) {
		ss << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << width << "\" height=\"" << height << "\" viewBox=\"0 0 " << width << " " << height << "\">";
		ss << "<rect x=\"0\" y=\"0\" width=\"" << width << "\" height=\"" << height << "\" fill=\"white\"/>";
	}

	void line(double x1, double y1, double x2, double y2, const std::string& stroke, double strokeWidth, const std::string& linecap = "round") {
		ss << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2
		   << "\" stroke=\"" << stroke << "\" stroke-width=\"" << strokeWidth << "\" stroke-linecap=\"" << linecap << "\" fill=\"none\"/>";
	}

	void polyline(const std::string& points, const std::string& stroke, double strokeWidth) {
		ss << "<polyline points=\"" << points << "\" stroke=\"" << stroke << "\" stroke-width=\"" << strokeWidth << "\" fill=\"none\" stroke-linecap=\"round\" stroke-linejoin=\"round\"/>";
	}

	void circle(double cx, double cy, double r, const std::string& fill) {
		ss << "<circle cx=\"" << cx << "\" cy=\"" << cy << "\" r=\"" << r << "\" fill=\"" << fill << "\"/>";
	}

	void text(double x, double y, const std::string& content, const std::string& fill = "black", int fontSize = 12, const std::string& anchor = "start", double rotateDeg = 0.0) {
		ss << "<text x=\"" << x << "\" y=\"" << y << "\" fill=\"" << fill << "\" font-family=\"sans-serif\" font-size=\"" << fontSize << "\" text-anchor=\"" << anchor << "\"";
		if (rotateDeg != 0.0) {
			ss << " transform=\"rotate(" << rotateDeg << " " << x << " " << y << ")\"";
		}
		ss << ">" << escape(content) << "</text>";
	}

	void rect(double x, double y, double w, double h, const std::string& stroke, double strokeWidth, const std::string& fill = "none") {
		ss << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << w << "\" height=\"" << h << "\" stroke=\"" << stroke
		   << "\" stroke-width=\"" << strokeWidth << "\" fill=\"" << fill << "\"/>";
	}

	void begin_group_translate(double tx, double ty) {
		ss << "<g transform=\"translate(" << tx << "," << ty << ")\">";
	}
	void end_group() {
		ss << "</g>";
	}

	std::string str() {
		std::ostringstream out;
		out << ss.str() << "</svg>";
		return out.str();
	}

private:
	int width;
	int height;
	std::ostringstream ss;

	static std::string escape(const std::string& s) {
		std::string out;
		out.reserve(s.size());
		for (char c : s) {
			switch (c) {
			case '&': out += "&amp;"; break;
			case '<': out += "&lt;"; break;
			case '>': out += "&gt;"; break;
			case '\"': out += "&quot;"; break;
			case '\'': out += "&apos;"; break;
			default: out += c; break;
			}
		}
		return out;
	}
};

} // namespace catplot


