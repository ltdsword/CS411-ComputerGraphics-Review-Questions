#include "class/benchmark_manager.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <cmath>

BenchmarkManager::BenchmarkManager() {}

// ============================================================================
// Time Measurement
// ============================================================================

BenchmarkManager::TimeResult BenchmarkManager::measureTime(
    Object2D* object, Renderer& renderer, int iterations) {
    
    TimeResult result;
    result.algorithmName = object->name();
    result.iterations = iterations;
    
    // Clear any previous pixels
    renderer.clearRecordedPixels();
    
    // Measure execution time
    auto start = std::chrono::high_resolution_clock::now();
    
    object->benchmark(renderer, iterations);
    
    auto end = std::chrono::high_resolution_clock::now();
    
    // Calculate time in microseconds
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    result.executionTime_us = static_cast<double>(duration.count());
    
    // Get pixel count (from single draw)
    renderer.clearRecordedPixels();
    object->draw(renderer);
    result.pixelCount = renderer.getRecordedPixels().size();
    
    return result;
}

// ============================================================================
// Strategy B: Distance-Based Accuracy Measurement
// ============================================================================

BenchmarkManager::AccuracyResult BenchmarkManager::measureAccuracy(
    Object2D* object, Renderer& renderer, const AccuracyCalculator& calculator) {
    
    AccuracyResult result;
    result.algorithmName = object->name();
    
    // Clear and draw
    renderer.clearRecordedPixels();
    object->draw(renderer);
    
    const auto& pixels = renderer.getRecordedPixels();
    result.pixelCount = pixels.size();
    
    if (pixels.empty()) {
        result.avgError = 0.0;
        result.maxError = 0.0;
        return result;
    }
    
    // Calculate distance for each pixel
    double totalError = 0.0;
    double maxError = 0.0;
    
    for (const auto& pixel : pixels) {
        double distance = calculator.computeDistance(pixel.first, pixel.second);
        totalError += distance;
        maxError = std::max(maxError, distance);
    }
    
    result.avgError = totalError / pixels.size();
    result.maxError = maxError;
    
    return result;
}

// ============================================================================
// Strategy A: Pixel-Set Comparison
// ============================================================================

BenchmarkManager::PixelSetComparisonResult BenchmarkManager::comparePixelSets(
    Object2D* algorithm1, Object2D* algorithm2, Renderer& renderer) {
    
    PixelSetComparisonResult result;
    result.algorithm1Name = algorithm1->name();
    result.algorithm2Name = algorithm2->name();
    
    // Draw first algorithm
    renderer.clearRecordedPixels();
    algorithm1->draw(renderer);
    std::set<std::pair<int,int>> pixels1 = renderer.getRecordedPixels();
    result.pixelCount1 = pixels1.size();
    
    // Draw second algorithm (reference)
    renderer.clearRecordedPixels();
    algorithm2->draw(renderer);
    std::set<std::pair<int,int>> pixels2 = renderer.getRecordedPixels();
    result.pixelCount2 = pixels2.size();
    
    // Calculate intersection: S1 ∩ S2
    std::set<std::pair<int,int>> intersection;
    std::set_intersection(
        pixels1.begin(), pixels1.end(),
        pixels2.begin(), pixels2.end(),
        std::inserter(intersection, intersection.begin())
    );
    result.matchingPixels = intersection.size();
    
    // Calculate union: S1 ∪ S2
    std::set<std::pair<int,int>> unionSet;
    std::set_union(
        pixels1.begin(), pixels1.end(),
        pixels2.begin(), pixels2.end(),
        std::inserter(unionSet, unionSet.begin())
    );
    
    // Calculate metrics
    // Mismatch count: |S1 \ S2| + |S2 \ S1| = |S1| + |S2| - 2|S1 ∩ S2|
    result.mismatchCount = result.pixelCount1 + result.pixelCount2 - 2 * result.matchingPixels;
    
    // Coverage ratio: |S1 ∩ S2| / |S2| (comparing algo1 to algo2 as reference)
    result.coverageRatio = pixels2.empty() ? 0.0 : 
                           static_cast<double>(result.matchingPixels) / pixels2.size();
    
    // Jaccard Index: |S1 ∩ S2| / |S1 ∪ S2| (similarity measure)
    result.jaccardIndex = unionSet.empty() ? 0.0 :
                          static_cast<double>(result.matchingPixels) / unionSet.size();
    
    return result;
}

// ============================================================================
// Report Generation
// ============================================================================

