#define WC          0x00 //Write Configuration Register Command
#define RC          0x10 //Read  Configuration Register Command
#define WTP         0x20 //Write TX Payload Command
#define RTXP        0x21 //Read  TX Payload Command
#define WTA         0x22 //Write TX Address Command
#define RTA         0x23 //Read  TX Address Command
#define RRP         0x24 //Read  RX Payload Command

#define ID          0xAE //THERMOSTAT ID

sbit LCD_RS at RE0_bit;
sbit LCD_EN at RA5_bit;
sbit LCD_D4 at RA4_bit;
sbit LCD_D5 at RA3_bit;
sbit LCD_D6 at RA2_bit;
sbit LCD_D7 at RA1_bit;

sbit LCD_RS_Direction at TRISE0_bit;
sbit LCD_EN_Direction at TRISA5_bit;
sbit LCD_D4_Direction at TRISA4_bit;
sbit LCD_D5_Direction at TRISA3_bit;
sbit LCD_D6_Direction at TRISA2_bit;
sbit LCD_D7_Direction at TRISA1_bit;

sbit LED1 at RD0_bit;
sbit LED2 at RD1_bit;
sbit RELAY at RC0_bit;
sbit PBUTTON at RB5_bit;
sbit ON_OFF_BUTTON at RD7_bit;
sbit INCREASE_BUTTON at RD6_bit;
sbit DECREASE_BUTTON at RD5_bit;
sbit BACKLIGHT at RA0_bit;

sbit TX_EN at RC1_bit;  //OUTPUT
sbit TRX_CE at RC2_bit; //OUTPUT
sbit PWR_UP at RB2_bit; //OUTPUT
sbit CSN at RD2_bit;    //OUTPUT
sbit DR at RB0_bit;     //INPUT
sbit AM at RB3_bit;     //INPUT
sbit CD at RB1_bit;     //INPUT

const char character[] = {6,9,9,6,0,0,0,0};
char FixedText1[] = "Temp:";
char FixedText2[] = "User Temp:";
char FixedText3[] = "ON ";
char FixedText4[] = "OFF";
char txt[15];

short PbuttonFlag=0, OnOffFlag=0, LcdTemperatureFlag=0, HysterisisFlag=1, DRFlag=0, InformEverythingFlag=0; //FLAGS
unsigned short OnOff=1, data0, data1, data2, data3, data4, data5;
int TemperatureCounter=0, HysterisisCounter=0;
unsigned short BacklightCounter=0, AutoRetranCounter=0;
float Temperature, UserTemperature=20.001, temp, temp1, temp2;

void CustomChar(char pos_row, char pos_char) {
  char i;
    Lcd_Cmd(64);
    for (i = 0; i<=7; i++) Lcd_Chr_CP(character[i]);
    Lcd_Cmd(_LCD_RETURN_HOME);
    Lcd_Chr(pos_row, pos_char, 0);
}

GetTemperature(){
 ADCON0.F1=1; //START CONVERSION
 while(ADCON0.F1); //WAIT FOR CONVERSION
 Delay_ms(3);
 Temperature=(ADRESH<<8)+ADRESL;

 Temperature=Temperature-240; //TEMP=(Vout-400mV)/19.5mV      (~400mV)
 Temperature=Temperature/9.75;
 
}

