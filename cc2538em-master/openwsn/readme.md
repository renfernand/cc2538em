Temporary branch for openwsn port for CC2538EM.
Part of UC Berkeley's OpenWSN project, http://www.openwsn.org/.

Update history
-----------------------------
Version: OWSNRFF V1_22_3 (20/04/2019)

This version implement the comand GET DIAG for the firmware download. The last definition is discribed in the document
Projeto Firmware download OpenWSN_Rev4.docx.

-------------------------------------
Version: OWSNRFF V1_22_2 (30/03/2019)

This version implement de firmware download using COAP with the same code used in the owsn_fw_r1_8_0 (january/2019).
Is a test version that save the code in a small flash memory at the address below, only to check the logic of firmware download. 
#define FLASH_BASE               0x00200000  // Flash
#define PAGE_SIZE                2048
#define PAGE_TO_ERASE            50




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