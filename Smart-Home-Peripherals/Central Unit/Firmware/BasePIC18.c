#define WC              0x00 //Write Configuration Register Command
#define RC              0x10 //Read  Configuration Register Command
#define WTP             0x20 //Write TX Payload Command
#define RTXP            0x21 //Read  TX Payload Command
#define WTA             0x22 //Write TX Address Command
#define RTA             0x23 //Read  TX Address Command
#define RRP             0x24 //Read  RX Payload Command

#define NOP             0x00 //DUMMY BYTE
#define SYNC            0xAA //SYNCHRONIZATION WITH RASPBERRY BYTE (DATA)
#define SYNC2           0xEE //SYNCHRONIZATION WITH RASPBERRY BYTE (SETTINGS)
#define NEWLINE         0x0A //NEW LINE BYTE

#define RADAR_ID        0x7E //RADAR ID
#define RADAR_REP       0xB5 //RADAR REPORT
#define RADAR_MOV       0x9A //RADAR MOVEMENT

#define SWITCH_ID       0xBF //SWITCH ID

#define THERM_ID        0xAE //THERMOSTAT ID

// LCD module connections
sbit LCD_RS at RE0_bit;
sbit LCD_EN at RA5_bit;
sbit LCD_D4 at RA3_bit;
sbit LCD_D5 at RA2_bit;
sbit LCD_D6 at RA1_bit;
sbit LCD_D7 at RA0_bit;

sbit LCD_RS_Direction at TRISE0_bit;
sbit LCD_EN_Direction at TRISA5_bit;
sbit LCD_D4_Direction at TRISA3_bit;
sbit LCD_D5_Direction at TRISA2_bit;
sbit LCD_D6_Direction at TRISA1_bit;
sbit LCD_D7_Direction at TRISA0_bit;
// End LCD module connections

sbit LED1 at RB1_bit;
sbit LED2 at RB2_bit;
sbit LED3 at RB3_bit;
sbit LED4 at RC2_bit;
sbit RELAY at RB0_bit;
sbit PBUTTON at RD5_bit;
sbit BACKLIGHT at RA4_bit;

sbit TX_EN at  RC1_bit;  //OUT
sbit TRX_CE at RC5_bit;  //OUT
sbit PRW_UP at RC0_bit;  //OUT
sbit CSN at RD2_bit;     //OUT
sbit DR at RB4_bit;      //IN
sbit AM at RD3_bit;      //IN
sbit CD at RB5_bit;      //IN

unsigned short data0, data1, data2, data3, data4, data5; //INCOMING DATA
int AlarmCounter=0, LightCounter=0, LumensCounter=0;
unsigned short SetLuminosity, SwitchOnOff,  MotionSensorLighting, LuminositySensorLighting;
unsigned short ReportCounter=0, i=0, AutoRetranCounter, j=0;
unsigned short RadarAlarmFlag=0, SyncFlag=0, InformThermostatFlag=0, DRFlag=0, SecurityBreachFlag=0, InformSwitchFlag=0, RadarMovementFlag=0, LightFlag=0, LumensFlag=0; //FLAGS
char receive[7], Synchronization;
unsigned short RadarMatrix[6], SwitchMatrix[6], ThermostatMatrix[6]; //DEVICES DATA MATRIX   MICROCONTROLLER-->>RASPBERRY PI
unsigned short InformSwitchData[6], InformThermostatData[6], InformAlarmData=0; //SETTINGS MATRIX   MICROCONTROLLER<<--RASPBERRY PI

