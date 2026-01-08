#include <stdio.h>
#include "serializer.h"

SerializeStatus serialize_to_json(
    const GatewayPacket *p,
    char *output,
    size_t output_size
) {
    if (!p || !output) return SERIALIZE_INVALID_INPUT;

    char *out = output;
    size_t remaining = output_size;

    #define APPEND(fmt, ...) \
        do { \
            int n = snprintf(out, remaining, fmt, ##__VA_ARGS__); \
            if (n < 0 || n >= (int)remaining) return SERIALIZE_BUFFER_TOO_SMALL; \
            out += n; \
            remaining -= n; \
        } while (0)

    APPEND("[{");
    APPEND("\"gatewayId\":\"%s\",", p->gatewayId);
    APPEND("\"date\":\"%s\",", p->date);
    APPEND("\"deviceType\":\"%s\",", p->deviceType);
    APPEND("\"interval_minutes\":%d,", p->interval_minutes);
    APPEND("\"total_readings\":%d,", p->total_readings);

    APPEND("\"values\":{");
    APPEND("\"device_count\":%d,", p->values.device_count);
    APPEND("\"readings\":[");

    for (int i = 0; i < p->values.device_count; i++) {
        const DeviceReading *d = &p->values.readings[i];

        APPEND("{");
        APPEND("\"media\":\"%s\",", d->media);
        APPEND("\"meter\":\"%s\",", d->meter);
        APPEND("\"deviceId\":\"%s\",", d->deviceId);
        APPEND("\"unit\":\"%s\",", d->unit);
        APPEND("\"data\":[");

        for (int j = 0; j < d->data_count; j++) {
            const MeterDataPoint *m = &d->data[j];

            APPEND("{");
            APPEND("\"timestamp\":\"%s\",", m->timestamp);
            APPEND("\"meter_datetime\":\"%s\",", m->meter_datetime);
            APPEND("\"total_m3\":%.3f,", m->total_m3);
            APPEND("\"status\":\"%s\"", m->status);
            APPEND("}%s", (j + 1 < d->data_count) ? "," : "");
        }

        APPEND("]}%s", (i + 1 < p->values.device_count) ? "," : "");
    }

    APPEND("]}}]");
    return SERIALIZE_OK;
}
