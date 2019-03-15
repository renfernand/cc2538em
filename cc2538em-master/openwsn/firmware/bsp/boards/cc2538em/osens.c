#include <stdint.h>
#include <stdio.h>
#include "opendefs.h"
#include "osens.h"
#include "opentimers.h"
#include "scheduler.h"
#include "debug.h"
#include "udma.h"
#include "hw_udma.h"
#include "hw_flash_ctrl.h"
#include "openserial.h"

#if 0
static osens_brd_id_t board_info;

unsigned char ucDMAControlTable[1024] __attribute__ ((aligned(1024)));

#define MAX_FRAME_CHUNCK 50
#define MAX_CHUNCK_WRITE 1024

osens_frm_t  osens_frm;

static uint8_t ucSourceBuffer[MAX_CHUNCK_WRITE];
static uint8_t ucBufferAux[MAX_CHUNCK_WRITE];

//=========================== prototypes ======================================
static void osens_mote_tick(void);
void osens_dmaflash_handling(void);
uint8_t writeflash(uint32_t srcaddr, uint32_t dstaddr, uint8_t datatype, uint32_t bufLen);
uint8_t readchunck(addr_cmd_t *plen, uint8_t *pbufin, uint8_t *pbufout);
uint8_t getAdLen(osens_frm_t *pucAddr);
uint8_t *setASLSVal(uint8_t lentype, addr_cmd_t *paddress, uint8_t *pbuf);
uint32_t getchunklenbytes(addr_cmd_t *plength);


uint8_t osens_init(void)
{
    //int32_t i32Res;

	memset(&board_info, 0, sizeof(board_info));
	
	osens_frm.flashnewcmd = iFlashNone;
	osens_frm.frwnewEndAddr = FRWNEW_START_ADDR + 200;
    //
    // Fill Source buffer (to be copied to flash) with some data
    //
	memset(&ucSourceBuffer,0x00,sizeof(ucSourceBuffer));
    // for(i=0; i<256; i++) {
    //    ucSourceBuffer[i] = i;
    //}

    opentimers_start(OSENS_SM_TICK_MS, TIMER_PERIODIC, TIME_MS, (opentimers_cbt) osens_mote_tick);

    return 1;
}

static void osens_mote_tick(void)
{
	scheduler_push_task((task_cbt) osens_dmaflash_handling, TASKPRIO_OSENS_MAIN);
}


