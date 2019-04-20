#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "board.h"
#include "opendefs.h"
#include "opencoap.h"
#include "osens_app.h"
#include "openqueue.h"
//#include "cinfo.h"
#include "osens.h"
#include "packetfunctions.h"
#include "opentimers.h"
#include "scheduler.h"
#include "osens_app.h"
#include "debugpins.h"
#include "openserial.h"
#include "debug.h"

#define TRACE_ON 0

//=========================== variables =======================================
coap_resource_desc_t osens_frm_vars;

const uint8_t osens_frm_path0 [] = "f";
const uint8_t osens_frm_path1 [] = "i";

extern osens_frm_t  osens_frm;


//=========================== prototypes ======================================

owerror_t osens_frm_receive(OpenQueueEntry_t* msg, coap_header_iht*  coap_header, coap_option_iht*  coap_options);
void osens_frm_sendDone(OpenQueueEntry_t* msg, owerror_t error);

owerror_t osens_val_receive(OpenQueueEntry_t* msg, coap_header_iht*  coap_header, coap_option_iht*  coap_options);
void osens_val_sendDone(OpenQueueEntry_t* msg, owerror_t error);
static uint8_t * insert_str(uint8_t *buffer, uint8_t *str, uint32_t len, uint8_t quotes);
static uint8_t * insert_uint(uint8_t *buffer, uint64_t value);
static double decode_number(uint8_t *buffer, uint8_t len);
static void set_point_val(osens_point_t *point, double value);
uint8_t *decode_chunk(uint8_t *bufin,uint8_t len,uint8_t *bufout, uint8_t type);
uint8_t *decode_framehexa(uint8_t *bufin,uint8_t len,uint8_t *bufout);

//=========================== public ==========================================

void osens_app_init(void) {
    int32_t i32Res;

    // prepare the resource descriptor for the /d and /s paths
	osens_frm_vars.path0len         = sizeof(osens_frm_path0) -1;
	osens_frm_vars.path0val         = (uint8_t*) (&osens_frm_path0);
	osens_frm_vars.path1len         = sizeof(osens_frm_path1) -1;
	osens_frm_vars.path1val         = (uint8_t*) (&osens_frm_path1);
	osens_frm_vars.componentID      = COMPONENT_CINFO;
	osens_frm_vars.securityContext  = NULL;
	osens_frm_vars.discoverable     = TRUE;
	osens_frm_vars.callbackRx       = &osens_frm_receive;
	osens_frm_vars.callbackSendDone = &osens_frm_sendDone;

	memset(&osens_frm,0,sizeof(osens_frm));

	// register with the CoAP modules
    opencoap_register(&osens_frm_vars);

}


//=========================== private =========================================


