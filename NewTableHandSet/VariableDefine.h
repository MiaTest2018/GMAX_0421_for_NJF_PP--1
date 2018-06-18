
/*-----( Declare Constants and Pin Numbers )-----*/
//#define RS485RX        10  //Serial Receive pin
//#define RS485TX        11  //Serial Transmit pin
#define RS485TxControl   53   //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define RH2				2
#define RE7				7
#define RD4				4

#define BatteryVOL_HIGH		2.18
#define BatteryVOL_MIDDLE	1.9
#define BatteryVOL_LOW		1.63

#define BtRX        17  //Serial Receive pin
#define BtTX        16  //Serial Transmit pin



#define BTConnectMode		4
#define RS485ConnectMode	5

//Keypad pins define
#define RowNumbers    8
#define ColNumbers    3

#define RowPin_1      48
#define RowPin_2      47
#define RowPin_3      46
#define RowPin_4	  45
#define RowPin_5      44
#define RowPin_6      43
#define RowPin_7      42
#define RowPin_8      36

#define ColPin_1      34
#define ColPin_2      33
#define ColPin_3      32




#define LED_BATTERYLOW_Pin			8

#define Button_POWERONPin		37
//#define BeepPin					50                                   //嗶嗶聲
#define BeepPin					13                                   //嗶嗶聲
//#define Pin13LED				13

#define LED_POWER_Pin			23
#define LED_BATTERY_Pin			24
#define LED_AC_Pin				25
#define LED_ReverseMode_Pin		26
#define LED_UnLock_Pin			29
#define LED_NormalMode_Pin		28
#define LED_Lock_Pin			27

#define LED_InterLock_Pin		31





//#define BatteryPowerDetectPin	37





#define buttonPinPOWEROFF_RE7		_BV(7)


#define intKeypadHoldTime 600
#define intKeypadDebounceTime 100

#define ReSendSurviveCommandTime	30000
#define ReSendCommandTime			3
#define LEDTwinklingTime			1

#define OffHandsentTime			2600000

#define BounceThreshold		1000
#define ReleaseThreshold		500

//Keypad - Button and Code Mapping
#define Button_LOCK				'D'
#define Button_MODE_NORMAL		'N'
#define Button_SAVE				'X'
#define Button_UNLOCK			'A'
#define Button_MODE_REV			'Q'
#define Button_RECALL			'C'
#define Button_TREND			'M'
#define Button_HEIGHT_UP		'I'
#define Button_TRENDREV			'F'
#define Button_TILT_LEFT		'S'
#define Button_HEIGHT_DOWN		'P'
#define Button_TILT_RIGHT		'L'
#define Button_SLIDE_LEG		'B' 
//#define Button_LEG_SW			'N'
#define Button_SLIDE_HEAD		'R'
#define Button_FLEX				'E'
#define Button_KIDNEY_DOWN		'Y'
#define Button_REFLEX			'U'
#define Button_BACK_UP			'O'
#define Button_KIDNEY_UP		'H'
#define Button_BACK_DOWN		'V'
#define Button_LEVEL			'K'
#define Button_RECALL_HEIGHT	'J'


#define Button_LEGLEFT_UP		'1' //need to define
#define Button_LEGLEFT_DOWN		'2' //need to define
#define Button_LEGRIGHT_UP      '3' //need to define
#define Button_LEGRIGHT_DOWN    '4' //need to define













//U8g display define
#define int_Mediland_Logo_diaplay_time 2000
#define u8g_SPI_Pin_SCK   10 
#define u8g_SPI_Pin_MOSI   9
//#define u8g_SPI_Pin_CS    12 
#define u8g_SPI_Pin_A0    11


//U8g OLED Display State TEXT
#define DisplayTEXT_LOCK			"Stanby"
#define DisplayTEXT_UNLOCK			"UnLock"
#define DisplayTEXT_MODE_NORMAL		"Normal"
#define DisplayTEXT_MODE_REV		"Rev."
#define DisplayTEXT_TREND			"TREND"
#define DisplayTEXT_TRENDREV		"REVERSE"
#define DisplayTEXT_HEIGHT_UP		"HEIGHT UP"
#define DisplayTEXT_HEIGHT_DOWN		"HEIGHT DOWN"
#define DisplayTEXT_TILT_LEFT		"TILT LEFT"
#define DisplayTEXT_TILT_RIGHT		"TILT RIGHT"
#define DisplayTEXT_BACK_UP			"BACK UP"
#define DisplayTEXT_BACK_DOWN		"BACK DOWN"
#define DisplayTEXT_LEGLEFT_UP      "L-LEG UP"
#define DisplayTEXT_LEGLEFT_DOWN    "L-LEG DOWN"
#define DisplayTEXT_LEGRIGHT_UP     "R-LEG UP"
#define DisplayTEXT_LEGRIGHT_DOWN   "R-LEG DOWN"
#define DisplayTEXT_LEGBOTH_UP      "LEG UP"
#define DisplayTEXT_LEGBOTH_DOWN    "LEG DOWN"
#define DisplayTEXT_FLEX			"FLEX"
#define DisplayTEXT_REFLEX			"REFLEX"
#define DisplayTEXT_SLIDING_FWD		"SLIDING FWD"
#define DisplayTEXT_SLIDING_BWD		"SLIDING BWD"
#define DisplayTEXT_LEVEL			"LEVEL"
#define DisplayTEXT_RECALL			"RECALL"
#define DisplayTEXT_SAVE			"SAVE"
#define DisplayTEXT_CHAIR			"CHAIR"


