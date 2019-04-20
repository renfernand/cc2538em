/**
@file osens.h
@brief Open sensors driver interface

@author Marcelo Barros de Almeida <marcelobarrosalmeida@gmail.com>
*/

#ifndef __OSENS_H__
#define __OSENS_H__

#define OSENS_LATEST_VERSION     0
#define OSENS_MODEL_NAME_SIZE    8
#define OSENS_MANUF_NAME_SIZE    8
#define OSENS_POINT_NAME_SIZE    8
#define OSENS_MAX_POINTS        32


#define FLASH_BASE               0x00200000  // Flash
#define PAGE_SIZE                2048
#define PAGE_TO_ERASE            50

#define FRWNEW_START_ADDR (FLASH_BASE + (PAGE_TO_ERASE * PAGE_SIZE))

#define OSENS_SM_TICK_MS 250

enum osens_access_rights_e
{
	OSENS_ACCESS_READ_ONLY = 0x01,
	OSENS_ACCESS_WRITE_ONLY = 0x02,
	OSENS_ACCESS_READ_WRITE = 0x03,
};

enum osens_sensor_capabilities_e
{
	OSENS_CAPABILITIES_BATTERY = 0x01,
	OSENS_CAPABILITIES_DISPLAY = 0x02,
	OSENS_CAPABILITIES_WPAN_STATUS = 0x04,
	OSENS_CAPABILITIES_BATTERY_STATUS = 0x08,
};

/** Sensor interface standard datatypes */
enum osens_datatypes_e
{
	OSENS_DT_U8     = 0x00, /**< 8 bits unsigned */
	OSENS_DT_S8     = 0x01, /**< 8 bits signed */
	OSENS_DT_U16    = 0x02, /**< 16 bits unsigned */
	OSENS_DT_S16    = 0x03, /**< 16 bits signed */
	OSENS_DT_U32    = 0x04, /**< 32 bits unsigned */
	OSENS_DT_S32    = 0x05, /**< 32 bits signed */
	OSENS_DT_U64    = 0x06, /**< 64 bits unsigned */
	OSENS_DT_S64    = 0x07, /**< 64 bits signed */
	OSENS_DT_FLOAT  = 0x08, /**< IEEE 754 single precision */
	OSENS_DT_DOUBLE = 0x09, /**< IEEE 754 double precision */
};

/* sub status from response */
enum status_frwupd_u {
   ST_FRWUPD_OK                        = 0,
   ST_FRWUPD_ERROR_ERASE_FLASH         = 1,
   ST_FRWUPD_GENERAL_ERROR             = 2,
   ST_FRWUPD_CODE3                     = 3,
};

union osens_point_data_u
{
	uint8_t  u8;
	int8_t   s8;
	uint16_t u16;
	int16_t  s16;
	uint32_t u32;
	int32_t  s32;
	uint64_t u64;
	int64_t  s64;
	float    fp32;
	double   fp64;
};

typedef struct osens_brd_id_s
{
	uint8_t model[OSENS_MODEL_NAME_SIZE];
	uint8_t manufactor[OSENS_MANUF_NAME_SIZE];
	uint32_t sensor_id;
	uint8_t hardware_revision;
	uint8_t num_of_points;
	uint8_t capabilities;
} osens_brd_id_t;


typedef struct osens_point_desc_s
{
	uint8_t name[OSENS_POINT_NAME_SIZE];
	uint8_t type;
	uint8_t unit;
	uint8_t access_rights;
	uint32_t sampling_time_x250ms;
} osens_point_desc_t;

typedef struct osens_point_s
{
	union osens_point_data_u value;
    uint8_t type;
} osens_point_t;

//BEGIN_PACK
typedef struct addr_cmd_t_s
{                                 // always written big endian, i.e. MSB in addr[0]
   uint8_t type;
   union {
      uint8_t  u8val;            //address type 0
      uint16_t u16val;           //address type 1
      uint32_t u32val;           //address type 2
   };
} addr_cmd_t;
//END_PACK;

typedef struct
{
	uint8_t flashnewcmd;
	uint32_t frwnewEndAddr;
	uint16_t frameID;
	uint8_t  frameLen;
	uint8_t    framestatus;
	uint8_t    header;
	addr_cmd_t addrold;
	addr_cmd_t addrnew;
	addr_cmd_t chuncklen;
    uint8_t  chunck[50];
} osens_frm_t;


/* Mask definition for each field of status atribute of input parameter - FF IRDS */
#define MC_HEAD             (uint8_t)0x80
#define AS_HEAD             (uint8_t)0x60
#define LS_HEAD             (uint8_t)0x18
#define OP_HEAD             (uint8_t)0x07

/* Number of shift required for each bit field - FF IRDS */
#define MC_NS               7
#define AS_NS               5
#define LS_NS               3
#define OP_NS               0


#define MC_SHIFT(i)         (uint8_t)(((i) & MC_HEAD) >> MC_NS)
#define AS_SHIFT(i)         (uint8_t)(((i) & AS_HEAD) >> AS_NS)
#define LS_SHIFT(i)         (uint8_t)(((i) & LS_HEAD) >> LS_NS)
#define OP_SHIFT(i)         (uint8_t)(((i) & OP_HEAD) >> OP_NS)

#define MC(i)               (uint8_t)(((i) << MC_NS) & MC_HEAD)
#define AS(i)               (uint8_t)(((i) << AS_NS) & AS_HEAD)
#define LS(i)               (uint8_t)(((i) << LS_NS) & LS_HEAD)
#define OP(i)               (uint8_t)(((i) << OP_NS) & OP_HEAD)

enum EOPCode {iEqual=0, iReplace, iInsert, iDelete, iInfo, iCommit, iRetransmit};
enum ECommand {iAdLen1B=0, iAdLen2B, iAdLen4B};
enum EFlashNewCmd {iFlashNone=0, iFlashErase, iFlashChunck, iFlashFrmEnd};

uint8_t osens_init(void);
uint8_t osens_get_brd_desc(osens_brd_id_t *brd);
uint8_t osens_get_num_points(void);
uint8_t osens_get_point(uint8_t index, osens_point_t *point);
uint8_t osens_get_pdesc(uint8_t index, osens_point_desc_t *desc);
int8_t osens_get_ptype(uint8_t index);
uint8_t osens_set_pvalue(uint8_t index, osens_point_t *point);
uint8_t osens_liga_lampada_local(void);
extern int32_t FlashMainPageErase(uint32_t ui32Address);
uint8_t exefunctionInsert(osens_frm_t *posens_frm);
uint8_t exefunctionReplace(osens_frm_t *posens_frm);
uint8_t exefunctionEqual(osens_frm_t *posens_frm);
#endif /* __OSENS_H__ */

