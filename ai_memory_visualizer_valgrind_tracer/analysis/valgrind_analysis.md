# Valgrind Analysis — AI Memory Tracer

Programs analyzed: `aliasing_example.c` and `heap_example.c`.

Command used:

    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./<program>

- `--leak-check=full`: details every leak with the allocation stack.
- `--show-leak-kinds=all`: shows all leak categories.
- `--track-origins=yes`: shows where uninitialized values come from.

---

## 1. aliasing_example.c

Result: `ERROR SUMMARY: 3 errors from 3 contexts`, no leaks.

| # | Valgrind message | Line | Classification | Memory object |
|---|---|---|---|---|
| 1 | Invalid read of size 4, 8 bytes inside a block of size 20 free'd | 42 | use-after-free (read) | `b[2]` |
| 2 | Invalid write of size 4, 12 bytes inside a block of size 20 free'd | 44 | use-after-free (write) | `b[3]` |
| 3 | Invalid read of size 4, 12 bytes inside a block of size 20 free'd | 45 | use-after-free (read) | `b[3]` |

### How I read these messages
- **size 4**: the access is one `int` (4 bytes).
- **block of size 20**: the array of 5 `int` allocated in `make_numbers` (line 12).
- **8 / 12 bytes inside**: `b[2]` is at 2 x 4 = 8, `b[3]` is at 3 x 4 = 12.
- **free'd at line 38**: the block was released by `free(a)`.

### Why these are errors
`b = a` (line 34) made `b` an alias of `a`: both point to the same block.
`free(a)` (line 38) released it, but `a` and `b` were not set to `NULL`.
Every access through `b` after line 38 uses memory whose lifetime has ended.
The write at line 44 is the most dangerous: it can corrupt allocator data.

### Leaks
`All heap blocks were freed -- no leaks are possible`.
`2 allocs, 2 frees, 4,116 bytes`: 20 bytes for the array + 4,096 bytes for the `printf` output buffer.

### Observation: different value under Valgrind
- Without Valgrind: `reading b[2]=891586013`.
- With Valgrind: `reading b[2]=22` (the old value).

Valgrind keeps freed blocks in quarantine and does not reuse them right away.
Without Valgrind, glibc writes its own bookkeeping data inside the freed block.
The value read after `free` depends on the allocator: it is undefined behavior.

---

## 2. heap_example.c

Result: `ERROR SUMMARY: 1 errors from 1 contexts`.

| Valgrind message | Classification | Memory object |
|---|---|---|
| 6 bytes in 1 blocks are definitely lost, malloc in `person_new` (line 21) called from `main` (line 51) | leak due to lost ownership | `alice->name` |

### Root cause
`person_free_partial` (lines 36-42) only calls `free(p)`, never `free(p->name)`.
- `bob` is freed correctly: `free(bob->name)` (line 66), then `free(bob)` (line 67).
- `alice` goes through `person_free_partial` (line 69): the struct is freed, not its name.

### Why 6 bytes
`"Alice"` = 5 characters + `'\0'` = 6 bytes (`malloc(len + 1)`, line 21).

### Why "definitely lost"
The only pointer to the name was inside the struct.
After `free(p)` (line 41), nothing can reach the name anymore.

### Heap summary check
`5 allocs, 4 frees, 4,138 bytes`:
- allocs: 2 `Person` + 2 names + 1 `printf` buffer.
- frees: `bob->name`, `bob`, `alice`, `printf` buffer. Missing: `alice->name`.
- bytes: 2 x 16 (`sizeof(Person)`, with 4 bytes of padding) + 6 + 4 + 4,096.

### Fix (for understanding)
Free the name before the struct: `free(p->name);` then `free(p);`.
The other order would read `p->name` after `free(p)`: a use-after-free.

---

## 3. AI errors found and corrected

### Error 1: "the bug is at line 21"
- **AI said:** Valgrind points to line 21, so the leak comes from line 21.
- **Why wrong:** line 21 is where the block was allocated, and this `malloc` is correct. Valgrind shows where a leaked block was born, not where it should have been freed.
- **Correction:** the bug is the missing `free(p->name)` in `person_free_partial`.

### Error 2: "the value read after free is random"
- **AI said:** in task 0, `891586013` was described as meaningless.
- **Why imprecise:** under Valgrind the same read gives `22`. The value depends on what the allocator does with the freed block.
- **Correction:** it is undefined behavior caused by allocator behavior, not randomness.

---

## AI usage
I used an AI assistant (Claude) to help interpret the Valgrind output.
I checked every explanation against the code, the Valgrind output and my task 0 memory maps, and corrected the errors above.
