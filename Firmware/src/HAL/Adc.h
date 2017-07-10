#pragma once

#include <stdint.h>
#include <cmath>

class Pin;

// define how many bits of extra resolution required
// 2 bits means the 12bit ADC is 14 bits of resolution
#define OVERSAMPLE 2

class Adc
{
public:
    static Adc *getInstance(int n) { return instances[n]; }

    Adc();
    virtual ~Adc();
    static bool setup();
    static bool start();
    static bool stop();

    // specific to each instance
    Adc* from_string(const char *name);
    uint32_t read();
    int get_channel() const { return channel; }
    bool connected() const { return enabled; }

    // return the maximum ADC value, base is 10bits 1024.
#ifdef OVERSAMPLE
    static int get_max_value() { return 1024 << OVERSAMPLE;}
#else
    static int get_max_value() { return 1024;}
#endif

private:
    static const int num_channels= 8;
    static Adc* instances[num_channels];
    static int ninstances;
    static int sample_isr(int irq, void *context, FAR void *arg);

#ifdef OVERSAMPLE
    // we need 4^n sample to oversample and we get double that to filter out spikes
    static const int num_samples= powf(4, OVERSAMPLE)*2;
#else
    static const int num_samples= 8;
#endif

    // instance specific fields
    void new_sample(uint32_t value);

    bool enabled{false};
    int channel;
    int instance_idx{-1};
    // buffer storing the last num_samples readings for each channel instance
    uint16_t sample_buffer[num_samples];
    uint16_t ave_buf[4]{0};
};

