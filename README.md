# cb — circular buffer

`cb` is a power-of-two circular buffer of fixed-size slots. Define a buffer
with the `proto_cb` macro, initialize it, then allocate, free, read, and send
slots by number or pointer. Sends can signal another process. Capacity is one
of the `cb_cap_t` masks (`CB10` is 1024 slots, and so on).

It builds with the [base](https://github.com/smythereen/base) unit make system, which handles C, C++, and Rust, including large mixed-language projects.

## Public interface (`cb.h`)

- `proto_cb(name, capacity, bufsize)` — define a buffer type with a `cb_head_t`
  header and `capacity + 1` slots of `bufsize` bytes
- `proto_cb_bio` / `proto_cb_Bio` / `proto_cb_mio` — mio descriptor tables
- `CirBuf_Initialize(start, circular_buffer_capability, buffer_size, signal)`
- `CirBuf_Push_Context` / `CirBuf_Pop_Context`
- `CirBuf_Pointer` / `CirBuf_Number` and the uppercase `CirBuf_POINTER` /
  `CirBuf_NUMBER` variants
- `CirBuf_Number_Allocate` / `CirBuf_Pointer_Allocate` and the uppercase
  `*_ALLOCATE` variants, plus the matching `*_Free` / `*_FREE` calls
- `CirBuf_Number_Read` / `CirBuf_Pointer_Read` — wait for a slot, optional fork
- `CirBuf_Number_Send` / `CirBuf_Pointer_Send` — hand a slot to a target pid

`cblocal.h` is an implementation header, not an aggregate application API.

## Dependencies and build

Use this directory as `cb/` inside a [base](https://github.com/smythereen/base)
project, beside the root `rules.mak` and `targets.mak`:

```sh
export UNITS_CATALOG_URL=https://unit-catalog.dejavu-archive.workers.dev
./repo/repo.sh add cb
HOME=$PWD make -C cb header library product
```

`UNIT.deps` requires `mio`, `msg`, `addr`, `tp`, `map`, `sem`, `far`, `now`,
`sys`, and `atomic`. The makefile links
`-lmio -lmsg -laddr -ltps -lmap -lsem -lfar -lnow -lsys`.
`tps.h` and `-ltps` come from the public [tp](https://github.com/smythereen/tp)
unit (`libtps.a` is `libtp.a`). Sources also include `atomic_32.h` from
[atomic](https://github.com/smythereen/atomic). Headers export to `cmn/`, the
archive to `lib/libcb.a`, and the `cb` check program to `bin/`.

## C and C++

This unit is usable from **C and C++**. Its public header already supplies
`extern "C"` guards. Compile the implementation `.c` files with GCC and link
that C library into your C++ application with G++; do not reinterpret the
legacy C source as C++.

```c
#include <cb.h>

proto_cb(ring, CB08, 64);

int main(void) {
    static ring_t ring;
    CirBuf_Initialize(&ring, CB08, 64, 0); /* already pushes context */
    void *slot = CirBuf_Pointer_Allocate();
    if (slot == 0)
        return 1;
    return CirBuf_Pointer_Free(slot) != 0;
}
```

From a project root with headers in `cmn/` and built libraries in `lib/`:

```sh
gcc -std=gnu11 -Icmn smoke.c -Llib \
  -Wl,--start-group -lcb -lmio -lmsg -laddr -ltps -lmap -lsem -lfar -lnow -lsys -Wl,--end-group -o cb-smoke
```

The GNU linker group handles cyclic static unit dependencies.

## Programs

`cb.c` builds the `cb` check program. It maps a test circular buffer with
`map` and `tp` (`tps.h`) and is a development aid, not a general CLI.

## Verification

The Guardian tree already built this unit with plain `make` in the established
base layout, including the `cb` program. This publication is a source snapshot,
not a fresh clean rebuild in a separate tree. C++ linkage guards were inspected.

## Source and license

Published from the current Guardian unit directory as a clean snapshot, not
its private Git history. C/header/makefile bytes are preserved; build outputs,
backups, and the compiled `cb` binary are excluded. This guide and `UNIT.deps`
are documentation additions. No release tag is created by this publication.
See `LICENSE` and `LICENSES/` for the existing Apache-2.0 OR GPL-2.0-or-later
licensing.