void PortInitialization(){
 TRISB1_bit=0; //LED1 PIN AS OUTPUT
 TRISB2_bit=0; //LED2 PIN AS OUTPUT
 TRISB3_bit=0; //LED3 PIN AS OUTPUT
 TRISC2_bit=0; //LED4 PIN AS OUTPUT
 TRISB0_bit=0; //RELAY PIN AS OUTPUT
 TRISD5_bit=1; //BUTTON PIN AS INPUT
 TRISA4_bit=0; //LCD BACKLIGHT PIN AS OUTPUT

 TRISC1_bit=0; //TX_EN PIN AS OUTPUT
 TRISC5_bit=0; //TRX_CE PIN AS OUTPUT
 TRISC0_bit=0; //PRW_UP PIN AS OUTPUT
 TRISD2_bit=0; //CSN PIN AS OUTPUT
 TRISB4_bit=1; //DR PIN AS INPUT
 TRISD3_bit=1; //AM PIN AS INPUT
 TRISB5_bit=1; //CD PIN AS INPUT

 ANSELA=0; //ALL PINS AS DIGITAL
 ANSELB=0;
 ANSELC=0;
 ANSELD=0;
 ANSELE=0;
 
 GIE_bit=1; //ENABLE GLOBAL INTERRUPTS
 TMR0IE_bit=1; //ENABLE TIMER0 OVERFLOW INTERRUPT
 TMR0IF_bit=0; //TIMER0 INTERRUPT FLAG IS DOWN
 
 //SET TIMER0
 T08BIT_bit=1; //TIMER0 IS CONFIGURED AS AN 9-BIT TIMER
 T0CS_bit=0; //INTERNAL INSTRUCTION CYCLE CLOCK (FOSC/4)
 PSA_bit=0; //PRESCALER ASSIGNED TO TIMER0
 T0PS2_bit=1; //PRESCALER 1:256
 T0PS1_bit=1;
 T0PS0_bit=1;
 TMR0L=0;
 TMR0H=0;
 TMR0ON_bit=1; //ENABLE TIMER
 
 //SET TIMER2
 TMR2MD_bit=0; //TIMER2 PERIPHERAL MODULE IS ENABLED
 TMR2IE_bit=1; //TIMER2 INTERRUPT IS ENABLED
 TMR2IF_bit=0; //TIMER2 INTERRUPT FLAG IS DOWN
 T2OUTPS3_bit=1; //TIMER2 OUPUT POSTSCALER 1:16
 T2OUTPS2_bit=1;
 T2OUTPS1_bit=1;
 T2OUTPS0_bit=1;
 TMR2ON_bit=0; //TIMER2 IS OFF
 T2CKPS1_bit=1; //TIMER2 PRESCALER IS 16
 T2CKPS0_bit=1;
 PR2=255;
 
 //SET TIMER4
 TMR4MD_bit=0; //TIMER4 PERIPHERAL MODULE IS ENABLED
 TMR4IE_bit=1; //TIMER4 INTERRUPT IS ENABLED
 TMR4IF_bit=0; //TIMER4 INTERRUPT FLAG IS DOWN
 T4OUTPS3_bit=1; //TIMER4 OUPUT POSTSCALER 1:16
 T4OUTPS2_bit=1;
 T4OUTPS1_bit=1;
 T4OUTPS0_bit=1;
 TMR4ON_bit=1; //TIMER4 IS ON
 T4CKPS1_bit=1; //TIMER4 PRESCALER IS 16
 T4CKPS0_bit=1;
 PR4=255;

 SPI2_Init();
 Delay_ms(100);
 UART1_Init(9600);
 Delay_ms(100);
 UART2_Init(9600);
 Delay_ms(100);

 RELAY=0;
 LED1=1; //GREEN
 LED2=1; //RED
 LED3=0;
 LED4=0;
 Delay_ms(1000);
 LED1=0;
 LED2=0;
}

void readConfigRegister(){
 short i, buffer, buffer1;

 PRW_UP=1; //SPI PROGRAMMING MODE
 TRX_CE=0;
 TX_EN=0;

 Delay_ms(2);
 CSN=0;
 SPI2_Write(RC); //READ CONFIG REGISTER

 for(i=0;i<10;i++){
  buffer1=SPI2_Read(buffer);
 }

 CSN=1;
 Delay_ms(2);

 TX_EN=0; //RECEIVE MODE
 PRW_UP=1;
 TRX_CE=1;
}

void InitNRF(short nRFAddress0, short nRFAddress1, short nRFAddress2, short nRFAddress3){
 short i;
 short nRFConfig[10];

 PRW_UP=1; //SPI PROGRAMMING MODE
 TRX_CE=0;
 TX_EN=0;

 Delay_ms(2);
 nRFConfig[0] = 0b01111011; //123 CHANNEL NO
 nRFConfig[1] = 0b00101100; //433MHZ, +10DBM, NORMAL OPERATION, NO RETRANSMISSION
 nRFConfig[2] = 0b01000100; //4 BYTE RX ADDRESS WIDTH, 4 BYTE TX ADDRESS WIDTH
 nRFConfig[3] = 0b00000110; //RX PAYLOAD 6 BYTE
 nRFConfig[4] = 0b00000110; //TX PAYLOAD 6 BYTE
 nRFConfig[5] = nrfAddress0; //ADDRESS
 nRFConfig[6] = nrfAddress1;
 nRFConfig[7] = nrfAddress2;
 nRFConfig[8] = nrfAddress3;
 nRFConfig[9] = 0b11011011; //16 BIT CRC, CRC ENABLE, 16MHZ OSCCILLATOR, NO EXTRNAL CLOCK SIGNAL

 CSN = 0;
 SPI2_Write(WC); //Write nRF configuration

 for(i=0;i<10;i++){
  SPI2_Write(nRFConfig[i]);
 }

 CSN = 1;
 Delay_ms(2);

 TX_EN=0; //RECEIVE MODE
 PRW_UP=1;
 TRX_CE=1;
 }

