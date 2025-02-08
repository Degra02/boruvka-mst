```bash
[filippo.degrandi@hpc-head-n1 boruvka-mst]$ cat error 
==10204== Memcheck, a memory error detector
==10204== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==10204== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==10204== Command: /home/filippo.degrandi/boruvka-mst/mst -g 20000 -v
==10204== 
==10205== Memcheck, a memory error detector
==10205== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==10205== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==10205== Command: /home/filippo.degrandi/boruvka-mst/mst -g 20000 -v
==10205== 
==10206== Memcheck, a memory error detector
==10206== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==10206== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==10206== Command: /home/filippo.degrandi/boruvka-mst/mst -g 20000 -v
==10206== 
==10207== Memcheck, a memory error detector
==10207== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==10207== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==10207== Command: /home/filippo.degrandi/boruvka-mst/mst -g 20000 -v
==10207== 
==10204== Warning: set address range perms: large range [0x59e40040, 0xe8ef4380) (undefined)
==10205== Warning: set address range perms: large range [0x59e40040, 0xe8ef4380) (undefined)
==10206== Warning: set address range perms: large range [0x59e40040, 0xe8ef4380) (undefined)
==10207== Warning: set address range perms: large range [0x59e40040, 0xe8ef4380) (undefined)
==10206== Warning: set address range perms: large range [0x8215040, 0x2be42110) (undefined)
==10205== Warning: set address range perms: large range [0x8215040, 0x2be42110) (undefined)
==10207== Warning: set address range perms: large range [0x8215040, 0x2be42110) (undefined)
==10204== Warning: set address range perms: large range [0x8215040, 0x2be42110) (undefined)
==10206== Argument 'size' of function malloc has a fishy (possibly negative) value: -947543648
==10206==    at 0x4C29BC3: malloc (vg_replace_malloc.c:299)
==10206==    by 0x4E8B0A8: MPIR_Scatter_intra (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==10206==    by 0x4E8BD2B: MPIR_Scatter (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==10206==    by 0x4E8B552: MPIR_Scatter_impl (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==10206==    by 0x4E8C187: PMPI_Scatter (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==10206==    by 0x401645: adj_boruvka (mst.c:51)
==10206==    by 0x402A17: main (main.c:105)
==10206== 
Assertion failed in file src/mpi/coll/helper_fns.c at line 84: ((sendbuf)) != NULL
internal ABORT - process 2
```


