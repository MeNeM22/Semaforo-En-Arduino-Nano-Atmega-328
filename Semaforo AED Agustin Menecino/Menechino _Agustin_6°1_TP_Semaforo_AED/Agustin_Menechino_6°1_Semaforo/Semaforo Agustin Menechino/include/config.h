#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define Valor_temporizador 50
uint8_t  cont_sem = 0;
uint8_t  tiempo_trafico = 0;
uint8_t  flag = 0;
uint8_t  ct_antirrebote = Valor_temporizador;
uint16_t tiempo_nocturno = 0;
uint32_t ct = 0;
                                // Poco trafico     Medio trafico     Mucho trafico
uint32_t estado_de_trafico[3][2] ={{5000 , 3000}, {3000 , 3000}, { 2000 , 5000}}; // Matriz con 6 elementos
                              //0 rojo, 1 verde //0 rojo, 1 verde //0 rojo, 1 verde

typedef enum Estado_semaforo
{
  ESTADO1,      //Estado rojo  
  ESTADO2,      //Estado rojo y amarillo
  ESTADO3,      //Estado verde
  ESTADO4,      //Estado amarillo
  ESTADO5 //Estado final
} Estado_semaforo_t;
Estado_semaforo_t Estado_semaforo = ESTADO1;

typedef enum Funciones_semaforo
{
  Sem_default,
  Sem_nocturno,
  Sem_precaucion,
  Sem_fuera_servicio
} Mod_semaforo_t;
Mod_semaforo_t Semaforo = Sem_default;

typedef enum antirrebote
{
  PULSADOR_ARRIBA,
  PULSADOR_PRESIONADO,
  PULSADOR_ABAJO,
  PULSADOR_LIBERADO
} Estado_Pulsador_t;
Estado_Pulsador_t PULSADOR1 = PULSADOR_ARRIBA;
Estado_Pulsador_t PULSADOR2 = PULSADOR_ARRIBA;
Estado_Pulsador_t PULSADOR3 = PULSADOR_ARRIBA;
Estado_Pulsador_t PULSADOR4 = PULSADOR_ARRIBA;