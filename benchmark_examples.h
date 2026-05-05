#pragma once

// This file demonstrates how to use the benchmarking system
// for comparing different drawing algorithms

#include "class/benchmark_manager.h"
#include "class/accuracy_calculator.h"
#include "class/renderer.h"

// Line algorithms
#include "class/line_dda.h"
#include "class/line_bresenham.h"
#include "class/line_parametric.h"

// Circle algorithms
#include "class/circle_midpoint.h"
#include "class/circle_parametric.h"

// Ellipse algorithms
#include "class/ellipse_midpoint.h"
#include "class/ellipse_parametric.h"

// Parabola algorithms
#include "class/parabola_midpoint.h"
#include "class/parabola_parametric.h"

// Hyperbola algorithms
#include "class/hyperbola_midpoint.h"
#include "class/hyperbola_parametric.h"

#include <vector>
#include <memory>

// Example 1: Benchmark Line Drawing Algorithms
inline void benchmarkLineAlgorithms() {
    BenchmarkManager manager;
    Renderer renderer;
    
    // Create test line (10, 10) to (200, 150)
    LineDDA lineDDA(10, 10, 200, 150);
    LineBresenham lineBresenham(10, 10, 200, 150);
    LineParametric lineParametric(10, 10, 200, 150);
    
    // Time benchmarking (1000 iterations each)
    std::vector<BenchmarkManager::TimeResult> timeResults;
    timeResults.push_back(manager.measureTime(&lineDDA, renderer, 1000));
    timeResults.push_back(manager.measureTime(&lineBresenham, renderer, 1000));
    timeResults.push_back(manager.measureTime(&lineParametric, renderer, 1000));
    
    // Strategy B: Distance-based accuracy
    LineAccuracyCalculator lineCalc(10, 10, 200, 150);
    std::vector<BenchmarkManager::AccuracyResult> accuracyResults;
    accuracyResults.push_back(manager.measureAccuracy(&lineDDA, renderer, lineCalc));
    accuracyResults.push_back(manager.measureAccuracy(&lineBresenham, renderer, lineCalc));
    accuracyResults.push_back(manager.measureAccuracy(&lineParametric, renderer, lineCalc));
    
    // Strategy A: Pixel-set comparison (compare DDA and Bresenham with Parametric as reference)
    std::vector<BenchmarkManager::PixelSetComparisonResult> pixelSetResults;
    pixelSetResults.push_back(manager.comparePixelSets(&lineDDA, &lineParametric, renderer));
    pixelSetResults.push_back(manager.comparePixelSets(&lineBresenham, &lineParametric, renderer));
    
    // Generate comprehensive report
    manager.generateFullReport(timeResults, accuracyResults, pixelSetResults);
    
    // Export to CSV
    manager.exportToCSV("line_benchmark.csv", timeResults, accuracyResults);
}

// Example 2: Benchmark Circle Drawing Algorithms
inline void benchmarkCircleAlgorithms() {
    BenchmarkManager manager;
    Renderer renderer;
    
    // Create test circle at (100, 100) with radius 50
    CircleMidpoint circleMidpoint(100, 100, 50);
    CircleParametric circleParametric(100, 100, 50);
    
    // Time benchmarking
    std::vector<BenchmarkManager::TimeResult> timeResults;
    timeResults.push_back(manager.measureTime(&circleMidpoint, renderer, 1000));
    timeResults.push_back(manager.measureTime(&circleParametric, renderer, 1000));
    
    // Strategy B: Distance-based accuracy
    CircleAccuracyCalculator circleCalc(100, 100, 50);
    std::vector<BenchmarkManager::AccuracyResult> accuracyResults;
    accuracyResults.push_back(manager.measureAccuracy(&circleMidpoint, renderer, circleCalc));
    accuracyResults.push_back(manager.measureAccuracy(&circleParametric, renderer, circleCalc));
    
    // Strategy A: Pixel-set comparison
    std::vector<BenchmarkManager::PixelSetComparisonResult> pixelSetResults;
    pixelSetResults.push_back(manager.comparePixelSets(&circleMidpoint, &circleParametric, renderer));
    
    // Generate report
    manager.generateFullReport(timeResults, accuracyResults, pixelSetResults);
    manager.exportToCSV("circle_benchmark.csv", timeResults, accuracyResults);
}

// Example 3: Benchmark Ellipse Drawing Algorithms
inline void benchmarkEllipseAlgorithms() {
    BenchmarkManager manager;
    Renderer renderer;
    
    // Create test ellipse at (100, 100) with rx=80, ry=50
    EllipseMidpoint ellipseMidpoint(100, 100, 80, 50);
    EllipseParametric ellipseParametric(100, 100, 80, 50);
    
    // Time benchmarking
    std::vector<BenchmarkManager::TimeResult> timeResults;
    timeResults.push_back(manager.measureTime(&ellipseMidpoint, renderer, 1000));
    timeResults.push_back(manager.measureTime(&ellipseParametric, renderer, 1000));
    
    // Strategy B: Accuracy
    EllipseAccuracyCalculator ellipseCalc(100, 100, 80, 50);
    std::vector<BenchmarkManager::AccuracyResult> accuracyResults;
    accuracyResults.push_back(manager.measureAccuracy(&ellipseMidpoint, renderer, ellipseCalc));
    accuracyResults.push_back(manager.measureAccuracy(&ellipseParametric, renderer, ellipseCalc));
    
    // Strategy A: Comparison
    std::vector<BenchmarkManager::PixelSetComparisonResult> pixelSetResults;
    pixelSetResults.push_back(manager.comparePixelSets(&ellipseMidpoint, &ellipseParametric, renderer));
    
    manager.generateFullReport(timeResults, accuracyResults, pixelSetResults);
    manager.exportToCSV("ellipse_benchmark.csv", timeResults, accuracyResults);
}

// Example 4: Benchmark All Algorithms
inline void benchmarkAllShapes() {
    std::cout << "\n=== LINE ALGORITHMS ===\n";
    benchmarkLineAlgorithms();
    
    std::cout << "\n=== CIRCLE ALGORITHMS ===\n";
    benchmarkCircleAlgorithms();
    
    std::cout << "\n=== ELLIPSE ALGORITHMS ===\n";
    benchmarkEllipseAlgorithms();
}
