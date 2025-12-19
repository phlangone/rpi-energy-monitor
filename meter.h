#ifndef METER_H
#define METER_H

#include <stdint.h>
#include <modbus.h>

#define MODBUS_PORT  "/dev/ttyS0"
#define MODBUS_BAUD  38400
#define MODBUS_SLAVE 1
#define MODBUS_TIMEOUT 2

modbus_t *init_meter(void);
void deinit_meter(modbus_t *ctx);

float read_frequency(modbus_t *ctx);
float read_voltage(modbus_t *ctx);
float read_current(modbus_t *ctx);
float read_active_power(modbus_t *ctx);
float read_reactive_power(modbus_t *ctx);
float read_apparent_power(modbus_t *ctx);
float read_power_factor(modbus_t *ctx);

uint16_t modbus_read16(modbus_t *ctx, int addr);
uint32_t modbus_read32(modbus_t *ctx, int addr);

float t5_to_float(uint32_t n);
float t6_to_float(uint32_t n);
float t7_to_float(uint32_t n);

#endif
