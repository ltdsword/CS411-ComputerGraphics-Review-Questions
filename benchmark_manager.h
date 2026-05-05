#pragma once

#include "object2d.h"
#include "renderer.h"
#include "accuracy_calculator.h"
#include <string>
#include <vector>
#include <set>
#include <utility>

class BenchmarkManager {
public:
    // Time measurement result
    struct TimeResult {
        std::string algorithmName;
        double executionTime_us;  // in microseconds
        int pixelCount;
        int iterations;
    };
    
    // Strategy B: Distance-based accuracy result
    struct AccuracyResult {
        std::string algorithmName;
        double avgError;          // Average distance to ideal curve (pixels)
        double maxError;          // Maximum distance to ideal curve (pixels)
        int pixelCount;
    };
    
    // Strategy A: Pixel-set comparison result
    struct PixelSetComparisonResult {
        std::string algorithm1Name;
        std::string algorithm2Name;
        int pixelCount1;
        int pixelCount2;
        int matchingPixels;       // |S1 ∩ S2|
        int mismatchCount;        // |S1 \ S2| + |S2 \ S1|
        double coverageRatio;     // |S1 ∩ S2| / |S2| (comparing algo1 to algo2 as reference)
        double jaccardIndex;      // |S1 ∪ S2| / |S1 ∪ S2| (similarity measure)
    };
    
    // Unified benchmark result (for file  I/O requirements)
    struct BenchmarkResult {
        std::string objectDescription;
        std::string customAlgoName;
        std::string parametricAlgoName;
        double customTime_ms;
        double parametricTime_ms;
        double customAvgError;
        double parametricAvgError;
        int customPixelCount;
        int parametricPixelCount;
    };
    
    BenchmarkManager();
    
    // Measure execution time
    TimeResult measureTime(Object2D* object, Renderer& renderer, int iterations);
    
    // Strategy B: Measure accuracy using distance to ideal curve
    AccuracyResult measureAccuracy(Object2D* object, Renderer& renderer, 
                                   const AccuracyCalculator& calculator);
    
    // Strategy A: Compare pixel sets between two algorithms
    PixelSetComparisonResult comparePixelSets(Object2D* algorithm1, 
                                              Object2D* algorithm2,
                                              Renderer& renderer);
    
    // NEW: Unified benchmark for single object (custom vs parametric)
    BenchmarkResult benchmarkObject(Object2D* customAlgo, 
                                   Object2D* parametricAlgo,
                                   Renderer& renderer,
                                   const AccuracyCalculator& calculator,
                                   const std::string& objectDescription,
                                   int iterations = 10000);
    
    // Generate console report for time results
    void printTimeReport(const std::vector<TimeResult>& results);
    
    // Generate console report for accuracy results (Strategy B)
    void printAccuracyReport(const std::vector<AccuracyResult>& results);
    
    // Generate console report for pixel-set comparison (Strategy A)
    void printPixelSetReport(const std::vector<PixelSetComparisonResult>& results);
    
    // Generate comprehensive report (both strategies)
    void generateFullReport(const std::vector<TimeResult>& timeResults,
                           const std::vector<AccuracyResult>& accuracyResults,
                           const std::vector<PixelSetComparisonResult>& pixelSetResults);
    
    // NEW: Console output (execution time in milliseconds)
    void printConsoleOutput(const std::vector<BenchmarkResult>& results);
    
    // NEW: File output (detailed comparison to text file)
    void writeOutputFile(const std::string& filename, const std::vector<BenchmarkResult>& results);
    
    // Export results to CSV file (legacy)
    void exportToCSV(const std::string& filename,
                     const std::vector<TimeResult>& timeResults,
                     const std::vector<AccuracyResult>& accuracyResults);
};
