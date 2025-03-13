#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>
#include <sys/sysctl.h>
#include <sys/sensors.h>


int main(void)
{
    int mib[5] = {CTL_HW, HW_SENSORS};

    struct sensordev sensordev;
    if (sysctl(mib, 3, &sensordev, &(size_t){sizeof(sensordev)}, NULL, 0) < 0)
        err(1, "sysctl()");

    for (mib[2] = 0; sysctl(mib, 3, &sensordev, &(size_t){sizeof(sensordev)}, NULL, 0) >= 0; ++mib[2])
    {
        if (memcmp(sensordev.xname, "acpibat", 7))
            continue;

        int64_t max, cur;

        mib[3] = SENSOR_WATTHOUR;
        struct sensor sensor;

        mib[4] = 0;
        if (sysctl(mib, 5, &sensor, &(size_t){sizeof(sensor)}, NULL, 0) < 0)
            err(1, "sysctl()");
        max = sensor.value;
        if (max == 0)
            continue;

        mib[4] = 3;
        if (sysctl(mib, 5, &sensor, &(size_t){sizeof(sensor)}, NULL, 0) < 0)
            err(1, "sysctl()");
        cur = sensor.value;

        printf("%s %d\n", sensordev.xname + 7, (int)round(((double)cur / max) * 100));
    }
}
