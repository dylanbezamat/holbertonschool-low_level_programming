# Memory Maps — AI Memory Visualizer

Programs analyzed: `stack_example.c` and `aliasing_example.c`.
Compiled with the provided Makefile (`-Wall -Wextra -Werror -pedantic -std=gnu89 -g`), run without Valgrind.

Note: addresses change on every run because of ASLR, so I reason with the
differences between addresses, not their absolute values.

---

## 1. stack_example.c

### What the program does
`main` calls `walk_stack(0, 3)`, which calls itself recursively until depth 3.
At each level, `walk_stack` calls `dump_frame("enter", depth)` before the
recursive call and `dump_frame("exit", depth)` after it.
There is no heap allocation in this program, everything is on the stack.

### Stack frames and lifetimes

| Frame | Local variables (stack) | Lifetime |
|---|---|---|
| `main` | none | whole program |
| `walk_stack(depth)` | `depth`, `max_depth`, `marker = depth * 10` | from function entry until its closing brace (line 30) |
| `dump_frame(label, depth)` | `label`, `depth`, `local_int`, `local_buf[16]`, `p_local` | only during the call (lines 4 to 17) |

- `p_local = &local_int` (line 7): `p_local` is an alias of `local_int`.
  The output confirms it: `&local_int` and `p_local` print the same address.
  This pointer is only valid while `dump_frame` is running.
- `local_buf`: only `local_buf[0]` and `local_buf[1]` are initialized.
  `local_buf[2]` to `local_buf[15]` are indeterminate, but they are never read, so this is not a bug.

### Stack at maximum depth (inside `dump_frame("enter", 3)`)

    high addresses
      main
      walk_stack(0)   marker = 0    &marker = ...7c4
      walk_stack(1)   marker = 10   &marker = ...794
      walk_stack(2)   marker = 20   &marker = ...764
      walk_stack(3)   marker = 30   &marker = ...734
      dump_frame(3)   local_int = 103 @ ...6e4, local_buf @ ...6f0
    low addresses

### What I observed in the output
- **The stack grows downward.** `&marker` goes from `...7c4` (depth 0) to
  `...794` (depth 1): `0x7c4 - 0x794 = 0x30`, so each `walk_stack` frame
  takes 48 bytes and each new frame is at a lower address.
- **"exit" is a new frame, not the old one.** `dump_frame("exit", 2)` prints
  exactly the same addresses as `dump_frame("enter", 2)`. The "enter" frame
  was destroyed when the function returned. The "exit" call is made from the
  same `walk_stack(2)` frame, so the new frame is created at the same place.
  The values are the same because they are computed again (`100 + depth`),
  not because they survived.
- **Stack memory is reused.** `local_buf` of `dump_frame("enter", 0)` was at
  `...780`, and later `marker` of `walk_stack(1)` is at `...794`. Both are in
  the same zone: once `dump_frame` returned, its memory was free to be used
  by the next frame. That is why a pointer to a local variable must never be
  used after the function returns.
- **`label` vs the string it points to.** `label` (the pointer) is a
  parameter of `dump_frame`, so it lives in its frame. But the string
  `"enter"` is a string literal: it is stored in a static read-only zone and
  exists for the whole program, not on the stack.

---

## 2. aliasing_example.c

### Step-by-step memory map

**Step 1 — start of `main` (lines 24-26)**

| Stack (`main`) | Value |
|---|---|
| `a` | `NULL` |
| `b` | `NULL` |
| `n` | `5` |

**Step 2 — `make_numbers(5)` (lines 4-20)**
- Its frame on the stack: `n = 5`, `i`, `arr`.
- Line 12: `malloc(5 * sizeof(int))` allocates 20 bytes on the heap
  (`0x...06b0` in my run). `arr` points to it.
- Lines 16-17: the block contains `[0, 11, 22, 33, 44]`.
- Line 19: `return arr`. The frame of `make_numbers` is destroyed, but the
  heap block is not: heap memory lives until someone calls `free`.
  The responsibility to free it goes to the caller (`main`).

**Step 3 — `b = a` (line 34)**

    STACK (main)                HEAP
    a = 0x...06b0  ─────┐
                        ├──►  [0][11][22][33][44]   (20 bytes, allocated)
    b = 0x...06b0  ─────┘

`b = a` copies the address, not the array. `a` and `b` are two aliases of
the same block. Proof: same address printed, and `a[2] == b[2] == 22`.

**Step 4 — `free(a)` (line 38)**
- The block goes back to the allocator. The program does not own it anymore.
- `free` does not change the variable `a`: it still contains `0x...06b0`.
- So **both `a` and `b` are dangling pointers** now.

**Step 5 — use after free**

| Line | Operation | Problem |
|---|---|---|
| 42 | read `b[2]` | use-after-free (invalid read), undefined behavior |
| 44 | `b[3] = 1234` | use-after-free (invalid write), can corrupt the heap |
| 45 | read `b[3]` | use-after-free (invalid read) again |

The program did not crash, but that proves nothing: it is undefined
behavior that "works" by luck.

### Ownership
There is only one block but two pointers on it. Only one of them should be
considered the owner and call `free`. If the code did `free(a)` then
`free(b)`, it would free the same block twice: a **double free**, and glibc
would abort the program. Good practice: set `a = NULL` (and `b = NULL`)
right after `free(a)`.

### Leaks
One `malloc` (line 12) and one `free` (line 38): no memory leak.
The bugs here are use-after-free, not leaks.

---

## 3. AI errors found and corrected

### Error 1 — the AI said only `b` was dangling
- **What the AI said:** "after `free(a)`, `b` contains a dangling pointer."
- **Why it was incomplete:** `free(a)` frees the block but does not modify
  the variable `a`. `a` still holds the same address, so `a` is dangling too.
  The AI (and the program's own printf) only mentions `b`, which can make
  you believe `a` is safe to use after `free`.
- **Evidence:** line 38 only calls `free(a)`, nothing sets `a` to `NULL`.
  `a` and `b` have the same value, so they are in the same state.
- **Correction:** after line 38, both `a` and `b` are dangling. Any access
  through either of them is a use-after-free.

### Error 2 — the AI said the value 891586013 "has no meaning"
- **What the AI said:** the value read at line 42 is meaningless.
- **Why it was imprecise:** it is true that we cannot rely on it, but it is
  not random. When a block is freed, glibc reuses the inside of the block to
  store its own bookkeeping data (free-list information). Reading `b[2]`
  reads part of that allocator data.
- **Consequence:** this also explains why line 44 is dangerous: writing
  `b[3] = 1234` can overwrite allocator data and corrupt the heap.
- **Correction:** the value is undefined behavior and must not be used, and
  it most likely comes from allocator metadata written into the freed
  block. To be confirmed with Valgrind in the next task.

---

## AI usage
I used an AI assistant (Claude) to generate the first version of the memory
maps. I checked each claim against the real program output, found the two
errors described in section 3, and corrected them.
