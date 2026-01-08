#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <stddef.h>

#define MAX_DEVICES 3
#define MAX_DATA_POINTS 3

typedef struct {
    char timestamp[20];
    char meter_datetime[20];
    float total_m3;
    char status[8];
} MeterDataPoint;

typedef struct {
    char media[16];
    char meter[16];
    char deviceId[32];
    char unit[8];
    int data_count;
    MeterDataPoint data[MAX_DATA_POINTS];
} DeviceReading;

typedef struct {
    int device_count;
    DeviceReading readings[MAX_DEVICES];
} ValuesBlock;

typedef struct {
    char gatewayId[32];
    char date[11];
    char deviceType[16];
    int interval_minutes;
    int total_readings;
    ValuesBlock values;
} GatewayPacket;

typedef enum {
    SERIALIZE_OK = 0,
    SERIALIZE_BUFFER_TOO_SMALL,
    SERIALIZE_INVALID_INPUT
} SerializeStatus;

SerializeStatus serialize_to_json(
    const GatewayPacket *input,
    char *output_buffer,
    size_t buffer_size
);

#endif
