/*
Amadis Hand Controller
2015-05-20 Owen Cheng
*/

/*-----( include lib )-----*/

#include "./Keypad.h"
#include "./VariableDefine.h"
//#include "./BitmapData.h"
#include <Wire.h> 
//#include <LiquidCrystal_I2C.h>
#include "./TimerThree.h"
#include <stdlib.h>

//#include <SoftwareSerial.h>


/*-----( Declare objects )-----*/
//SoftwareSerial RS485Serial(RS485RX, RS485TX); // RX, TX

/*-----( Declare Variables )-----*/

//byte serialInString[100];
//int serialInIndex =0;
//int serialOutIndex =0;

byte rs485InString[50];
byte rs485Command[50];
int rs485InIndex = 0;
int rs485OutIndex = 0;

boolean RS485ReadComplete = false;
int statusSendCMD = 0;

byte CommandNow[3];

/*-----( Define the Keymap )-----*/
char keys[RowNumbers][ColNumbers] = {
	//{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' },
	//{ 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P' },
	//{ 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X' }
	{ 'A', 'B', 'C' },
	{ 'D', 'E', 'F' },
	{ 'G', 'H', 'I' },
	{ 'J', 'K', 'L' },
	{ 'M', 'N', 'O' },
	{ 'P', 'Q', 'R' },
	{ 'S', 'T', 'U' },
	{ 'V', 'X', 'Y' }
};

byte rowPins[RowNumbers] = { RowPin_1, RowPin_2, RowPin_3, RowPin_4, RowPin_5, RowPin_6, RowPin_7, RowPin_8 };
byte colPins[ColNumbers] = { ColPin_1, ColPin_2, ColPin_3 };

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, RowNumbers, ColNumbers);



/*-----( Variables - Status )-----*/

bool status_Power = false;
bool status_Lock = true;


bool ButtonLockFlag = false;
bool InterLockFlag = false;








unsigned char state_Action = '0';

int StateTimerAction = 0;
int MODE_Repeat = 1;

int flagAceeptLEVEL = 1;
int flagAceeptFLEX_UP = 1;
int flagAceeptFLEX_DOWN = 1;
int flagAceeptREVFLEX_UP = 1;
int flagAceeptREVFLEX_DOWN = 1;
int flagAceeptRECALL = 1;
int flagAceeptRECALLHEIGHT = 1;
unsigned int PowerButtonCnt = 0;

unsigned int ConnectionSurviveCnt = 0;

unsigned int RS485ReSendCnt = 0;//有線通訊 重送 Survive Command 次數

unsigned int LEDTwinklingCnt = 0;

unsigned int ServiceLEDTwinklingCnt = 0;

bool ServiceLEDTwinklingON = false;

bool PowerButtonPressed = false;

unsigned long OffHandsentCnt = 0;
bool OffHandsetFlag = false;

bool TREND_DisableFlag = false;
bool TRENDRev_DisableFlag = false;
bool Tile_Right_DisableFlag = false;
bool Tile_Left_DisableFlag = false;

bool Back_Up_DisableFlag = false;
bool Back_Down_DisableFlag = false;
bool Leg_Up_DisableFlag = false;
bool Leg_Down_DisableFlag = false;

bool Kidney_Up_DisableFlag = false;
bool Kidney_Down_DisableFlag = false;

byte InterLockStatus_1 = 0;//右背與左背
byte InterLockStatus_2 = 0;//右腳與左腳
byte InterLockStatus_3 = 0;//平移
byte InterLockStatus_4 = 0;//縱轉與橫轉

bool Button_Click_Enable = false;

bool Button_Hold_Enable = false;

unsigned long POWER_OFF_ButtonBounceCnt = 0;
unsigned long POWER_OFF_ButtonReleaseCnt = 0;

unsigned long POWER_ON_ButtonBounceCnt = 0;
unsigned long POWER_ON_ButtonReleaseCnt = 0;

bool Button_TrendRev_Pressed = false;
bool Button_Trend_Pressed = false;

bool Button_Tile_Left_Pressed = false;
bool Button_Tile_Right_Pressed = false;


bool Button_Back_Up_Pressed = false;
bool Button_Back_Down_Pressed = false;

bool Button_Slide_Head_Pressed = false;
bool Button_Slide_Leg_Pressed = false;

bool Button_Height_Up_Pressed = false;
bool Button_Height_Down_Pressed = false;

bool Button_Level_Pressed = false;


bool Button_Lock_Presssed = false;
bool Button_UnLock_Presssed = false;

unsigned int AutoMove_Flag = 0;//1-->AutoMove_Enable=true; 0-->AutoMove_Enable=false
bool AutoMove_Enable = false;//
bool LevelEnable_Flag = false;

unsigned long Star_ScanTime = 0;
unsigned long End_ScanTime = 0;
unsigned long CNTTime = 0;
unsigned long CNTTime2 = 0;

bool Button_AutoMove_Level = false;
unsigned int countPowerOFF = 0;

