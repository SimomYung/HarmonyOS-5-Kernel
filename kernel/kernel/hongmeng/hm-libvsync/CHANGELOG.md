# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project comply partially with [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

Breaking changes in code that is not part of [atomic-APIs](doc/api/vsync/atomic/README.md)
is not guaranteed to result in increment of major version.

### [3.5.0]

### Added

- simple hashtable simpleht.h

### Changed

- reduced gdump thread object size
- turn off buff support in gdump by default

### [3.4.0.1]

### Removed

- cds/bbq.h
- cds/rbuffer.h

### [3.4.0]

### Added

- Bitmap implementation in bitmap.h
- BBQ multi-producer multi-consumer variant in bbq_mpmc.h

### [3.3.1]

- GDump supports lock-free registration, and allows for cooperation between
de-registering threads and syncers.
- GDump local thread buffer size is configurable and the user can reduce its size.

## [3.3.0]

### Added

- introduce BBQ SPSC `bbq_spsc.h`

### Changed

- remove `compat.h` from DRC-Distro

## [3.2.0]

### Added

- support for `atomicsz_t`

### Fixed

- citation in documentation

### Changed

- removed consecutive newlines
- used vbool_t instead of bool
- used vsize_t instead of size_t

## [3.1.0]

### Added

- elimination stack (`<vsync/stack/elimination_stack.h>`)
- exponential backoff stack (`<vsync/stack/xbo_stack.h>`)

## [3.0.3]

### Changed

- vtypes use typedef instead of macros
- listset find without goto
- fallback max refactored loop condition to not have sideeffects
- small fixes

## [3.0.2]

### Changed

- GDump tolerate dereg without explicit exit
- Markdowns update

## [3.0.1]

### Fixed

- rbbuffer barrier bug
- removed VLA from GDump

### Changed

- atomics functions are no longer implemented with macros
- vatomicptr_markable and vatomicptr_stamped are
  no longer implemented with macros
- fix documentation in several data structures and locks

## [3.0.0]

### Added

- Gdump v3 & v4
- Listset implementations
- hashtable_standard and hashtable_evictable

### Changed

- Reorganized distributions
- smr lock functions are passed as a parameter
  instead of being defined in a static header
- decoupled `unbounded_queue_lf` from `cleanup` SMR
  and changed API prefix to `vqueue_ub`
- decoupled `skiplist_lf` from `cleanup` SMR
  and changed API prefix to `vskiplist_`

## [2.4.0] - 2023-03-28

### Fixed

- All SC and all RLX atomics
- Futex compilation on non-linux targets
- Added API documentation markdown

### Added

- Cached memory pool
- EBR: epoch based reclamation
- GDump: garbage dump
- Support to vatomic8 and vatomic16

### Changed

- Rename spinlock into caslock to avoid confusion
- Moved internal files of atomics and queues to their internal subdirs
- Moved maps (skiplist, hashset, listset) to vsync/map

## [2.3.0] - 2023-02-01

### Removed

- Doxybook files

### Added

- Made abstract_lock and exchanger as part of utils
- memcpy_s and memset_s as well as BUG_ON macro

### Changed

- Moved smr into `vsync/smr`
- Moved queues into `vsync/queue`
- Moved stack into `vsync/stack`
- Moved futex-based synchronization primitives to `vsync/thread`
- Moved spinlock to `vsync/spinlock`

## [2.2.4] - 2023-01-13

### Fixed

- Missing utils_rand.h necessary for skiplist in DRC-release

## [2.2.3] - 2023-01-11

### Fixed

- bbq_mpmc configuration bug

## [2.2.2] - 2023-01-10

### Fixed

- HongMeng release script

## [2.2.1] - 2023-01-10

### Added

- HongMeng release files
- kevrb in HongMeng release

### Fixed

- Removed some unnecessary files
- Enabled skiplist in drc- and hm-releases

## [2.2.0] - 2023-01-06

### Added

- Cachedq: a MPMC bounded queue with cached variables to improve the performance
- req_seqlock: recursive sequence lock (writer_lock recursive side)

### Fixed

- vatomic_cpu_pause() on arm.

## [2.1.0] - 2023-01-04

### Added

- BBQ: Block-based bounded queue data structure (see ATC'22 paper).

### Fixed

- Arm64 atomic RMW bug when compiling with O2.
- Arm64 atomic max bug.

## [2.0.0] - 2022-12-09

This release eases the distinction between libvsync and other atomic
primitives that may be used in projects (C11 stdatomic.h, compiler
builtins, wrappers, etc.) and allows future upstream of libvsync data
structures and synchronization primitives.

### Added

- a dispatcher allows to ducktype/overload style functions using the
  same function name on different types (e.g. one can call `vatomic_add`
  on both `vatomic64_t` and `vatomic32_t`).

### Changed

- APIs change: `atomic_*` has been replaced by `vatomic_*`.
- APIs change: no more `atomic_t` type, use explicit `vatomic32_t` or
  `vatomic64_t` types instead.
- APIs change: add `vatomic_*_get` on the functions that returns a value
  (`add`, `sub`, `inc`, `dec`, `or`, `and`, etc.) and now `vatomic_*`
  without `_get` suffix has `void` return type.
- directories: re-organized the directories of the atomic primitives,
  but this change is not visible to client code as we updated the build
  system files as well.

## [1.0.0] - 2021-04-23

Initial version.
