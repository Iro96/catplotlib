#define _USE_MATH_DEFINES
#include "catplot/catplot.hpp"
#include "numbits/numbits.hpp"
#include <cmath>
#include <vector>
#include <iostream>

using namespace catplot;
using namespace numbits;
using namespace std;

int main() {
    
    cout << "=== NumBits Integration Example with catplot ===" << endl;
    cout << "This example demonstrates how to use NumBits arrays directly with catplot" << endl;
    cout << "for plotting, showing both old (vector) and new (NumBits) approaches." << endl << endl;

    // 1. Traditional vector approach
    cout << "1. Creating data with traditional vector approach..." << endl;
    
    vector<double> x_vector, y_sin_vector, y_cos_vector;
    
    for (int i = 0; i <= 100; ++i) {
        double t = i * 0.0628318530718; // ~ 2*pi/100
        x_vector.push_back(t);
        y_sin_vector.push_back(sin(t));
        y_cos_vector.push_back(cos(t));
    }
    
    // Additional scatter data
    vector<double> scatter_x = {0.0, 1.57, 3.14, 4.71, 6.28};
    vector<double> scatter_y = {0.0, 1.0, 0.0, -1.0, 0.0};

    cout << "   ✓ Created vector data: " << x_vector.size() << " points" << endl;

    // 2. Modern NumBits array approach
    cout << "2. Creating equivalent data with NumBits arrays..." << endl;
    
    // Create NumBits arrays using zeros and fill them
    ndarrayd x_numbits = ndarrayd::zeros({101});
    ndarrayd y_sin_numbits = ndarrayd::zeros({101});
    ndarrayd y_cos_numbits = ndarrayd::zeros({101});
    
    // Fill NumBits arrays with the same data
    for (int i = 0; i <= 100; ++i) {
        double t = i * 0.0628318530718; // ~ 2*pi/100
        x_numbits[i] = t;
        y_sin_numbits[i] = sin(t);
        y_cos_numbits[i] = cos(t);
    }
    
    // Create scatter data as NumBits arrays
    ndarrayd scatter_x_numbits = ndarrayd::zeros({5});
    ndarrayd scatter_y_numbits = ndarrayd::zeros({5});
    
    for (size_t i = 0; i < 5; ++i) {
        scatter_x_numbits[i] = scatter_x[i];
        scatter_y_numbits[i] = scatter_y[i];
    }
    
    cout << "   ✓ Created NumBits arrays: " << x_numbits.size() << " points" << endl;
    cout << "   ✓ NumBits array shape: [" << x_numbits.shape()[0] << "]" << endl;
    cout << "   ✓ NumBits array dimensions: " << x_numbits.ndim() << "D" << endl;

    // 3. First plot - vector approach
    cout << "3. Creating first plot using vector approach..." << endl;
    
    Figure fig1(800, 600);
    auto& ax1 = fig1.axes();
    
    // Traditional plotting with vector
    ax1.plot(x_vector, y_sin_vector, Rgba::Blue(), 2.0, "sin (vector)");
    ax1.plot(x_vector, y_cos_vector, Rgba::Red(), 2.0, "cos (vector)");
    ax1.scatter(scatter_x, scatter_y, 6.0, Rgba::Green(), "key points (vector)");
    
    ax1.grid(true);
    ax1.legend(true);
    ax1.set_title("catplot: Traditional vector approach");
    ax1.set_xlabel("x (radians)");
    ax1.set_ylabel("f(x)");
    
    fig1.save("simple_vector_approach.svg");
    cout << "   ✓ Saved: simple_vector_approach.svg" << endl;

    // 4. Second plot - NumBits array approach
    cout << "4. Creating second plot using NumBits array approach..." << endl;
    
    Figure fig2(800, 600);
    auto& ax2 = fig2.axes();
    
    // Modern plotting with NumBits arrays
    ax2.plot(x_numbits, y_sin_numbits, Rgba::Blue(), 2.0, "sin (NumBits)");
    ax2.plot(x_numbits, y_cos_numbits, Rgba::Red(), 2.0, "cos (NumBits)");
    ax2.scatter(scatter_x_numbits, scatter_y_numbits, 6.0, Rgba::Green(), "key points (NumBits)");
    
    ax2.grid(true);
    ax2.legend(true);
    ax2.set_title("catplot: Modern NumBits array approach");
    ax2.set_xlabel("x (radians)");
    ax2.set_ylabel("f(x)");
    
    fig2.save("simple_numbits_approach.svg");
    cout << "   ✓ Saved: simple_numbits_approach.svg" << endl;

    // 5. Combined comparison plot
    cout << "5. Creating combined comparison plot..." << endl;
    
    Figure fig3(1000, 700);
    auto& main_ax = fig3.axes();
    
    // Plot both approaches on the same axes to show they're identical
    main_ax.plot(x_vector, y_sin_vector, Rgba::Blue(), 1.5, "sin (vector)");
    main_ax.plot(x_numbits, y_sin_numbits, Rgba::Blue(), 1.5, "sin (NumBits)");
    main_ax.plot(x_vector, y_cos_vector, Rgba::Red(), 1.5, "cos (vector)");
    main_ax.plot(x_numbits, y_cos_numbits, Rgba::Red(), 1.5, "cos (NumBits)");
    main_ax.scatter(scatter_x, scatter_y, 8.0, Rgba::Green(), "key points");
    
    main_ax.grid(true);
    main_ax.legend(true);
    main_ax.set_title("NumBits Integration: Both approaches produce identical results");
    main_ax.set_xlabel("x (radians)");
    main_ax.set_ylabel("f(x)");
    
    fig3.save("simple_combined_comparison.svg");
    cout << "   ✓ Saved: simple_combined_comparison.svg" << endl;

    // 6. Subplot demonstration with NumBits
    cout << "6. Creating subplot demonstration with NumBits arrays..." << endl;
    
    Figure fig4(1000, 800);
    auto& sub_ax1 = fig4.subplot(2, 2, 1);
    auto& sub_ax2 = fig4.subplot(2, 2, 2);
    auto& sub_ax3 = fig4.subplot(2, 2, 3);
    auto& sub_ax4 = fig4.subplot(2, 2, 4);
    
    // Create different mathematical functions with NumBits
    ndarrayd x_sub = ndarrayd::zeros({50});
    ndarrayd y_tan = ndarrayd::zeros({50});
    ndarrayd y_exp = ndarrayd::zeros({50});
    ndarrayd y_sqrt = ndarrayd::zeros({50});
    ndarrayd y_poly = ndarrayd::zeros({50});
    
    // Fill the arrays with mathematical functions
    for (size_t i = 0; i < x_sub.size(); ++i) {
        double x_val = 4.0 * M_PI * static_cast<double>(i) / 49.0; // 0 to 4π
        x_sub[i] = x_val;
        y_tan[i] = tan(x_val) * 0.1; // scaled tan for visibility
        y_exp[i] = exp(-x_val * 0.1) * sin(x_val);
        y_sqrt[i] = sqrt(abs(x_val));
        y_poly[i] = 0.01 * x_val * x_val - 0.1 * x_val + 1.0;
    }
    
    // Use NumBits arrays directly in subplots
    sub_ax1.plot(x_sub, y_tan, Rgba::Blue(), 2.0, "tan(x)");
    sub_ax1.legend(true); sub_ax1.grid(true); sub_ax1.set_title("Tan function (scaled)");
    
    sub_ax2.plot(x_sub, y_exp, Rgba::Red(), 2.0, "exp(-x)*sin(x)");
    sub_ax2.legend(true); sub_ax2.grid(true); sub_ax2.set_title("Damped oscillation");
    
    sub_ax3.scatter(x_sub, y_sqrt, 4.0, Rgba::Green(), "sqrt(x)");
    sub_ax3.legend(true); sub_ax3.grid(true); sub_ax3.set_title("Square root (scatter)");
    
    sub_ax4.plot(x_sub, y_poly, Rgba::Black(), 2.0, "quadratic");
    sub_ax4.legend(true); sub_ax4.grid(true); sub_ax4.set_title("Polynomial");
    
    fig4.save("simple_numbits_subplots.svg");
    cout << "   ✓ Saved: simple_numbits_subplots.svg" << endl;

    // 7. Benefits demonstration
    cout << "7. Demonstrating NumBits benefits..." << endl;
    
    // Show some NumBits-specific advantages
    cout << "   • NumBits arrays have shape information: [" << x_numbits.shape()[0] << "]" << endl;
    cout << "   • NumBits arrays support iterator access: ";
    double sum_check = 0.0;
    for (const auto& val : x_numbits) {
        sum_check += val;
    }
    cout << "sum = " << sum_check << endl;
    
    cout << "   • NumBits arrays support direct memory access: ";
    cout << "data() pointer = " << static_cast<void*>(const_cast<double*>(x_numbits.data())) << endl;
    
    // Create a view (NumBits feature)
    auto x_view = x_numbits.create_view({25}, {4}, const_cast<double*>(x_numbits.data()) + 10);
    cout << "   • NumBits supports array views: shape [" << x_view.shape()[0] << "]" << endl;

    // Final summary
    cout << endl;
    cout << " Summary " << endl;
    cout << "✓ Successfully demonstrated NumBits integration with catplot" << endl;
    cout << "✓ Generated 4 SVG files showing different approaches" << endl;
    cout << "✓ Both vector and NumBits approaches produce identical results" << endl;
    cout << "✓ NumBits provides additional features like shape info, views, and iterators" << endl;
    cout << "✓ Seamless integration - no manual conversion needed!" << endl;
    cout << endl;
    cout << "Generated files:" << endl;
    cout << "  - simple_vector_approach.svg (traditional approach)" << endl;
    cout << "  - simple_numbits_approach.svg (NumBits approach)" << endl;
    cout << "  - simple_combined_comparison.svg (comparison)" << endl;
    cout << "  - simple_numbits_subplots.svg (advanced NumBits usage)" << endl;

    return 0;
}
