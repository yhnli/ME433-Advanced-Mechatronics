#ifndef HX711_H
#define HX711_H

#include "pico/stdlib.h"

#define DT 15
#define SCK 14

#define clock_time_us 10

// function prototypes
// function to init hx711
void init_hx711();
// function to read hx711
int read_hx711();

#endif