PortInitialization(){
 TRISD0_bit=0; //LED1 PIN AS OUTPUT
 TRISD1_bit=0; //LED2 PIN AS OUTPUT
 TRISC0_bit=0; //RELAY PIN AS OUTPUT
 TRISA0_bit=0; //LCD BACKLIGHT PIN AS OUTPUT
 TRISB5_bit=1; //PBUTTON PIN AS INPUT
 TRISB4_bit=1; //TEMPERATURE SENSOR PIN AS INPUT
 TRISD7_bit=1; //ON_OFF_BUTTON AS INPUT
 TRISD6_bit=1; //INCREASE_BUTTON AS INPUT
 TRISD5_bit=1; //DECREASE_BUTTON AS INPUT

 TRISC1_bit=0; //TX_EN PIN AS OUTPUT
 TRISC2_bit=0; //TRX_CE PIN AS OUTPUT
 TRISB2_bit=0; //PWR_UP PIN AS OUPUT
 TRISD2_bit=0; //CSN PIN AS OUTPUT
 TRISB0_bit=1; //DR PIN AS INPUT
 TRISB3_bit=1; //AM PIN AS INPUT
 TRISB1_bit=1; //CD PIN AS INPUT

 ANSELA=0; //ALL PINS AS DIGITAL
 ANSELB=0;
 ANSELD=0;
 ANSELE=0;
 ANSELB.F4=1; //TEMPERATURE SENSOR PIN AS ANALOG PIN

 OPTION_REG.F7=0; //WEAK PULL-UPS ARE ENABLED BY INDIVIDUAL WPU LATCH VALUES
 WPUB5_bit=1; //ENABLE PULL-UP RESISTOR FOR PBUTTON
 WPUB4_bit=0; //DISABLE PULL-UPS FOR OTHER INPUT PINS
 WPUB3_bit=0;
 WPUB1_bit=0;
 WPUB0_bit=0;

 GIE_bit=1; //ENABLE GLOBAL INTERRUPTS

 IOCIE_bit=1; //INTERRUPT ON CHANGE ENABLED
 IOCIF_bit=0; //INTERRUPT ON CHANGE FLAG IS DOWN

 IOCBN5_bit=1; //INTERRUPT ON CHANGE FOR BUTTON ON NEGATIVE EDGE
 IOCBF5_bit=0; //INTERRUPT ON CHANGE FLAG FOR BUTTON IS DOWN
 
 IOCBN0_bit=1; //INTERRUPT ON CHANGE FOR DR ON NEGATIVE EDGE
 IOCBF0_bit=0; //INTERRUPT ON CHANGE FLAG FOR DR IS DOWN

 //FIXED VOLTAGE REFERNCE REGISTER
 FVREN_bit=1; //FIXED VOLTAGE REFERENCE ENABLE
 FVRRDY_bit=1; //FIXED VOLTAGE REFERENCE OUTPUT IS READY FOR USE
 CDAFVR1_bit=0; //COMPARATOR AND DAC FVR IS OFF
 CDAFVR0_bit=0;
 ADFVR1_bit=1; //FIXED VOLTAGE REFERENCE OUPUT IS 2.048 VOLT
 ADFVR0_bit=0;

 //ADCON0 REGISTER
 CHS0_bit=1; //CHANNEL AN11 IS SELECTED
 CHS1_bit=1;
 CHS2_bit=0;
 CHS3_bit=1;
 CHS4_bit=0;
 ADCON0.F1=0; //A/D CONVERSION NOT IN PROGRESS
 ADON_bit=1; //ENABLE A/D CONVERTER

 //ADCON1 REGISTER
 ADFM_bit=1; //RESULT IS RIGHT JUSTIFIED
 ADCS0_bit=0; //Fosc/2
 ADCS1_bit=0;
 ADCS2_bit=0;
 ADNREF_bit=0; //VOLTAGE- REFERENCE TO VSS(GND)
 ADPREF1_bit=1; //VOLTAGE+ REFERENCE TO FVR MODULE
 ADPREF0_bit=1;

 //TIMER2
 PEIE_bit=1; //PERIPHERAL INTERRUPT ENABLED
 TMR2IE_bit=1; //TIMER2 INTERRUPT ENABLED
 TMR2IF_bit=0; //TIMER2 INTERRUPT FLAG IS DOWN
 T2OUTPS3_bit=1; //TIMER2 POSTSCALER 1:16
 T2OUTPS2_bit=1;
 T2OUTPS1_bit=1;
 T2OUTPS0_bit=1;
 TMR2ON_bit=1; //TIMER2 IS ON
 T2CKPS1_bit=1; //TIMER2 PRESCALER 1:16
 T2CKPS0_bit=0;
 PR2=255; //PR2 REGISTER IS 255
 
 //TIMER4
 TMR4IE_bit=1; //TIMER4 INTERRUPT ENABLED
 TMR4IF_bit=0; //TIMER4 INTERRUPT FLAG IS DOWN
 T4OUTPS3_bit=1; //TIMER4 POSTSCALER 1:16
 T4OUTPS2_bit=1;
 T4OUTPS1_bit=1;
 T4OUTPS0_bit=1;
 TMR4ON_bit=0; //TIMER4 IS OFF
 T4CKPS1_bit=1; //TIMER4 PRESCALER 1:16
 T4CKPS0_bit=0;
 PR4=255; //PR4 REGISTER IS 255
 
 //TIMER6
 TMR6IE_bit=1; //TIMER6 INTERRUPT ENABLED
 TMR6IF_bit=0; //TIMER6 INTERRUPT FLAG IS DOWN
 T6OUTPS3_bit=1; //TIMER6 POSTSCALER 1:16
 T6OUTPS2_bit=1;
 T6OUTPS1_bit=1;
 T6OUTPS0_bit=1;
 TMR6ON_bit=1; //TIMER6 IS ON
 T6CKPS1_bit=1; //TIMER6 PRESCALER 1:16
 T6CKPS0_bit=0;
 PR6=255; //PR6 REGISTER IS 255

 Lcd_Init(); //LCD INITIALIZATION
 Delay_ms(10);
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Cmd(_LCD_CURSOR_OFF);
 
 Spi1_Init();
 Delay_ms(100);

 RELAY=0;

 BACKLIGHT=1;
 LED1=1;
 Delay_ms(1000);
 BACKLIGHT=0;
 LED1=0;
}

