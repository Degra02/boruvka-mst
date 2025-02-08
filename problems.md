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


```bash
==38326== Memcheck, a memory error detector
==38326== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==38326== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==38326== Command: /home/filippo.degrandi/boruvka-mst/mst -g 20000 -v
==38326== 
==38327== Memcheck, a memory error detector
==38327== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==38327== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==38327== Command: /home/filippo.degrandi/boruvka-mst/mst -g 20000 -v
==38327== 
==38329== Memcheck, a memory error detector
==38329== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==38329== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==38329== Command: /home/filippo.degrandi/boruvka-mst/mst -g 20000 -v
==38329== 
==38328== Memcheck, a memory error detector
==38328== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==38328== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==38328== Command: /home/filippo.degrandi/boruvka-mst/mst -g 20000 -v
==38328== 
==38326== Warning: set address range perms: large range [0x59e40040, 0xe8ef4380) (undefined)
=>> PBS: job killed: walltime 76 exceeded limit 60
==38328== 
==38328== Process terminating with default action of signal 15 (SIGTERM)
==38329== 
==38329== Process terminating with default action of signal 15 (SIGTERM)
==38327== 
==38327== Process terminating with default action of signal 15 (SIGTERM)
==38329==    at 0x57E91EE: __poll_nocancel (in /usr/lib64/libc-2.17.so)
==38329==    by 0x4FAC5CD: MPID_nem_tcp_connpoll (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38329==    by 0x4F97890: MPIDI_CH3I_Progress (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38329==    by 0x4F2AB54: MPIC_Wait (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38329==    by 0x4F2AE1B: MPIC_Recv (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38329==    by 0x4E80CA3: MPIR_Bcast_binomial (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38329==    by 0x4E8279A: MPIR_Bcast_intra (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38329==    by 0x4E8306C: MPIR_Bcast_impl (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38329==    by 0x4E83670: PMPI_Bcast (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38329==    by 0x4028F8: main (in /home/filippo.degrandi/boruvka-mst/mst)
==38327==    at 0x57E91EE: __poll_nocancel (in /usr/lib64/libc-2.17.so)
==38327==    by 0x4FAC5CD: MPID_nem_tcp_connpoll (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38327==    by 0x4F97890: MPIDI_CH3I_Progress (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38327==    by 0x4F2AB54: MPIC_Wait (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38327==    by 0x4F2AE1B: MPIC_Recv (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38327==    by 0x4E80CA3: MPIR_Bcast_binomial (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38327==    by 0x4E8279A: MPIR_Bcast_intra (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38327==    by 0x4E8306C: MPIR_Bcast_impl (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38327==    by 0x4E83670: PMPI_Bcast (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38327==    by 0x4028F8: main (in /home/filippo.degrandi/boruvka-mst/mst)
==38326== 
==38326== Process terminating with default action of signal 15 (SIGTERM)
==38328==    at 0x57E91EE: __poll_nocancel (in /usr/lib64/libc-2.17.so)
==38328==    by 0x4FAC5CD: MPID_nem_tcp_connpoll (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38326==    at 0x57E4F6E: __read_nocancel (in /usr/lib64/libc-2.17.so)
==38328==    by 0x4F97890: MPIDI_CH3I_Progress (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38326==    by 0x5771B13: _IO_file_underflow@@GLIBC_2.2.5 (in /usr/lib64/libc-2.17.so)
==38326==    by 0x57705C7: _IO_file_xsgetn (in /usr/lib64/libc-2.17.so)
==38326==    by 0x5764FDE: fread (in /usr/lib64/libc-2.17.so)
==38328==    by 0x4F2AB54: MPIC_Wait (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38328==    by 0x4F2AE1B: MPIC_Recv (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38326==    by 0x402C81: generate_graph (in /home/filippo.degrandi/boruvka-mst/mst)
==38326==    by 0x402870: main (in /home/filippo.degrandi/boruvka-mst/mst)
==38328==    by 0x4E80CA3: MPIR_Bcast_binomial (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38328==    by 0x4E8279A: MPIR_Bcast_intra (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38328==    by 0x4E8306C: MPIR_Bcast_impl (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38328==    by 0x4E83670: PMPI_Bcast (in /apps/mpich-3.2/lib/libmpi.so.12.1.0)
==38328==    by 0x4028F8: main (in /home/filippo.degrandi/boruvka-mst/mst)
==38327== 
==38327== HEAP SUMMARY:
==38327==     in use at exit: 4,118,871 bytes in 398 blocks
==38327==   total heap usage: 523 allocs, 125 frees, 4,182,462 bytes allocated
==38327== 
==38329== 
==38329== HEAP SUMMARY:
==38329==     in use at exit: 4,118,871 bytes in 398 blocks
==38329==   total heap usage: 523 allocs, 125 frees, 4,182,462 bytes allocated
==38329== 
==38326== 
==38326== HEAP SUMMARY:
==38326==     in use at exit: 2,403,999,455 bytes in 401 blocks
==38326==   total heap usage: 528 allocs, 127 frees, 2,404,063,142 bytes allocated
==38326== 
==38328== 
==38328== HEAP SUMMARY:
==38328==     in use at exit: 4,118,871 bytes in 398 blocks
==38328==   total heap usage: 523 allocs, 125 frees, 4,182,462 bytes allocated
==38328== 
==38327== LEAK SUMMARY:
==38327==    definitely lost: 512 bytes in 2 blocks
==38327==    indirectly lost: 0 bytes in 0 blocks
==38327==      possibly lost: 0 bytes in 0 blocks
==38327==    still reachable: 4,118,359 bytes in 396 blocks
==38327==         suppressed: 0 bytes in 0 blocks
==38327== Rerun with --leak-check=full to see details of leaked memory
==38327== 
==38327== For counts of detected and suppressed errors, rerun with: -v
==38327== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
==38329== LEAK SUMMARY:
==38329==    definitely lost: 512 bytes in 2 blocks
==38329==    indirectly lost: 0 bytes in 0 blocks
==38329==      possibly lost: 0 bytes in 0 blocks
==38329==    still reachable: 4,118,359 bytes in 396 blocks
==38329==         suppressed: 0 bytes in 0 blocks
==38329== Rerun with --leak-check=full to see details of leaked memory
==38329== 
==38329== For counts of detected and suppressed errors, rerun with: -v
==38329== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
==38328== LEAK SUMMARY:
==38328==    definitely lost: 512 bytes in 2 blocks
==38328==    indirectly lost: 0 bytes in 0 blocks
==38328==      possibly lost: 0 bytes in 0 blocks
==38328==    still reachable: 4,118,359 bytes in 396 blocks
==38328==         suppressed: 0 bytes in 0 blocks
==38328== Rerun with --leak-check=full to see details of leaked memory
==38328== 
==38328== For counts of detected and suppressed errors, rerun with: -v
==38328== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