void txPacket(unsigned short packet0,unsigned short packet1,unsigned short packet2,unsigned short packet3,unsigned short packet4,unsigned short packet5,short ad0,short ad1,short ad2,short ad3){
 TX_EN = 0; //POWER DOWN
 PRW_UP = 0;
 TRX_CE = 0;

 CSN=0;
 Delay_ms(2);
 
 SPI2_Write(WTA); //WRITE TX ADRESS
 SPI2_Write(ad0);
 SPI2_Write(ad1);
 SPI2_Write(ad2);
 SPI2_Write(ad3);
 CSN=1;

 Delay_ms(5);

 CSN=0;
 Delay_ms(2);
 
 SPI2_Write(WTP); //WRITE TX PAYLOAD
 SPI2_Write(packet0);
 SPI2_Write(packet1);
 SPI2_Write(packet2);
 SPI2_Write(packet3);
 SPI2_Write(packet4);
 SPI2_Write(packet5);

 CSN=1;
 Delay_ms(2);
 
 while( CD == 1 );

 PRW_UP=1;   //START TRANSMITTING
 TX_EN=1;
 TRX_CE=1;

 delay_ms(10);
 //while(DR==0); //WAIT FOR TRASMIT

 TRX_CE=1; //ENTER IN RECEIVE MODE
 TX_EN=0;
 PRW_UP=1;
 
}

void rxPacket(){
 short buffer;

 TX_EN=0; //RECEIVE MODE
 PRW_UP=1;
 TRX_CE=1;

 if(AM==1 && DR==1){

  CSN = 0;
  Delay_ms(2);
  SPI2_Write(RRP); //Read RX Payload

  data0 = SPI2_Read(buffer);
  data1 = SPI2_Read(buffer);
  data2 = SPI2_Read(buffer);
  data3 = SPI2_Read(buffer);
  data4 = SPI2_Read(buffer);
  data5 = SPI2_Read(buffer);

  Delay_ms(2);
  CSN = 1;

 }

}

SplitPacketsByDeviceID(){
 if(data0==RADAR_ID){
  RadarMatrix[0]=data0;
  RadarMatrix[1]=data1;
  RadarMatrix[2]=data2+30;
  RadarMovementFlag=data2+30; //FOR MOTION SENSOR LIGHTING
  RadarMatrix[3]=data3+30;
  RadarMatrix[4]=data4+30;
  RadarMatrix[5]=data5+30;
 }
 if(data0==SWITCH_ID){
  SwitchMatrix[0]=data0;
  SwitchMatrix[1]=data1;
  SwitchMatrix[2]=data2+30;
  SwitchMatrix[3]=data3+30;
  SwitchMatrix[4]=data4+30;
  SwitchMatrix[5]=data5+30;
 }
 if(data0==THERM_ID){
  ThermostatMatrix[0]=data0;
  ThermostatMatrix[1]=data1;
  ThermostatMatrix[2]=data2+30;
  ThermostatMatrix[3]=data3+30;
  ThermostatMatrix[4]=data4+30;
  ThermostatMatrix[5]=data5+30;
 }
}

SendPacketsToRaspberry(){

 if(data0==RADAR_ID){
  data0=0;

  if(RadarMatrix[1]==RADAR_REP){
   RadarMatrix[1]=0;
   ReportCounter++;
  }
 }


 if(data0==THERM_ID){
  data0=0;
  UART2_Write(SYNC);
  UART2_Write(ThermostatMatrix[0]);
  UART2_Write(ThermostatMatrix[1]);
  UART2_Write(ThermostatMatrix[2]);
  UART2_Write(ThermostatMatrix[3]);
  UART2_Write(ThermostatMatrix[4]);
  UART2_Write(ThermostatMatrix[5]);
  UART2_Write(NEWLINE);
 }
 
 if(data0==SWITCH_ID){
  data0=0;
  UART2_Write(SYNC);
  UART2_Write(SwitchMatrix[0]);
  UART2_Write(SwitchMatrix[1]);
  UART2_Write(SwitchMatrix[2]);
  UART2_Write(SwitchMatrix[3]);
  UART2_Write(SwitchMatrix[4]);
  UART2_Write(SwitchMatrix[5]);
  UART2_Write(NEWLINE);
 }

}

