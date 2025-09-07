enum class LevelMode { RMS, Peak };
#include "rms.h"
#include "peak.h"

class LevelTracker {
public:
    LevelTracker(int bufferSize)
        : rms(bufferSize), peak(4410, 0.9995f) {}

    void setMode(LevelMode newMode) { mode = newMode; }

    float trackSignal(float input) {
        if (mode == LevelMode::RMS) {
            return rms.trackSignal(input);
        } else {
            return peak.trackSignal(input);
        }
    }

    void reset() {
        rms.resetRmsSize();
        peak.reset();
    }

private:
    Rms rms;
    Peak peak;
    LevelMode mode = LevelMode::RMS;
};

