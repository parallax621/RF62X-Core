#ifndef RF62X_TYPES_H
#define RF62X_TYPES_H


#include "platform_types.h"
#include <stddef.h>
#include <stdarg.h>


typedef struct{
    rfUint32 version;
    rfUint32 length;
    rfUint8 validated;
    rfUint8 reserved;
}config_options_t;

typedef struct
{
/*-------Общие параметры устройства-----------------------*/
    struct
    {
        rfUint16					device_id;			//Идентификатор типа сканера
        rfUint32					serial;				//Серийный номер сканера
        rfUint32					serial_of_pcb;		//Серийный номер электроники
        rfUint32					operating_time_h;	//Наработка сканера, часов
        rfUint8						operating_time_m;	//Наработка сканера, минут
        rfUint8						operating_time_s;	//Наработка сканера, секунд
        rfUint32					runtime_h;			//Время работы после запуска, часов
        rfUint8						runtime_m;	        //Время работы после запуска, минут
        rfUint8						runtime_s;	        //Время работы после запуска, секунд
        rfUint32					startup_counter;    //Количество запусков сканера
        rfUint32					firmware_ver;		//Версия прошивки
        rfUint32					hardware_ver;		//Версия аппаратного обеспечения
        rfUint32					customer_id;
        rfUint32					fpga_freq;		//Системная тактовая для PL - частота AXI в Гц
        rfUint32					base_z;
        rfUint32					range_z;
        rfUint32					range_x_start;
        rfUint32					range_x_end;
        rfUint16					pixels_divider;
        rfUint16					profiles_divider;
        rfUint32					fsbl_version;		//Версия загрузчика
        rfChar                      oem_device_name[16];
        rfUint8						reserved[76];
    }general;
/*-------Параметры установленной матрицы-------------------*/
    struct
    {
        rfChar						name[32];					//Имя матрицы - для отображения
        rfUint16					width;						//Количество пикселов по горизонтали
        rfUint16					height;						//Количество пикселов но вертикали
        rfUint32					pixel_clock;					//Пиксельная частота в Гц
        rfUint16					black_odd_lines;
        rfUint16					black_even_lines;
        rfUint32					frame_cycle_const_part;
        rfUint32					frame_cycle_per_line_part;
        rfUint8						frame_rate_or_exposure;		//Селектор по чем настраивать матрицу - по частоте кадров или экспозиции: 0 - по частоте, 1 - по экспозиции
        rfUint32					min_exposure;				//Минимальное время экспозиции в нс.
        rfUint8						image_flipping;				//Режим отражения изображения: 0 - без отражения, 1 - по X, 2 - по Y, 3 - и по X и по Y
        rfUint32					max_exposure;				//Максимальное время экспозиции, заданное при настройке сканера
        rfUint8						edr_point1_value;				//Значение напряжения (в условных единицах 0..63) в 1-й точке излома прямой накопления (стр.30-31 даташита CMV300)
        rfUint8						edr_point2_value;				//Значение напряжения (в условных единицах 0..63) во 2-й точке излома прямой накопления (стр.30-31 даташита CMV300)
        rfUint16					edr_point1_pos;				//Положение в 1/1000 точки относительно начала экспозиции, т.е. 0 - одновременно с началом экспозиции, 999 - сразу в конце
        rfUint16					edr_point2_pos;				//Положение в 1/1000 точки относительно начала экспозиции, т.е. 0 - одновременно с началом экспозиции, 999 - сразу в конце
        rfUint8						reserved[113];
        struct
        {
            rfUint16 addr;
            rfUint16 value;
        }init_regs[64];               //Структура с параметрами инициализации регистров матрицы
    }sensor;
/*-------Параметры сетевого подключения-------------------*/
    struct
    {
        rfUint8						mac[6];
        rfUint16					eip_vendor_id;
        rfUint16					eip_device_type;
        rfUint8						force_autoneg_time;			//Количество секунд, через которое сканер установит автосогласование принудительно в случае отсутствия связи по Ethernet
        rfUint8						reserved[31];
    }network;
/*-------Параметры управления лазером----------------------*/
    struct
    {
        rfUint16					wave_length;				//Длина волны в нм
        rfUint8						koeff1;					//Коээфициент крутизны регулирования отпределяется как Koeff1/128
        rfUint8						koeff2;
        rfUint32					min_value;				//Значение, при котором лазер зажигается
        rfUint32					max_value;				//Максимальное допустимое значение
        rfUint8						enable_mode_change;		//Разрешение изменения режима работы лазера: 0 - запрещено, 1 - разрешено
        rfUint8						reserved[31];
    }laser;
/*------------Параметры входных каналов--------------------*/
    struct
    {
        rfUint16					in1_min_delay;			//Минимальная задержка в нс
        rfUint32					in1_max_delay;			//Максимальная задержка в нс
        rfUint16					max_divider_in1;			//Максимальное значение делителя частоты кадров
        rfUint16					min_divider_in1;			//Минимальное значение делителя частоты кадров
        rfUint8						reserved[62];
    }inputs;
/*------------Параметры выходных каналов-------------------*/
    struct
    {
        rfUint16					out1_min_delay;
        rfUint32					out1_max_delay;
        rfUint16					out1_min_pulse_width;
        rfUint32					out1_max_pulse_width;
        rfUint16					out2_min_delay;
        rfUint32					out2_max_delay;
        rfUint16					out2_min_pulse_width;
        rfUint32					out2_max_pulse_width;
        rfUint8						reserved[32];
    }outputs;
    /*------------Параметры обработки профилей-----------------*/
    struct
    {
        rfUint32					max_dump_size;
        rfUint8						reserved[60];
    }profiles;
    /*------------Параметры протокола EthernetIP---------------*/
    struct
    {
        struct
        {
            rfUint16				vendor_iD;				// Attribute 1: Vendor ID - идентификационный номер производителя устройства
            rfUint16				device_type;				// Attribute 2: Device Type - тип устройства, который определяет его область использования
            rfUint16				product_code;			// Attribute 3: Product Code - идентифицирует номер конкретного продукта
            struct
            {	uint8_t					major_rev;			// Attribute 4: Revision	USINT Major
                rfUint8					minor_rev;			//							USINT Minor
            }revision;										// Структура, хранящая EIP-версию девайса
        }identity;

        struct
        {
            rfUint32				config_capability;		// Attribute 2: Данный атрибут описывает поддержку устройством дополнительных возможностей конфигурации сети
            struct
            {	uint16_t				path_size;			// Path size in 16 bit words (path_size * 16 bit)
                rfUint16				classId;			// Class ID of the linked object
                rfUint16				instance_number;		// Requested Instance Number of the linked object
                rfUint16				attribute_number;	// Requested Attribute Number of the linked object
            }phyLinkObject;									// Attribute 4: Этот атрибут идентифицирует объект, связанный с базовым физическим интерфейсом связи (Ethernet Link Object)
        }tcpInterface;

        rfUint8						intrf_type;				//Type of rfInterface: twisted pair, fiber, rfInternal, etc
        struct
        {	uint16_t					capability_bits;
            rfUint8						speed_dup_count;
            struct
            {
                rfUint16					speed;
                rfUint8						duplex;
            }speedDuplex[4];
        }intrfCapability;									//Path to physical link object

        rfUint8						reserved[44];
    }eip;
    /*-------Резерв--------------------------------------------*/
    rfUint8							reserved[167];
    /*---------------------------------------------------------*/
}rf627_old_factory_params_t;