uint8_t writeflash(uint32_t srcaddr, uint32_t dstaddr, uint8_t datatype, uint32_t bufLen){

    uint8_t ret=FALSE;
    uint32_t channelctrl=0;
    uint32_t len=0;


	if ((bufLen == 0) || (bufLen > MAX_CHUNCK_WRITE)){
		return ret;
	}

    //
    // Enable the uDMA controller.
    //
    uDMAEnable();

    //
    // Disable the uDMA channel to be used, before modifications are done.
    //
    uDMAChannelDisable(UDMA_CH2_FLASH);

    //
    // Set the base for the channel control table.
    //
    uDMAControlBaseSet(&ucDMAControlTable[0]);

    //
    // Assign the DMA channel
    //
    uDMAChannelAssign(UDMA_CH2_FLASH);

    //
    // Set attributes for the channel.
    //
    uDMAChannelAttributeDisable(UDMA_CH2_FLASH, UDMA_ATTR_HIGH_PRIORITY);

    //
    // Now set up the characteristics of the transfer.
    // 32-bit data size, with source increments in words (32 bits),
    // no destination increment.
    // A bus arbitration size of 1 must be used.
    //
/*
	switch (datatype){
		case iAdLen1B:
			channelctrl = UDMA_SIZE_8 | UDMA_SRC_INC_8  |UDMA_DST_INC_NONE | UDMA_ARB_1;
			break;
		case iAdLen2B:
			channelctrl = UDMA_SIZE_16 | UDMA_SRC_INC_16 |UDMA_DST_INC_NONE | UDMA_ARB_2;
			break;
		case iAdLen4B:
			channelctrl = UDMA_SIZE_32 | UDMA_SRC_INC_32 |UDMA_DST_INC_NONE | UDMA_ARB_1;
			break;
		default:
			break;
	}
*/
	channelctrl = UDMA_SIZE_32 | UDMA_SRC_INC_32 |UDMA_DST_INC_NONE | UDMA_ARB_1;

    uDMAChannelControlSet(UDMA_CH2_FLASH, channelctrl);

    //
    // Set transfer parameters.
    // Source address is the location of the data to write
    // and destination address is the FLASH_CTRL_FWDATA register.


    uDMAChannelTransferSet(UDMA_CH2_FLASH, UDMA_MODE_BASIC,  (void *) srcaddr,
                           (void *) FLASH_CTRL_FWDATA, bufLen);

    //
    // Asure that the flash controller is not busy.
    //
    while(HWREG(FLASH_CTRL_FCTL) & FLASH_CTRL_FCTL_BUSY)
    {
    }

    //
    // Initialize Flash control register without changing the cache mode.
    //
    HWREG(FLASH_CTRL_FCTL) &= FLASH_CTRL_FCTL_CM_M;

    //
    // Setup Flash Address register to address of first data word (32-bit)
    //

	HWREG(FLASH_CTRL_FADDR) = dstaddr;

    //
    // Finally, the DMA channel must be enabled.
    //
    uDMAChannelEnable(UDMA_CH2_FLASH);

    //
    // Set FCTL.WRITE, to trigger flash write
    //
    HWREG(FLASH_CTRL_FCTL) |= FLASH_CTRL_FCTL_WRITE;

    //
    // Wait until all words has been programmed.
    //
    while( HWREG(FLASH_CTRL_FCTL) & FLASH_CTRL_FCTL_FULL )
    {
    }

    //
    // Check if flash write was successfull
    //
    if (HWREG(FLASH_CTRL_FCTL) & FLASH_CTRL_FCTL_ABORT)
    { //Write error
          ret = FALSE;
    }
    else
    {  //write oK
    	  ret = TRUE;
    }

    //
    // Set control register back to reset value without changing the cache mode.
    //
    HWREG(FLASH_CTRL_FCTL) &= FLASH_CTRL_FCTL_CM_M;

#if  0 //ENABLE_DEBUG_RFF
{
		 uint8_t pos=0;
		 uint8_t i, len;
		 uint8_t *pucaux;

		 //a1 02 aa 00 48 00 20 bb 1a 90 21 00 cc 02 04 dd
		 //DBG_MOTE > [15:32:33.009000] 82 07 00 01 51 00 00 00 0c 00 00 00 03 00 00 00 01 00 00 00 02 00 00 00 03
		 //DBG_MOTE > [15:32:33.021000] 81 01 17 51 cc 02 0c 00 00 00 cc 02 03 00 00 00 01 00 00 00 02 00 00 00 03
		 //DBG_MOTE > [15:32:33.038000] 91 02 aa 00 48 00 20 bb 0c 90 21 00 cc 02 0c dd 00 00 00 01 00 00 00 02 00 00 00 03

		 rffbuf[pos++]= 0xA1;
		 rffbuf[pos++]= ret;
		 pucaux = (uint8_t *) &srcaddr;
		 rffbuf[pos++]= 0xAA;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= 0xBB;
		 pucaux = (uint8_t *) &dstaddr;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= 0xCC;
		 rffbuf[pos++]= datatype;
		 rffbuf[pos++]= (uint8_t) bufLen;

		 if (bufLen > 10)
		   bufLen = 10;
		 bufLen*=4;
		 rffbuf[pos++]= 0xDD;
		 pucaux = (uint8_t *) srcaddr;
		 for (i=0;i<bufLen;i++){
			 rffbuf[pos++]= *pucaux++ ;
		 }

		 openserial_printStatus(STATUS_RFF,(uint8_t*)&rffbuf,pos);
}
#endif
    return ret;
}