void serialEvent3()//RS485
{
	byte sb;
	byte tempCKS = 0;
	unsigned long ScanTimeTemp = 0;
	while (Serial3.available())
	{
		sb = Serial3.read();
		rs485InString[rs485InIndex] = sb;
		//Serial.print("485IN :");
		//Serial.print(rs485InIndex);
		//Serial.print(": ");
		//Serial.println(rs485InString[rs485InIndex], HEX);
		rs485InIndex++;
		delay(5);
		switch (rs485InString[0])
		{
		//case 0xCF:
		//	End_ScanTime = micros();
		//	ScanTimeTemp = End_ScanTime - Star_ScanTime;
		//	Serial.print("ScanTimeTemp :");
		//	Serial.println(ScanTimeTemp);
			//break;
		case 0xAE://手術床回傳狀態值
			if (rs485InString[12] == '\r')
			{

				tempCKS = rs485InString[0] + rs485InString[1] + rs485InString[2] + rs485InString[3] + rs485InString[4]
					+ rs485InString[5] + rs485InString[6] + rs485InString[7] + rs485InString[8] + rs485InString[9] + rs485InString[10];

				if (tempCKS == rs485InString[11])
				{
					RS485ReadComplete = true;
					for (int i = 0; i <= rs485InIndex; i++)
					{
						rs485Command[i] = rs485InString[i];
						rs485InString[i] = 0;
					}
					rs485InIndex = 0;
					return;
				}
				else
				{
					for (int a = 0; a < 50; a++)
					{
						rs485InString[a] = 0;
					}
					rs485InIndex = 0;
				}
			}
			else if ((rs485InString[12] != '\r') && (rs485InIndex > 12))
			{
				for (int a = 0; a < 50; a++)
				{
					rs485InString[a] = 0;
				}
				rs485InIndex = 0;
			}
			break;
		case 0x43://Normal Mode
		case 0x44://Reverse Mode
		case 0xC6://Lock
		case 0xC7://UnLock
		case 0xC8://ReFlex Up
		case 0xC9://ReFlex Down
		case 0xCA://Flex up
		case 0xCB://Flex down
		case 0xCD://Level
		case 0xCE://Recall
		case 0xC2://Recall Height
		case 0xC5://Survive	
		case 0xB1: //Warring Beep
		//case 0xCF:
			if (rs485InString[1] == 0x0D)  //CR
			{
				for (int i = 0; i <= rs485InIndex; i++)
				{
					rs485Command[i] = rs485InString[i];
					rs485InString[i] = 0;
				}
				rs485InIndex = 0;
				RS485ReadComplete = true;
				return;
			}
			else if ((rs485InString[1] != '\r') && (rs485InIndex > 1))
			{
				for (int a = 0; a < 50; a++)
				{
					rs485InString[a] = 0;
				}
				rs485InIndex = 0;
			}
			break;
		default:
			for (int a = 0; a < 50; a++)
			{
				rs485InString[a] = 0;
			}
			rs485InIndex = 0;
			break;
		}
	}
}

void Send485Command(byte* cmd)
{
	digitalWrite(RS485TxControl, RS485Transmit);
	Serial3.write(cmd, 3);
	Serial3.write(0x0D);

	CommandNow[0] = cmd[0];
	CommandNow[1] = cmd[1];
	CommandNow[2] = cmd[2];

	delay(5);
	digitalWrite(RS485TxControl, RS485Receive);
	delay(5);

}

void Send485Command(byte* cmd, int _times)
{
	for (int i = 0; i < _times; i++)
	{
		digitalWrite(RS485TxControl, RS485Transmit);
		Serial3.write(cmd, 3);
		Serial3.write(0x0D);

		CommandNow[0] = cmd[0];
		CommandNow[1] = cmd[1];
		CommandNow[2] = cmd[2];

		delay(5);
		digitalWrite(RS485TxControl, RS485Receive);
		delay(5);
	}
}

void Send485RepeatCommand()
{
	if (StateTimerAction == 1)
	{
		digitalWrite(RS485TxControl, RS485Transmit);
		delay(1);
		Serial1.write(CommandNow, 3);
		delay(2);
		Serial1.write(0x0D);
		//Serial1.flush();
		delay(5);
		digitalWrite(RS485TxControl, RS485Receive);
		delay(5);
	}
	else
	{
		CommandNow[0] = 0x00;
		CommandNow[1] = 0x00;
		CommandNow[2] = 0x00;
	}
}

void ChangeAllFlagAccept()
{
	flagAceeptLEVEL = 1;
	flagAceeptFLEX_UP = 1;
	flagAceeptFLEX_DOWN = 1;
	flagAceeptREVFLEX_UP = 1;
	flagAceeptREVFLEX_DOWN = 1;
	flagAceeptRECALL = 1;
	flagAceeptRECALLHEIGHT = 1;
}

