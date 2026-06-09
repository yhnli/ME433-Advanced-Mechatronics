#include <stdio.h>
#include "pico/stdlib.h"
#include "hx711.h"

int main()
{
    // init
    stdio_init_all();
    sleep_ms(1000);
    init_hx711();

    while (true) {
        int num = 0;
        printf("Send number of samples to collect: ");
        scanf("%d", &num);

        int v[1000];
        int raw_v[1000];
        uint64_t t[1000];
        int iir = 828000; // initial value estimate

        for (int i = 0; i < num; i++) {
            // raw value
            int raw_val = read_hx711();
            raw_v[i] = raw_val;
            // iir filtered value
            iir = 0.1*raw_val + 0.9*iir;
            v[i] = (int)iir;
            // time in milliseconds
            t[i] = to_ms_since_boot(get_absolute_time());
        }

        // print all data back
        for (int i = 0; i < num; i++) {
            printf("%llu %d %d\r\n", t[i], raw_v[i], v[i]);
        }
    }
}
