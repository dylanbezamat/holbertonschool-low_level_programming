# Comparison Algorithms - Metrics

| Run | Naive (s) | Single-pass (s) |
|-----|-----------|-----------------|
| 1 | 2.773379 | 0.000106 |
| 2 | 2.863098 | 0.000106 |
| 3 | 2.770868 | 0.000118 |
| **Average** | **2.802448** | **0.000110** |

Both implementations return the expected results (naive: 625025000, single-pass: 25000).

**Relative difference:** the naive implementation is ~25,477x slower than the single-pass implementation (2.802448 / 0.000110).

Naive performs n(n+1)/2 = 1,250,025,000 checks (O(n^2)), single-pass performs 50,000 checks (O(n)): ~25,000x more work, consistent with the measured ratio.
