#define WC          0x00 //Write Configuration Register Command
#define RC          0x10 //Read  Configuration Register Command
#define WTP         0x20 //Write TX Payload Command
#define RTXP        0x21 //Read  TX Payload Command
#define WTA         0x22 //Write TX Address Command
#define RTA         0x23 //Read  TX Address Command
#define RRP         0x24 //Read  RX Payload Command

#define ID          0x7E //RADAR ID
#define REPORT      0xB5 //RADAR REPORT
#define MOVEMENT    0x9A //RADAR MOVEMENT
#define NOP         0x00 //DUMMY BYTE

sbit LED1 at RA0_bit;
sbit LED2 at RA2_bit;
sbit RADAR at RB4_bit;
sbit PBUTTON at RB5_bit;
sbit CHARGING at RC1_bit;

sbit TX_EN at RB2_bit;  //OUTPUT
sbit TRX_CE at RB3_bit; //OUTPUT
sbit PWR_UP at RC7_bit; //OUTPUT
sbit CSN at RC2_bit;    //OUTPUT
sbit DR at RB1_bit;     //INPUT
sbit AM at RB0_bit;     //INPUT
sbit CD at RC6_bit;     //INPUT

short PbuttonFlag=0, RadarFlag=0, RadarFlag1=0, DRFlag=0; //FLAGS
short AutoRetranCounter=0;

void PortInitialization(){
 TRISA0_bit=0; //LED1 PIN AS OUTPUT
 TRISA2_bit=0; //LED2 PIN AS OUTPUT
 TRISB4_bit=1; //RADAR PIN AS INPUT
 TRISB5_bit=1; //PBUTTON PIN AS INPUT
 TRISC1_bit=1; //CHARGING INDICATOR PIN AS INPUT

 TRISB2_bit=0; //TX_EN PIN AS OUTPUT
 TRISB3_bit=0; //TRX_CE PIN AS OUTPUT
 TRISC7_bit=0; //PWR_UP PIN AS OUPUT
 TRISC2_bit=0; //CSN PIN AS OUTPUT
 TRISB1_bit=1; //DR PIN AS INPUT
 TRISB0_bit=1; //AM PIN AS INPUT
 TRISC6_bit=1; //CD PIN AS INPUT

 ANSELA=0; //ALL PINS AS DIGITAL
 ANSELB=0;

 OPTION_REG.F7=0;
 WPUB5_bit=1; //ENABLE WEAK PULL-UP RESISTOR FOR BUTTON
 WPUB4_bit=0; //DISABLE WEAK PULL-UP RESISTOR FOR RADAR
 WPUB1_bit=0; //DISABLE WEAK PULL-UP RESISTOR FOR DR
 WPUB0_bit=0; //DISABLE WEAK PULL-UP RESISTOR FOR AM
 
 GIE_bit=1; //ENABLE GLOBAL INTERRUPTS
 PEIE_bit=1;
 IOCIE_bit=1; //INTERRUPT ON CHANGE ENABLED
 IOCIF_bit=0; //INTERRUPT ON CHANGE FLAG IS DOWN

 IOCBN5_bit=1; //INTERRUPT ON CHANGE FOR BUTTON ON NEGATIVE EDGE
 IOCBF5_bit=0; //INTERRUPT ON CHANGE FLAG FOR BUTTON IS DOWN

 IOCBP4_bit=1; //INTERRUPT ON CHANGE FOR RADAR ON POSITIVE EDGE
 IOCBF4_bit=0; //INTERRUPT ON CHANGE FLAG FOR RADAR IS DOWN
 
 IOCBN1_bit=1; //INTERRUPT ON CHANGE FOR DR ON NEGATIVE EDGE
 IOCBF1_bit=0; //INTERRUPT ON CHANGE FLAG FOR DR IS DOWN

 WDTPS4_bit=1; //WATCHDOG TIMER IS SET TO 64S ~= 1MIN
 WDTPS3_bit=0;
 WDTPS2_bit=0;
 WDTPS1_bit=0;
 WDTPS0_bit=0;

 SPI1_Init();
 Delay_ms(100);
 
 LED1=1;
 LED2=1;
 Delay_ms(1000);
 LED1=0;
 LED2=0;

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

void InitNRF(short nRFAddress0, short nRFAddress1, short nRFAddress2, short nRFAddress3){
 short i;
 short nRFConfig[10];

 PWR_UP=1; //SPI PROGRAMMING MODE
 TRX_CE=0;
 TX_EN=0;

 nRFConfig[0] = 0b01111011; //123 CHANNEL NO
 nRFConfig[1] = 0b00101100; //433MHZ, +10DBM, NORMAL OPERATION, AUTO RETRANSMISSION
 nRFConfig[2] = 0b01000100; //4 BYTE RX ADDRESS WIDTH, 4 BYTE TX ADDRESS WIDTH
 nRFConfig[3] = 0b00000001; //RX PAYLOAD 1 BYTE
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
 PWR_UP=0;
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
 if(DRFlag==1){
  DRFlag=0;
  TRX_CE=0; //ENTER IN POWER DOWN MODE
  TX_EN=0;
  PWR_UP=0;
 }

}


CheckButton(){
 if(PbuttonFlag){
  //CODE
  PbuttonFlag=0;
 }
}

SendReport(){
 if(!RadarFlag1){
  txPacket(ID,REPORT,NOP,NOP,NOP,NOP,0x92,0xC5,0xA6,0xB0);
 }
 RadarFlag1=0;
}

CheckRadar(){
 if(RadarFlag==1 && RADAR==1){
  txPacket(ID,REPORT,MOVEMENT,NOP,NOP,NOP,0x92,0xC5,0xA6,0xB0);
  RadarFlag=0;
 }
}

void interrupt(){
 if(IOCBF5_bit){ //BUTTON PRESSED
  PbuttonFlag=1;
  IOCBF5_bit=0;
 }

 if(IOCBF4_bit){ //RADAR SIGNAL
  RadarFlag=1;
  RadarFlag1=1;
  IOCBF4_bit=0;
 }
 
 if(IOCBF1_bit){ //DR IS SET TO LOW
  AutoRetranCounter++;
  if(AutoRetranCounter==6){
   AutoRetranCounter=0;
   DRFlag=1;
  }
  IOCBF1_bit=0;
 }
}

Sleep(){
   asm SLEEP;
}


void main() {
 PortInitialization();
 readConfigRegister();
 initNrf(0x4C,0x3A,0x85,0xE9);

 while(1){
  SendReport();
  CheckRadar();
  asm{sleep};
 }

}