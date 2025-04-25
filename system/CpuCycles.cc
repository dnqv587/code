#include <iostream>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
static inline uint64_t rdtsc()
{
    size_t lo, hi;
    __asm__ __volatile__("rdtsc" : "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

class CpuCycles {
public:
    CpuCycles() {
        if (cyclesPerSec > 1000)
            return;
        struct timeval startTime, stopTime;
        uint64_t startCycles, stopCycles, micros;
        double oldCycles;

        oldCycles = 0;
        while (1) {
            gettimeofday(&startTime, NULL);
            startCycles = rdtsc();
            while (1) {
                gettimeofday(&stopTime, NULL);
                stopCycles = rdtsc();
                micros = (stopTime.tv_usec - startTime.tv_usec) +
                        (stopTime.tv_sec - startTime.tv_sec)*1000000;
                if (micros > 10000) {
                    cyclesPerSec = static_cast<double>(stopCycles - startCycles);
                    cyclesPerSec = 1000000.0*cyclesPerSec/
                            static_cast<double>(micros);
                    break;
                }
            }
            double delta = cyclesPerSec/100000.0;
            if ((oldCycles > (cyclesPerSec - delta)) &&
                    (oldCycles < (cyclesPerSec + delta))) {
                goto calc_diff;
            }
            oldCycles = cyclesPerSec;
        }
    calc_diff:
        CalcDiff();
    }

    uint64_t GetTimeStampNs() {
        if (diff == 0) {
            CalcDiff();
        }
        uint64_t cycles = rdtsc();
        uint64_t timeNs = 1e9 * cycles / cyclesPerSec + diff;
        return timeNs;
    }

private:
    void CalcDiff() {
        if (diff == 0) {
            struct timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts); //这里又是clock_, 上面是gettimeofday
            uint64_t cycles = rdtsc();

            uint64_t t1 = ts.tv_nsec + ts.tv_sec * 1e9;
            uint64_t t2 = 1e9 * cycles / cyclesPerSec;

            diff = t1 - t2;
        }
    }

    double cyclesPerSec = 0;
    int64_t diff = 0;
};

int main(){

    CpuCycles cpuCycles;
    std::cout<<cpuCycles.GetTimeStampNs()<<std::endl;

    return 0;
}
