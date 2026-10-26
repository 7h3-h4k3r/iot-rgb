#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/sys/printk.h>

#define PWMGLED DT_NODELABEL(gled)
#define PWMBLED DT_NODELABEL(bled)
#define PWMRLED DT_NODELABEL(rled)

static const struct pwm_dt_spec gled = 
    PWM_DT_SPEC_GET(PWMGLED);

static const struct pwm_dt_spec bled = 
    PWM_DT_SPEC_GET(PWMBLED);

static const struct pwm_dt_spec rled =
    PWM_DT_SPEC_GET(PWMRLED);


int set_rgb(uint8_t r ,uint8_t g , uint8_t b){
    uint32_t r_pulse =
        ((uint64_t)rled.period * r) / 255;

    uint32_t g_pulse =
        ((uint64_t)gled.period * g) / 255;

    uint32_t b_pulse =
        ((uint64_t)bled.period * b) / 255;
    
    int ret;

    ret = pwm_set_pulse_dt(&rled,r_pulse);
    if (ret){
        return ret;
    }
    ret = pwm_set_pulse_dt(&bled,b_pulse);
    if (ret){
        return ret;
    }
    ret = pwm_set_pulse_dt(&gled,g_pulse);
    if (ret){
        return ret;
    }

    return 0;

}



int main(void)
{
    int ret;

    printk("RP2040: Smooth Full RGB Traversal\n");

    if (!pwm_is_ready_dt(&rled) ||
        !pwm_is_ready_dt(&gled) ||
        !pwm_is_ready_dt(&bled)) {

        printk("PWM device not ready!\n");
        return 0;
    }


    printk("Smooth RGB Colour Transition\n");

    while (1) {
        /* Red -> Yellow */
        for (int g = 0; g <= 255; g++) {
            set_rgb(255, g, 0);
            k_msleep(10);
        }

        /* Yellow -> Green */
        for (int r = 255; r >= 0; r--) {
            set_rgb(r, 255, 0);
            k_msleep(10);
        }

        /* Green -> Cyan */
        for (int b = 0; b <= 255; b++) {
            set_rgb(0, 255, b);
            k_msleep(10);
        }

        /* Cyan -> Blue */
        for (int g = 255; g >= 0; g--) {
            set_rgb(0, g, 255);
            k_msleep(10);
        }

        /* Blue -> Magenta */
        for (int r = 0; r <= 255; r++) {
            set_rgb(r, 0, 255);
            k_msleep(10);
        }

        /* Magenta -> Red */
        for (int b = 255; b >= 0; b--) {
            set_rgb(255, 0, b);
            k_msleep(10);
        }
    }

    return 0;
}