owerror_t osens_frm_receive(OpenQueueEntry_t* msg, coap_header_iht*  coap_header, coap_option_iht*  coap_options)
{
	owerror_t outcome = E_FAIL;
	//uint8_t n = 0;
	uint8_t buf[50];
	uint8_t *pucbuf;
	uint8_t *pucAux;
	uint8_t optnum = 0;
	uint8_t optidx = 2;
	uint8_t index=0;
	uint8_t len=0;
	uint8_t type=0;
	uint8_t j=0;

    switch (coap_header->Code)
    {
		case COAP_CODE_REQ_GET:
			// reset packet payload
			msg->payload = &(msg->packet[127]);
			msg->length = 0;

			 //=== prepare  CoAP response  - esta resposta eh retirada do comando info
			 if (((coap_options[1].pValue[0] == 'i') || (coap_options[1].pValue[0] == 'I')) &&
					 (coap_options[1].type == COAP_OPTION_NUM_URIPATH)) {

			// stack name and version
			 packetfunctions_reserveHeaderSize(msg,1);
			 msg->payload[0] = '\n';
			 packetfunctions_reserveHeaderSize(msg,sizeof(infoStackName)-1+5);
			 memcpy(&msg->payload[0],&infoStackName,sizeof(infoStackName)-1);
			 msg->payload[sizeof(infoStackName)-1+5-5] = '0'+OPENWSN_VERSION_MAJOR;
			 msg->payload[sizeof(infoStackName)-1+5-4] = '.';
			 msg->payload[sizeof(infoStackName)-1+5-3] = '0'+OPENWSN_VERSION_MINOR;
			 msg->payload[sizeof(infoStackName)-1+5-2] = '.';
			 msg->payload[sizeof(infoStackName)-1+5-1] = '0'+OPENWSN_VERSION_PATCH;

			 // set the CoAP header
			 coap_header->Code                = COAP_CODE_RESP_CONTENT;
			 outcome                          = E_SUCCESS;

			 }
			 else if (((coap_options[1].pValue[0] == 'd') || (coap_options[1].pValue[0] == 'd')) &&
						 (coap_options[1].type == COAP_OPTION_NUM_URIPATH)) {
				 uint8_t *pucaux =  (uint8_t *) &osens_frm.frameID;

				// stack name and version
				 packetfunctions_reserveHeaderSize(msg,4);
/*
				 msg->payload[0] = '0' + *(pucaux+1);
				 msg->payload[1] = '0' + *(pucaux+0);
				 msg->payload[2] = '0' + osens_frm.header;
				 msg->payload[3] = '0' + osens_frm.framestatus;
*/
				 msg->payload[0] =  *(pucaux+1);
				 msg->payload[1] =  *(pucaux+0);
				 msg->payload[2] =  osens_frm.header;
				 msg->payload[3] =  osens_frm.framestatus;

				 // set the CoAP header
				 coap_header->Code                = COAP_CODE_RESP_CONTENT;
				 outcome                          = E_SUCCESS;

			}


			 break;

		case COAP_CODE_REQ_PUT:
			// reset packet payload
			msg->payload = &(msg->packet[127]);
			msg->length = 0;

			 if (((coap_options[1].pValue[0] == 'e') || (coap_options[1].pValue[0] == 'E')) &&
					 (coap_options[1].type == COAP_OPTION_NUM_URIPATH)) {
					//erase the flash firmware new area
					osens_frm.flashnewcmd = iFlashErase;
					coap_header->Code = COAP_CODE_RESP_VALID;
					outcome = E_SUCCESS;

					#if ENABLE_DEBUG_RFF
					{
						 uint8_t pos=0;

						 rffbuf[pos++]= RFF_COMPONENT_OPENCOAP_TX;
						 rffbuf[pos++]= 0x81;
						 rffbuf[pos++]= COAP_CODE_RESP_VALID;

						 openserial_printStatus(STATUS_RFF,(uint8_t*)&rffbuf,pos);
					}
					#endif
			 }
			 else {
				 //get number of sections
				 optnum = (coap_options[1].pValue[0] - 0x30) + 2;

				if ( (coap_options[1].length == 1)  &&
					 (coap_options[1].type == COAP_OPTION_NUM_URIPATH) &&
					 (optnum > 0)) {

					osens_frm.frameLen = 0;
					//Leitura do buffer da mensagem...o restante da mensagem é codificado 2bytes hexadecimal (AA BB 01 03...)
					if (optnum < 10) {
						 //read payload
						 pucbuf = &buf[0];
						 for (optidx=2;optidx<(optnum+2); optidx++){
							 pucbuf = decode_framehexa(coap_options[optidx].pValue,coap_options[optidx].length,pucbuf);
						 }
					}

					#if ENABLE_DEBUG_RFF
					{
						 uint8_t pos=0;
						 uint8_t j=0;
						 rffbuf[pos++]= RFF_COMPONENT_OPENCOAP_TX;
						 rffbuf[pos++]= 0x82;
						 rffbuf[pos++]= 0x82;
						 rffbuf[pos++]= 0x82;
						 rffbuf[pos++]= optnum;
						 for (j=0;j<(osens_frm.frameLen);j++){
						   rffbuf[pos++] = buf[j];
						 }

						 openserial_printStatus(STATUS_RFF,(uint8_t*)&rffbuf,pos);
					}
					#endif

					osens_frame_parser(buf);

				}

				// set the CoAP header
				coap_header->Code  = COAP_CODE_RESP_VALID;
				outcome = E_SUCCESS;
			}


		   break;
		default:
			outcome = E_FAIL;
			break;
	}








    return outcome;
}