void CheckRS485CmdRequest()
{
	unsigned long ScanTimeTemp = 0;
	//if (rs485InIndex > 0) {
	if (RS485ReadComplete)
	{
		switch (rs485Command[0])
		{
		case 0xCA://FLEX STOP
			if (flagAceeptFLEX_UP)
			{
				flagAceeptFLEX_UP = 0;
				if (status_Power)
					Beep(1000);
			}
			break;
		case 0xCB://REFLEX STOP
			if (flagAceeptFLEX_DOWN)
			{
				flagAceeptFLEX_DOWN = 0;
				if (status_Power)
					Beep(1000);
			}
			break;
		case 0xC8: //REFLEX STOP
			if (flagAceeptREVFLEX_UP)
			{
				flagAceeptREVFLEX_UP = 0;
				if (status_Power)
					Beep(1000);
			}
			break;
		case 0xC9: //REFLEX STOP
			if (flagAceeptREVFLEX_DOWN)
			{
				flagAceeptREVFLEX_DOWN = 0;
				if (status_Power)
					Beep(1000);
			}
			break;
		case 0xCD: //LEVEL STOP 
			if (flagAceeptLEVEL)
			{
				flagAceeptLEVEL = 0;
				Serial.println("LEVEL Finish");
				if (status_Power)
					Beep(1000);
			}
			break;
		case 0xCE: //RECALL STOP
			if (flagAceeptRECALL)
			{
				flagAceeptRECALL = 0;
				if (status_Power)
					Beep(1000);
			}
			break;
		case 0xC2: //RECALL HEGIHT STOP
			if (flagAceeptRECALLHEIGHT)
			{
				flagAceeptRECALLHEIGHT = 0;
				if (status_Power)
					Beep(1000);
			}
			break;
		case 0xC6: //LOCK Finish
			status_Lock = true;
			//ButtonLockFlag = false;
			if (status_Power)
			{
				digitalWrite(LED_Lock_Pin, LOW);
				digitalWrite(LED_UnLock_Pin, HIGH);
				Button_Lock_Presssed = false;
				Beep(1000);
			}
			break;
		case 0xC7: //UnLOCK Finish
			status_Lock = false;
			//ButtonLockFlag = false;
			if (status_Power)
			{
				digitalWrite(LED_Lock_Pin, HIGH);
				digitalWrite(LED_UnLock_Pin, LOW);
				Button_UnLock_Presssed = false;
				Beep(1000);
			}
			break;

		case 0xC5: //Check Survive
			RS485ReSendCnt = 0;

			ButtonLockFlag = false;

			if (status_Power)
				LEDStatusCtrl();
			else if (!status_Power)
				LEDAllOFF();
			break;
		
		case 0xAE://手術床回傳狀態值
			if (status_Power)
			{
				LEDStatusCtrl();
				if (rs485Command[2] == 0x20)//AC ON
				{
					digitalWrite(LED_AC_Pin, LOW);
					delayMicroseconds(1);
				}
				else if (rs485Command[2] == 0x21)//AC OFF
				{
					digitalWrite(LED_AC_Pin, HIGH);
					delayMicroseconds(1);
				}

				switch (rs485Command[3])
				{
				case 0x23://DC OFF
					digitalWrite(LED_BATTERY_Pin, HIGH);

					digitalWrite(LED_BATTERYLOW_Pin, HIGH);
					delayMicroseconds(1);
					break;
				case 0x24://DC LOW


					digitalWrite(LED_BATTERYLOW_Pin, LOW);
					digitalWrite(LED_BATTERY_Pin, HIGH);
					delayMicroseconds(1);
					break;
				case 0x22://DC MIDDLE

					digitalWrite(LED_BATTERY_Pin, LOW);// low action

					digitalWrite(LED_BATTERYLOW_Pin, HIGH);
					delayMicroseconds(1);
					break;
				case 0x26://DC HIGH
					digitalWrite(LED_BATTERYLOW_Pin, HIGH);

					digitalWrite(LED_BATTERY_Pin, LOW);
					delayMicroseconds(1);
					break;
				}
				if (rs485Command[4] == 0xC6)//LOCK
				{
					digitalWrite(LED_Lock_Pin, LOW);
					digitalWrite(LED_UnLock_Pin, HIGH);
					status_Lock = true;
					delayMicroseconds(1);
				}
				else if (rs485Command[4] == 0xC7)//UnLOCK
				{
					digitalWrite(LED_Lock_Pin, HIGH);
					digitalWrite(LED_UnLock_Pin, LOW);
					status_Lock = false;
					delayMicroseconds(1);
				}
				if (rs485Command[8] != 0)
				{
					InterLockFlag = true;
					digitalWrite(LED_InterLock_Pin, LOW);//inter lock LED ON
				}
				else if (rs485Command[8] == 0)
				{
					InterLockFlag = false;
					digitalWrite(LED_InterLock_Pin, HIGH);//inter lock LED OFF
				}
				//else if (rs485Command[10] == 0xE2)
				//	digitalWrite(LED_InterLock_Pin, HIGH);//inter lock LED OFF
				//else if (rs485Command[10] == 0xE1)
				//	digitalWrite(LED_InterLock_Pin, LOW);//inter lock LED ON


				if (rs485Command[8] != 0)
				{
					delayMicroseconds(1);
				}

				//RS485SlideInterLockSet();
				//RS485BackInterLockSet();
				//RS485LegInterLockSet();
				RS485PumpInterLockSet();
			}
			break;
		case 0xB1:
			Beep(200);
			break;
		}
		RS485ReadComplete = false;
		rs485InIndex = 0;
		delay(10);
	}
}

void CheckPowerButton()
{
	if (!digitalRead(Button_POWERONPin))
	{

		if ((!PowerButtonPressed) && (PowerButtonCnt >= 5000))
		{
			status_Power = !status_Power;
			PowerButtonPressed = true;
			OffHandsentCnt = 0;
			//Beep(100);
			if ((status_Power)) //&& (RS485ReSendCnt < ReSendCommandTime))//重送次數必須小於規定值，否則視為通訊異常，按鍵即無法運作保護病患
			{
				PowerButtonCnt = 0;
				OffHandsetFlag = true;

				Send485Command(CMD_STATUS_CHECK);
				Beep(100);
				//Send485Command(CMD_NORMAL_MODE);


				Serial.println("Power on");
			}
			else if ((!status_Power))
			{
				OffHandsetFlag = false;
				Beep(100);

				PowerButtonCnt = 0;
				Serial.println("Power off");
				LEDAllOFF();
			}
		}
		else
			PowerButtonCnt++;
	}
	else
	{
		PowerButtonPressed = false;
		PowerButtonCnt = 0;
	}


}

void ButtonDetectPowerON()
{
	if ((!status_Power) && (!digitalRead(Button_POWERONPin)))//power key
	{
		if ((POWER_ON_ButtonReleaseCnt >= ReleaseThreshold) && (POWER_ON_ButtonBounceCnt >= BounceThreshold))
		{
			POWER_ON_ButtonBounceCnt = 0;
			POWER_ON_ButtonReleaseCnt = 0;
			status_Power = !status_Power;
			OffHandsetFlag = true;
			digitalWrite(LED_POWER_Pin, LOW);
			Send485Command(CMD_STATUS_CHECK);
			//Beep(100);
			//Send485Command(CMD_NORMAL_MODE);


			Serial.println("Power on");
			Beep(100);
		}
		else
			POWER_ON_ButtonBounceCnt++;
	}
	else
	{
		POWER_ON_ButtonBounceCnt = 0;
		POWER_ON_ButtonReleaseCnt++;
	}
}