CheckRadarAlarm(){
 if(RadarAlarmFlag==1){
  RadarAlarmFlag=0;
  if(ReportCounter==0){
   RELAY=1;
   LED2=1;
   UART2_Write(SYNC); //SEND ALARM FAILURE
   UART2_Write(RADAR_ID);
   UART2_Write(NOP);
   UART2_Write(NOP+30);
   UART2_Write(NOP+30);
   UART2_Write(NOP+30);
   UART2_Write(RADAR_REP+30); //ALARM REPORT FAILURE ID
   UART2_Write(NEWLINE);

  }else{
   ReportCounter=0;
  }
 }
}

SerialPort1Read(){
 if(UART1_Data_Ready()) {
  Synchronization = UART1_Read();
  
  if(Synchronization=='T'){ //FOR THERMOSTAT
   i=0;
   SyncFlag=10;
  }
  if(SyncFlag==10){
   receive[i]=Synchronization;
   i++;
   if(i==7){
    i=0;
    SyncFlag=0;
    InformThermostatFlag=1;
   }
  }

  if(Synchronization=='R'){ //FOR RADAR
   i=0;
   SyncFlag=20;
  }
  if(SyncFlag==20){
   receive[i]=Synchronization;
   i++;
   if(i==7){
    i=0;
    SyncFlag=0;
    InformAlarmData=1;
   }
  }
  

  if(Synchronization=='S'){ //FOR SWITCH
   i=0;
   SyncFlag=30;
  }
  if(SyncFlag==30){
   receive[i]=Synchronization;
   i++;
   if(i==7){
    i=0;
    SyncFlag=0;
    InformSwitchFlag=1;
   }
  }

 }
}

SerialPort2Read(){
 if(UART2_Data_Ready()){
  Synchronization = UART2_Read();

  if(Synchronization=='T'){ //FOR THERMOSTAT
   i=0;
   SyncFlag=10;
  }
  if(SyncFlag==10){
   receive[i]=Synchronization;
   i++;
   if(i==7){
    i=0;
    SyncFlag=0;
    InformThermostatFlag=1;
   }
  }

  if(Synchronization=='R'){ //FOR RADAR
   i=0;
   SyncFlag=20;
  }
  if(SyncFlag==20){
   receive[i]=Synchronization;
   i++;
   if(i==7){
    i=0;
    SyncFlag=0;
    InformAlarmData=1;
   }
  }


  if(Synchronization=='S'){ //FOR SWITCH
   i=0;
   SyncFlag=30;
  }
  if(SyncFlag==30){
   receive[i]=Synchronization;
   i++;
   if(i==7){
    i=0;
    SyncFlag=0;
    InformSwitchFlag=1;
   }
  }

  
 }
}

InformThermostat(){

 if(InformThermostatFlag==1){
  InformThermostatFlag=0;

  InformThermostatData[0]=NOP;
  InformThermostatData[1]=receive[1]-0x30; //ON OFF
  receive[2]=receive[2]-0x30;
  receive[2]=receive[2]*10;
  receive[3]=receive[3]-0x30;
  InformThermostatData[2]=receive[2]+receive[3]; //USER TEMPERATURE
  InformThermostatData[3]=receive[4]-0x30; //USER TEMPERATURE DECIMAL DIGIT
  InformThermostatData[4]=NOP;
  InformThermostatData[5]=NOP;

  txPacket(InformThermostatData[0],InformThermostatData[1],InformThermostatData[2],InformThermostatData[3],InformThermostatData[4],InformThermostatData[5],0x3A,0x84,0xB3,0xE6);

 }

}

AlarmProcedure(){

 if(InformAlarmData==1){
  InformAlarmData=0;
  
  if(receive[1]=='1')   SecurityBreachFlag=1;

  if(receive[1]=='0')   SecurityBreachFlag=0;

  if(receive[4]=='1'){
   RELAY=0;
   LED2=0;
  }


  if(receive[2]=='1'){
   RadarAlarmFlag=0;
   AlarmCounter=0;
   RELAY=0;
   LED2=0;
  }

  UART2_Write(SYNC); //SEND FFEDBACK
  UART2_Write(RADAR_ID);
  UART2_Write(NOP);
  UART2_Write(receive[4]+30); //ALERT BY MOVEMENT CLEAR
  UART2_Write(receive[2]+30); //ALERT CLEAR
  UART2_Write(receive[1]+30); //ALARM SET
  UART2_Write(NOP+30);
  UART2_Write(NEWLINE);

 }

 if(SecurityBreachFlag==1 && RadarMatrix[2]==RADAR_MOV+30){
  RadarMatrix[2]=0;
  RELAY=1;
  LED2=1;

  UART2_Write(SYNC); //SEND FFEDBACK
  UART2_Write(RADAR_ID);
  UART2_Write(NOP);
  UART2_Write(NOP+30);
  UART2_Write(NOP+30);
  UART2_Write(NOP+30);
  UART2_Write(RADAR_MOV+30); //SECURITY BREACH
  UART2_Write(NEWLINE);

 }else{
  RadarMatrix[2]=0;
 }

}