void BenchmarkManager::printTimeReport(const std::vector<TimeResult>& results) {
    if (results.empty()) return;
    
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    TIME BENCHMARK RESULTS                          ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Algorithm           │ Time (μs) │ Time/Iter │ Pixels │ Iter      ║\n";
    std::cout << "╠═════════════════════╪═══════════╪═══════════╪════════╪═══════════╣\n";
    
    for (const auto& result : results) {
        double timePerIter = result.executionTime_us / result.iterations;
        std::cout << "║ " << std::left << std::setw(19) << result.algorithmName
                  << " │ " << std::right << std::setw(9) << std::fixed << std::setprecision(2) 
                  << result.executionTime_us
                  << " │ " << std::setw(9) << timePerIter
                  << " │ " << std::setw(6) << result.pixelCount
                  << " │ " << std::setw(9) << result.iterations << " ║\n";
    }
    
    std::cout << "╚═════════════════════╧═══════════╧═══════════╧════════╧═══════════╝\n";
    
    // Find fastest
    auto fastest = std::min_element(results.begin(), results.end(),
        [](const TimeResult& a, const TimeResult& b) {
            return a.executionTime_us < b.executionTime_us;
        });
    
    std::cout << "\n✓ FASTEST: " << fastest->algorithmName 
              << " (" << std::fixed << std::setprecision(2) 
              << fastest->executionTime_us << " μs)\n";
}

void BenchmarkManager::printAccuracyReport(const std::vector<AccuracyResult>& results) {
    if (results.empty()) return;
    
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║          STRATEGY B: DISTANCE-BASED ACCURACY RESULTS              ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Algorithm           │ Avg Error (px) │ Max Error (px) │ Pixels   ║\n";
    std::cout << "╠═════════════════════╪════════════════╪════════════════╪══════════╣\n";
    
    for (const auto& result : results) {
        std::cout << "║ " << std::left << std::setw(19) << result.algorithmName
                  << " │ " << std::right << std::setw(14) << std::fixed << std::setprecision(4)
                  << result.avgError
                  << " │ " << std::setw(14) << result.maxError
                  << " │ " << std::setw(8) << result.pixelCount << " ║\n";
    }
    
    std::cout << "╚═════════════════════╧════════════════╧════════════════╧══════════╝\n";
    
    // Find most accurate (lowest average error)
    auto mostAccurate = std::min_element(results.begin(), results.end(),
        [](const AccuracyResult& a, const AccuracyResult& b) {
            return a.avgError < b.avgError;
        });
    
    std::cout << "\n✓ MOST ACCURATE: " << mostAccurate->algorithmName
              << " (Avg: " << std::fixed << std::setprecision(4) 
              << mostAccurate->avgError << " px)\n";
}

void BenchmarkManager::printPixelSetReport(const std::vector<PixelSetComparisonResult>& results) {
    if (results.empty()) return;
    
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║            STRATEGY A: PIXEL-SET COMPARISON RESULTS                          ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Algorithm vs Reference    │ Mismatch │ Coverage │ Jaccard │ Pixels (A/R)   ║\n";
    std::cout << "╠═══════════════════════════╪══════════╪══════════╪═════════╪════════════════╣\n";
    
    for (const auto& result : results) {
        std::string comparison = result.algorithm1Name + " vs " + result.algorithm2Name;
        if (comparison.length() > 25) {
            comparison = comparison.substr(0, 22) + "...";
        }
        
        std::cout << "║ " << std::left << std::setw(25) << comparison
                  << " │ " << std::right << std::setw(8) << result.mismatchCount
                  << " │ " << std::setw(8) << std::fixed << std::setprecision(2) 
                  << (result.coverageRatio * 100) << "%"
                  << " │ " << std::setw(7) << (result.jaccardIndex * 100) << "%"
                  << " │ " << std::setw(5) << result.pixelCount1 
                  << "/" << std::setw(5) << result.pixelCount2 << " ║\n";
    }
    
    std::cout << "╚═══════════════════════════╧══════════╧══════════╧═════════╧════════════════╝\n";
    std::cout << "\nNOTE: Coverage = matching pixels / reference pixels\n";
    std::cout << "      Jaccard Index = similarity measure (1.0 = identical)\n";
}

void BenchmarkManager::generateFullReport(
    const std::vector<TimeResult>& timeResults,
    const std::vector<AccuracyResult>& accuracyResults,
    const std::vector<PixelSetComparisonResult>& pixelSetResults) {
    
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                  COMPREHENSIVE BENCHMARK REPORT                              ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
    
    printTimeReport(timeResults);
    printAccuracyReport(accuracyResults);
    printPixelSetReport(pixelSetResults);
    
    std::cout << "\n";
}

