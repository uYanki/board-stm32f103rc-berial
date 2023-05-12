Cortex-M 里有个 DWT（Data Watchpoint and Trace）外设，是个 32 位的向上计数器（CYCCNT），记录的是内核时钟运行的个数。

2^32 = 4,294,967,296 ≈ 4.3GHz, 当 CYCCNT 溢出之后，会清 0 重新向上计数。

假设内核频率为 72M，内核跳一次的时间大概为 1/72M = 13.8ns，可记录的时长为 59.65s。

注：Event Recorder 的时间测量使用的就是 DWT
