# RPi Energy Monitor

Este projeto é uma aplicação em linguagem C desenvolvida para Raspberry Pi (especificamente projetada para RPi 4). O sistema atua como um gateway de aquisição de dados de energia, lendo grandezas elétricas de um medidor via protocolo Modbus e publicando essas informações periodicamente em um broker MQTT no formato JSON.

O sistema foi desenhado para aplicações de automação e monitoramento industrial/residencial, permitindo a análise de consumo, fator de potência e qualidade de energia.

## Funcionalidades

A aplicação realiza a leitura sequencial (com intervalos de 100ms para estabilidade do barramento) das seguintes grandezas:
* Tensão (V)
* Corrente (A)
* Frequência (Hz)
* Potência Ativa (W)
* Potência Reativa (VAr)
* Potência Aparente (VA)
* Fator de Potência

## Arquitetura de Software

O código principal (`main.c`) depende de dois módulos customizados para abstrair a comunicação de hardware e rede:
* `meter.h`: Responsável pela inicialização e comunicação Modbus com o medidor de energia físico.
* `mqtt.h`: Responsável por gerenciar a conexão com o broker MQTT e a publicação das mensagens.

## Formato de Dados (Payload MQTT)

Após realizar as leituras, os dados são exibidos no console local e formatados em uma string JSON. A publicação ocorre a cada 5 segundos no tópico `energy/7m`.

**Tópico MQTT:** `energy/7m`  
**Client ID:** `rpi4-energy`  

**Exemplo de Payload JSON:**
```json
{
  "voltage": 220,
  "current": 5.42,
  "frequency": 60,
  "active_power": 1100,
  "reactive_power": 150,
  "apparent_power": 1110,
  "power_factor": 0.95
}
