cc2538em
========
openwsn port for CC2538EM

-------------------------------
22_aug_2014
renfernand:
Merge with Marcelo version.
Now is working the both serial UART0 e UART1 using only one firmware (without conditional compilation)


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