#define DisplayTEXT_Y_AXIS_MAIN 50
#define DisplayTEXT_Y_AXIS_UPPER 26

//設定 指令連續發送(1)或只送一次(0)
#define SettingCommandRepeat 1

//ACTION COMMAND
byte  CMD_TREND[3]			= { 0xA0, 0xD0, 0x70 };
byte  CMD_TREND_REV[3]		= { 0xA0, 0xD1, 0x71 };
byte  CMD_TREND_STOP[3]		= { 0xA0, 0xDF, 0x7F };

byte  CMD_HEIGHT_UP[3]		= { 0xA1, 0xD0, 0x71};
byte  CMD_HEIGHT_DOWN[3]	= { 0xA1, 0xD1, 0x72 };
byte  CMD_HEIGHT_STOP[3]	= { 0xA1, 0xDF, 0x80 };

byte  CMD_TILE_LEFT[3]		= { 0xA2, 0xD0, 0x72 };
byte  CMD_TILE_RIGHT[3]		= { 0xA2, 0xD1, 0x73 };
byte  CMD_TILE_STOP[3]		= { 0xA2, 0xDF, 0x81 };

byte  CMD_BACK_UP[3]		= { 0xA3, 0xD0, 0x73 };
byte  CMD_BACK_DOWN[3]		= { 0xA3, 0xD1, 0x74 };
byte  CMD_BACK_STOP[3]		= { 0xA3, 0xDF, 0x82 };

byte  CMD_LEGLEFT_UP[3]		= { 0xA4, 0xD0, 0x74 };
byte  CMD_LEGLEFT_DOWN[3]	= { 0xA4, 0xD1, 0x75 };
byte  CMD_LEGLEFT_STOP[3]	= { 0xA4, 0xDF, 0x83 };

byte  CMD_LEGRIGHT_UP[3]	= { 0xA5, 0xD0, 0x75 };
byte  CMD_LEGRIGHT_DOWN[3]	= { 0xA5, 0xD1, 0x76 };
byte  CMD_LEGRIGHT_STOP[3]	= { 0xA5, 0xDF, 0x84 };

byte  CMD_LEGBOTH_UP[3]		= { 0xA6, 0xD0, 0x76 };
byte  CMD_LEGBOTH_DOWN[3]	= { 0xA6, 0xD1, 0x77 };
byte  CMD_LEGBOTH_STOP[3]	= { 0xA6, 0xDF, 0x85 };
byte  CMD_REV_MODE[3]		= {	0xA6, 0x44, 0xEA };//RETURN 0X44
byte  CMD_NORMAL_MODE[3]	= { 0xA6, 0x43, 0xE9 };

byte  CMD_KIDNEY_UP[3]		= { 0xA7, 0xD0, 0x77 };
byte  CMD_KIDNEY_DOWN[3]	= { 0xA7, 0xD1, 0x78 };
byte  CMD_KIDNEY_STOP[3]	= { 0xA7, 0xDF, 0x86 };

byte  CMD_FLEX_UP[3]		= { 0xAA, 0xD2, 0x7C }; //RETURN CA
byte  CMD_FLEX_DOWN[3]		= { 0xAA, 0xD3, 0x7D }; //RETURN CB
byte  CMD_FLEX_STOP[3]		= { 0xAA, 0xDF, 0x89 }; 

byte  CMD_FLEXREV_UP[3]		= { 0xAA, 0xD4, 0x7E }; //RETURN C8
byte  CMD_FLEXREV_DOWN[3]	= { 0xAA, 0xD5, 0x7F }; //RETURN C9
byte  CMD_FLEXREV_STOP[3]	= { 0xAA, 0xDF, 0x89 }; 

byte  CMD_LEVEL[3]			= { 0xAC, 0xDD, 0x89 };
byte  CMD_LEVEL_STOP[3]		= { 0xAC, 0xDF, 0x8B }; //RETURN CD

byte  CMD_LOCK[3]			= { 0xB0, 0xD0, 0x80 };
byte  CMD_UNLOCK[3]			= { 0xB0, 0xD1, 0x81 };

byte  CMD_SAVE[3]			= { 0xAB, 0xD6, 0x81 };
byte  CMD_SAVE_HEIGHT[3]	= { 0xAB, 0xD8, 0x83 };
byte  CMD_RECALL[3]			= { 0xAD, 0xD7, 0x84 };
byte  CMD_RECALLHEIGHT[3]	= { 0xAD, 0xD9, 0x86 };
byte  CMD_RECALL_STOP[3]	= { 0xAD, 0xDF, 0x8C }; //RETURN CE

byte  CMD_STATUS_CHECK[3]	= { 0xAE, 0x42, 0xF0 }; //要求手術床回傳狀態

byte  CMD_SURVIVE_CHECK[3]	= {0xAE, 0x49, 0xF7};//確認床台與通訊 正常

byte  CMD_HANDSET_OFF[3]	= { 0xAE, 0x4A, 0xF8 };//告知手術床手控盒關閉

byte  CMD_AutoMove[3] = { 0xB5, 0xDA, 0x8F };//手術床自動移動_展會用
byte  CMD_AutoMove_STOP[3] = { 0xB5, 0xDF, 0x94 };
