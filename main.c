/**
 * main.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "meter.h"
#include "mqtt.h"

/* Estrutura de energia */
typedef struct
{
    float   current;
    float   powerFactor;
    int16_t voltage;
    int16_t activePower;
    int16_t reactivePower;
    int16_t apparentPower;
    int16_t frequency;
} energy_data_t;

int main(void)
{
    modbus_t *ctx;
    energy_data_t energy;

    ctx = init_meter();
    if (ctx == NULL)
    {
        fprintf(stderr, "Erro ao inicializar o medidor\n");
        return EXIT_FAILURE;
    }

    if (mqtt_init("rpi4-energy") != 0)
    {
        printf("Erro MQTT\n");
        return 1;
    }    

    while (1)
    {
        /* Leituras */
        energy.voltage        = (int16_t)read_voltage(ctx);
        usleep(100000);
        energy.current        = read_current(ctx);
        usleep(100000);
        energy.frequency      = (int16_t)read_frequency(ctx);
        usleep(100000);
        energy.activePower    = (int16_t)read_active_power(ctx);
        usleep(100000); 
        energy.reactivePower  = (int16_t)read_reactive_power(ctx);
        usleep(100000); 
        energy.apparentPower  = (int16_t)read_apparent_power(ctx);
        usleep(100000); 
        energy.powerFactor    = read_power_factor(ctx);
        usleep(100000); 

        /* Exibição */
        printf("---- ENERGY DATA ----\n");
        printf("Voltage        : %d V\n", energy.voltage);
        printf("Current        : %.2f A\n", energy.current);
        printf("Frequency      : %d Hz\n", energy.frequency);
        printf("Active Power   : %d W\n", energy.activePower);
        printf("Reactive Power : %d VAr\n", energy.reactivePower);
        printf("Apparent Power : %d VA\n", energy.apparentPower);
        printf("Power Factor   : %.2f\n", energy.powerFactor);
        printf("----------------------\n\n");

        char json[256];
        snprintf(json, sizeof(json),
            "{"
            "\"voltage\":%d,"
            "\"current\":%.2f,"
            "\"frequency\":%d,"
            "\"active_power\":%d,"
            "\"reactive_power\":%d,"
            "\"apparent_power\":%d,"
            "\"power_factor\":%.2f"
            "}",
            energy.voltage,
            energy.current,
            energy.frequency,
            energy.activePower,
            energy.reactivePower,
            energy.apparentPower,
            energy.powerFactor
        );        

        mqtt_publish("energy/7m", json);

        sleep(5);
    }

	mqtt_cleanup();
    deinit_meter(ctx);
    return EXIT_SUCCESS;
}