uint8_t *decode_framehexa(uint8_t *bufin,uint8_t len,uint8_t *bufout){

	uint8_t *pbuf = bufin;
	uint8_t *pbufout = bufout;
	uint8_t *pbuf_end = bufin + len;
	uint8_t  u8number = 0;
	char bufaux[10];

	while (pbuf < pbuf_end)
	{
		bufaux[0] = *pbuf++;
		bufaux[1] = *pbuf++;
		bufaux[2] = 0x00;

		u8number = strtol((char *) &bufaux[0], NULL, 16);
		*pbufout++ = u8number;
		osens_frm.frameLen++;
	}

	return pbufout;
}
/*
 * transforma o buffer de entrada hexadecimal (ASCII) (2 bytes) em inteiro uint8_t (1,2,4 bytes)
 * ex: bufin = "FFFE0502" -
 *     var1b[0] = 0xFF var1b[1]=FE ....
 *     var2b[0] = 0xFFFE var2b[1] = 0x0502
 *     var4b[0] = 0xFFFE0502
 */

uint8_t *decode_chunk(uint8_t *bufin,uint8_t len,uint8_t *bufout, uint8_t type){

	uint8_t *pbuf = bufin;
	uint8_t *pbufout = bufout;
    uint8_t *pbuf_end = bufin + len;
    uint8_t  u8number = 0;
    uint16_t u16number = 0;
    uint32_t u32number = 0;
    char bufaux[10];

	while (pbuf < pbuf_end)
    {
		switch (type){
		  case iAdLen1B:
				bufaux[0] = *pbuf++;
				bufaux[1] = *pbuf++;
				bufaux[2] = 0x00;

				u8number = strtol((char *) &bufaux[0], NULL, 16);
				*pbufout++ = u8number;
				break;
		  case iAdLen2B:
				bufaux[0] = *pbuf++;
				bufaux[1] = *pbuf++;
				bufaux[2] = *pbuf++;
				bufaux[3] = *pbuf++;
				bufaux[4] = 0x00;

				u16number = strtol((char *) &bufaux[0], NULL, 16);
				*((uint16_t *)pbufout) = u16number;
				pbufout+=2;
				break;
		  case iAdLen4B:
				bufaux[0] = *pbuf++;
				bufaux[1] = *pbuf++;
				bufaux[2] = *pbuf++;
				bufaux[3] = *pbuf++;
				bufaux[4] = *pbuf++;
				bufaux[5] = *pbuf++;
				bufaux[6] = *pbuf++;
				bufaux[7] = *pbuf++;
				bufaux[8] = 0x00;

				u32number = strtol((char *) &bufaux[0], NULL, 16);
				*((uint32_t *)pbufout) = u32number;
				pbufout+=4;
				break;
		  default:
			  break;
		}

    }

	return pbufout;
}

static double decode_number(uint8_t *buffer, uint8_t len)
{
    uint8_t *pbuf = buffer;
    uint8_t *pbuf_end = buffer + len;
    uint8_t neg = 0;
    uint8_t frac = 0;
    double div = 10.0;
    double number = 0;

    if (*pbuf == '-')
    {
        neg = 1;
        pbuf++;
    }

    while (pbuf < pbuf_end)
    {
        if (*pbuf == '.')
        {
            frac = frac == 0 ? 1 : frac; // only one period per number
            pbuf++;
            continue;
        }

        if (frac == 1)
        {
            number = number + (*pbuf++ - 0x30) / div;
            div = div * 10;
            // protect div near zero here ?
        }
        else
            number = number * 10 + (*pbuf++ - 0x30);
    }

    if (neg)
        number = -1 * number;

    return number;
}


static uint8_t * insert_str(uint8_t *buffer, uint8_t *str, uint32_t len, uint8_t quotes)
{
    uint8_t *pbuf = buffer;
    if (quotes)
        *pbuf++ = '"';

    memcpy(pbuf, str, len);
    pbuf += len;

    if (quotes)
        *pbuf++ = '"';

    return pbuf;
}