void osens_dmaflash_handling(void){

	int32_t i32Res=0;
	uint8_t ret=FALSE;
	uint8_t opt;

 	 if (osens_frm.flashnewcmd == iFlashErase) {
 		osens_frm.flashnewcmd = iFlashNone;

		i32Res = FlashMainPageErase(FRWNEW_START_ADDR);
		if (i32Res > 0)
		{
			// log the error Adaptado...criar um codigo para o erro de download de firmware
			openserial_printError(COMPONENT_CINFO,ERR_FLASH_WRITE_ERROR,
								  (errorparameter_t)1,
								  (errorparameter_t)i32Res);
		}

 	 }
 	 else if (osens_frm.flashnewcmd == iFlashChunck) {
  		osens_frm.flashnewcmd = iFlashNone;

		opt = OP_SHIFT(osens_frm.header);

		 switch (opt) {
			 case iEqual :
				 ret = exefunctionEqual(&osens_frm);
				 break;
			 case iReplace :
				 ret = exefunctionReplace(&osens_frm);
				 break;
			 case iInsert :
				 ret = exefunctionInsert(&osens_frm);
				 break;
			 default:
				 break;
		 }

		  if (ret == 0) {
		      // log the error
		      openserial_printError(COMPONENT_CINFO,ERR_FLASH_WRITE_ERROR,
		                            (errorparameter_t)2,
		                            (errorparameter_t)ret);
		  }

  	 }
}

/*
 * Equal Function - deve copiar o bloco determinado do endereco old para o endereco new
 * nao eh possivel ler da flash e escrever da flash ao mesmo tempo...desta forma é
 * necessario guardar o dado a ser escrito na memoria SRAM.
 */
