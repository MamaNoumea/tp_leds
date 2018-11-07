

#include "contiki.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "net/rime/trickle.h"
#include "net/rime.h"

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
static struct etimer et_hello;
static uint8_t blinks;
/*---------------------------------------------------------------------------*/
PROCESS(blink_process, "LED blink process");
PROCESS(example_abc_process, "ABC example");
AUTOSTART_PROCESSES(&blink_process, &example_abc_process);
/*---------------------------------------------------------------------------*/
static void
abc_recv(struct abc_conn *c)
{
  printf("abc message received '%s'\n", (char *)packetbuf_dataptr());
  leds_on(2);
}
static const struct abc_callbacks abc_call = {abc_recv};
static struct abc_conn abc;

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(blink_process, ev, data)
{
  PROCESS_BEGIN();

  SENSORS_ACTIVATE(button_sensor);

  blinks = 0;

  while(1) {

    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event &&
			     data == &button_sensor);
    if ((blinks==0) || (blinks == 4)){
	leds_off(blinks);
	blinks = 1;
    	leds_on(blinks);
	printf("Blink... (state %0.2X)\n", leds_get());
    }
    else if (blinks == 1){
	leds_off(blinks);	
	blinks = 4;
    	leds_on(blinks);
	printf("Blink... (state %0.2X)\n", leds_get());
    }
  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_abc_process, ev, data)
{
  PROCESS_BEGIN();

  etimer_set(&et_hello, CLOCK_SECOND * 4);

  while(1) {
    leds_off(2);
    PROCESS_WAIT_EVENT();

    if(ev == PROCESS_EVENT_TIMER) {
      leds_on(2);
      PROCESS_WAIT_EVENT_UNTIL(0.02);
      etimer_reset(&et_hello);
    }
  }

  PROCESS_END();


  /*PROCESS_EXITHANDLER(abc_close(&abc);)

  PROCESS_BEGIN();

  abc_open(&abc, 128, &abc_call);

  PROCESS_END();*/
}  
/*---------------------------------------------------------------------------*/