// based on http://stackoverflow.com/questions/9655202/how-to-convert-integer-to-string-in-c
static uint8_t * insert_uint(uint8_t *buffer, uint64_t value)
{
    uint8_t* pbuf = buffer;
    uint8_t *pend;
    uint64_t shifter;
    uint8_t digits[10] = {'0','1','2','3','4','5','6','7','8','9'};

    // count the number of digits
    shifter = value;
    do
    {
        ++pbuf;
        shifter = shifter / 10;
    } while(shifter);
    //*p = '\0';
    pend = pbuf;

    // now fill the digits
    do
    {
        *--pbuf = digits[value % 10];
        value = value / 10;
    } while(value);

    return pend;
}

static uint8_t * insert_int(uint8_t *buffer, int64_t value)
{
    uint8_t* pbuf = buffer;

    // add signal, invert value and call the unsigned version
    if(value < 0)
    {
        *pbuf++ = '-';
        value *= -1;
    }

    return insert_uint(pbuf, (uint64_t) value);
}


static uint8_t * insert_float(uint8_t *buffer, double value)
{
	uint64_t itg, frc;
	uint8_t neg;
	uint8_t* pbuf = buffer;

	neg = value < 0 ? 1 : 0;
	itg = (uint64_t) value;
	frc = (uint64_t) ((value - itg)*100000); // precision

	if(neg)
		*pbuf++ = '-';

	pbuf = insert_uint(pbuf,itg);
	*pbuf++ = '.';
	pbuf = insert_uint(pbuf,frc);

	return pbuf;
}

static uint8_t * insert_point_val(uint8_t *buffer, osens_point_t *point)
{
	uint8_t *pbuf = buffer;
    switch (point->type)
    {
    case OSENS_DT_U8:
    	pbuf = insert_uint(pbuf,(uint64_t) point->value.u8);
        break;
    case OSENS_DT_U16:
    	pbuf = insert_uint(pbuf,(uint64_t) point->value.u16);
        break;
    case OSENS_DT_U32:
    	pbuf = insert_uint(pbuf,(uint64_t) point->value.u32);
        break;
    case OSENS_DT_U64:
    	pbuf = insert_uint(pbuf,(uint64_t) point->value.u64);
        break;
    case OSENS_DT_S8:
    	pbuf = insert_int(pbuf,(uint64_t) point->value.s8);
        break;
    case OSENS_DT_S16:
    	pbuf = insert_int(pbuf,(uint64_t) point->value.s16);
        break;
    case OSENS_DT_S32:
    	pbuf = insert_int(pbuf,(uint64_t) point->value.s32);
        break;
    case OSENS_DT_S64:
    	pbuf = insert_int(pbuf,(uint64_t) point->value.s64);
        break;
    case OSENS_DT_FLOAT:
    	pbuf = insert_float(pbuf,point->value.fp32);
        break;
    case OSENS_DT_DOUBLE:
    	pbuf = insert_float(pbuf,(float) point->value.fp64);
        break;
    default:
        break;
    }

    return pbuf;
}


static void set_point_val(osens_point_t *point, double value)
{
    switch (point->type)
    {
    case OSENS_DT_U8:
    	point->value.u8 = (uint8_t) value;
        break;
    case OSENS_DT_U16:
    	point->value.u16 = (uint16_t) value;
        break;
    case OSENS_DT_U32:
    	point->value.u32 = (uint32_t) value;
        break;
    case OSENS_DT_U64:
    	point->value.u64 = (uint64_t) value;
        break;
    case OSENS_DT_S8:
    	point->value.s8 = (int8_t) value;
        break;
    case OSENS_DT_S16:
    	point->value.s16 = (int16_t) value;
        break;
    case OSENS_DT_S32:
    	point->value.s32 = (int32_t) value;
        break;
    case OSENS_DT_S64:
    	point->value.s64 = (int64_t) value;
        break;
    case OSENS_DT_FLOAT:
    	point->value.fp32 = (float) value;
        break;
    case OSENS_DT_DOUBLE:
    	point->value.fp64 = (double) value;
        break;
    default:
        break;
    }
}

void osens_frm_sendDone(OpenQueueEntry_t* msg, owerror_t error) {
    openqueue_freePacketBuffer(msg);
}