uint8_t exefunctionEqual(osens_frm_t *posens_frm){

	//uint8_t i;
	uint32_t  srcaddr=0;
	uint32_t  dstaddr=0;
	uint32_t len;
	uint8_t ret=FALSE;

    len = getchunklenbytes (&posens_frm->chuncklen);
    //aponto para o endereco do buffer da SRAM
	srcaddr = (uint32_t) &ucSourceBuffer[0];
	switch (posens_frm->addrold.type) {
		case iAdLen1B:
			dstaddr = (uint32_t) (FRWNEW_START_ADDR +  posens_frm->addrnew.u8val);

			//copio o dado da flash para buffer da SRAM
			memcpy((void *)&ucSourceBuffer[0],(void *) (FRWNEW_START_ADDR + posens_frm->addrold.u8val),len);

			//write in the flash the SRAM buffer in the dstaddr position
            ret = writeflash(srcaddr,dstaddr,posens_frm->chuncklen.type,posens_frm->chuncklen.u8val);

            // Compare source buffer and destination flash page
            if (ret) {
            	if (memcmp(&ucSourceBuffer[0], (void *) (FRWNEW_START_ADDR + posens_frm->addrnew.u8val),len) == 0)
            	   	ret = TRUE;
            	else
            		ret = FALSE;
            }
			break;
		case iAdLen2B:
			dstaddr = (uint32_t) (FRWNEW_START_ADDR +  posens_frm->addrnew.u16val);

			//copio o dado da flash para buffer da SRAM
			memcpy((void *)&ucSourceBuffer[0],(void *) (FRWNEW_START_ADDR +posens_frm->addrold.u16val),len);

			//write in the flash the SRAM buffer in the dstaddr position
            ret = writeflash(srcaddr,dstaddr,posens_frm->chuncklen.type,posens_frm->chuncklen.u16val);

            // Compare source buffer and destination flash page
            if (ret) {
            	if (memcmp(&ucSourceBuffer[0], (void *) (FRWNEW_START_ADDR + posens_frm->addrnew.u16val),len) == 0)
            	   	ret = TRUE;
            	else
            		ret = FALSE;
            }
			break;
		case iAdLen4B:
			dstaddr = (uint32_t ) (FRWNEW_START_ADDR + posens_frm->addrnew.u32val);

			//write in the flash the SRAM buffer in the dstaddr position
			memcpy((void *)&ucSourceBuffer[0],(void *) (FRWNEW_START_ADDR+posens_frm->addrold.u32val),len);

            ret = writeflash(srcaddr,dstaddr,posens_frm->chuncklen.type,posens_frm->chuncklen.u32val);

            // Compare source buffer and destination flash page
            if (ret) {
            	if (memcmp(&ucSourceBuffer[0], (void *) (FRWNEW_START_ADDR + posens_frm->addrnew.u32val),len) == 0)
            	   	ret = TRUE;
            	else
            		ret = FALSE;
            }
            break;
		default:
			ret = FALSE;
			break;
	}


		#if 0//ENABLE_DEBUG_RFF
		{
			uint8_t pos=0;
			uint8_t j=0;
			uint8_t *pucAux;
			//DBG_MOTE > [19:42:39.720000] a1 aa 00 48 00 20 bb 30 90 21 00 cc 02 09
			//DBG_MOTE > [19:42:39.780000] 90 01 02 cc 00 48 00 20 cc 30 90 21 00 cc 02 24

			rffbuf[pos++]= 0x90;
			rffbuf[pos++]= ret;
			rffbuf[pos++]= posens_frm->addrold.type;
			rffbuf[pos++] = 0xCC;
			pucAux = (uint8_t *) &srcaddr;
			rffbuf[pos++] = *pucAux++;
			rffbuf[pos++] = *pucAux++;
			rffbuf[pos++] = *pucAux++;
			rffbuf[pos++] = *pucAux++;
			rffbuf[pos++] = 0xCC;
			pucAux = (uint8_t *) &dstaddr;
			rffbuf[pos++] = *pucAux++;
			rffbuf[pos++] = *pucAux++;
			rffbuf[pos++] = *pucAux++;
			rffbuf[pos++] = *pucAux++;
			rffbuf[pos++] = 0xCC;
			rffbuf[pos++]= posens_frm->chuncklen.type;
			rffbuf[pos++]= (uint8_t) len;
			rffbuf[pos++]= 0xDD;
			if (len > 20)
				len = 20;
			pucAux = (uint8_t *) srcaddr;
			for (j=0; j<len; j++){
			 rffbuf[pos++]= *pucAux++ ;
			}
		    openserial_printStatus(STATUS_RFF,(uint8_t*)&rffbuf,pos);
	   }
	   #endif

	return ret;
}

/*
 * Funcao Replace deve copiar o bloco do frame no endereco new
 */
