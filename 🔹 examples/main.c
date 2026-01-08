#include <stdio.h>
#include <string.h>
#include "serializer.h"

int main(void) {
    GatewayPacket packet = {0};

    strcpy(packet.gatewayId, "gateway_1234");
    strcpy(packet.date, "1970-01-01");
    strcpy(packet.deviceType, "stromleser");
    packet.interval_minutes = 15;
    packet.total_readings = 1;

    packet.values.device_count = 1;

    DeviceReading *d = &packet.values.readings[0];
    strcpy(d->media, "water");
    strcpy(d->meter, "waterstarm");
    strcpy(d->deviceId, "stromleser_50898527");
    strcpy(d->unit, "m3");

    d->data_count = 1;
    strcpy(d->data[0].timestamp, "1970-01-01 00:00");
    strcpy(d->data[0].meter_datetime, "1970-01-01 00:00");
    d->data[0].total_m3 = 107.752f;
    strcpy(d->data[0].status, "OK");

    char buffer[1024];

    if (serialize_to_json(&packet, buffer, sizeof(buffer)) == SERIALIZE_OK) {
        printf("%s\n", buffer);
    }

    return 0;
}