typedef struct
{
    rfChar                        Tag[4];            //<! File header tag = "DTF+"
    rfUint16                    FormatVersion;     //<! Version of header format (1)
    struct
    {
        struct
        {
            rfUint8            Day;
            rfUint8            Month;
            rfUint16            Year;
        }date;
        struct
        {
            rfUint8            Hour;
            rfUint8            Minute;
            rfUint8            Second;
        }time;
    }timestamp;                                     //<! date and time of calibration
    rfUint16                    DevType;            //<! Device type (627)
    rfUint32                    SerialNumber;       //<! Serial Number of Scanner
    rfUint16					BaseZ;              //<! Z base, mm
    rfUint16					RangeZ;             //<! Z measurement range, mm
    rfUint16					RangeXStart;        //<! X start measurement range, mm
    rfUint16					RangeXEnd;          //<! X end measurement range, mm
    rfUint16					YSteps;             //<! Number of calibration steps
    rfUint16					StartPosition;      //<! Start position
    rfUint16					EndPosition;        //<! End position
    rfUint16					CutXLeft;           //<! Amount of pixels to cut from left
    rfUint16					CutXRight;          //<! Amount of pixels to cut from right
    rfUint8                     PlaneType;          //<! 0 - ZPlane, 1 - TrianglesXY
    rfUint8                     InvertZ;            //<! 0 - no invert, other - invert
    rfUint32					TriangleWidth;      //<! Triangle width, um
    rfUint32					TriangleHeight;     //<! Triangle height, um
    rfUint8                     AveragingSize;      //<! Number of pixels profiles for averaging
    rfUint32                    Exposure;           //<! Exposure time, ns (0 for Auto)
    rfInt16                     Temp;               //<! Temperature upon calibration, C/10
    rfDouble                      StepLength;         //<! Z Step Length
    rfUint32					FirmwareVer;		//<! Firmware Version
    rfUint32					HardwareVer;		//<! Hardware Version
    rfUint8                     Reserved[186];      //<!- Reserved
}DTF_header;

