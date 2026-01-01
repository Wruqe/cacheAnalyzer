#include <iterator>
#include <vector>
#include <iostream>
#include <chrono>
#include <cstddef>  

int main() {
    const std::size_t N = 67'108'864;    
    std::vector<int> a(N, 1);

    // this makes tells the compiler not optimize right away
    volatile long long sink = 0;            

    const std::size_t strides[] = {1,2,4,8,16,32,64,128,256,512,1024};

    for (std::size_t stride : strides) {
        // warm up page tolerance from OS
        for (std::size_t i = 0; i < N; i += stride) sink += a[i];

        auto t0 = std::chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < N; i += stride) sink += a[i];
        auto t1 = std::chrono::high_resolution_clock::now();

        const double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        const std::size_t visited = (N + stride - 1) / stride;
        const double ns_per_elem = (ms * 1e6) / visited;

        std::cout << "numbers we are striding by: " << stride
                  << " (≈ " << (stride * sizeof(int)) << " bytes/step)"
                  << "  visited=" << visited
                  << "  time=" << ms << " ms"
                  << "  ns/elem=" << ns_per_elem;

                  if (stride * sizeof(int) <= 64)
                      std::cout << " likely in (L1) cache";
                  else if (stride * sizeof(int) <= 512)
                      std::cout << " likely in (L2) cache";
                  else if (stride * sizeof(int) <= 8192)
                      std::cout << " likely in (L3) cache";
                  else
                   std::cout << "likely DRAM (main memory)";
                    
                  std::cout << '\n';
    }

    return (int)sink;
}

