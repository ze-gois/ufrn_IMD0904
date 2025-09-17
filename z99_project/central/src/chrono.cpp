#include "./chrono.h"

static unsigned long time_local_last_update = 0;
static unsigned long time_local_update_interval = 60000;

void time_local_update() {
    if (millis() - time_local_last_update < time_local_update_interval) {
        return;
    }
    if (getLocalTime(&time_local)) {
        time_local_last_update = millis();
    }
}