#include "catplot/catplot.hpp"
#include "numbits/numbits.hpp"
#include <cmath>
#include <iostream>

using namespace catplot;
using namespace numbits;
using namespace std;

int main() {
    
    cout << "Testing NumBits array support in catplot..." << endl;
    
    // Test 1: Basic functionality with ndarrayd (double)
    cout << "Test 1: Basic line plot with ndarrayd" << endl;
    Figure fig1(800, 600);
    auto& ax1 = fig1.axes();
    
    // Create NumBits arrays
    ndarrayd x_data = ndarrayd::zeros({21});
    ndarrayd y_data = ndarrayd::zeros({21});
    
    for (int i = 0; i <= 20; ++i) {
        double t = i * 0.314159265359; // ~ pi/10
        x_data[i] = t;
        y_data[i] = sin(t);
    }
    
    // Test plot with NumBits arrays
    ax1.plot(x_data, y_data, Rgba::Blue(), 2.0, "sin");
    ax1.set_title("NumBits Test: Line Plot with ndarrayd");
    ax1.set_xlabel("x");
    ax1.set_ylabel("sin(x)");
    ax1.grid(true);
    ax1.legend(true);
    
    fig1.save("numbits_test1_line.svg");
    cout << "✓ Test 1 passed: Line plot with ndarrayd" << endl;
    
    // Test 2: Scatter plot with ndarrayf (float)
    cout << "Test 2: Scatter plot with ndarrayf" << endl;
    Figure fig2(800, 600);
    auto& ax2 = fig2.axes();
    
    // Create float arrays
    ndarrayf x_float = ndarrayf::zeros({10});
    ndarrayf y_float = ndarrayf::zeros({10});
    
    for (int i = 0; i < 10; ++i) {
        x_float[i] = static_cast<float>(i);
        y_float[i] = static_cast<float>(i * i);
    }
    
    // Test scatter with NumBits arrays
    ax2.scatter(x_float, y_float, 5.0, Rgba::Red(), "quadratic");
    ax2.set_title("NumBits Test: Scatter Plot with ndarrayf");
    ax2.set_xlabel("x");
    ax2.set_ylabel("x²");
    ax2.grid(true);
    ax2.legend(true);
    
    fig2.save("numbits_test2_scatter.svg");
    cout << "✓ Test 2 passed: Scatter plot with ndarrayf" << endl;
    
    // Test 3: Combined plot
    cout << "Test 3: Combined line and scatter plots" << endl;
    Figure fig3(800, 600);
    auto& ax3 = fig3.axes();
    
    // Line plot data
    ndarrayd x_line = ndarrayd::zeros({50});
    ndarrayd y_line = ndarrayd::zeros({50});
    
    for (int i = 0; i < 50; ++i) {
        double t = i * 0.125663706144; // ~ 2*pi/50
        x_line[i] = t;
        y_line[i] = cos(t);
    }
    
    // Scatter points
    ndarrayd x_scatter = ndarrayd::zeros({5});
    ndarrayd y_scatter = ndarrayd::zeros({5});
    
    for (int i = 0; i < 5; ++i) {
        x_scatter[i] = i * 0.785398163397; // pi/4 intervals
        y_scatter[i] = cos(x_scatter[i]);
    }
    
    ax3.plot(x_line, y_line, Rgba::Green(), 1.5, "cosine");
    ax3.scatter(x_scatter, y_scatter, 4.0, Rgba::Red(), "samples");
    ax3.set_title("NumBits Test: Combined Plot");
    ax3.set_xlabel("x");
    ax3.set_ylabel("cos(x)");
    ax3.grid(true);
    ax3.legend(true);
    
    fig3.save("numbits_test3_combined.svg");
    cout << "✓ Test 3 passed: Combined plot" << endl;
    
    // Test 4: Error handling - dimension mismatch
    cout << "Test 4: Error handling for invalid inputs" << endl;
    try {
        ndarrayd x_1d = ndarrayd::zeros({5});
        ndarrayd y_2d = ndarrayd::zeros({2, 3}); // 2D array
        
        Figure fig4(800, 600);
        auto& ax4 = fig4.axes();
        ax4.plot(x_1d, y_2d);
        cout << "✗ Test 4 failed: Should have thrown exception for 2D array" << endl;
        return 1;
    } catch (const invalid_argument& e) {
        cout << "✓ Test 4 passed: Correctly caught 2D array error: " << e.what() << endl;
    }
    
    // Test 5: Error handling - size mismatch
    try {
        ndarrayd x_5 = ndarrayd::zeros({5});
        ndarrayd y_3 = ndarrayd::zeros({3});
        
        Figure fig5(800, 600);
        auto& ax5 = fig5.axes();
        ax5.plot(x_5, y_3);
        cout << "✗ Test 5 failed: Should have thrown exception for size mismatch" << endl;
        return 1;
    } catch (const invalid_argument& e) {
        cout << "✓ Test 5 passed: Correctly caught size mismatch error: " << e.what() << endl;
    }
    
    // Test 6: Error handling - empty arrays
    try {
        ndarrayd x_empty = ndarrayd::zeros({0});
        ndarrayd y_empty = ndarrayd::zeros({0});
        
        Figure fig6(800, 600);
        auto& ax6 = fig6.axes();
        ax6.plot(x_empty, y_empty);
        cout << "✗ Test 6 failed: Should have thrown exception for empty arrays" << endl;
        return 1;
    } catch (const invalid_argument& e) {
        cout << "✓ Test 6 passed: Correctly caught empty array error: " << e.what() << endl;
    }
    
    cout << "\nAll tests passed! NumBits array support is working correctly." << endl;
    cout << "Generated test files:" << endl;
    cout << "- numbits_test1_line.svg" << endl;
    cout << "- numbits_test2_scatter.svg" << endl;
    cout << "- numbits_test3_combined.svg" << endl;
    
    return 0;
}