# libvsync - formally-verified and high-performance concurrency kit

libvsync is a header library of atomics, synchronization primitives, and
concurrent data structures.  The synchronization primitives and concurrent data
structures are formally verified on WMM.

Our ASPLOS'21 [publication][publication] describes part of the research effort
put in this library.

Check all available algorithms in [doc](doc/api/vsync/README.md)

## Atomic primitives

Examples: vatomic_cmpxchg, vatomic_add, vatomic_fence

Atomic primitives provide a rich and clean interface of atomic
operations and fences.  Atomics are implemented in several flavors:
For ARMv7 (32 and 64 bits) and ARMv8 (32 and 64 bits) we provide optimized and
tested assembly implementations.
For all other architectures, including x86_64, we provide a builtin-based
version that can be compiled to the target architecture.

See our [API documentation][api_atomics] for details and usage examples.

## Synchronization primitives

Examples: Ticket lock, MCS lock, mutex

The synchronization primitives were verified and optimized with [vsyncer]
on weak memory model (WMM), and are correct on ARMv7, ARMv8,
RISC-V, and x86_64 architectures.  The memory barriers are maximally relaxed,
ie, they were carefully optimized to a weakest set of barriers that is still
correct.  A synchronization primitive is correct if it provides mutual
exclusion and does not hang.

Note that some of the algorithms were also verified with respect to other
properties such as fairness.  For that we have employed the [TLA+/TLC][tla]
model checker.

See [doc](doc/api/vsync/spinlock/README.md) for details and usage examples.

## Concurrent data structures

Examples: ring buffer (rbuffer)

The concurrent data structures were verified on WMMs with [vsyncer],
and are correct on ARMv7, ARMv8, RISC-V, and x86_64 architectures.


See [doc](doc/api/vsync/queue/README.md) for details and usage examples.

## Installation

To install the header library, you can use CMake.

	mkdir build
	cmake ..
	make install

Alternatively, you can simply copy the header files or configure your compiler
to point to the headers directly. You must keep the files under `vsync/`, e.g.,
`my_project/include/vsync/*.h`.

## Contact

For questions write us an email: `vsync AT huawei DOT com`,

If you have a good use case, we can also design custom concurrent data
structures for your project.

## Other Distributions

There are other libvsync distributions available, which may contain algorithms
that are not part of hm-distribution. Let us know if you need any of those
to be available in this distribution as well.

- [DRC-distribution][inner]
- [open source distribution][open]

[tla]: https://github.com/tlaplus/tlaplus
[publication]: https://dl.acm.org/doi/abs/10.1145/3445814.3446748
[api_atomics]: doc/api/vsync/atomic/README.md
[vsyncer]: http://gitee.com/s4c/vsyncer
[open]: http://gitee.com/s4c/libvsync
[inner]: https://open.codehub.huawei.com/innersource/DRC_G/libvsync

