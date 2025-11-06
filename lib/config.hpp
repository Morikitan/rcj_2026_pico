#pragma once

#include <string>
/*******************
ang 機体の角度(AngleX)
bal ボールセンサーの値(BallDistance,BallAngle)
lin ラインセンサーの値(0か1で受け取る)
mot モーターに加わる電圧(AngleXもセットで)
tim 1回の経過時間(ミリ秒)
ble BLEの接続状況
*******************/
extern std::string SerialWatch;

//BLDC
#define ESCpin 0

//UART for RP2350
#define RP2350_UART_IRQpin 1
#define RP2350_UART_RXpin 4
#define RP2350_UART_TXpin 3
#define SERIAL_BAUD 125000

//ボール検知
#define BallDetectpin 6

//エンコーダー
#define encoderUART uart0
#define Encoder_TXpin 16
#define Encoder_RXpin 17

extern bool isMotorClockWise[4];
extern float motorFrequency[4];
extern uint8_t encoderData[8];

//スイッチ類
#define Offence_Switchpin 7 //攻撃か防御かを決めるスイッチ
#define Start_Switchpin 8 //初期状態の向きを決めるスイッチ
#define Determination_Switchpin 9 //機体の電源をonにするタクトスイッチ
#define Switchpin1 10
#define Switchpin2 11
#define Switchpin3 12

//その他
extern int mode;