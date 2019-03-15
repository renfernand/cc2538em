Temporary branch for openwsn port for CC2538EM.
Part of UC Berkeley's OpenWSN project, http://www.openwsn.org/.

Update history
-----------------------------
Version: V1_22_1 (14/03/2019)

This is the OpenWSN develop raw with few changes to include the bsp CC2538EM board.
based in the openmote-cc2538.
This version works fine with OpenVisualizer and Coap with security enable or not
but have to work in the multichannel configuration.
In a single channel version (IEEE802154e.c):
ieee154e_vars.singleChannel     = 0;
this version does not work fine.

-----------------------------
Version: V1_22_1 (02/03/2019)

Merge with last version of OpenWSN github site (branch develop) Version 1.22.1
This version has the security module, but is disabled in the inc\opendefs.h