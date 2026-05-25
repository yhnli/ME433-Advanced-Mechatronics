#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "tusb.h"
#include "usb_descriptors.h"
#include "mpu6050.h"

// I2C defines
#define I2C_PORT i2c0
#define I2C_SDA 4
#define I2C_SCL 5

// GPIO pin defines
#define BTN_PIN 14
#define LED_PIN 15

// Mouse mode defines
typedef enum {
    MODE_ACTIVE = 0,
    MODE_REMOTE = 1
} mouse_mode_t;
static volatile mouse_mode_t current_mode = MODE_ACTIVE;

#define X_SIGN -1
#define Y_SIGN 1

// Helper Functions
// convert IMU acceleration to mouse movement
static int8_t accel_to_mouse(float accel) {
    const float scale_factor = 10.0f; // Adjust this factor as needed
    int8_t movement = (int8_t)(accel * scale_factor);

    // Clamp the movement to the range of -127 to 127
    if (movement > 127) {
        movement = 127;
    } else if (movement < -127) {
        movement = -127;
    }

    return movement;
}

// mode button
static bool last_button_level = true;
static absolute_time_t last_debounce_time;

static void update_mode(void) {
    bool level = gpio_get(BTN_PIN);

    if (level != last_button_level) {
        last_debounce_time = get_absolute_time();
    }

    if (absolute_time_diff_us(last_debounce_time, get_absolute_time()) > 30000) {
        static bool stable_level = true;

        if (level != stable_level) {
            stable_level = level;

            // falling edge = button pressed
            if (stable_level == false) {
                current_mode = (current_mode == MODE_ACTIVE) ? MODE_REMOTE : MODE_ACTIVE;
            }
        }
    }

    last_button_level = level;
}

// mode led
static void update_mode_led(void) {
    uint32_t now_ms = to_ms_since_boot(get_absolute_time());

    if (current_mode == MODE_ACTIVE) {
        // regular mode: solid on
        gpio_put(LED_PIN, 1);
    } else {
        // remote mode: fast blinking
        // toggle every 80 ms
        bool blink_state = ((now_ms / 80) % 2) == 0;
        gpio_put(LED_PIN, blink_state);
    }
}

int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // mpu6050 init
    mpu6050_init();

    // GPIO Initialisation
    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);    
    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
