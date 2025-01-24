# Parallel Boruvka's algorithm

The following is a parallel implementation of Boruvka's algorithm using MPI and OpenMP, used to find the minimum spanning tree of a graph.


## How to run
The code can be compiled using the provided `Makefile`:
```bash
make
```

Some options can be passed to `make`:
- `GEN=<num>`: Generate a random graph with `<num>` vertices and edges. The default value is 0, meaning that a preexisting `graph.txt` file will be used.
- `INPUT=/path/to/file`: Use a custom input file. The default value is `graph.txt`.
- `OUTPUT=/path/to/file`: Use a custom output file. The default value is `mst.txt`.


Finally, run the code using `mpirun` or via `make`:
```bash
mpirun -np <procs> ./mst <input_file> <output_file>

# or
make run
```

When running with `make`, the number of processes can be set using the `NP` variable:
```bash
make run NP=<procs>
```
Here, also the `GEN`, `INPUT`, and `OUTPUT` variables can be set, since the `run` target triggers a `clean` before being executed.