void readConfigRegister(){
 short i,buffer,buffer2;

 PWR_UP=1; //SPI PROGRAMMING MODE
 TRX_CE=0;
 TX_EN=0;

 CSN=0;
 SPI1_Write(RC); //READ CONFIG REGISTER

 for(i=0;i<10;i++){
  buffer2=SPI1_Read(buffer);
 }

 CSN=1;
 Delay_ms(2);

 PWR_UP=1; //STANDBY MODE
 TX_EN=0;
 TRX_CE=0;
}

void InitNRF(unsigned short nRFAddress0,unsigned short nRFAddress1,unsigned short nRFAddress2,unsigned short nRFAddress3){
 short i;
 short nRFConfig[10];

 PWR_UP=1; //SPI PROGRAMMING MODE
 TRX_CE=0;
 TX_EN=0;

 nRFConfig[0] = 0b01111011; //123 CHANNEL NO
 nRFConfig[1] = 0b00101100; //433MHZ, +10DBM, NORMAL OPERATION, AUTO RETRANSMISSION
 nRFConfig[2] = 0b01000100; //4 BYTE RX ADDRESS WIDTH, 4 BYTE TX ADDRESS WIDTH
 nRFConfig[3] = 0b00000110; //RX PAYLOAD 6 BYTE
 nRFConfig[4] = 0b00000110; //TX PAYLOAD 6 BYTE
 nRFConfig[5] = nrfAddress0; //ADDRESS
 nRFConfig[6] = nrfAddress1;
 nRFConfig[7] = nrfAddress2;
 nRFConfig[8] = nrfAddress3;
 nRFConfig[9] = 0b11011011; //16 BIT CRC, CRC ENABLE, 16MHZ OSCCILLATOR, NO EXTRNAL CLOCK SIGNAL

 CSN = 0;
 SPI1_Write(WC); //Write nRF configuration

 for(i=0;i<10;i++){
  SPI1_Write(nRFConfig[i]);
 }

 CSN = 1;
 Delay_ms(2);

 TX_EN=0; //STANDBY MODE
 PWR_UP=1;
 TRX_CE=0;
}

txPacket(unsigned short packet0,unsigned short packet1,unsigned short packet2,unsigned short packet3,unsigned short packet4,unsigned short packet5,short ad0,short ad1,short ad2,short ad3){
 TX_EN = 0; //POWER DOWN
 PWR_UP = 0;
 TRX_CE = 0;

 CSN=0;
 Delay_ms(2);

 SPI1_Write(WTA); //WRITE TX ADRESS
 SPI1_Write(ad0);
 SPI1_Write(ad1);
 SPI1_Write(ad2);
 SPI1_Write(ad3);
 CSN=1;

 Delay_ms(5);

 CSN=0;
 Delay_ms(2);

 SPI1_Write(WTP); //WRITE TX PAYLOAD
 SPI1_Write(packet0);
 SPI1_Write(packet1);
 SPI1_Write(packet2);
 SPI1_Write(packet3);
 SPI1_Write(packet4);
 SPI1_Write(packet5);

 CSN=1;
 Delay_ms(2);

 while( CD == 1 );

 PWR_UP=1;   //START TRANSMITTING
 TX_EN=1;
 TRX_CE=1;

 while(DRFlag==0); //WAIT FOR 3 TIMES TRANSMISSION

  DRFlag=0;
  TRX_CE=1; //ENTER IN RECEIVE MODE
  TX_EN=0;
  PWR_UP=1;


}

