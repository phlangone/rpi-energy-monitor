/**
 * Bibliotecas C
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdint.h>

/**
 * Modbus
 */
#include <modbus.h>

/**
 * Application
 */
#include "meter.h"


modbus_t *init_meter(void)
{
    modbus_t *ctx = modbus_new_rtu(MODBUS_PORT, MODBUS_BAUD, 'N', 8, 2);
    if (ctx == NULL)
    {
        perror("modbus_new_rtu");
        return NULL;
    }

    modbus_set_response_timeout(ctx, MODBUS_TIMEOUT, 0);
    modbus_set_slave(ctx, MODBUS_SLAVE);

    if (modbus_connect(ctx) < 0)
    {
        perror("modbus_connect");
        modbus_free(ctx);
        return NULL;
    }

    return ctx;
}

void deinit_meter(modbus_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }

    modbus_close(ctx);
    modbus_free(ctx);
}

float read_frequency(modbus_t *ctx)
{
    uint32_t raw = modbus_read32(ctx, 105);
    return t5_to_float(raw);
}

float read_voltage(modbus_t *ctx)
{
    uint32_t raw = modbus_read32(ctx, 107);
    return t5_to_float(raw);
}

float read_current(modbus_t *ctx)
{
    uint32_t raw = modbus_read32(ctx, 126);
    return t5_to_float(raw);
}

float read_active_power(modbus_t *ctx)
{
    uint32_t raw = modbus_read32(ctx, 140);
    return t6_to_float(raw);
}

float read_reactive_power(modbus_t *ctx)
{
    uint32_t raw = modbus_read32(ctx, 148);
    return t6_to_float(raw);
}

float read_apparent_power(modbus_t *ctx)
{
    uint32_t raw = modbus_read32(ctx, 156);
    return t5_to_float(raw);
}

float read_power_factor(modbus_t *ctx)
{
    uint32_t raw = modbus_read32(ctx, 164);
    return t7_to_float(raw);
}

uint16_t modbus_read16(modbus_t *ctx, int addr)
{
    uint16_t reg;

    if (modbus_read_input_registers(ctx, addr, 1, &reg) != 1)
    {
        perror("modbus_read_input_registers");
        return 0;
    }

    return reg;
}

uint32_t modbus_read32(modbus_t *ctx, int addr)
{
    uint16_t regs[2];

    if (modbus_read_input_registers(ctx, addr, 2, regs) != 2)
    {
        perror("modbus_read_input_registers");
        return 0;
    }

    return ((uint32_t)regs[0] << 16) | regs[1];
}

float t5_to_float(uint32_t n)
{
    if (n == 0xFFFFFFFFU)
    {
        return 0.0f;
    }

    uint32_t s = (n & 0x80000000U) >> 31;
    int32_t  e = (n & 0x7F000000U) >> 24;

    if (s == 1U)
    {
        e -= 0x80;
    }

    uint32_t m = n & 0x00FFFFFFU;

    return (float)m * powf(10.0f, (float)e);
}

float t6_to_float(uint32_t n)
{
    if (n == 0xFFFFFFFFU)
    {
        return 0.0f;
    }

    uint32_t s = (n & 0x80000000U) >> 31;
    int32_t  e = (n & 0x7F000000U) >> 24;

    if (s == 1U)
    {
        e -= 0x80;
    }

    int32_t m = (int32_t)(n & 0x00FFFFFFU);

    if (m & 0x00800000)
    {
        m |= 0xFF000000;
    }

    return (float)m * powf(10.0f, (float)e);
}

float t7_to_float(uint32_t n)
{
    if (n == 0xFFFFFFFFU)
    {
        return 0.0f;
    }

    /* Magnitude do Power Factor (16 bits menos significativos) */
    uint16_t mv = (uint16_t)(n & 0x0000FFFFU);

    /* Finder T7: valor = mv / 10000 */
    return (float)mv * 0.0001f;
}
