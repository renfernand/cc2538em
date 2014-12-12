OpenWSN firmware: stuff that runs on a mote

Part of UC Berkeley's OpenWSN project, http://www.openwsn.org/.

Build status
------------

|              builder                                                           |      build               | outcome
| ------------------------------------------------------------------------------ | ------------------------ | -------
| [Travis](https://travis-ci.org/openwsn-berkeley/openwsn-fw)                    | compile                  | [![Build Status](https://travis-ci.org/openwsn-berkeley/openwsn-fw.png?branch=develop)](https://travis-ci.org/openwsn-berkeley/openwsn-fw)
| [OpenWSN builder](http://builder.openwsn.org/job/Firmware%20TelosB/)           | compile (TelosB)         | [![Build Status](http://builder.openwsn.org/buildStatus/icon?job=Firmware%20TelosB)](http://builder.openwsn.org/job/Firmware%20TelosB/)
| [OpenWSN builder](http://builder.openwsn.org/job/Firmware%20GINA/)             | compile (GINA)           | [![Build Status](http://builder.openwsn.org/buildStatus/icon?job=Firmware%20GINA)](http://builder.openwsn.org/job/Firmware%20GINA/)
| [OpenWSN builder](http://builder.openwsn.org/job/Firmware%20wsn430v13b/)       | compile (wsn430v13b)     | [![Build Status](http://builder.openwsn.org/buildStatus/icon?job=Firmware%20wsn430v13b)](http://builder.openwsn.org/job/Firmware%20wsn430v13b/)
| [OpenWSN builder](http://builder.openwsn.org/job/Firmware%20wsn430v14/)        | compile (wsn430v14)      | [![Build Status](http://builder.openwsn.org/buildStatus/icon?job=Firmware%20wsn430v14)](http://builder.openwsn.org/job/Firmware%20wsn430v14/)
| [OpenWSN builder](http://builder.openwsn.org/job/Firmware%20Z1/)               | compile (Z1)             | [![Build Status](http://builder.openwsn.org/buildStatus/icon?job=Firmware%20Z1)](http://builder.openwsn.org/job/Firmware%20Z1/)
| [OpenWSN builder](http://builder.openwsn.org/job/Firmware%20OpenMoteCC2538/)   | compile (OpenMoteCC2538) | [![Build Status](http://builder.openwsn.org/buildStatus/icon?job=Firmware%20OpenMoteCC2538)](http://builder.openwsn.org/job/Firmware%20OpenMoteCC2538/)
| [OpenWSN builder](http://builder.openwsn.org/job/Firmware%20OpenMoteSTM/)      | compile (OpenMoteSTM)    | [![Build Status](http://builder.openwsn.org/buildStatus/icon?job=Firmware%20OpenMoteSTM)](http://builder.openwsn.org/job/Firmware%20OpenMoteSTM/)
| [OpenWSN builder](http://builder.openwsn.org/job/Firmware%20IoT-LAB_M3/)       | compile (IoT-LAB_M3)     | [![Build Status](http://builder.openwsn.org/buildStatus/icon?job=Firmware%20IoT-LAB_M3)](http://builder.openwsn.org/job/Firmware%20IoT-LAB_M3/)
| [OpenWSN builder](http://builder.openwsn.org/job/Firmware%20Agilefox/)         | compile (Agilefox)     | [![Build Status](http://builder.openwsn.org/buildStatus/icon?job=Firmware%20Agilefox)](http://builder.openwsn.org/job/Firmware%20Agilefox/)
| [OpenWSN builder](http://builder.openwsn.org/job/Firmware%20OpenSim/)          | compile (OpenSim)        | [![Build Status](http://builder.openwsn.org/buildStatus/icon?job=Firmware%20OpenSim)](http://builder.openwsn.org/job/Firmware%20OpenSim/)
| [OpenWSN builder](http://builder.openwsn.org/job/Firmware%20Docs%20Publisher/) | publish documentation    | [![Build Status](http://builder.openwsn.org/buildStatus/icon?job=Firmware Docs Publisher)](http://builder.openwsn.org/job/Firmware%20Docs%20Publisher/)

Documentation
-------------

- overview: https://openwsn.atlassian.net/wiki/
- source code: http://openwsn-berkeley.github.io/firmware/

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

