#include <mpi.h>
#include <iostream>
#include <cmath>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Rank of the current process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Total number of processes

    const double alpha = 0.01; // Thermal diffusivity
    const int N = 100; // Number of spatial divisions
    const double L = 1.0; // Length of rod
    const double T_max = 0.1; // Total simulation time
    const double dx = L / N;
    const double dt = 0.5 * dx * dx / alpha; // Stability condition
    const int steps = T_max / dt;

    int local_N = N / size; // Number of points handled by each process
    int start = rank * local_N; // Starting index of this process
    int end = (rank == size - 1) ? N : start + local_N - 1; // Ending index

    std::vector<double> T(local_N + 2, 0.0); // Local temperature, including ghost cells
    std::vector<double> T_next(local_N + 2, 0.0);

    // Initial condition: Populate the local segment
    for (int i = 1; i <= local_N; ++i) {
        double x = (start + i - 1) * dx;
        T[i] = 100 * sin(M_PI * x);
    }

    for (int t = 0; t < steps; ++t) {
        // Exchange boundary values with neighbors
        if (rank > 0) {
            MPI_Send(&T[1], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&T[0], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (rank < size - 1) {
            MPI_Send(&T[local_N], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&T[local_N + 1], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Compute the next time step for the local segment
        for (int i = 1; i <= local_N; ++i) {
            T_next[i] = T[i] + alpha * dt / (dx * dx) * (T[i + 1] - 2 * T[i] + T[i - 1]);
        }

        // Update current temperature
        T.swap(T_next);
    }

    // Gather results at rank 0
    std::vector<double> global_T;
    if (rank == 0) {
        global_T.resize(N + 1);
    }

    MPI_Gather(&T[1], local_N, MPI_DOUBLE, global_T.data(), local_N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Print results at rank 0
    if (rank == 0) {
        for (int i = 0; i <= N; ++i) {
            std::cout << "T[" << i << "] = " << global_T[i] << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}
