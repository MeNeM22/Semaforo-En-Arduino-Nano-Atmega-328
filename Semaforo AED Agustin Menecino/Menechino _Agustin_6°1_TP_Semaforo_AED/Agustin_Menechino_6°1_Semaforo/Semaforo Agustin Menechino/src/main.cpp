#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"

void config_TIMER0(void)
{
  TCCR0A = (1 << WGM01);   // Activa el bit CTC (clear timer on compare match)
                           // del TCCR0A (timer counter/control register)
  OCR0A  = 83;                                   
  TCCR0B = (1 << CS02);    // valor de preescaler en 256
  TIMSK0 = (1 << OCIE0A);  // Habilita las interrupciones entimer compare
}

void Semaforo_MEF(void)
{
  switch (Estado_semaforo)
  {
  case ESTADO1:
    PORTD = (1 << PD4); //Prende la luz roja del semaforo 1
    if (ct == estado_de_trafico[tiempo_trafico][0])
    {
      Estado_semaforo = ESTADO2;
      ct = 0;
    }
    PORTB = (1 << PB2); //Prende la luz verde del semaforo 2
    if (ct == estado_de_trafico[tiempo_trafico][1])
    {
      Estado_semaforo = ESTADO2;
      ct = 0;
    }
    break;

  case ESTADO2:
    PORTD = (1 << PD4) | (1 << PD5); //Prende la luz rojo / amarilla del semaforo 1
    if (ct == 3000)
    {
      Estado_semaforo = ESTADO3;
      ct = 0;
    }
    PORTB = (1 << PB1); //Prende la luz amarilla del semaforo 2
    if (ct == 5000)
    {
      Estado_semaforo = ESTADO3;
      ct = 0;
    }
    break;

  case ESTADO3:
    PORTD = (1 << PD6); //Prende la luz verde del semaforo 1
    if (ct == estado_de_trafico[tiempo_trafico][1])
    {
      Estado_semaforo = ESTADO4;
      ct = 0;
    }
    PORTB = (1 << PB0); //Prende la luz roja del semaforo 2
    if (ct == estado_de_trafico[tiempo_trafico][0])
    {
       Estado_semaforo = ESTADO4;
      ct = 0;
    }
    break;

 case ESTADO4:
   PORTD = (1 << PD5); //Prende la luz amarilla del semaforo 1
   if (ct == 5000)
   {
     Estado_semaforo = ESTADO5;
     ct = 0;
   }
   PORTB = (1 << PB0) | (1 << PB1); //Prende la luz rojo y amarillo de semaforo 2
   if (ct == 3000)
   {  
     Estado_semaforo = ESTADO5;
     ct = 0;
   }
   break;

   case ESTADO5:
   PORTD = (1 << PD4); //Prende la luz roja del semaforo 1
    if (ct == estado_de_trafico[tiempo_trafico][0])
    {
      Estado_semaforo = ESTADO2;
      ct = 0;
    }
    PORTB = (1 << PB2); //Prende la luz verde del semaforo 2
    if (ct == estado_de_trafico[tiempo_trafico][1])
    {
      Estado_semaforo = ESTADO2;
      ct = 0;
    }
   break;

   }
 }

void Antirrebote_P1(void)
{
  switch (PULSADOR1)
  {
  case PULSADOR_ARRIBA:
    flag = 0;
    if bit_is_clear (PINC, PINC0) //Si el pulsador 1 es presionado
    {
      PULSADOR1 = PULSADOR_PRESIONADO;
      ct_antirrebote = Valor_temporizador;
    }
    break;
   
  case PULSADOR_PRESIONADO:
    if (bit_is_set(PINC, PC0))
      PULSADOR1 = PULSADOR_ARRIBA;
    if (bit_is_clear(PINC, PINC0) && ct_antirrebote == 0)
    {
      tiempo_trafico++; //Se le suma 1 a tiempo_trafico
      PULSADOR1 = PULSADOR_ABAJO;
    }

    break;

  case PULSADOR_ABAJO:

    if (bit_is_set(PINC, PINC0))//Si el pulsador 1 es liberado
    {
      PULSADOR1 = PULSADOR_LIBERADO;
      ct_antirrebote = Valor_temporizador;
    }

    flag = 1;
    if (flag == 1)
    {
      ct = 0;
      Semaforo = Sem_default;
      flag = 0;
    }
    if (tiempo_trafico == 3)
    {
      tiempo_trafico = 0;
    }
    break;

  case PULSADOR_LIBERADO:
    PULSADOR1 = PULSADOR_ARRIBA;
    break;
  }
}


void Antirrebote_P2(void)
{
  switch (PULSADOR2)
  {
  case PULSADOR_ARRIBA:
    flag = 0;
    if bit_is_clear (PINC, PINC1) //Si el pulsador 2 es presionado
    {
      PULSADOR2 = PULSADOR_PRESIONADO;
      ct_antirrebote = Valor_temporizador;
    }
    break;

  case PULSADOR_PRESIONADO:
    if (bit_is_set(PINC, PINC1))
      PULSADOR2 = PULSADOR_ARRIBA;
    if (bit_is_clear(PINC, PINC1) && ct_antirrebote == 0)
      PULSADOR2 = PULSADOR_ABAJO;
    break;

  case PULSADOR_ABAJO:
    flag = 1;
    if (flag == 1)
    {
      PORTB = 0x00;
      PORTD = 0x00;
      Semaforo = Sem_nocturno;
      flag = 0;
      tiempo_nocturno = 0;
    }

    if (bit_is_set(PINC, PINC1))
    {
      PULSADOR2 = PULSADOR_LIBERADO; //Si el pulsador 2 es liberado
      ct_antirrebote = Valor_temporizador;
    }
    break;

  case PULSADOR_LIBERADO:
    PULSADOR2 = PULSADOR_ARRIBA;
    break;
  }
}