uint8_t exefunctionReplace(osens_frm_t *posens_frm){

	uint32_t srcaddr=0;
	uint32_t dstadrr=0;
	uint8_t  ret=FALSE;
	uint8_t  ret1=FALSE;
	uint32_t len;
	uint32_t buflen=0,rest=0;

    len = getchunklenbytes (&posens_frm->chuncklen);
    srcaddr = (uint32_t) &ucSourceBuffer[0];
    switch (posens_frm->addrnew.type){
		case iAdLen1B:
			dstadrr = (uint32_t) (FRWNEW_START_ADDR +  posens_frm->addrnew.u8val);
			//write always 32bits...
			rest = posens_frm->chuncklen.u8val % 4;
			if (rest > 0)
				buflen = (posens_frm->chuncklen.u8val/4) + 1;
			else
				buflen = (posens_frm->chuncklen.u8val/4);

		    ret = writeflash(srcaddr,dstadrr,posens_frm->chuncklen.type,buflen);
		    break;
		case iAdLen2B:
			dstadrr = (uint32_t) (FRWNEW_START_ADDR +  posens_frm->addrnew.u16val);
			//write always 32bits...
			rest = posens_frm->chuncklen.u16val % 4;
			if (rest > 0)
				buflen = (posens_frm->chuncklen.u16val/4) + 1;
			else
				buflen = (posens_frm->chuncklen.u16val/4);
		    ret = writeflash(srcaddr,dstadrr,posens_frm->chuncklen.type,buflen);
			break;
		case iAdLen4B:
			dstadrr = (uint32_t) (FRWNEW_START_ADDR +  posens_frm->addrnew.u32val);
			buflen = posens_frm->chuncklen.u32val;
		    ret = writeflash(srcaddr,dstadrr,posens_frm->chuncklen.type,posens_frm->chuncklen.u32val);
			break;
		default:
			dstadrr = 0;
			break;
    }

    // Compare source buffer and destination flash page
    if (ret) {
    	if (memcmp(&ucSourceBuffer[0], (void *) dstadrr,len) == 0)
    	   	ret1 = TRUE;
    	else
    		ret1 = FALSE;
    }


#if  0 //ENABLE_DEBUG_RFF
{
		 uint8_t pos=0;
		 uint8_t i;
		 uint8_t *pucaux;

		 rffbuf[pos++]= 0x91;
		 rffbuf[pos++]= ret;
		 rffbuf[pos++]= ret1;
		 rffbuf[pos++]= osens_frm.addrnew.type;
		 rffbuf[pos++]= posens_frm->chuncklen.type;
		 rffbuf[pos++]= (uint8_t) buflen;
		 pucaux = (uint8_t *) &srcaddr;
		 rffbuf[pos++]= 0xAA;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= 0xBB;
		 pucaux = (uint8_t *) &dstadrr;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= 0xCC;
		 rffbuf[pos++]= osens_frm.chuncklen.type;
		 len = getchunklenbytes (&osens_frm.chuncklen);
		 rffbuf[pos++]= (uint8_t) len;
		 pucaux = (uint8_t *) dstadrr;
		 rffbuf[pos++]= 0xDD;
		 if (len > 20)
			 len = 20;
		 for (i=0;i<len;i++){
			 rffbuf[pos++]= *pucaux++ ;
		 }

		 openserial_printStatus(STATUS_RFF,(uint8_t*)&rffbuf,pos);
}
#endif

	return ret;
}

/*
 * Funcao Insert deve copiar o bloco do frame no endereco new e deslocar o restante do codigo
 */