void ButtonDetectPowerOFF()
{
	if ((status_Power) && (!(PINE & buttonPinPOWEROFF_RE7)))//power key
	{

		if ((POWER_OFF_ButtonReleaseCnt >= ReleaseThreshold) && (POWER_OFF_ButtonBounceCnt >= BounceThreshold))
		{
			POWER_OFF_ButtonBounceCnt = 0;
			POWER_OFF_ButtonReleaseCnt = 0;
			status_Power = !status_Power;
			OffHandsetFlag = false;

			Serial.println("Power off");
			LEDAllOFF();
			Beep(100);
			Send485Command(CMD_HANDSET_OFF);


		}
		else
			POWER_OFF_ButtonBounceCnt++;
	}
	else if ((!status_Power) && (!(PINE & buttonPinPOWEROFF_RE7)))//power key
	{
		if ((POWER_OFF_ButtonReleaseCnt >= ReleaseThreshold) && (POWER_OFF_ButtonBounceCnt >= BounceThreshold))
		{
			countPowerOFF += 1;
			Serial.print("countPowerOFF: ");
			Serial.println(countPowerOFF);
		}
	}
	
	else
	{
		POWER_OFF_ButtonBounceCnt = 0;
		POWER_OFF_ButtonReleaseCnt++;
	}
}

void CheckConnectMode()
{
	if (ConnectionSurviveCnt >= ReSendSurviveCommandTime)
	{
		ConnectionSurviveCnt = 0;

		Send485Command(CMD_SURVIVE_CHECK);
		if (InterLockFlag)
		{
			delay(5);
			Send485Command(CMD_STATUS_CHECK);
			Serial.println("1234");
		}

		if (RS485ReSendCnt >= ReSendCommandTime)
		{
			LEDTwinklingCtrl();
		}
		else
			RS485ReSendCnt++;


		//Serial.print("RS485ReSendCnt :");
		//Serial.println(RS485ReSendCnt);
	}
	else
		ConnectionSurviveCnt++;
}

void LEDStatusCtrl()
{
	
}

void LEDTwinklingCtrl()
{
	//if (ServiceLEDTwinklingCnt >= LEDTwinklingTime)
	{
		//ServiceLEDTwinklingCnt = 0;

	}
	//else
	//ServiceLEDTwinklingCnt++;
}

// - - - - - - - - - - - - - - -
// Method - delete blank char
// - - - - - - - - - - - - - - -
char* deblank(char* input)
{
	int i, j;
	char *output = input;
	for (i = 0, j = 0; i<strlen(input); i++, j++)
	{
		if (input[i] != ' ')
			output[j] = input[i];
		else
			j--;
	}
	output[j] = 0;
	return output;
}

// - - - - - - - - - - - - - - -
// Method - LEDPin initial
// - - - - - - - - - - - - - - -
void initLEDPin()
{
	//DDRE = 0b10000000;//設定腳位輸入或輸出
	//PORTE = 0b10000000;//設定腳位高或低電位

	//DDRH = 0b00000100;//設定腳位輸入或輸出
	//PORTH = 0b00000000;//設定腳位高或低電位
	//PORTH = 0b01111100;//設定腳位高或低電位

	pinMode(LED_POWER_Pin, OUTPUT);
	digitalWrite(LED_POWER_Pin, HIGH);// low action

	pinMode(LED_BATTERYLOW_Pin, OUTPUT);
	digitalWrite(LED_BATTERYLOW_Pin, HIGH);// low action
	delayMicroseconds(1);
	pinMode(LED_BATTERY_Pin, OUTPUT);
	digitalWrite(LED_BATTERY_Pin, HIGH);// low action
	delayMicroseconds(1);
	pinMode(LED_AC_Pin, OUTPUT);
	digitalWrite(LED_AC_Pin, HIGH);// low action

	pinMode(LED_UnLock_Pin, OUTPUT);
	digitalWrite(LED_UnLock_Pin, HIGH);// low action

	pinMode(LED_Lock_Pin, OUTPUT);
	digitalWrite(LED_Lock_Pin, HIGH);// low action
}

void LEDAllOFF()
{
	digitalWrite(LED_POWER_Pin, HIGH);

	digitalWrite(LED_BATTERYLOW_Pin, HIGH);

	digitalWrite(LED_BATTERY_Pin, HIGH);
	digitalWrite(LED_AC_Pin, HIGH);
	digitalWrite(LED_ReverseMode_Pin, HIGH);
	digitalWrite(LED_UnLock_Pin, HIGH);
	digitalWrite(LED_NormalMode_Pin, HIGH);

	digitalWrite(LED_InterLock_Pin, HIGH);
	digitalWrite(LED_Lock_Pin, HIGH);
}

void PowerSaveCtrl()
{
	if (OffHandsetFlag)
	{
		if (OffHandsentCnt>OffHandsentTime)
		{
			if ((!Button_Click_Enable) && (!Button_Hold_Enable))
			{
				Beep(100);
				delayMicroseconds(1);
				OffHandsentCnt = 0;
				OffHandsetFlag = false;

				Send485Command(CMD_HANDSET_OFF);


				PowerButtonCnt = 0;
				status_Power = false;
				Serial.println("Power off");
				LEDAllOFF();
				Send485Command(CMD_HANDSET_OFF);
				//Beep(100);
			}
			else
				OffHandsentCnt = 0;
		}
		else
			OffHandsentCnt++;
	}
	else
	{
		OffHandsentCnt = 0;
	}
}

bool InterLockAnalysis(byte tempInterLockStatus, unsigned char tempShiftIndex)
{
	if ((tempInterLockStatus >> tempShiftIndex) & 0x01)
		return true;
	else
		return false;
}