void Antirrebote_P3(void)
{
  switch (PULSADOR3)
  {
  case PULSADOR_ARRIBA:
    flag = 0;
    if bit_is_clear (PINC, PINC2) //Si el pulsador 3 es presionado
    {
      PULSADOR3 = PULSADOR_PRESIONADO;
      ct_antirrebote = Valor_temporizador;
    }
    break;

  case PULSADOR_PRESIONADO:
    if (bit_is_set(PINC, PINC2))
      PULSADOR3 = PULSADOR_ARRIBA;
    if (bit_is_clear(PINC, PINC2) && ct_antirrebote == 0)
      PULSADOR3 = PULSADOR_ABAJO;
    break;

  case PULSADOR_ABAJO:

    if (bit_is_set(PINC, PINC2)) //Si el pulsador 3 es liberado
    {
      PULSADOR3 = PULSADOR_LIBERADO;
      ct_antirrebote = Valor_temporizador;
    }

    flag = 1;
    if (flag == 1)
    {
      PORTB = 0x00;
      PORTD = 0x00;
      Semaforo = Sem_precaucion;
      flag = 0;
      tiempo_nocturno = 0;
    }
    break;

  case PULSADOR_LIBERADO:
    PULSADOR3 = PULSADOR_ARRIBA;
    break;
  }
}

void Antirrebote_P4(void)
{
  switch (PULSADOR4)
  {
  case PULSADOR_ARRIBA:
    flag = 0;
    if bit_is_clear (PINC, PINC3) //Si el pulsador 4 es presionado
    {
      PULSADOR4 = PULSADOR_PRESIONADO;
      ct_antirrebote = Valor_temporizador;
    }
    break;

  case PULSADOR_PRESIONADO:
    if (bit_is_set(PINC, PINC3))
      PULSADOR4 = PULSADOR_ARRIBA;
    if (bit_is_clear(PINC, PINC3) && ct_antirrebote == 0)
      PULSADOR4 = PULSADOR_ABAJO;
    break;

  case PULSADOR_ABAJO:

    if (bit_is_set(PINC, PINC3)) //Si el pulsador 4 es liberado
    {
      PULSADOR4 = PULSADOR_LIBERADO;
      ct_antirrebote = Valor_temporizador;
    }

    flag = 1;
    if (flag == 1)
    {
      PORTB = 0b00000000;
      PORTD = 0b00000000;
      Semaforo = Sem_fuera_servicio;
      flag = 0;
      tiempo_nocturno = 0;
    }
    break;

  case PULSADOR_LIBERADO:
    PULSADOR4 = PULSADOR_ARRIBA;
    break;
  }
}

void Sem_Modo_nocturno(void)
{

  if (tiempo_nocturno > 500)
  {
    PORTD ^= (1 << PORTD5); //Invierte el estado
    PORTB ^= (1 << PORTB0);

    tiempo_nocturno = 0;
  }
}

void Sem_Modo_precaucion(void)
{
  if (tiempo_nocturno == 500)
  {
    PORTD ^= (1 << PORTD4);
    PORTB ^= (1 << PORTB0);
    tiempo_nocturno = 0;
  }
}

void Sem_Fuera_servicio(void)
{
  if (tiempo_nocturno == 500)
  {
    PORTD ^= (1 << PORTD4); //Invierte el estado
    PORTD ^= (1 << PORTD5);
    PORTD ^= (1 << PORTD6);

    PORTB ^= (1 << PORTB0);
    PORTB ^= (1 << PORTB1);
    PORTB ^= (1 << PORTB2);

    tiempo_nocturno = 0;
  }
}

int main(void)
{
  DDRD |= (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
  DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4);
  config_TIMER0();
  sei(); //Habilita interrupciones globales

  while (1)
  {
  Antirrebote_P1(); //Llama las funciones de antirrebote de los displays.
  Antirrebote_P2();
  Antirrebote_P3();
  Antirrebote_P4();

  switch (Semaforo)
  {
  case Sem_default:
    Semaforo_MEF();

    break;

  case Sem_nocturno:
    Sem_Modo_nocturno();
    break;

  case Sem_precaucion:
    Sem_Modo_precaucion();
    break;

  case Sem_fuera_servicio:
    Sem_Fuera_servicio();
    break;

  default:
    Semaforo = Sem_default;
    break;
  }
  }
}

ISR(TIMER0_COMPA_vect)
{
  tiempo_nocturno++;
  ct++;

  if (ct_antirrebote != 0) //Si ct_antirrebote no es igual a 0.
  {
    ct_antirrebote--; //se le resta 1.
  }
  else 
  {
    ct_antirrebote = Valor_temporizador;
  }
}