typedef struct
{
    rfUint8                    FormatVersion;     //<! Version of header format (1)
    rfUint16                    CRC;                //<!- CRC
    rfUint32                    SerialNumber;       //<! Serial Number of Scanner
    struct
    {
        struct
        {
            rfUint16            Year;
            rfUint8            Month;
            rfUint8            Day;
        }date;
        struct
        {
            rfUint8            Hour;
            rfUint8            Minute;
            rfUint8            Second;
        }time;
    }timestamp;                                     //<! date and time of calibration
    rfUint8                     Zero[242];          //<! Must be filled with zeroes
}TBL_header;

typedef struct
{
    struct
    {
        rfChar        name[64];
        rfUint8     save_log_to_spi;
        rfUint8		reserved[127];
    }general;

    struct
    {
        rfInt16		fpga_temp;
        rfUint8		params_changed;			//Параметры были изменены, но не сохранены: 1 - factory, 2 - user, 3 - factory & user
        rfInt16		sens00_temp;
        rfInt16		sens00_max;
        rfInt16		sens00_min;
        rfInt16		sens01_temp;
        rfInt16		sens01_max;
        rfInt16		sens01_min;
        rfInt16		sens10_temp;
        rfInt16		sens10_max;
        rfInt16		sens10_min;
        rfInt16		sens11_temp;
        rfInt16		sens11_max;
        rfInt16		sens11_min;
        rfUint8		reserved[55];
    }sysmon;

    struct
    {
        rfUint8		enable;
        rfUint16	tcp_port;
        rfUint8		reserved[32];
    }rf625compat;

    struct
    {
        rfUint8		dhs;
        rfUint8		gain_analog;
        rfUint8		gain_digital;
        rfUint32	exposure;
        rfUint32	max_exposure;
        rfUint32	frame_rate;
        rfUint32	max_frame_rate;
        rfUint8     exposure_hdr_mode;
        rfUint8     auto_exposure;
        rfUint8		column_edr_mode;
        rfUint8		column_exposure_div;
        rfUint8     column_exposure_max_div;
        rfUint8		reserved_1[59];
    }sensor;

    struct
    {
        rfUint8		enable;
        rfUint8		active;
        rfUint16	size;
        rfUint8		position_mode;
        rfUint16	manual_position;
        rfUint16	auto_position;
        rfUint16	required_profile_size;
        rfUint8		reserved[80];
    }roi;

    struct
    {
        rfUint16	speed;
        rfUint8		autonegotiation;
        rfUint8     ip_address[4];
        rfUint8     net_mask[4];
        rfUint8     gateway_ip[4];
        rfUint8     host_ip[4];
        rfUint16    stream_port;
        rfUint16    http_port;
        rfUint16    service_port;
    //    rfUint16    eip_broadcast_port;
    //    rfUint16    eip_port;
        rfUint8		reserved[68];
    }network;

    struct
    {
        rfUint8		enable;
        rfUint8		format;
        rfUint8		ack;
        rfUint8     include_intensivity;
        rfUint8		reserved[31];
    }stream;

    struct
    {
        rfUint32	brightness_threshold;
        rfUint8		filter_width;
        rfUint8		processing_mode;
        rfUint8		reduce_noise;
        rfUint32    frame_rate;
        rfUint8		median_filter_mode;
        rfUint8		bilateral_filter_mode;
        rfUint8     peak_select_mode;
        rfUint8     profile_flip;
        rfUint8		reserved[56];
    }image_processing;

    struct
    {
        rfUint8		enable;
        rfUint8		level_mode;
        rfUint16	level;
        rfUint8		reserved[32];
    }laser;

    struct
    {
        rfUint8                 preset_index;
        struct
        {
            rfUint16    params_mask;
            rfUint8		in1_enable;
            rfUint8		in1_mode;
            rfUint32	in1_delay;
            rfUint8		in1_decimation;
            rfUint8		in2_enable;
            rfUint8		in2_mode;
            rfUint8		in2_invert;
            rfUint8		in3_enable;
            rfUint8		in3_mode;
            rfUint8		reserved[12];
        }params[12];
        rfUint8                 reserved[32];
    }inputs;

    struct
    {
        rfUint8		out1_enable;
        rfUint8		out1_mode;
        rfUint32	out1_delay;
        rfUint32	out1_pulse_width;
        rfUint8		out1_invert;
        rfUint8		out2_enable;
        rfUint8		out2_mode;
        rfUint32	out2_delay;
        rfUint32	out2_pulse_width;
        rfUint8		out2_invert;
        rfUint8		reserved[32];
    }outputs;

    rfUint8                   reserved[283];
}rf627_old_user_params_t;

