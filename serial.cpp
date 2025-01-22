#include <iostream>
#include <iomanip> // Include for setprecision
#include <cmath>
#include <vector>
#include <chrono> // Include for measuring time

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
        for (int i = 1; i < N; ++i)
        {
            T_next[i] = T[i] + alpha * dt / (dx * dx) * (T[i + 1] - 2 * T[i] + T[i - 1]);
        }
        T = T_next; // Update
    }

    // Measure end time
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Convert time to minutes and seconds if needed
    double time_seconds = elapsed.count();
    if (time_seconds >= 60)
    {
        int minutes = static_cast<int>(time_seconds / 60);
        double seconds = time_seconds - (minutes * 60);
        std::cout << "Serial Execution Time: " << minutes << " minutes and "
                  << std::fixed << std::setprecision(2) << seconds << " seconds\n";
    }
    else
    {
        std::cout << "Serial Execution Time: " << std::fixed << std::setprecision(6)
                  << time_seconds << " seconds\n";
    }

    // Print final temperatures
    for (int i = 0; i <= N; ++i)
    {
        std::cout << "T[" << i << "] = " << T[i] << "\n";
    }

    return 0;
}
