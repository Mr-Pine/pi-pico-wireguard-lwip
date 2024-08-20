#include "wireguard-platform.h"
#include "pico/time.h"
#include "pico/rand.h"
#include <pico/types.h>
#include <stdbool.h>
#include <stdint.h>
#include "crypto.h"

uint32_t wireguard_sys_now() {
    absolute_time_t time = get_absolute_time();
    return to_ms_since_boot(time);
}

void wireguard_random_bytes(void *bytes, size_t size) {
    size_t i = 0;
    while (i < size) {
        size_t remaining = size - i;
        if (remaining >= 128 / 8) {
            get_rand_128(bytes + i);
            i += 128 / 8;
        } else if (remaining >= 64 / 8) {
            ((uint64_t *) bytes)[i] = get_rand_64();
            i += 64 / 8;
        } else if (remaining >= 32 / 8){
            ((uint32_t *) bytes)[i] = get_rand_32();
            i += 32 / 8;
        } else {
            char buf[4];
            *((uint32_t *) buf) = get_rand_32();
            for (; remaining > 0; remaining--) {
                ((char *)bytes)[size - 1 - remaining] = buf[remaining - 1];
            }
        }
    }
}


void wireguard_tai64n_now(uint8_t *output) {
    // See https://cr.yp.to/libtai/tai64.html
	// 64 bit seconds from 1970 = 8 bytes
	// 32 bit nano seconds from current second

    absolute_time_t now = get_absolute_time();
	uint64_t millis = absolute_time_diff_us(nil_time, now);

	// Split into seconds offset + nanos
	uint64_t seconds = 0x400000000000000aULL + (millis / 1000);
	uint32_t nanos = (millis % 1000) * 1000;
	U64TO8_BIG(output + 0, seconds);
	U32TO8_BIG(output + 8, nanos);
}

bool wireguard_is_under_load() {
    return false;
}