void BenchmarkManager::exportToCSV(
    const std::string& filename,
    const std::vector<TimeResult>& timeResults,
    const std::vector<AccuracyResult>& accuracyResults) {
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    
    file << "Algorithm,Time_us,Time_per_iter_us,Pixels,Iterations,Avg_Error_px,Max_Error_px\n";
    
    for (size_t i = 0; i < timeResults.size() && i < accuracyResults.size(); i++) {
        const auto& timeRes = timeResults[i];
        const auto& accRes = accuracyResults[i];
        
        file << timeRes.algorithmName << ","
             << timeRes.executionTime_us << ","
             << (timeRes.executionTime_us / timeRes.iterations) << ","
             << timeRes.pixelCount << ","
             << timeRes.iterations << ","
             << accRes.avgError << ","
             << accRes.maxError << "\n";
    }
    
    file.close();
    std::cout << "Results exported to: " << filename << std::endl;
}

// ============================================================================
// NEW: Unified Benchmark for Assignment Requirements
// ============================================================================

BenchmarkManager::BenchmarkResult BenchmarkManager::benchmarkObject(
    Object2D* customAlgo, 
    Object2D* parametricAlgo,
    Renderer& renderer,
    const AccuracyCalculator& calculator,
    const std::string& objectDescription,
    int iterations) {
    
    BenchmarkResult result;
    result.objectDescription = objectDescription;
    result.customAlgoName = customAlgo->name();
    result.parametricAlgoName = parametricAlgo->name();
    
    // Measure time for custom algorithm
    TimeResult customTime = measureTime(customAlgo, renderer, iterations);
    result.customTime_ms = customTime.executionTime_us / 1000.0; // Convert to milliseconds
    result.customPixelCount = customTime.pixelCount;
    
    // Measure time for parametric algorithm
    TimeResult parametricTime = measureTime(parametricAlgo, renderer, iterations);
    result.parametricTime_ms = parametricTime.executionTime_us / 1000.0; // Convert to milliseconds
    result.parametricPixelCount = parametricTime.pixelCount;
    
    // Measure accuracy for custom algorithm
    AccuracyResult customAcc = measureAccuracy(customAlgo, renderer, calculator);
    result.customAvgError = customAcc.avgError;
    
    // Measure accuracy for parametric algorithm
    AccuracyResult parametricAcc = measureAccuracy(parametricAlgo, renderer, calculator);
    result.parametricAvgError = parametricAcc.avgError;
    
    return result;
}

void BenchmarkManager::printConsoleOutput(const std::vector<BenchmarkResult>& results) {
    std::cout << "\n";
    std::cout << "==================================================\n";
    std::cout << "  EXECUTION TIME (milliseconds)\n";
    std::cout << "==================================================\n\n";
    
    for (size_t i = 0; i < results.size(); i++) {
        const auto& r = results[i];
        std::cout << "Object " << (i + 1) << ": " << r.objectDescription << "\n";
        std::cout << "  " << r.customAlgoName << ": "
                  << std::fixed << std::setprecision(4) << r.customTime_ms << " ms\n";
        std::cout << "  " << r.parametricAlgoName << " (OpenGL): "
                  << std::fixed << std::setprecision(4) << r.parametricTime_ms << " ms\n";
        std::cout << "\n";
    }
}

void BenchmarkManager::writeOutputFile(const std::string& filename, 
                                       const std::vector<BenchmarkResult>& results) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot create output file: " << filename << "\n";
        return;
    }
    
    file << "==================================================\n";
    file << "  2D DRAWING ALGORITHMS BENCHMARK RESULTS\n";
    file << "==================================================\n\n";
    
   for (size_t i = 0; i < results.size(); i++) {
        const auto& r = results[i];
        file << "Object " << (i + 1) << ": " << r.objectDescription << "\n";
        file << "  Custom Algorithm (" << r.customAlgoName << "):\n";
        file << "    Execution Time: " << std::fixed << std::setprecision(4) 
             << r.customTime_ms << " ms\n";
        file << "    Average Error:  " << std::fixed << std::setprecision(4) 
             << r.customAvgError << " px\n";
        file << "    Pixel Count:    " << r.customPixelCount << "\n";
        file << "\n";
        file << "  OpenGL Function (" << r.parametricAlgoName << "):\n";
        file << "    Execution Time: " << std::fixed << std::setprecision(4) 
             << r.parametricTime_ms << " ms\n";
        file << "    Average Error:  " << std::fixed << std::setprecision(4) 
             << r.parametricAvgError << " px\n";
        file << "    Pixel Count:    " << r.parametricPixelCount << "\n";
        file << "\n";
        file << "  Comparison:\n";
        double speedup = r.parametricTime_ms / r.customTime_ms;
        file << "    Speed Ratio (OpenGL/Custom): " << std::fixed << std::setprecision(2) 
             << speedup << "x\n";
        if (speedup > 1.0) {
            file << "    Custom algorithm is FASTER\n";
        } else {
            file << "    OpenGL function is FASTER\n";
        }
        file << "\n";
        file << "--------------------------------------------------\n\n";
    }
    
    file.close();
    std::cout << "Benchmark results written to: " << filename << "\n";
}
