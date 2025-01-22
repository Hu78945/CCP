# Compilation and Execution Guide

This guide provides instructions on how to compile and run the following files:

1. `serial.cpp` - Serial implementation.
2. `parallel.cpp` - Parallel implementation using OpenMP.
3. `distributed.cpp` - Distributed implementation using MPI.

---

## Prerequisites

- **Compiler**: Ensure you have `g++` (for serial and OpenMP programs) and `mpic++` (for MPI programs) installed.
- **MPI Library**: Install an MPI implementation such as OpenMPI or MPICH.
- **OpenMP Support**: Ensure your compiler supports OpenMP (usually available in modern versions of `g++`).

### Installing Required Tools (if not already installed):

#### Ubuntu:
```bash
sudo apt update
sudo apt install g++ openmpi-bin libopenmpi-dev
```

---

## Compilation Instructions

### **1. Serial Implementation**

#### Compile:
```bash
g++ -o serial_exec serial.cpp
```
- `-o serial_exec`: Specifies the output executable name.

#### Run:
```bash
./serial_exec
```

---

### **2. Parallel Implementation (OpenMP)**

#### Compile:
```bash
g++ -o parallel_exec -fopenmp parallel.cpp
```
- `-fopenmp`: Enables OpenMP support.
- `-o parallel_exec`: Specifies the output executable name.

#### Run:
```bash
./parallel_exec
```

#### Optional: Set the number of threads:
```bash
export OMP_NUM_THREADS=4
./parallel_exec
```
- Replace `4` with the desired number of threads.

---

### **3. Distributed Implementation (MPI)**

#### Compile:
```bash
mpic++ -o distributed_exec distributed.cpp
```
- `-o distributed_exec`: Specifies the output executable name.

#### Run:
```bash
mpirun -np <num_processes> ./distributed_exec
```
- Replace `<num_processes>` with the number of processes to use (e.g., `4`).

#### Example:
```bash
mpirun -np 4 ./distributed_exec
```

---

## Notes

1. **Execution Environment**:
   - Ensure your system supports the required number of processes or threads when using OpenMP or MPI.
   - For MPI, ensure all processes can communicate effectively in a distributed environment.

2. **Performance**:
   - The OpenMP and MPI implementations may require fine-tuning to fully utilize your hardware's capabilities.

3. **Error Handling**:
   - If you encounter issues, verify the installed tools and libraries are up-to-date.
   - Use `mpirun --oversubscribe` if running more MPI processes than available cores on some systems.

---

## Example Workflow

### Compiling All Files:
```bash
g++ -o serial_exec serial.cpp
mpic++ -o distributed_exec distributed.cpp
g++ -o parallel_exec -fopenmp parallel.cpp
```

### Running All Programs:
```bash
./serial_exec
export OMP_NUM_THREADS=4 && ./parallel_exec
mpirun -np 4 ./distributed_exec
```