uint8_t exefunctionInsert(osens_frm_t *posens_frm){

	uint32_t srcaddr=0;
	uint32_t dstadrr=0;
	uint8_t  ret=FALSE;
	uint32_t len,len4b;
	uint32_t buflen,rest;
	uint8_t  *pucAddr;
    int32_t lenmove,auxlen;
    uint32_t dstaddr2,dstaddr3;

    len = getchunklenbytes (&posens_frm->chuncklen);

    srcaddr = (uint32_t) &ucSourceBuffer[0];
    switch (posens_frm->addrnew.type){
		case iAdLen1B:
			dstadrr = (uint32_t) (FRWNEW_START_ADDR +  posens_frm->addrnew.u8val);
		    pucAddr = (uint8_t *) &dstadrr;
			lenmove = posens_frm->frwnewEndAddr - dstadrr;
			if (lenmove > 0) {
			    //Mover a area abaixo do endereco a ser inserido de baixo para cima do tamanho len
				uint32_t auxendaddr = posens_frm->frwnewEndAddr;
				while(lenmove > 0){
					if (lenmove > MAX_FRAME_CHUNCK)
						auxlen = MAX_FRAME_CHUNCK;
					else
						auxlen = lenmove;
					dstaddr2 = auxendaddr - auxlen;
					memcpy(&ucBufferAux[0],(void *) dstaddr2,auxlen);
					dstaddr3 = dstaddr2 + len;
					len4b = auxlen/4;
					ret = writeflash((uint32_t) &ucBufferAux[0],dstaddr3,posens_frm->chuncklen.type,len4b);
					lenmove = dstaddr2 - dstadrr;
					auxendaddr = dstaddr2;

				}
				posens_frm->frwnewEndAddr += len;

				//incluir chunck no endereco de destino
				dstadrr = (uint32_t) (FRWNEW_START_ADDR +  posens_frm->addrnew.u8val);
			    ret = writeflash(srcaddr,dstadrr,posens_frm->chuncklen.type,posens_frm->chuncklen.u8val);
			}
		    break;
		case iAdLen2B:
			dstadrr = (uint32_t) (FRWNEW_START_ADDR +  posens_frm->addrnew.u16val);
			//write always 32bits...
			rest = posens_frm->chuncklen.u16val % 4;
			if (rest > 0)
				buflen = (posens_frm->chuncklen.u16val/4) + 1;
			else
				buflen = (posens_frm->chuncklen.u16val/4);
		    ret = writeflash(srcaddr,dstadrr,posens_frm->chuncklen.type,buflen);
			break;
		case iAdLen4B:
			dstadrr = (uint32_t) (FRWNEW_START_ADDR +  posens_frm->addrnew.u32val);
		    pucAddr = (uint8_t *) &dstadrr;
			lenmove = posens_frm->frwnewEndAddr - dstadrr;
			if (lenmove > 0) {
			    //Mover a area abaixo do endereco a ser inserido de baixo para cima do tamanho len
				uint32_t auxendaddr = posens_frm->frwnewEndAddr;
				while(lenmove > 0){
					if (lenmove > MAX_FRAME_CHUNCK)
						auxlen = MAX_FRAME_CHUNCK;
					else
						auxlen = lenmove;
					dstaddr2 = auxendaddr - auxlen;
					memcpy(&ucBufferAux[0],(void *) dstaddr2,auxlen);
					dstaddr3 = dstaddr2 + len;
					len4b = auxlen/4;
					ret = writeflash((uint32_t) &ucBufferAux[0],dstaddr3,posens_frm->chuncklen.type,len4b);

#if  0 //ENABLE_DEBUG_RFF
{
		 uint8_t pos=0;
		 uint8_t i;
		 uint8_t *pucaux;

		 rffbuf[pos++]= 0x92;
		 rffbuf[pos++]= ret;
		 rffbuf[pos++]= (uint8_t) lenmove;
		 rffbuf[pos++]= (uint8_t) len4b;
		 pucaux = (uint8_t *) &dstaddr3;
		 rffbuf[pos++]= 0xCC;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 pucaux = (uint8_t *) &dstaddr3;
		 rffbuf[pos++]= 0xCC;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= *pucaux++;
		 rffbuf[pos++]= 0xCC;
		 len = getchunklenbytes (&osens_frm.chuncklen);
		 rffbuf[pos++]= (uint8_t) len;
		 pucaux = (uint8_t *) dstaddr3;
/*
		 rffbuf[pos++]= 0xDD;
		 if (len > 20)
			 len = 20;
		 for (i=0;i<len;i++){
			 rffbuf[pos++]= *pucaux++ ;
		 }
*/
		 openserial_printStatus(STATUS_RFF,(uint8_t*)&rffbuf,pos);
}
#endif


					lenmove = dstaddr2 - dstadrr;
					auxendaddr = dstaddr2;
				}
				posens_frm->frwnewEndAddr += len;

				//incluir chunck no endereco de destino
				dstadrr = (uint32_t) (FRWNEW_START_ADDR +  posens_frm->addrnew.u32val);
			    ret = writeflash(srcaddr,dstadrr,posens_frm->chuncklen.type,posens_frm->chuncklen.u32val);

			}
		    ret = writeflash(srcaddr,dstadrr,posens_frm->chuncklen.type,posens_frm->chuncklen.u32val);
			break;
		default:
			dstadrr = 0;
			break;
    }

    // Compare source buffer and destination flash page
    if (ret) {
    	if (memcmp(&ucSourceBuffer[0], (void *) dstadrr,len) == 0)
    	   	ret = TRUE;
    	else
    		ret = FALSE;
    }

	return ret;
}