InformSwitch(){
 if(InformSwitchFlag==1){

  InformSwitchData[0]=NOP;
  InformSwitchData[1]=receive[1]-0x30; //ON-OFF
  InformSwitchData[2]=receive[2]-0x30; //DIMMER
  InformSwitchData[3]=NOP;
  InformSwitchData[4]=NOP;
  InformSwitchData[5]=NOP;

  txPacket(InformSwitchData[0],InformSwitchData[1],InformSwitchData[2],InformSwitchData[3],InformSwitchData[4],InformSwitchData[5],0x98,0xD5,0x1A,0xFA);

  SwitchOnOff=receive[1];
  MotionSensorLighting=receive[3];
  LuminositySensorLighting=receive[4];

  receive[5]=receive[5]-0x30;
  if(receive[6]=='N'){
   receive[6]=0;
  }else{
   receive[5]=receive[5]*10;
   receive[6]=receive[6]-0x30;
  }
  SetLuminosity=receive[5]+receive[6];

  InformSwitchFlag=0;
 }
 
 if(SwitchOnOff=='2' && MotionSensorLighting=='1' && RadarMovementFlag==RADAR_MOV+30){
  RadarMovementFlag=0;
 
  txPacket(NOP,1,0,NOP,NOP,NOP,0x98,0xD5,0x1A,0xFA);

  PR2=255;
  TMR2ON_bit=1; //START TIMER

 }else{
  RadarMovementFlag=0;
 }

 if(LightFlag==1){
  LightFlag=0;
  TMR2ON_bit=0;
  txPacket(NOP,2,0,NOP,NOP,NOP,0x98,0xD5,0x1A,0xFA);
 }

 if(LuminositySensorLighting=='1' && LumensFlag==1){
  SwitchMatrix[5] = SwitchMatrix[5]-30;
  if( SwitchMatrix[5] < SetLuminosity ){
   txPacket(NOP,1,0,NOP,NOP,NOP,0x98,0xD5,0x1A,0xFA);
  }
  if( SwitchMatrix[5] > SetLuminosity ){
   txPacket(NOP,2,0,NOP,NOP,NOP,0x98,0xD5,0x1A,0xFA);
  }
  
  LumensFlag=0;
 }
 
}

GetSettings(){
 LED1=1;
 Delay_ms(500);
 LED1=0;
 Delay_ms(500);

 LED1=1;
 Delay_ms(500);
 LED1=0;
 Delay_ms(500);

 LED1=1;

 Uart2_Write(SYNC2); //GET SETTINGS COMMAND
 Uart2_Write(NOP);
 Uart2_Write(NOP);
 Uart2_Write(NOP+30);
 Uart2_Write(NOP+30);
 Uart2_Write(NOP+30);
 Uart2_Write(NOP+30);
 UART2_Write(NEWLINE);
}

void interrupt(){
 if(TMR0IF_bit){
  AlarmCounter++;
  if(AlarmCounter==8832){ //3 TIMES RADAR' S REPORT
   AlarmCounter=0;
   RadarAlarmFlag=1;
  }
  T0CS_bit=0;
  TMR0IF_bit=0;
 }
 
 if(TMR2IF_bit){
  LightCounter++;
  if(LightCounter==920){ //20 SECONDS
   LightCounter=0;
   LightFlag=1;
  }
  TMR2IF_bit=0;
 }
 
 if(TMR4IF_bit){
  LumensCounter++;
  if(LumensCounter==1900){ //~40 SECONDS
   LumensCounter=0;
   LumensFlag=1;
  }
  TMR4IF_bit=0;
 }

}


void main() {

PortInitialization();
readConfigRegister();
InitNRF(0x92,0xC5,0xA6,0xB0);
Delay_ms(60000);
GetSettings();

 while(1){
  rxPacket();
  SplitPacketsByDeviceID();
  SendPacketsToRaspberry();
  CheckRadarAlarm();
  SerialPort1Read();
  SerialPort2Read();
  InformThermostat();
  AlarmProcedure();
  InformSwitch();
 }
}