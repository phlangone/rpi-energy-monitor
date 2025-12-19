#include <stdio.h>
#include <string.h>
#include <mosquitto.h>

static struct mosquitto *mosq = NULL;

int mqtt_init(const char *client_id)
{
    mosquitto_lib_init();

    mosq = mosquitto_new(client_id, true, NULL);
    if (!mosq)
        return -1;

    if (mosquitto_connect(mosq, "localhost", 1883, 60) != MOSQ_ERR_SUCCESS)
        return -2;

    return 0;
}

int mqtt_publish(const char *topic, const char *payload)
{
    if (!mosq)
        return -1;

    return mosquitto_publish(
        mosq,
        NULL,
        topic,
        strlen(payload),
        payload,
        0,
        false
    );
}

void mqtt_cleanup(void)
{
    if (mosq)
    {
        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
    }
    mosquitto_lib_cleanup();
}