/* esta funcao le o buffer da mensagem recebida e armazena em um buffer interno em memoria SRAM
 * para ser posteriormente escrito na flash.
 */
uint8_t readchunck(addr_cmd_t *plen, uint8_t *pbufin, uint8_t *pbufout){

	uint32_t i;
    uint32_t len=0;
    uint8_t ret=FALSE;

    len = getchunklenbytes(plen);
	if (len < MAX_FRAME_CHUNCK){
		for (i=0;i<len;i++){
			*pbufout++ = *pbufin++;
		}
		ret = TRUE;
	}

   return ret;
}

void osens_frame_parser(uint8_t *pbuf){

	uint8_t *pframe = pbuf;
	uint8_t *pu8aux;
    uint8_t opt,ast,lst;
    uint8_t ret=FALSE;

	 //get the Frame ID (2 bytes)
     pu8aux = (uint8_t *) &osens_frm.frameID;
     *(pu8aux+1) = *pframe++;
     *(pu8aux+0) = *pframe++;

	 //get the header (1 byte)
     osens_frm.header = *pframe++;

	 //get the address (depends on the OPcode)
	 getAdLen(&osens_frm);
	 opt = OP_SHIFT(osens_frm.header);
	 ast = AS_SHIFT(osens_frm.header);
	 lst = LS_SHIFT(osens_frm.header);
	 switch (opt) {
		 case iEqual :
			 //read AS old
			 pframe = setASLSVal(ast, &osens_frm.addrold, pframe);
			 //read AS new
			 pframe = setASLSVal(ast, &osens_frm.addrnew, pframe);
			 //read LS
			 pframe = setASLSVal(lst, &osens_frm.chuncklen, pframe);

			 osens_frm.flashnewcmd = iFlashChunck;
			 break;
		 case iReplace :
		 case iInsert :
			 //read AS new
			 pframe = setASLSVal(ast, &osens_frm.addrnew, pframe);
			 //read LS
			 pframe = setASLSVal(lst, &osens_frm.chuncklen, pframe);
			 //read chunck
			 ret = readchunck(&osens_frm.chuncklen,pframe,ucSourceBuffer);

			 osens_frm.flashnewcmd = iFlashChunck;
			 break;
		 default:
			 break;
	 }

		#if  0 //ENABLE_DEBUG_RFF
		{
			 uint8_t pos=0;
			 uint8_t i;
			 uint32_t len;
			 uint8_t *pucaux = (uint8_t *) &osens_frm.addrnew.u32val;
			 //DBG_MOTE > 81 05 13 52 cc 02 0c 00 00 00 cc 02 01 b1 b2 b3 b4

			 rffbuf[pos++]= 0x81;
			 rffbuf[pos++]= (uint8_t) osens_frm.frameID;
			 rffbuf[pos++]= (uint8_t) osens_frm.frameLen;
			 rffbuf[pos++]= osens_frm.header;
			 rffbuf[pos++]= 0xCC;
			 rffbuf[pos++]= osens_frm.addrnew.type;
			 pucaux = (uint8_t *) &osens_frm.addrnew.u32val;
			 rffbuf[pos++]= *pucaux++;
			 rffbuf[pos++]= *pucaux++;
			 rffbuf[pos++]= *pucaux++;
			 rffbuf[pos++]= *pucaux++;
			 rffbuf[pos++]= 0xCC;
			 rffbuf[pos++]= osens_frm.chuncklen.type;
			 rffbuf[pos++]= (uint8_t) osens_frm.chuncklen.u32val;

			 pucaux = &ucSourceBuffer[0];
			 len = getchunklenbytes (&osens_frm.chuncklen);
			 if (len > 20)
				 len = 20;
			 for (i=0;i<len;i++){
				 rffbuf[pos++]= *pucaux++ ;
			 }

			 openserial_printStatus(STATUS_RFF,(uint8_t*)&rffbuf,pos);
		}
		#endif

}