typedef struct
{
    rfUint8     data_type;
    rfUint8     flags;
    rfUint16    device_type;
    rfUint32    serial_number;
    rfUint64    system_time;

    rfUint8     proto_version_major;
    rfUint8     proto_version_minor;
    rfUint8     hardware_params_offset;
    rfUint8     data_offset;
    rfUint32    packet_count;
    rfUint32    measure_count;

    rfUint16    zmr;
    rfUint16    xemr;
    rfUint16    discrete_value;
    rfUint8     reserved_0[10];
    rfUint32    license_hash;

    rfUint32    exposure_time;
    rfUint32    laser_value;
    rfUint32    step_count;
    rfUint8     dir;
    rfUint16    payload_size;
    rfUint8     bytes_per_point;
}
rf627_old_profile_header_t;

typedef struct{
    struct{
        rfUint32 H;
        rfUint8  M;
        rfUint8  S;
    }Time;
    rfUint8  ModuleID;
    rfUint8  EventID;
    rfChar     String[128];
}log_record_t;

typedef enum
{
    DTY_PixelsNormal        = 0x10,
    DTY_ProfileNormal       = 0x11,
    DTY_PixelsInterpolated  = 0x12,
    DTY_ProfileInterpolated = 0x13
} rf627_old_data_type_t;

