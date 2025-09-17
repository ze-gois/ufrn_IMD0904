#pragma once

#include <Arduino.h>

#define SOUND_SPEED 0.034

int trigPin_entrada = 13;
int echoPin_entrada = 15;
int trigPin_saida = 14;
int echoPin_saida = 27;
int ledVermelho = 25;
int btPin = 34;

long duracaoPulso_entrada, duracaoPulso_saida;
float distanciaCm_entrada, distanciaCm_saida;
int contador = 0;
bool sensorEntrada_ativo = false;
bool sensorSaida_ativo = false;
bool controleAutomaticoAtivo = true;
unsigned long tempoUltimaInteracaoManual = 0;
const unsigned long tempoRetornoControleAutomatico = 60000;
int leitura;
int ultimoEstado = LOW;
const unsigned long debounceDelay = 200;
unsigned long tempoUltimoClique = 0;
int ultimoEstadoLed = -1;