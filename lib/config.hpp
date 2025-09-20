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
//extern std::string SerialWatch = "ang";

//BLDC
#define ESCpin 0

//SPI
#define SPI_CSpin 1
#define SPI_SCKpin 2
#define SPI_RXpin 4
#define SPI_TXpin 3
#define SERIAL_BAUD 125000
//ボール検知
#define BallDetectpin 6