/*! Structure to store a point of profile
 */
typedef struct
{
    rfFloat x;
    rfFloat z;
}rf627_old_point2D_t;

/*! Structure to store a point of profile
 */
typedef struct
{
    rfFloat x;
    rfFloat y;
    rfFloat z;
}rf627_old_point3D_t;

/*! Structure to store a profile for rf627_old
 */
typedef struct
{
    rf627_old_profile_header_t header;
    union{
        struct{
            rfUint16* pixels;
            rfUint32 pixels_count;
        }pixels_format;
        struct{
            rf627_old_point2D_t* points;
            rfUint32 points_count;
        }profile_format;
    };
    rfUint8* intensity;
    rfUint32 intensity_count;
}rf627_old_profile2D_t;

/*! Structure to store a profile for rf627_old
 */
typedef struct
{
    rf627_old_profile_header_t header;
    union{
        struct{
            rfUint16* pixels;
            rfUint32 pixels_count;
        }pixels_format;
        struct{
            rf627_old_point3D_t* points;
            rfUint32 points_count;
        }profile_format;
    };
    rfUint8* intensity;
    rfUint32 intensity_count;
}rf627_old_profile3D_t;


typedef struct
{
    rfChar* data;
    rfUint32 data_size;
    rfUint32 width;
    rfUint32 height;
    rfUint8 pixel_size;

    rfBool user_roi_active;
    rfBool user_roi_enabled;
    rfUint32 user_roi_pos;
    rfUint32 user_roi_size;
}rf627_old_frame_t;

typedef struct
{
    rfChar* data;
}rf627_old_calib_table_t;

/*! Structure to store a profile for rf627_old
 */
typedef struct
{
    rf627_old_profile_header_t header;
    union{
        struct{
            rfUint16* pixels;
            rfUint32 pixels_count;
        }pixels_format;
        struct{
            rf627_old_point2D_t* points;
            rfUint32 points_count;
        }profile_format;
    };
    rfUint8* intensity;
    rfUint32 intensity_count;
}rf627_smart_profile2D_t;

/*! Structure to store a profile for rf627_old
 */
typedef struct
{
    rf627_old_profile_header_t header;
    union{
        struct{
            rfUint16* pixels;
            rfUint32 pixels_count;
        }pixels_format;
        struct{
            rf627_old_point3D_t* points;
            rfUint32 points_count;
        }profile_format;
    };
    rfUint8* intensity;
    rfUint32 intensity_count;
}rf627_smart_profile3D_t;

typedef struct
{
    rfChar* data;
    rfUint32 data_size;
    rfUint32 frame_width;
    rfUint32 frame_height;
    rfUint8 pixel_size;

    rfBool user_roi_active;
    rfBool user_roi_enabled;
    rfUint32 user_roi_pos;
    rfUint32 user_roi_size;
    rfUint32 fact_sensor_width;
    rfUint32 fact_sensor_height;
}rf627_smart_frame_t;

typedef struct
{
    rfUint16 m_Type;
    rfUint16 m_CRC16;
    rfUint32 m_Serial;
    rfUint32 m_DataRowLength;
    rfUint32 m_Width;
    rfUint32 m_Height;
    rfUint32 m_MultW;
    rfUint32 m_MultH;
    rfInt m_TimeStamp;

    rfUint8* m_Data;
    rfUint32 m_DataSize;
}rf627_smart_calib_table_t;

typedef struct
{
    rfChar* device_name;
    rfUint32 serial_number;
    rfUint8 ip_address[4];
    rfUint8 mac_address[6];
    rfUint16 profile_port;
    rfUint16 service_port;
    rfUint32 firmware_version;
    rfUint32 hardware_version;
    rfUint32 z_begin;
    rfUint32 z_range;
    rfUint32 x_begin;
    rfUint32 x_end;
}rf627_old_hello_info_by_service_protocol;

