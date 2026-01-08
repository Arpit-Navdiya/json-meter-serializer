# json-meter-serializer
JSON Serialization Library for w-M-Bus–Related Firmware
Project Overview

This project implements an embedded-friendly C library that serializes structured smart-meter data into a strictly predefined JSON format.

The library represents a typical firmware data flow used in smart-meter gateways:
Measurement data → Internal data structures → JSON serialization → Backend interface
Note: w-M-Bus communication, OMS protocol, encryption, and real meter interaction are intentionally out of scope.
The focus is on software architecture, API design, memory safety, and correctness.

Chosen Platform & Language
Programming Language

C

Justification:

Widely used in embedded and firmware development

Allows strict control over memory usage

Suitable for microcontrollers (STM32, ESP32, etc.)

No external dependencies required

Development Platform

PC-based development (GCC / Online Compiler)

Justification:

Same C code can be reused on embedded targets

Faster iteration and testing

No hardware dependency for serialization logic

Common practice for validating core firmware modules

/
├── src/            # Serialization logic (serializer.c)
├── include/        # Public API and data structures (serializer.h)
├── examples/       # Demo application (main.c)
├── README.md
└── LICENSE         # Optional

Data Model Design

The internal data structures mirror the required JSON hierarchy while remaining embedded-safe.

Key characteristics:

Fixed-size arrays (no dynamic allocation)

No use of malloc

Deterministic memory usage

Clear separation between gateway metadata and meter readings

Main structures:

GatewayPacket

ValuesBlock

DeviceReading

MeterDataPoint

Public API
Function Prototype

SerializeStatus serialize_to_json(
    const GatewayPacket *input,
    char *output_buffer,
    size_t buffer_size
);

Parameters

input – Pointer to populated data structure

output_buffer – Caller-provided buffer for JSON output

buffer_size – Size of the output buffer in bytes

Return Values

SERIALIZE_OK – Serialization successful

SERIALIZE_BUFFER_TOO_SMALL – Output buffer insufficient

SERIALIZE_INVALID_INPUT – Null or invalid input

Design Notes

Transport-agnostic (no UART, MQTT, radio, etc.)

Prevents buffer overflows using snprintf

Const-correct input handling

Suitable for bare-metal and RTOS environments


JSON Output Format (Mandatory)

The library generates exactly the following structure

[
  {
    "gatewayId": "gateway_1234",
    "date": "1970-01-01",
    "deviceType": "stromleser",
    "interval_minutes": 15,
    "total_readings": 1,
    "values": {
      "device_count": 1,
      "readings": [
        {
          "media": "water",
          "meter": "waterstarm",
          "deviceId": "stromleser_50898527",
          "unit": "m3",
          "data": [
            {
              "timestamp": "1970-01-01 00:00",
              "meter_datetime": "1970-01-01 00:00",
              "total_m3": 107.752,
              "status": "OK"
            }
          ]
        }
      ]
    }
  }
]

Important constraints:

Field names are fixed

JSON structure must not change

Numbers are not serialized as strings

Outer JSON element is always an array

No additional fields allowed

Build & Run Instructions
Using GCC (PC / Linux / Windows)

gcc main.c -o serializer
./serializer

Using Online Compiler

Open OnlineGDB / Programiz / Replit

Select C

Paste the source code

Click Run

The generated JSON is printed to the console.

Example Application

The demo application:

Creates sample gateway and meter data

Calls the serialization library

Outputs the resulting JSON to standard output

This demonstrates correct usage of the API and validates the JSON format.

Embedded Suitability Considerations

No dynamic memory allocation

Fixed upper bounds for devices and data points

Deterministic execution time

Clear error handling

Easily portable to STM32 / ESP32 firmware projects
Design Decisions & Assumptions

Maximum number of devices and data points is compile-time configurable

Floating-point values are serialized using snprintf

String lengths are chosen conservatively for embedded use

Serialization logic is separated from transport/output logic

Possible Extensions

Support for additional meter units or media types

Binary-to-JSON gateway integration

Streaming serialization for very large datasets

Optional CRC or checksum generation

Integration with UART, MQTT, or file systems

Context

This project simulates a real-world firmware task commonly found in smart-meter gateways and w-M-Bus systems, where reliable and deterministic data serialization is critical for backend integration.

Author

Arpit Navdiya
Embedded / Firmware Developer