void RS485PumpInterLockSet()
{
	if (rs485Command[8] == 0)
	{
		TREND_DisableFlag = false;
		TRENDRev_DisableFlag = false;
		Back_Up_DisableFlag = false;
		Back_Down_DisableFlag = false;
		Kidney_Up_DisableFlag = false;
		Kidney_Down_DisableFlag = false;
	}
	else if (rs485Command[8] != 0)
	{
		//if (InterLockAnalysis(rs485Command[8], 2) || InterLockAnalysis(rs485Command[8], 4))
		{  // 因背板保護感應器 背部與縱轉往頭無法向下
			//	TREND_DisableFlag = true;
			//	TRENDRev_DisableFlag = false;
			//	Back_Down_DisableFlag = true;
			//	Back_Up_DisableFlag = false;
		}

		if (InterLockAnalysis(rs485Command[8], 1))
		{
			Kidney_Up_DisableFlag = true;
		}


		if (InterLockAnalysis(rs485Command[8], 2))
		{
			TREND_DisableFlag = true;
			//TRENDRev_DisableFlag = false;
		}

		if (InterLockAnalysis(rs485Command[8], 3))
		{
			//TREND_DisableFlag = false;
			TRENDRev_DisableFlag = true;
		}


		if (InterLockAnalysis(rs485Command[8], 4))
		{
			Back_Down_DisableFlag = true;
			//Back_Up_DisableFlag = false;
		}


		if (InterLockAnalysis(rs485Command[8], 5))
		{
			//Back_Down_DisableFlag = false;
			Back_Up_DisableFlag = true;
		}

	}
}