typedef struct
{
    uint32_t fact_general_firmwareVer[3];
    uint32_t fact_general_hardwareVer;

    uint32_t fact_general_mr;
    uint32_t fact_general_smr;
    uint32_t fact_general_xsmr;

    uint32_t fact_laser_waveLength;

    char* user_general_deviceName;
    uint32_t fact_general_productCode;
    uint32_t fact_general_serial;

    char* user_network_ip;
    char* user_network_mask;
    char* user_network_gateway;
    char* user_network_hostIP;
    char* fact_network_macAddr;

    uint32_t user_network_hostPort;
    uint32_t user_network_servicePort;
    uint32_t user_network_webPort;

    uint32_t user_network_speed;
    uint8_t user_network_autoNeg;

    uint32_t user_streams_udpEnabled;
    uint32_t user_streams_format;

    uint32_t fact_general_xemr;
    uint32_t fact_maxPacketSize;

}rf627_smart_hello_info_by_service_protocol;

typedef struct
{
    char* cmd_name;
    uint8_t is_check_crc;
    uint8_t is_confirmation;
    uint8_t is_one_answ;
    uint32_t waiting_time;
    uint32_t resends_count;
}rf627_smart_protocol_cmd_settings_t;

//Структура для перечислений - сопоставляет значение параметра и его текстовый ключ
typedef struct
{
    rfInt32				value;
    rfChar*				label;
    rfChar*				key;
}enumRec_t;

typedef struct
{
    rfUint32			recCount;
    enumRec_t*			rec;
}valuesEnum_t;

typedef struct
{
    rfChar*             name;
    rfChar*             type;
    rfChar*             access;
    rfUint16			index;
    rfUint32			offset;
    rfUint32			size;
    rfChar*             units;
}value_base_t;

typedef struct value_uint32_t
{
    rfUint32			min;
    rfUint32			max;
    rfUint32			step;
    rfUint32			defValue;
    valuesEnum_t*		enumValues;
    rfUint32			value;

}value_uint32_t;

typedef struct value_uint64_t
{
    rfUint64			min;
    rfUint64			max;
    rfUint64			step;
    rfUint64			defValue;
    valuesEnum_t*		enumValues;
    rfUint64			value;
}value_uint64_t;

typedef struct value_int32_t
{
    rfInt32				min;
    rfInt32				max;
    rfInt32				step;
    rfInt32				defValue;
    valuesEnum_t*		enumValues;
    rfInt32				value;
}value_int32_t;

typedef struct value_int64_t
{
    rfInt64				min;
    rfInt64				max;
    rfInt64				step;
    rfInt64				defValue;
    valuesEnum_t*		enumValues;
    rfInt64				value;
}value_int64_t;

typedef struct value_flt_t
{
    rfFloat				min;
    rfFloat				max;
    rfFloat				step;
    rfFloat				defValue;
    rfFloat				value;
}value_flt_t;

typedef struct value_dbl_t
{
    rfDouble				min;
    rfDouble				max;
    rfDouble				step;
    rfDouble				defValue;
    rfDouble				value;
}value_dbl_t;

typedef struct array_uint32_t
{
    rfUint32			min;
    rfUint32			max;
    rfUint32			step;
    rfUint32			maxCount;
    rfUint32			defCount;
    rfUint32*			defValue;
    rfUint32			count;
    rfUint32*			value;
}array_uint32_t;

typedef struct array_uint64_t
{
    rfUint64			min;
    rfUint64			max;
    rfUint64			step;
    rfUint32			maxCount;
    rfUint32			defCount;
    rfUint64*			defValue;
    rfUint32			count;
    rfUint64*			value;
}array_uint64_t;

typedef struct array_int32_t
{
    rfInt32				min;
    rfInt32				max;
    rfInt32				step;
    rfUint32			maxCount;
    rfUint32			defCount;
    rfInt32*			defValue;
    rfUint32			count;
    rfInt32*			value;
}array_int32_t;

