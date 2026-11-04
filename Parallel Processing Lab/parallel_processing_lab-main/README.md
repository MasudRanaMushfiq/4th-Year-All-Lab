# parallel_processing_lab

## Build and run an MPI C file on Windows

Open Git Bash in the folder containing the C file.

Build an executable:

```bash
gcc problem_4.c -o problem_4.exe -DMSMPI_NO_SAL -I"C:/Program Files (x86)/Microsoft SDKs/MPI/Include" -L"C:/Program Files (x86)/Microsoft SDKs/MPI/Lib/x64" -lmsmpi
```

Run the executable with 2 MPI processes:

```bash
"/c/Program Files/Microsoft MPI/Bin/mpiexec.exe" -n 2 ./problem_4.exe
```

Change `2` to the required number of processes:

```bash
"/c/Program Files/Microsoft MPI/Bin/mpiexec.exe" -n 4 ./problem_4.exe
```

The source code must support the selected process count. For example,
`problem_4.c` currently requires exactly 2 processes.
