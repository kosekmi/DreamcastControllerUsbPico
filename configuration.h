#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

// Adjust the CPU clock frequency here
#define CPU_FREQ_KHZ 133000

// Adjust the minimum time between each edge here
// 320 ns achieves 2 mbps, just as the dreamcast does
#define MIN_CLOCK_PERIOD_NS 320

// The minimum amount of time we check for an open line before taking control of it
// This should be at least as long as the longest clock period of any device on the line.
// Note: The Dreamcast controller has a period of about 500 ns.
#define OPEN_LINE_CHECK_TIME_US 2

// Added percentage on top of the expected completion time
#define MAPLE_WRITE_TIMEOUT_EXTRA_PERCENT 20

// Maximum amount of time to spend trying to read on the maple bus
// TODO: Add a shorter timeout if read hasn't seen start sequence in X ns
#define MAPLE_READ_TIMEOUT_US 3000

#endif // __CONFIGURATION_H__