typedef struct array_int64_t
{
    rfInt64				min;
    rfInt64				max;
    rfInt64				step;
    rfUint32			maxCount;
    rfUint32			defCount;
    rfInt64*			defValue;
    rfUint32			count;
    rfInt64*			value;
}array_int64_t;

typedef struct array_flt_t
{
    rfFloat				min;
    rfFloat				max;
    rfFloat				step;
    rfUint32			maxCount;
    rfUint32			defCount;
    rfFloat*			defValue;
    rfUint32			count;
    rfFloat*			value;
}array_flt_t;

typedef struct array_dbl_t
{
    rfDouble				min;
    rfDouble				max;
    rfDouble				step;
    rfUint32			maxCount;
    rfUint32			defCount;
    rfDouble*				defValue;
    rfUint32			count;
    rfDouble*				value;
}array_dbl_t;

typedef struct value_str_t
{
    rfUint16			maxLen;
    rfChar*				defValue;
    rfChar*				value;
}value_str_t;


typedef struct
{
    value_base_t			base;
    union{
        void*				rawData;
        value_uint32_t*		val_uint32;
        value_uint64_t*		val_uint64;
        value_int32_t*		val_int32;
        value_int64_t*		val_int64;
        value_flt_t*		val_flt;
        value_dbl_t*		val_dbl;
        array_uint32_t*		arr_uint32;
        array_uint64_t*		arr_uint64;
        array_int32_t*		arr_int32;
        array_int64_t*		arr_int64;
        array_flt_t*		arr_flt;
        array_dbl_t*		arr_dbl;
        value_str_t*		val_str;
    };

    uint8_t is_changed;
}parameter_t;


typedef struct
{
    const rfChar* name;

    va_list arg_list;
}command_t;

typedef struct
{
    const rfChar* name;
    struct input_t{
        rfUint16 size;
        rfUint8* payload;
    }input;
    struct output_t{
        rfUint16 size;
        rfUint8* payload;
    }output;
}command2_t;




typedef enum {
    kRF627_OLD = 1,
    kRF627_SMART = 2
}scanner_types_t;

typedef enum {
    kSERVICE = 1,
    kETHERNET_IP = 2,
    kMODBUS_TCP = 3,
}protocol_types_t;

typedef enum {
    kSTEP = 1,
    kMEASURE = 2,
    kPACKET = 3,
}count_types_t;

/*! Structure to store a profile
 */
typedef struct
{
    scanner_types_t type;
    union{
        rf627_old_profile2D_t* rf627old_profile2D;
        rf627_smart_profile2D_t* rf627smart_profile2D;
    };
}rf627_profile2D_t;

typedef struct
{
    scanner_types_t type;
    union{
        rf627_old_frame_t* rf627old_frame;
        rf627_smart_frame_t* rf627smart_frame;
    };
}rf627_frame_t;

typedef struct
{
    scanner_types_t type;
    union{
        rf627_old_calib_table_t* rf627old_calib_table;
        rf627_smart_calib_table_t* rf627smart_calib_table;
    };
}rf627_calib_table_t;

/*! Structure to store a profile
 */
typedef struct
{
    scanner_types_t type;
    union{
        rf627_old_profile3D_t* rf627_profile3D;
        rf627_smart_profile3D_t* rf627smart_profile3D;
    };
}rf627_profile3D_t;

/*! Structure of hello info about scanner
 */
typedef struct
{
    scanner_types_t scanner_type;
    protocol_types_t protocol_type;
    union{
        union{
            rf627_old_hello_info_by_service_protocol* hello_info_service_protocol;
        }rf627old;
        union{
            rf627_smart_hello_info_by_service_protocol* hello_info_service_protocol;
        }rf627smart;
    };
}hello_information;

#endif // RF62X_TYPES_H