void rxPacket(){
 short buffer;

 TX_EN=0; //RECEIVE MODE
 PWR_UP=1;
 TRX_CE=1;

 if(AM==1 && DR==1){

  CSN = 0;
  SPI1_Write(RRP); //Read RX Payload

  data0 = SPI1_Read(buffer);
  data1 = SPI1_Read(buffer);
  data2 = SPI1_Read(buffer);
  data3 = SPI1_Read(buffer);
  data4 = SPI1_Read(buffer);
  data5 = SPI1_Read(buffer);

  CSN = 1;

  InformEverythingFlag=1;

  TX_EN=0; //REMAIN IN RECEIVE MODE
  TRX_CE=1;
  PWR_UP=1;

 }
}

LcdWrite(){
 if(OnOff==1) Lcd_Out(1,1,FixedText3); //1ST LINE
 if(OnOff==0) Lcd_Out(1,1,FixedText4);
 Lcd_Out(1,6,FixedText1);
 GetTemperature();
 FloatToStr(Temperature, txt);
 Lcd_Out(1,11,txt);
 CustomChar(1, 15);
 Lcd_Out(1,16,"C");

 Lcd_Out(2,1,FixedText2); //2ND LINE
 FloatToStr(UserTemperature, txt);
 Lcd_Out(2,11,txt);
 CustomChar(2, 15);
 Lcd_Out(2,16,"C");
}

IncreaseButton(){
 if(INCREASE_BUTTON==0){

  BACKLIGHT=1;
  BacklightCounter=0;
  TMR4ON_bit=1;

  UserTemperature+=0.5;
  if(UserTemperature>30.1) UserTemperature=30.001;
  
  if(OnOff==1){
   if(Temperature<UserTemperature){
    RELAY=1;
    LED1=1;
   }
   if(Temperature>UserTemperature){
    RELAY=0;
    LED1=0;
   }
  }else{
   RELAY=0;
   LED1=0;
  }

  temp=Temperature-(unsigned short)Temperature;
  temp*=10;
  temp2=UserTemperature-(unsigned short)UserTemperature;
  temp2*=10;
  txPacket(ID,OnOff,(unsigned short)UserTemperature,(unsigned short)temp2,(unsigned short)Temperature,(unsigned short)temp,0x92,0xC5,0xA6,0xB0);

  FloatToStr(UserTemperature, txt);
  Lcd_Out(2,11,txt);
  CustomChar(2, 15);
  Lcd_Out(2,16,"C");
  Delay_ms(200);
 }

}

DecreaseButton(){
 if(DECREASE_BUTTON==0){

  BACKLIGHT=1;
  BacklightCounter=0;
  TMR4ON_bit=1;

  UserTemperature-=0.5;
  if(UserTemperature<10.1) UserTemperature=10.001;

  if(OnOff==1){
   if(Temperature<UserTemperature){
    RELAY=1;
    LED1=1;
   }
   if(Temperature>UserTemperature){
    RELAY=0;
    LED1=0;
   }
  }else{
   RELAY=0;
   LED1=0;
  }

  temp=Temperature-(unsigned short)Temperature;
  temp*=10;
  temp2=UserTemperature-(unsigned short)UserTemperature;
  temp2*=10;
  txPacket(ID,OnOff,(unsigned short)UserTemperature,(unsigned short)temp2,(unsigned short)Temperature,(unsigned short)temp,0x92,0xC5,0xA6,0xB0);

  FloatToStr(UserTemperature, txt);
  Lcd_Out(2,11,txt);
  CustomChar(2, 15);
  Lcd_Out(2,16,"C");
  Delay_ms(200);
 }
}

OnOffButton(){
 if(ON_OFF_BUTTON==0){

  BACKLIGHT=1;
  BacklightCounter=0;
  TMR4ON_bit=1;

  if(OnOff==0){
   OnOff=1;
   OnOffFlag=1;
   Lcd_Out(1,1,FixedText3);
  }

  if(OnOff==1 && OnOffFlag==0){
   OnOff=0;
   Lcd_Out(1,1,FixedText4);
  }
  OnOffFlag=0;
  
  if(OnOff==1){
   if(Temperature<UserTemperature){
    RELAY=1;
    LED1=1;
   }
   if(Temperature>UserTemperature){
    RELAY=0;
    LED1=0;
   }
  }else{
   RELAY=0;
   LED1=0;
  }
  
  temp=Temperature-(unsigned short)Temperature;
  temp*=10;
  temp2=UserTemperature-(unsigned short)UserTemperature;
  temp2*=10;
  txPacket(ID,OnOff,(unsigned short)UserTemperature,(unsigned short)temp2,(unsigned short)Temperature,(unsigned short)temp,0x92,0xC5,0xA6,0xB0);
  
  Delay_ms(200);
 }
}

