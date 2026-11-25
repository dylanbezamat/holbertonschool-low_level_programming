# Crash Report — crash_example.c

## 1. Description of the crash
Running `./crash_example` prints:

    crash_example: deterministic NULL dereference (segmentation fault)
      requesting n=0
    Segmentation fault (core dumped)

The program is killed by the kernel with the signal SIGSEGV before printing `nums[0]`.
It crashes the same way on every run.

## 2. Root cause
The root cause is a **missing NULL check** in `main` after calling `allocate_numbers`.

- `main` sets `n = 0` (line 25) and calls `allocate_numbers(n)` (line 30).
- In `allocate_numbers`, the condition `n <= 0` (line 9) is true, so the function returns `NULL` at line 10.
- `malloc` (line 12) is **never called**.
- `main` stores this `NULL` in `nums` and never checks it.

## 3. Full causal chain
1. Line 25: `n = 0`.
2. Line 30: `allocate_numbers(0)` returns `NULL` (line 10), so `nums == NULL`.
3. Line 32: `nums[0] = 42` writes 4 bytes at address `0x0`.
4. Address `0x0` is never mapped in the process (the kernel keeps the first page unmapped on purpose).
5. The CPU raises a page fault, the kernel sends SIGSEGV, and the program dies.
6. Lines 34 and 36 are never executed.

## 4. Why the memory access is invalid
- **Category of undefined behavior:** NULL pointer dereference (CERT EXP34-C: do not dereference null pointers).
- **Stack or heap?** Neither. `nums` itself is a local variable on the stack of `main`, but its value is `NULL`. The write targets address `0x0`, which belongs to no valid memory region. No heap block exists, because `malloc` was never called.
- **Why it is deterministic:** `n` is hard-coded to 0, so `allocate_numbers` always returns `NULL`, and address `0x0` is never mapped. The same instruction faults on every run.
- **Segfault is the effect, not the bug:** the bug is using a pointer without checking it. The segmentation fault is only the moment the system detects it.

## 5. Critique of the AI explanations
I asked an AI tool for possible causes and fixes. Here is my evaluation.

| AI suggestion | Verdict | Why |
|---|---|---|
| "The pointer `nums` is NULL when it is dereferenced at line 32" | Correct | `allocate_numbers(0)` returns `NULL` and `main` does not check it. |
| "`malloc` failed and returned NULL" | Incorrect | `malloc` is never called: the function returns at line 10 because `n <= 0`. |
| "The crash happens at line 34 when reading `nums[0]`" | Incorrect | The first invalid access is the write at line 32, so the program dies there. Line 34 is never reached. |
| "Invalid access to heap memory" | Incorrect | No heap block exists. The access targets address `0x0`, which is neither stack nor heap. |
| "Fix: remove `free(nums)`" | Incorrect | `free(NULL)` is allowed and does nothing. It is not related to the crash. |
| "Fix: check the return value before using `nums`" | Correct | This prevents the dereference. |

## 6. Suggested fix (optional, not applied)
Check the return value right after line 30:

    nums = allocate_numbers(n);
    if (!nums)
    {
        fprintf(stderr, "allocation failed or invalid size\n");
        return 1;
    }

Another option is to never request `n <= 0`. The NULL check is still needed, because `malloc` can also fail.

---

## AI usage
I used an AI assistant (Claude) to propose possible causes and fixes.
I checked each suggestion against the source code and the program output, and kept only the ones I could justify.
