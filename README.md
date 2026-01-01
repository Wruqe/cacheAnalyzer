# Stride Walker

A C++ performance analysis tool that measures memory access latency across different stride patterns to demonstrate CPU cache hierarchy behavior.

## Overview

Stride Walker demonstrates how memory access patterns affect performance by measuring access times at various stride sizes. As stride increases, memory accesses move from fast cache levels (L1/L2/L3) to slower main memory (DRAM), revealing the performance characteristics of your CPU's memory subsystem.

## What It Does

The program:

- Creates a large vector (67,108,864 integers ≈ 256 MB)
- Accesses elements at different stride sizes (1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024)
- Measures access time for each stride pattern
- Classifies each stride based on likely cache level (L1, L2, L3, or DRAM)

## Building

### Prerequisites

- C++ compiler with C++11 support (g++, clang++, etc.)
- Standard C++ library

### Compilation

```bash
g++ -O2 -std=c++11 strideWalker.cpp -o strideWalker
```

Or with clang:

```bash
clang++ -O2 -std=c++11 strideWalker.cpp -o strideWalker
```

**Note:** The `-O2` optimization flag is recommended to see realistic performance characteristics, though the code uses `volatile` to prevent over-optimization of the benchmark loop.

## Running

```bash
./strideWalker
```

## Output Explanation

The program outputs performance metrics for each stride:

- **stride**: The step size between accessed elements
- **bytes/step**: Memory distance between accesses (stride × sizeof(int))
- **visited**: Number of elements accessed
- **time**: Total time in milliseconds
- **ns/elem**: Nanoseconds per element access
- **Cache classification**: Estimated cache level based on stride size

### Expected Behavior

- **Small strides (1-16)**: Fast access, likely in L1 cache (~1-4 cycles)
- **Medium strides (32-128)**: Moderate speed, likely in L2 cache
- **Large strides (256-1024)**: Slower access, likely in L3 cache or DRAM

As stride increases, you should observe:

- Increasing `ns/elem` values
- Transitions between cache levels
- Performance degradation when exceeding cache line sizes

### Sample Output

Here's an example of actual benchmark results:

```
stride/skip of numbers: 1 (≈ 4 bytes/step)  visited=67108864  time=124.998 ms  ns/elem=1.86261 likely in (L1) cache
stride/skip of numbers: 2 (≈ 8 bytes/step)  visited=33554432  time=59.954 ms  ns/elem=1.78677 likely in (L1) cache
stride/skip of numbers: 4 (≈ 16 bytes/step)  visited=16777216  time=16.6611 ms  ns/elem=0.99308 likely in (L1) cache
stride/skip of numbers: 8 (≈ 32 bytes/step)  visited=8388608  time=8.58846 ms  ns/elem=1.02382 likely in (L1) cache
stride/skip of numbers: 16 (≈ 64 bytes/step)  visited=4194304  time=4.78008 ms  ns/elem=1.13966 likely in (L1) cache
stride/skip of numbers: 32 (≈ 128 bytes/step)  visited=2097152  time=9.83192 ms  ns/elem=4.68822 likely in (L2) cache
stride/skip of numbers: 64 (≈ 256 bytes/step)  visited=1048576  time=5.54054 ms  ns/elem=5.28387 likely in (L2) cache
stride/skip of numbers: 128 (≈ 512 bytes/step)  visited=524288  time=3.03037 ms  ns/elem=5.77998 likely in (L2) cache
stride/skip of numbers: 256 (≈ 1024 bytes/step)  visited=262144  time=1.21804 ms  ns/elem=4.64646 likely in (L3) cache
stride/skip of numbers: 512 (≈ 2048 bytes/step)  visited=131072  time=0.739583 ms  ns/elem=5.64257 likely in (L3) cache
stride/skip of numbers: 1024 (≈ 4096 bytes/step)  visited=65536  time=0.389542 ms  ns/elem=5.94394 likely in (L3) cache
```

**Observations from these results:**

- **L1 cache (strides 1-16)**: Very fast access times (~1 ns/elem), demonstrating excellent cache locality
- **L2 cache (strides 32-128)**: Noticeable performance drop (~4-6 ns/elem) when exceeding L1 cache line boundaries
- **L3 cache (strides 256-1024)**: Similar performance to L2 (~4-6 ns/elem), indicating effective L3 cache utilization
- The transition from L1 to L2 occurs around stride 32 (128 bytes), showing the cache line size boundary
- Total time decreases with larger strides because fewer elements are accessed, but per-element cost increases

## Technical Details

### Cache Hierarchy Thresholds

The program uses approximate thresholds:

- **L1 Cache**: ≤ 64 bytes per step
- **L2 Cache**: ≤ 512 bytes per step
- **L3 Cache**: ≤ 8192 bytes per step
- **DRAM**: > 8192 bytes per step

These thresholds are approximations and may vary based on your specific CPU architecture.

### Why This Matters

Understanding stride patterns is crucial for:

- **Performance optimization**: Sequential access patterns are much faster
- **Cache-aware algorithms**: Designing data structures that fit in cache
- **Memory layout**: Understanding how data locality affects performance
- **Parallel programming**: Minimizing false sharing and cache contention

## Example Use Cases

- Educational tool for understanding CPU cache behavior
- Benchmarking memory subsystem performance
- Validating cache-aware algorithm designs
- Demonstrating the importance of data locality

## Notes

- The program uses `volatile` to prevent compiler optimization of the benchmark loop
- A warm-up pass is performed before timing to account for OS page allocation
- Results may vary based on CPU architecture, cache sizes, and system load