uint8_t getAdLen(osens_frm_t *pucAddr){

	uint8_t u8AS = AS(pucAddr->header);
	uint8_t u8LS = LS(pucAddr->header);
	uint8_t ret=0;

	switch (u8AS) {
		case iAdLen1B:
			pucAddr->addrnew.type = iAdLen1B;
			pucAddr->addrold.type = iAdLen1B;
			ret=1;
			break;
		case iAdLen2B:
			pucAddr->addrnew.type = iAdLen2B;
			pucAddr->addrold.type = iAdLen2B;
			ret=1;
			break;
		case iAdLen4B:
			pucAddr->addrnew.type = iAdLen4B;
			pucAddr->addrold.type = iAdLen4B;
			ret=1;
			break;
	}

	switch (u8LS) {
		case iAdLen1B:
			pucAddr->chuncklen.type = iAdLen1B;
			ret=2;
			break;
		case iAdLen2B:
			pucAddr->chuncklen.type = iAdLen2B;
			ret=2;
			break;
		case iAdLen4B:
			pucAddr->chuncklen.type = iAdLen4B;
			ret=2;
			break;
	}
	return ret;
}

uint8_t *setASLSVal(uint8_t lentype, addr_cmd_t *paddress, uint8_t *pbuf){

 //uint8_t RFFRoute;
 uint8_t *pu8frame = (uint8_t *) pbuf;
 uint8_t *pu8val;

	paddress->type = lentype;
	//RFFRoute=0x30;
	switch (lentype) {
		case iAdLen1B:{
			//RFFRoute=0x31;
			paddress->u8val = *pu8frame++;
			break;
		}
		case iAdLen2B: {
			//RFFRoute=0x32;
			pu8val = (uint8_t *) &paddress->u16val;
			*(pu8val+1) = (uint8_t) *pu8frame++;
			*(pu8val+0) = *pu8frame++;
			break;
		}
		case iAdLen4B: {
			//RFFRoute=0x34;
			pu8val = (uint8_t *) &paddress->u32val;
			*(pu8val+3) = *pu8frame++;
			*(pu8val+2) = *pu8frame++;
			*(pu8val+1) = *pu8frame++;
			*(pu8val+0) = *pu8frame++;
			break;
		}
		default:
			break;
	}

    return (uint8_t *) pu8frame;

}

uint32_t getchunklenbytes(addr_cmd_t *plength) {
    uint32_t len=0;

	switch (plength->type) {
	case iAdLen1B:
		len = (uint32_t) plength->u8val;
		break;
	case iAdLen2B:
		len = (uint32_t) plength->u16val * 2;
		break;
	case iAdLen4B:
		len = (uint32_t) plength->u32val * 4;
		break;
	default:
		len = 0;
		break;
	}
	return len;
}

/*
uint8_t osens_get_brd_desc(osens_brd_id_t *brd)
{
    return 0;
}
uint8_t osens_get_num_points(void)
{
    return 0;
}

uint8_t osens_get_pdesc(uint8_t index, osens_point_desc_t *desc)
{
    return 0;
}

int8_t osens_get_ptype( uint8_t index)
{
    return 0;
}

uint8_t osens_get_point(uint8_t index, osens_point_t *point)
{
    return 0;
}

uint8_t osens_set_pvalue(uint8_t index, osens_point_t *point)
{
    return 0;
}

*/
#endif
