#include <iostream>
#include <cmath>
#include <vector>
#include <chrono> // Include for measuring time
#include <omp.h>  // Include for OpenMP

int main()
{
    const double alpha = 0.01; // Thermal diffusivity
    const int N = 100;         // Number of spatial divisions
    const double L = 1.0;      // Length of rod
    const double T_max = 0.1;  // Total simulation time
    const double dx = L / N;
    const double dt = 0.5 * dx * dx / alpha; // Stability condition
    const int steps = T_max / dt;

    std::vector<double> T(N + 1, 0.0);      // Current temperatures
    std::vector<double> T_next(N + 1, 0.0); // Next time step temperatures

    // Initial condition
    for (int i = 0; i <= N; ++i)
    {
        double x = i * dx;
        T[i] = 100 * sin(M_PI * x);
    }

    // Measure start time
    auto start = std::chrono::high_resolution_clock::now();

    // Time iteration
    for (int t = 0; t < steps; ++t)
    {
#pragma omp parallel for
        for (int i = 1; i < N; ++i)
        {
            T_next[i] = T[i] + alpha * dt / (dx * dx) * (T[i + 1] - 2 * T[i] + T[i - 1]);
        }
        T = T_next; // Update
    }

    // Measure end time
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Print final temperatures
    for (int i = 0; i <= N; ++i)
    {
        std::cout << "T[" << i << "] = " << T[i] << "\n";
    }

    // Print execution time
    std::cout << "Parallel Execution Time: " << elapsed.count() << " seconds\n";

    return 0;
}
