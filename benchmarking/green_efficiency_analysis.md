# Green Efficiency Analysis

## Measurement Methodology

To measure execution time, I used the `clock()` function from `<time.h>`, which returns the CPU time used by the program, and converted it to seconds with `CLOCKS_PER_SEC`. All programs were compiled with gcc using the same strict flags (`-Wall -Werror -Wextra -pedantic -std=gnu89 -Wno-long-long`) and executed 3 times in the same sandbox. In the instrumentation experiment, each phase (BUILD_DATA, PROCESS, REDUCE) has its own start and end measurement, and TOTAL wraps all three phases, including the checksum check. All `printf` calls are placed outside the measured regions, because printing takes time and would distort the results.

## Observed Performance Differences

The baseline program took 0.229068 s, 0.228629 s and 0.228736 s over 3 runs. The gap between the fastest and slowest run is about 0.4 ms (less than 0.2%), so the measurement is stable.

In the algorithm comparison, the naive implementation took 2.802448 s on average, versus 0.000110 s for the single-pass implementation, on the same 50,000-element array. The naive version is therefore about 25,000 times slower (2.802448 / 0.000110 = ~25,477). It performs n(n+1)/2 = 1,250,025,000 checks instead of 50,000: this is O(n^2) versus O(n), roughly 25,000 times more work, which matches the measurement.

In the instrumentation experiment, the slowest phase is PROCESS, at about 0.00031 s (around 44% of the total). The fastest is REDUCE, at 0.000135 s in all three runs. BUILD_DATA takes about 0.00026 s. TOTAL (0.000710 to 0.000744 s) is slightly larger than the sum of the three phases, by 5 to 6 microseconds, because of the time spent by the `clock()` calls and calculations between the measurements.

## Relation Between Runtime and Energy Consumption

When a program runs longer, the processor stays active longer and therefore consumes more energy. Execution time can thus be used as a proxy for energy consumption. In the algorithm comparison, the naive program keeps the processor busy about 25,000 times longer than the single-pass program to process the same data. At the scale of thousands of servers or millions of executions, this choice of algorithm has a huge impact on energy use.

## Limitations of the Experiment

This experiment has several limitations. First, it measures execution time with `clock()`, not actual power consumption in watts. Second, each program was run only 3 times, which is a small sample. Third, the sandbox is a shared cloud server, so other users' processes can disturb the measurements. Finally, very short times (0.000110 s for the single-pass algorithm, a few microseconds in the instrumentation experiment) are close to the resolution of `clock()` and are therefore less reliable.

## Practical Engineering Takeaway

The main lesson is that the choice of algorithm matters more than anything else: no micro-optimization could compensate for a 25,000x difference. Before optimizing, it is necessary to measure first in order to find where time is really spent: in the instrumentation experiment, it is the PROCESS phase. Finally, measurements should always be repeated several times, and their limitations should be stated clearly.
