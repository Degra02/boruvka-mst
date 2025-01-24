# Parallel Boruvka's algorithm

The following is a parallel implementation of Boruvka's algorithm using MPI and OpenMP, used to find the minimum spanning tree of a graph.


## How to run
The code can be compiled using CMake. Firstly, create a build directory and run CMake from there:
```bash
mkdir build
cd build
cmake ..
```

Some options can be passed to CMake to configure the build:
- `-DNP=<procs>`: Number of processes to use in the MPI execution.
- `-DINPUT="../path/to/input/file"`: Path to the input file.
- `-DOUTPUT="../path/to/output/file"`: Path to the output file.

Then, compile the code using make:
```bash
make
```

Finally, run the code using `mpirun` or via `make`:
```bash
mpirun -np <procs> ./mst <input_file> <output_file>

# or
make run
```

