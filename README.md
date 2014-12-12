cc2538em
========
openwsn port for CC2538EM

----------------------------------------
Renato Fernandes
12/dez/2014
This version has:
1) Include the MYLINKXS_REMOTE_CONTROL that you can configure up to 5 buttons from a remote control
   to be controled by the IOT.
  This version use the ARDUINO SENSOR MOTE using almost the same structure of the OpenSensor.
  In order to use this feature you need to enable the MYLINKXS_REMOTE_CONTROL in the osens.h
  using the UART1 in the pins 18 and 19 as the OSENS
  TODO!!!! Only test with the Phillips Control RC5 that has 12 bits.
  DOes not implemented CRC in the serial communication.
  
2) Include the MYLINKXS_LIGHT_CONTROL, that used to control a light using IOT. This feature 
   use only the I/O pins directly from the board.   
  In order to use this feature you need to enable the MYLINKXS_LIGHT_CONTROL in the osens.h

3) Serial debug that use UART0 to debug using DBG_LOG function.

----------------------------------------
08_nov_2014
renfernand:
Update OSENS
Alteracoes:
Para ficar compativel com o ultimo firmware de berkley fiz o merge entre a ultima versao 
que tinhamos com o versao de berkley.
Arquivos alterados:
 - uart.h - inclusao dos defines.
 

-------------------------------
09_aug_2014
renfernand:
1) Change the Baudrate from UART1 to 115200 and enable the CRC calculation
2) In order to test the sink include Two defines (Provisorio) in the uart.h
   ENABLE_UART0_DAG  e ENABLE_DAG_ROOT_ON_FIRST_TIME
boths can be enable in the board.h use the define sink.
ENABLE_UART0_DAG is the interface with PC but is not working in the mote.
ENABLE_DAG_ROOT_ON_FIRST_TIME is used only to enable the sink as dag root.

There is a dfine in the board.h that used to define SINK or MOTE.

-------------------------------
08_aug_2014
renfernand:
Include changes in the CC2538 do suport sens_itf created by Marcelo barros.
The code is working using the UART1 (instead of SPI1).
TODO!!!
1)Needs to implement the interface with Coap.
2)The code disable the UART0 because there is a conflict with DAG Root Code. 
3)The end reception of a frame is not ok. I used the scan of the task to check the end based if have some bytes received.
4)Use FIFO in the uart