OpenRelay(){
 if(OnOff==1){
 
  if(HysterisisFlag==1){
   HysterisisFlag=0; 
 
   if(Temperature<UserTemperature){
    RELAY=1;
    LED1=1;
   }
   else if(Temperature>UserTemperature){
    RELAY=0;
    LED1=0;
   }
 
  }
 
 }else{
  RELAY=0;
  LED1=0;
 }
}

InformTemperature(){
 if(LcdTemperatureFlag==1){
  GetTemperature();
  FloatToStr(Temperature, txt);
  Lcd_Out(1,11,txt);
  CustomChar(1, 15);
  Lcd_Out(1,16,"C");
  LcdTemperatureFlag=0;
  
  temp=Temperature-(unsigned short)Temperature;
  temp*=10;
  temp2=UserTemperature-(unsigned short)UserTemperature;
  temp2*=10;
  txPacket(ID,OnOff,(unsigned short)UserTemperature,(unsigned short)temp2,(unsigned short)Temperature,(unsigned short)temp,0x92,0xC5,0xA6,0xB0);

 }
}

InformEverything(){
 if(InformEverythingFlag==1){
  InformEverythingFlag=0;
  
  BACKLIGHT=1;
  BacklightCounter=0;
  TMR4ON_bit=1;

  OnOff=data1;
  UserTemperature=(float)data2;
  temp1=(float)data3;
  temp1/=10;
  UserTemperature=(UserTemperature+temp1)+0.001;
 
  if(OnOff==1){
   if(Temperature<UserTemperature){
    RELAY=1;
    LED1=1;
   }
   if(Temperature>UserTemperature){
    RELAY=0;
    LED1=0;
   }
  }else{
   RELAY=0;
   LED1=0;
  }

  if(OnOff==1) Lcd_Out(1,1,FixedText3); //1ST LINE
  if(OnOff==0) Lcd_Out(1,1,FixedText4);

  FloatToStr(UserTemperature, txt); //2ND LINE
  Lcd_Out(2,11,txt);
  CustomChar(2, 15);
  Lcd_Out(2,16,"C");
  
  temp=Temperature-(unsigned short)Temperature;
  temp*=10;
  temp2=UserTemperature-(unsigned short)UserTemperature;
  temp2*=10;
  txPacket(ID,OnOff,(unsigned short)UserTemperature,(unsigned short)temp2,(unsigned short)Temperature,(unsigned short)temp,0x92,0xC5,0xA6,0xB0);
  
 }
}

void interrupt(){
 if(IOCBF5_bit==1){
  PbuttonFlag=1;
  IOCBF5_bit=0;
 }
 
 if(TMR2IF_bit==1){
  TemperatureCounter++;
  if(TemperatureCounter==4500){ //5MIN
   TemperatureCounter=0;
   LcdTemperatureFlag=1;  
  }
  TMR2IF_bit=0;
 }
 
 if(TMR4IF_bit==1){
  BacklightCounter++;
  if(BacklightCounter==180){ //12SEC
   BacklightCounter=0;
   BACKLIGHT=0;
   TMR4ON_bit=0;
  }
  TMR4IF_bit=0;
 }
 
 if(TMR6IF_bit==1){
  HysterisisCounter++;
  if(HysterisisCounter==900){ //1MIN
   HysterisisCounter=0;
   HysterisisFlag=1;
  }
  TMR6IF_bit=0;
 }
 
 if(IOCBF0_bit){ //DR IS SET TO LOW
     if(TX_EN == 1){
     AutoRetranCounter++;
        if(AutoRetranCounter==6){
        AutoRetranCounter=0;
        DRFlag=1;
        }
     }
  IOCBF0_bit=0;
 }
 
}

void main() {
 PortInitialization();
 readConfigRegister();
 initNrf(0x3A,0x84,0xB3,0xE6);
 LcdWrite();

 while(1){
  GetTemperature();
  IncreaseButton();
  DecreaseButton();
  OnOffButton();
  OpenRelay();
  InformTemperature();
  rxPacket();
  InformEverything();
 }
}