void ScandkeyEvent()
{
	unsigned char KeyStatetemp = 0;
	unsigned long ScanTimeTemp = 0;
	/*
	if (CNTTime2>20000)
	{
		CNTTime++;
		CNTTime2 = 0;
	}
	else
	{
		CNTTime2++;
	}
	*/
	
	
	if (kpd.getKeys())
	{
		delayMicroseconds(1);
		for (int i = 0; i < LIST_MAX; i++)   // Scan the whole key list.
		{
			delayMicroseconds(1);
			//Serial.println(kpd.key[i].kchar);
			if (kpd.key[i].stateChanged)   // Only find keys that have changed state.
			{
				
				switch (kpd.key[i].kstate)
				{  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED

				case PRESSED:
					Button_Click_Enable = true;
					switch (kpd.key[i].kchar)
					{
					case Button_LOCK:
						if ((status_Power) && (!status_Lock))
						{
							if ((!Button_TrendRev_Pressed) && (!Button_Trend_Pressed)
								&& (!Button_Tile_Right_Pressed) && (!Button_Tile_Left_Pressed)
								&& (!Button_Height_Up_Pressed) && (!Button_Height_Down_Pressed)
								&& (!Button_Slide_Head_Pressed) && (!Button_Slide_Leg_Pressed)
								&& (!Button_Back_Up_Pressed) && (!Button_Back_Down_Pressed)
								&& (!Button_Level_Pressed))
							{
								Send485Command(CMD_LOCK);
								ButtonLockFlag = true;
								
								Button_Click_Enable = false;
								Button_Lock_Presssed = true;
								Beep(100);
							}
							Serial.println("LOCK");
							OffHandsentCnt = 0;
						}
						break;
					case Button_UNLOCK:
						if ((status_Power) && (status_Lock))
						{
							if ((!Button_TrendRev_Pressed) && (!Button_Trend_Pressed)
								&& (!Button_Tile_Right_Pressed) && (!Button_Tile_Left_Pressed)
								&& (!Button_Height_Up_Pressed) && (!Button_Height_Down_Pressed)
								&& (!Button_Slide_Head_Pressed) && (!Button_Slide_Leg_Pressed)
								&& (!Button_Back_Up_Pressed) && (!Button_Back_Down_Pressed)
								&& (!Button_Level_Pressed))
							{
								Send485Command(CMD_UNLOCK);

								ButtonLockFlag = true;
								status_Lock = false;

								Button_Click_Enable = false;
								Button_UnLock_Presssed = true;
								Beep(100);
							}
							Serial.println("UNLOCK"); 
							OffHandsentCnt = 0;
						}
						break;
						/*-- ACTION --*/
					case Button_TREND:
						if ((status_Power) && (status_Lock))
						{
							if ((!Button_Height_Up_Pressed) && (!Button_Height_Down_Pressed)
								&& (!Button_Slide_Head_Pressed) && (!Button_Slide_Leg_Pressed)
								&& (!Button_Back_Up_Pressed) && (!Button_Back_Down_Pressed)
								&& (!Button_UnLock_Presssed) && (!Button_Lock_Presssed)
								&& (!Button_Level_Pressed))
							{
								if (!TREND_DisableFlag)
								{
									Send485Command(CMD_TREND);
									ChangeAllFlagAccept();
									Button_Trend_Pressed = true;
									Beep(100);
								}
							}
							Serial.println("TREND");
							OffHandsentCnt = 0;
						}
					break;
					case Button_TRENDREV:
						if ((status_Power) && (status_Lock))
						{

							if ((!Button_Height_Up_Pressed) && (!Button_Height_Down_Pressed)
								&& (!Button_Slide_Head_Pressed) && (!Button_Slide_Leg_Pressed)
								&& (!Button_Back_Up_Pressed) && (!Button_Back_Down_Pressed)
								&& (!Button_UnLock_Presssed) && (!Button_Lock_Presssed)
								&& (!Button_Level_Pressed))
							{
								Send485Command(CMD_TREND_REV);
								ChangeAllFlagAccept();
								Button_TrendRev_Pressed = true;
								Beep(100);
							}

							Serial.println("TRENDREV");
							OffHandsentCnt = 0;
						}
					break;
					case Button_HEIGHT_UP:
						if ((status_Power) && (status_Lock))
						{
							if ((!Button_TrendRev_Pressed) && (!Button_Trend_Pressed)
								&& (!Button_Tile_Right_Pressed) && (!Button_Tile_Left_Pressed)
								&& (!Button_Slide_Head_Pressed) && (!Button_Slide_Leg_Pressed)
								&& (!Button_Back_Up_Pressed) && (!Button_Back_Down_Pressed)
								&& (!Button_UnLock_Presssed) && (!Button_Lock_Presssed)
								&& (!Button_Level_Pressed))
							{
								Send485Command(CMD_HEIGHT_UP);
								Button_Height_Up_Pressed = true;
								
								ChangeAllFlagAccept();
								Beep(100);
							}
							Serial.println("HEIGHT_UP");
							OffHandsentCnt = 0;
							//Beep(100);
						}
						//Serial.println(key);
						break;
					case Button_HEIGHT_DOWN:
						if ((status_Power) && (status_Lock))
						{
							if ((!Button_TrendRev_Pressed) && (!Button_Trend_Pressed)
								&& (!Button_Tile_Right_Pressed) && (!Button_Tile_Left_Pressed)
								&& (!Button_Slide_Head_Pressed) && (!Button_Slide_Leg_Pressed)
								&& (!Button_Back_Up_Pressed) && (!Button_Back_Down_Pressed)
								&& (!Button_UnLock_Presssed) && (!Button_Lock_Presssed)
								&& (!Button_Level_Pressed))
							{
								Send485Command(CMD_HEIGHT_DOWN);
								Button_Height_Down_Pressed = true;
								
								ChangeAllFlagAccept();
								Beep(100);
							}
							Serial.println("HEIGHT_DOWN");
							OffHandsentCnt = 0;
							
						}
						//Serial.println(key);
						break;
					case Button_TILT_LEFT:
						if ((status_Power) && (status_Lock))
						{

							if ((!Button_Height_Up_Pressed) && (!Button_Height_Down_Pressed)
								&& (!Button_Slide_Head_Pressed) && (!Button_Slide_Leg_Pressed)
								&& (!Button_Back_Up_Pressed) && (!Button_Back_Down_Pressed)
								&& (!Button_UnLock_Presssed) && (!Button_Lock_Presssed)
								&& (!Button_Level_Pressed))
							{
								Send485Command(CMD_TILE_LEFT);
								ChangeAllFlagAccept();
								Button_Tile_Left_Pressed = true;
								Beep(100);
							}

							Serial.println("TILT_LEFT");
							OffHandsentCnt = 0;
							//Beep(100);
						}
						break;
					case Button_TILT_RIGHT:
						if ((status_Power) && (status_Lock))
						{
							if ((!Button_Height_Up_Pressed) && (!Button_Height_Down_Pressed)
								&& (!Button_Slide_Head_Pressed) && (!Button_Slide_Leg_Pressed)
								&& (!Button_Back_Up_Pressed) && (!Button_Back_Down_Pressed)
								&& (!Button_UnLock_Presssed) && (!Button_Lock_Presssed)
								&& (!Button_Level_Pressed))
							{
								if (!Tile_Right_DisableFlag)
								{
									Send485Command(CMD_TILE_RIGHT);
									ChangeAllFlagAccept();
									Button_Tile_Right_Pressed = true;
									Beep(100);
								}
							}
							Serial.println("TILT_RIGHT");
							OffHandsentCnt = 0;
							//Beep(100);
						}
						break;
					case Button_BACK_UP:
						if ((status_Power) && (status_Lock))
						{
							if (!Back_Up_DisableFlag)
							{
								if ((!Button_TrendRev_Pressed) && (!Button_Trend_Pressed)
									&& (!Button_Tile_Right_Pressed) && (!Button_Tile_Left_Pressed)
									&& (!Button_Height_Up_Pressed) && (!Button_Height_Down_Pressed)
									&& (!Button_Slide_Head_Pressed) && (!Button_Slide_Leg_Pressed)
									&& (!Button_UnLock_Presssed) && (!Button_Lock_Presssed)
									&& (!Button_Level_Pressed) && (!Button_AutoMove_Level))
								{
									Send485Command(CMD_BACK_UP);
									Button_Back_Up_Pressed = true;
									LevelEnable_Flag = true;
									Serial.println("BACK_UP");
									//	Star_ScanTime = micros();
									//	CNTTime = 0;
									//	CNTTime2 = 0;
									ChangeAllFlagAccept();
									Beep(100);
								}
								else if ((!Button_TrendRev_Pressed) && (!Button_Trend_Pressed)
									&& (!Button_Tile_Right_Pressed) && (!Button_Tile_Left_Pressed)
									&& (!Button_Height_Up_Pressed) && (!Button_Height_Down_Pressed)
									&& (!Button_Slide_Head_Pressed) && (!Button_Slide_Leg_Pressed)
									&& (!Button_UnLock_Presssed) && (!Button_Lock_Presssed)
									&& (!Button_Level_Pressed) && (Button_AutoMove_Level))
								{
									Send485Command(CMD_AutoMove);
									Serial.println("Auto_Move");
									Button_AutoMove_Level = false;
									ChangeAllFlagAccept();
									Beep(100);
								}
							}

							//Serial.println("BACK_UP");
							OffHandsentCnt = 0;
							//Beep(100);
						}
						break;
					case Button_BACK_DOWN:
						if ((status_Power) && (status_Lock))
						{
							if (!Back_Down_DisableFlag)
							{
								if ((!Button_TrendRev_Pressed) && (!Button_Trend_Pressed)
									&& (!Button_Tile_Right_Pressed) && (!Button_Tile_Left_Pressed)
									&& (!Button_Height_Up_Pressed) && (!Button_Height_Down_Pressed)
									&& (!Button_Slide_Head_Pressed) && (!Button_Slide_Leg_Pressed)
									&& (!Button_UnLock_Presssed) && (!Button_Lock_Presssed)
									&& (!Button_Level_Pressed))
								{
									Send485Command(CMD_BACK_DOWN);

									Button_Back_Down_Pressed = true;
									Serial.println("BACK_DOWN");
									ChangeAllFlagAccept();
									Beep(100);
								}
							}

							//Serial.println("BACK_DOWN");
							OffHandsentCnt = 0;
							//Beep(100);
						}
						break;
					case Button_SLIDE_HEAD:
						if ((status_Power) && (status_Lock))
						{
							if (!Kidney_Up_DisableFlag)
							{
								if ((!Button_TrendRev_Pressed) && (!Button_Trend_Pressed)
									&& (!Button_Tile_Right_Pressed) && (!Button_Tile_Left_Pressed)
									&& (!Button_Height_Up_Pressed) && (!Button_Height_Down_Pressed)
									&& (!Button_Back_Up_Pressed) && (!Button_Back_Down_Pressed)
									&& (!Button_UnLock_Presssed) && (!Button_Lock_Presssed)
									&& (!Button_Level_Pressed))
								{
									Send485Command(CMD_KIDNEY_UP);
								}
								Button_Slide_Head_Pressed = true;
								ChangeAllFlagAccept();
								Beep(100);
							}
							Serial.println("SLIDING_FWD");
							OffHandsentCnt = 0;
							//Beep(100);
						}
						break;
					case Button_SLIDE_LEG:
						if ((status_Power) && (status_Lock))
						{
							if (!Kidney_Down_DisableFlag)
							{
								if ((!Button_TrendRev_Pressed) && (!Button_Trend_Pressed)
									&& (!Button_Tile_Right_Pressed) && (!Button_Tile_Left_Pressed)
									&& (!Button_Height_Up_Pressed) && (!Button_Height_Down_Pressed)
									&& (!Button_Back_Up_Pressed) && (!Button_Back_Down_Pressed)
									&& (!Button_UnLock_Presssed) && (!Button_Lock_Presssed)
									&& (!Button_Level_Pressed))
								{
									Send485Command(CMD_KIDNEY_DOWN);
								}
								Button_Slide_Leg_Pressed = true;
								ChangeAllFlagAccept();
								Beep(100);
							}
							Serial.println("SLIDING_BWD");
							OffHandsentCnt = 0;
							//Beep(100);
						}
						break;
						
					}
					break;
				case HOLD:
					Button_Hold_Enable = true;
					switch (kpd.key[i].kchar)
					{
					case Button_LEVEL:
						if ((status_Power) && (status_Lock))
						{
							//Serial.println("LEVEL");
							OffHandsentCnt = 0;
							Beep(100);
							if (flagAceeptLEVEL == 1)
							{
								if ((!Button_TrendRev_Pressed) && (!Button_Trend_Pressed)
									&& (!Button_Tile_Right_Pressed) && (!Button_Tile_Left_Pressed)
									&&(!Button_Height_Up_Pressed) && (!Button_Height_Down_Pressed)
									&& (!Button_Slide_Head_Pressed) && (!Button_Slide_Leg_Pressed)
									&& (!Button_UnLock_Presssed) && (!Button_Lock_Presssed)
									&& (!Button_Back_Up_Pressed) && (!Button_Back_Down_Pressed))
								{
									Send485Command(CMD_LEVEL);
									Serial.println("LEVEL");
									InterLockFlag = false;
									Button_Level_Pressed = true;
									if (!InterLockFlag)
									{
										digitalWrite(LED_InterLock_Pin, HIGH);//inter lock LED OFF
										TREND_DisableFlag = false;
										TRENDRev_DisableFlag = false;
										Tile_Right_DisableFlag = false;
										Tile_Left_DisableFlag = false;
										Back_Up_DisableFlag = false;
										Back_Down_DisableFlag = false;
										Leg_Up_DisableFlag = false;
										Leg_Down_DisableFlag = false;


										Kidney_Up_DisableFlag = false;
										Kidney_Down_DisableFlag = false;
									}
									//flagAceeptLEVEL = 1;
									flagAceeptFLEX_UP = 1;
									flagAceeptFLEX_DOWN = 1;
									flagAceeptREVFLEX_UP = 1;
									flagAceeptREVFLEX_DOWN = 1;
									flagAceeptRECALL = 1;
									flagAceeptRECALLHEIGHT = 1;
								}
								
							}
						}
						else if (((!status_Power) && (status_Lock)))
						{
							Button_AutoMove_Level = true;
						}
						break;
										

					case Button_HEIGHT_UP:
					case Button_HEIGHT_DOWN:
					case Button_TREND:
					case Button_TRENDREV:
					case Button_TILT_LEFT:
					case Button_TILT_RIGHT:
					case Button_BACK_UP:
					case Button_BACK_DOWN:
					case Button_KIDNEY_UP:
					case Button_KIDNEY_DOWN:
						//if ((status_Power) && (status_Lock))
					{
						OffHandsentCnt = 0;
					}
					break;
					}
					break;
				case RELEASED:
					Button_Hold_Enable = false;
					Button_Click_Enable = false;
					switch (kpd.key[i].kchar)
					{
					case Button_TREND:
						if ((status_Power) && (!Button_Level_Pressed) && (status_Lock))
						{
							OffHandsentCnt = 0;
							Button_Trend_Pressed = false;
							if (!Button_Level_Pressed)
							{
								Send485Command(CMD_TREND_STOP, 2);
							}
						}
						break;
					case Button_TRENDREV:
						if ((status_Power) && (!Button_Level_Pressed) && (status_Lock))
						{
							OffHandsentCnt = 0;
							Button_TrendRev_Pressed = false;
							if (!Button_Level_Pressed)
							{
								Send485Command(CMD_TREND_STOP, 2);
							}
						}
						break;
					case Button_HEIGHT_UP:
					case Button_HEIGHT_DOWN:
						if ((status_Power) && (!Button_Level_Pressed) && (status_Lock))
						{
							Button_Height_Up_Pressed = false;
							Button_Height_Down_Pressed = false;
							OffHandsentCnt = 0;
							Send485Command(CMD_HEIGHT_STOP, 2);
						}
						break;
					case Button_TILT_LEFT:
						if ((status_Power) && (!Button_Level_Pressed) && (status_Lock))
						{
							OffHandsentCnt = 0;
							Button_Tile_Left_Pressed = false;
						//	if ((!Button_TrendRev_Pressed) && (!Button_Trend_Pressed))
							{
								Send485Command(CMD_TILE_STOP, 2);
							}
						}
						break;
					case Button_TILT_RIGHT:
						if ((status_Power) && (!Button_Level_Pressed) && (status_Lock))
						{
							OffHandsentCnt = 0;
							Button_Tile_Right_Pressed = false;
						//	if ((!Button_TrendRev_Pressed) && (!Button_Trend_Pressed))
							{
								Send485Command(CMD_TILE_STOP, 2);
							}
						}
						break;
					case Button_BACK_UP:
					case Button_BACK_DOWN:
						if ((status_Power) && (!Button_Level_Pressed) && (status_Lock))
						{
							OffHandsentCnt = 0;
							
					
								Send485Command(CMD_BACK_STOP, 2);
								Button_Back_Up_Pressed = false;
								Button_Back_Down_Pressed = false;
								LevelEnable_Flag = true;
								Star_ScanTime = micros();
							//End_ScanTime = micros();
							//ScanTimeTemp = End_ScanTime - Star_ScanTime;
							//Serial.print("ScanTimeTemp :");
							//Serial.println(ScanTimeTemp);
						}
						break;
					case Button_LEGLEFT_UP:
					case Button_LEGLEFT_DOWN:
						if ((status_Power))
						{
							OffHandsentCnt = 0;
							Send485Command(CMD_LEGLEFT_STOP, 2);
						}
						break;
					case Button_LEGRIGHT_UP:
					case Button_LEGRIGHT_DOWN:
						if ((status_Power))
						{
							OffHandsentCnt = 0;
							Send485Command(CMD_LEGRIGHT_STOP, 2);
						}
						break;
					
					case Button_FLEX:
					case Button_REFLEX:
						if ((status_Power))
						{
							OffHandsentCnt = 0;
							Send485Command(CMD_FLEX_STOP, 2);
						}
						break;
					case Button_SLIDE_HEAD:
					case Button_SLIDE_LEG:
						if ((status_Power) && (!Button_Level_Pressed) && (status_Lock))
						{
							OffHandsentCnt = 0;
							Send485Command(CMD_KIDNEY_STOP, 2);
							Button_Slide_Head_Pressed = false;
							Button_Slide_Leg_Pressed = false;
						}
						break;
					case Button_LEVEL:
						if ((status_Power) && (status_Lock))
						{
							OffHandsentCnt = 0;
							Send485Command(CMD_LEVEL_STOP, 2);
							Button_Level_Pressed = false;
							//Button_AutoMove_Level = false;
						}
						break;
					case Button_RECALL:
					case Button_RECALL_HEIGHT:
						if ((status_Power))
						{
							OffHandsentCnt = 0;
							Send485Command(CMD_RECALL_STOP, 2);
						}
						break;
					}
					break;
				case IDLE:
					break;
				}
			}
		}
	}
}


