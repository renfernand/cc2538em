/** @file */

#ifndef __SENS_ITF_MOTE_H__
#define __SENS_ITF_MOTE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sens_itf.h"

#define SENS_ITF_DBG_FRAME  0
#define SENS_ITF_OUTPUT     1
#define SENS_ITF_SM_TICK_MS 250
#define DUMMY_BYTE 0x00


// UART return codes
#define BSP_UART_SUCCESS        0x00
#define BSP_UART_UNCONFIGURED   0x01
#define BSP_UART_NOT_SUPPORTED  0x02
#define BSP_UART_BAUDRATE_ERROR 0x04
#define BSP_UART_ERROR          0x08


#define BSP_UART_INT_BM         0xF0            //!< Interrupts handled by bsp uart


// minimum tick time is 1
#define MS2TICK(ms) (ms) > SENS_ITF_SM_TICK_MS ? (ms) / SENS_ITF_SM_TICK_MS : 1

#if (USE_SPI_INTERFACE == 0)
//
// This macro gets the byte size of the specified BSP UART ring buffer.
//
#define BU_GET_BUF_SIZE(pRbc)       ((uint32_t)((pRbc)->pui8End) -            \
                                     (uint32_t)((pRbc)->pui8Start))
//
// This macro gets the number of used bytes in the specified BSP UART
// ring buffer.
//
#define BU_GET_USED_SPACE(pRbc)     ((pRbc)->ui16NumBytes)

//

// This macro gets the number of unused bytes in the specified BSP UART
// ring buffer.
//
#define BU_GET_FREE_SPACE(pRbc)     (BU_GET_BUF_SIZE(pRbc) -                  \
                                     ((pRbc)->ui16NumBytes))

//
// This macro checks if the specified BSP UART ring buffer is empty.
//
#define BU_IS_BUF_EMPTY(pRbc)       ((pRbc)->ui16NumBytes == 0)

//
// This macro checks if the specified BSP UART ring buffer is full.
//
#define BU_IS_BUF_FULL(pRbc)        (BU_GET_USED_SPACE(pRbc) ==               \
                                     BU_GET_BUF_SIZE(pRbc))
//
// This macro gets the byte count to tail wrap for the specified BSP UART
// ring buffer.
//
#define BU_BYTES_TO_TAIL_WRAP(pRbc) ((uint32_t)((pRbc)->pui8End) -            \
                                     (uint32_t)((pRbc)->pui8Tail))


#endif



#if 0
enum {
    SENS_ITF_STATE_INIT = 0,
    SENS_ITF_STATE_SEND_ITF_VER,
    SENS_ITF_STATE_WAIT_ITF_VER_ANS,
    SENS_ITF_STATE_PROC_ITF_VER,
    SENS_ITF_STATE_SEND_BRD_ID,
    SENS_ITF_STATE_WAIT_BRD_ID_ANS,
    SENS_ITF_STATE_PROC_BRD_ID,
    SENS_ITF_STATE_SEND_PT_VAL,
    SENS_ITF_STATE_WAIT_PT_VAL_ANS,
    SENS_ITF_STATE_PROC_PT_VAL
};
#else

enum {
    SENS_ITF_STATE_INIT = 0,
    SENS_ITF_STATE_SEND_ITF_VER = 1,
    SENS_ITF_STATE_WAIT_ITF_VER_ANS = 2,
    SENS_ITF_STATE_PROC_ITF_VER = 3,
    SENS_ITF_STATE_SEND_BRD_ID = 4,
    SENS_ITF_STATE_WAIT_BRD_ID_ANS = 5,
    SENS_ITF_STATE_PROC_BRD_ID = 6,
    SENS_ITF_STATE_SEND_PT_DESC = 7,
    SENS_ITF_STATE_WAIT_PT_DESC_ANS = 8,
    SENS_ITF_STATE_PROC_PT_DESC = 9,
    SENS_ITF_STATE_BUILD_SCH = 10,
    SENS_ITF_STATE_RUN_SCH = 11,
    SENS_ITF_STATE_SEND_PT_VAL = 12,
    SENS_ITF_STATE_WAIT_PT_VAL_ANS = 13,
    SENS_ITF_STATE_PROC_PT_VAL = 14
};
#endif

enum {
	SENS_ITF_STATE_EXEC_OK = 0,
	SENS_ITF_STATE_EXEC_WAIT_OK,
	SENS_ITF_STATE_EXEC_WAIT_STOP,
	SENS_ITF_STATE_EXEC_WAIT_ABORT,
	SENS_ITF_STATE_EXEC_ERROR
};

typedef struct sens_itf_mote_sm_state_s
{
	volatile uint16_t trmout_counter;
	volatile uint16_t trmout;
	volatile uint8_t point_index;
	volatile uint8_t frame_arrived;
	volatile uint8_t state;
	volatile uint8_t retries;
} sens_itf_mote_sm_state_t;

typedef uint8_t (*sens_itf_mote_sm_func_t)(sens_itf_mote_sm_state_t *st);

typedef struct sens_itf_mote_sm_table_s
{
	sens_itf_mote_sm_func_t func;
	uint8_t next_state;
	uint8_t abort_state; // for indicating timeout or end of cyclic operation
	uint8_t error_state;
} sens_itf_mote_sm_table_t;


typedef struct sens_itf_acq_schedule_s
{
	uint8_t num_of_points;
	struct
	{
		uint8_t index;
		uint32_t sampling_time_x250ms;
        uint32_t counter;
	} points[SENS_ITF_MAX_POINTS];

	struct
	{
		uint8_t num_of_points;
		uint8_t index[SENS_ITF_MAX_POINTS];
	} scan;
} sens_itf_acq_schedule_t;


typedef struct
{
    //
    //! Lowest address in buffer
    //
    uint8_t *pui8Start;

    //
    //! Highest address in buffer + 1
    //
    uint8_t *pui8End;

    //
    //! Buffer data input pointer (push)
    //
    volatile uint8_t *pui8Head;

    //
    //! Buffer data output pointer (pop)
    //
    volatile uint8_t *pui8Tail;

    //
    //! Number of stored bytes
    //
    volatile uint16_t ui16NumBytes;
}tBuBuf;

uint8_t sens_itf_mote_init(void);
void bspUartIsrHandler (void);
uint8_t sens_itf_mote_send_frame(uint8_t *frame, uint8_t size);
uint16_t bspUartRxCharsAvail(void);


#ifdef __cplusplus
}
#endif

#endif /* __SENS_ITF_MOTE_H__ */