// - - - - -
// - Setup
// - - - - -
void setup(void) {
	Serial.begin(9600);

	Serial3.begin(19200); //用9600傳輸到電腦，會有雜訊

	//Serial2.begin(38400); //藍芽

	//pinMode(13, OUTPUT);
	//digitalWrite(13, HIGH);

	//initial LED
	initLEDPin();
	//RS485 CS pin
	pinMode(RS485TxControl, OUTPUT);
	digitalWrite(RS485TxControl, RS485Receive);

	pinMode(BeepPin, OUTPUT);
	digitalWrite(BeepPin, HIGH);

	pinMode(Button_POWERONPin, INPUT);//實體電源按鍵偵測

	DDRE &= ~buttonPinPOWEROFF_RE7;// RE7 INPUT
	Send485Command(CMD_NORMAL_MODE);

	//initial keypad
	kpd.setHoldTime(intKeypadHoldTime);
	kpd.setDebounceTime(intKeypadDebounceTime);
	//kpd.addEventListener(keypadEvent);

	MODE_Repeat = SettingCommandRepeat;
}

void Beep(unsigned int delayms){
	//analogWrite(8, 128);      // Almost any value can be used except 0 and 255
	digitalWrite(BeepPin, LOW);
	// experiment to get the best tone
	delay(delayms);          // wait for a delayms ms
	//analogWrite(8, 0);       // 0 turns it off
	digitalWrite(BeepPin, HIGH);
	delay(delayms);          // wait for a delayms ms  
}

// - - - - -
// - Loop
// - - - - -
void loop(void)
{
	//key event

	LEDStatusCtrl();
	CheckConnectMode();
	//if (RS485ReSendCnt < ReSendCommandTime)//重送次數必須小於規定值，否則視為通訊異常，按鍵即無法運作保護病患
	{
		//char key = kpd.getKey();
		//Serial.println("key :");
		//Serial.println(key);
		//delay(100);
	}
	ScandkeyEvent();
	ButtonDetectPowerON();
	ButtonDetectPowerOFF();
	//PowerSaveCtrl();
	CheckRS485CmdRequest();
	//CheckBTCmdRequest();
}


