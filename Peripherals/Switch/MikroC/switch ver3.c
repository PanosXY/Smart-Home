#define WC          0x00 //Write Configuration Register Command
#define RC          0x10 //Read  Configuration Register Command
#define WTP         0x20 //Write TX Payload Command
#define RTXP        0x21 //Read  TX Payload Command
#define WTA         0x22 //Write TX Address Command
#define RTA         0x23 //Read  TX Address Command
#define RRP         0x24 //Read  RX Payload Command

#define ID          0xBF //switch ID

sbit TX_EN at PORTC.B7;  //OUT
sbit TRX_CE at PORTC.B6; //OUT
sbit PRW_UP at PORTB.B0; //OUT
sbit CSN at PORTC.B2;    //OUT
sbit DR at PORTB.B2;     //IN
sbit AM at PORTB.B3;      //IN
sbit CD at PORTB.B1;      //IN

sbit zeroCross at PORTB.B4; //IN
sbit led at PORTC.B1;   //OUT
sbit triac at PORTA.B0; //OUT
sbit buton at PORTB.B6; //IN

unsigned int upCpsLimit=0,downCpsLimit=0,timer1Count=0,timerH=40000,timerL=0,light=0,timer0counter=0,timer4Counter=0,cp1=0;
int dimerCounter=0,dimer=0,reportCounter=0;
double cps[11],cpsFinal=0,cpsFinal2=0;
unsigned short timer1GateFlag=0,buttonCounter=5,buttonFlag=0,k=0,packetFlag=0,packet0=0,packet1=0,packet2=0,packet3=0,packet4=0,packet5=0,reportFlag=0,temperature=0;

unsigned short packet21,packet22,packet20,packet23,packet24,packet25;

short aux1,aux2,buffer;
short cnt;
//LUMENS
short lumensAverageTable[7],lumensCounter=0,lumensAverage=0;

//Temp
unsigned short temperatureInt=0,temperatureDec=0;

//AD
unsigned int adc=0;
unsigned short temp=0,temp2=0;

//filter
short i,temp11;

//CPS FILTER
long cpsAverage[2];
short cpsCounter[2];

void portInit(){
ANSELA=0X00;
ANSELB=0X00;


ANSELA.F1=1;//PHOTRESISTOR ANALOG
ANSELA.F4=1;//CPS ANALOG
ANSELA.F5=1;//CPS ANALOG
ANSELA.F2=1;//TEMP SENSOR
ANSELB=0X00;
//ANSELB.F5=1;//CPS ANALOG

TRISC.F7=0;// TX_EN  OUT
TRISC.F6=0;// TRX_CE OUT
TRISB.F0=0;// PRW_UP OUT
TRISC.F2=0;// CSN    OUT
TRISB.F2=1;// DR     IN
TRISB.F3=1;// AM     IN
TRISB.F1=1;// CD     IN

TRISB.F4=1;// ZEROCROSS IN
TRISC.F1=0;// LED    OUT
TRISA.F0=0;// TRIAC OUT
//TRISB.F6=1;// button IN
TRISB.F5=1;//CPS BUTTON IN
TRISA.F4=1;//CPS BUTTON IN
TRISA.F5=1;//CPS BUTTON IN
TRISA.F1=1;//PHOTORESISTOR IN
TRISA.F2=1;//TEMP SENSOR IN

PORTA=0x00;
PORTB=0x00;
PORTC=0x00;

//WPUB.F6=1;   //pull up on portb.f6

C1ON_bit = 0;        // Disable comparators
C2ON_bit = 0;

//CPS MODULE
CPSRM_bit=0; // FIX REF VOLTAGE
CPSRNG1_bit=1;
CPSRNG0_bit=1;//OSCILLATOR IS IN low CURENT

CPSCON1=0x05; //cps channel 5

//DAC
DACNSS_bit=1;
DACEN_bit=1;

//FVR
FVREN_bit=1; //enable
CDAFVR1_bit=1;  ////fix ref voltage for cps
CDAFVR0_bit=0;

ADFVR1_bit=1; //fix ref voltage for ADC
ADFVR0_bit=0;

CPSON_bit=1; // CPS IS ENABLE

//A/D
ADFM_bit=1;

ADCS2_bit=0; //fosc
ADCS1_bit=0;
ADCS0_bit=0;

ADNREF_bit=0;   //-vref vss

ADPREF1_bit=0;  //+Vref fix ref voltage
ADPREF0_bit=0;

//INTERRUPTS
GIE_bit=1;       //GLOBAL INTERUPTS ENABLE
TMR0IE_bit=1;    //ENABLE OVERFLOW TIMER0 INTERRUPT
PEIE_bit=1;      //ENABLE PERIPHERAL
TMR1GIE_bit=0;  //ENABLE TIMER1 GATE INTERRUPT
IOCIE_bit=1;    //ENABLE INTERRUPT ON CHANGE
TMR2IE_bit=0;   //TIMER2 TO PR2 MATCH ENABLE INTERRUPT
TMR4IE_bit=1;   //ENABLE TIMER4 PR2 MATCH INTERRUPT
IOCBP4_BIT=0;  //INTERRUPT ON CHANGE ON PORTB.F4  for Zero cross

//TIMER0
TMR0CS_bit=0; //FOSC/4 CLOCK SOURCE
PSA_bit=0;    //ENABLE PRESCALER
PS2_bit=1;    //PRESCALLER 1:256
PS1_bit=1;
PS0_bit=0;

//TIMER1
T1CON=0b11000101;  //timer1 cps
                   //prescaler 1:1
                   //enable timer1

T1GCON=0b10100001; //timer1 is controller by gate
                   //timer1 counts when gate is LOW
                   //timer1 gate is enable
                   //timer0 overflow output

//TIMER2 SET-UP
PR2=0xFF;           //SET PR2
T2CON=0b00000001;   //T2CON REGISTER SET-UP WITH 1:1 PRESCALER, 1:1 POSTSCALER AND TIMER2 IS OFF
TMR2ON_bit=0;       //TIMER 2 IS oFF
TMR2=0X00;

//TIMER4 SET-UP
PR4=0XFF;
T4CON=0b1111111;   //PRESCALER 1:64, POSTSCALER 1:16,TIMER4 IS ON
TMR4=0X00;

//clears
IOCBF4_BIT=0;     //CLEAR INTERRUPT ON CHANGE FLAG
TMR0IF_bit=0;     //CLEAR TIMER0 INTERRUPT
TMR1H=0x00;       //clear timer1
TMR1L=0x00;
TMR2IF_bit=0;     //clear Timer2 interrupt FLAG
TMR2=0X00;        //CLEAR TIMER2

SPI1_INIT();
delay_ms(5);

PORTA=0x00;
PORTB=0x00;
PORTC=0x00;
}

void InitNRF(short nRFAddress0, short nRFAddress1, short nRFAddress2, short nRFAddress3){
 short i;
 short nRFConfig[10];

 PRW_UP=1;        //spi programming mode
 TRX_CE=0;
 TX_EN=0;
 delay_ms(5);

Delay_ms(2);
 nRFConfig[0] = 0b01111011; //123 CHANNEL NO
 nRFConfig[1] = 0b00001100; //433MHZ, +10DBM, NORMAL OPERATION, AUTO RETRANSMISSION
 nRFConfig[2] = 0b01000100; //4 BYTE RX ADDRESS WIDTH, 4 BYTE TX ADDRESS WIDTH
 nRFConfig[3] = 0b00000110; //RX PAYLOAD 6 BYTE
 nRFConfig[4] = 0b00000110; //TX PAYLOAD 6 BYTE
 nRFConfig[5] = nrfAddress0; //ADDRESS
 nRFConfig[6] = nrfAddress1;
 nRFConfig[7] = nrfAddress2;
 nRFConfig[8] = nrfAddress3;
 nRFConfig[9] = 0b11011011; //16 BIT CRC, CRC ENABLE, 16MHZ OSCCILLATOR, NO EXTRNAL CLOCK SIGNAL
 delay_ms(2);
 CSN = 0;
 delay_ms(2);

     SPI1_Write(WC); //Write nRF configuration

     for(i=0;i<10;i++){
     SPI1_Write(nRFConfig[i]);
     }

 CSN = 1;
 delay_ms(2);

 TX_EN=0;  //rec mode
 PRW_UP=1;
 TRX_CE=1;
 }

void txPacket(unsigned short packetID,unsigned short packetON_OFF,unsigned short packetDimer,unsigned short packetTemp,unsigned short packetTempDec,unsigned short packetLumens,short ad0,short ad1,short ad2,short ad3){

     TX_EN=0;
     PRW_UP=0;
     TRX_CE=0;

     CSN=0;
     delay_ms(2);
     SPI1_WRITE(WTA);  //WRITE TX ADRESS
     SPI1_WRITE(ad0);
     SPI1_WRITE(ad1);
     SPI1_WRITE(ad2);
     SPI1_WRITE(ad3);
     delay_ms(2);
     CSN=1;

     CSN=0;
     delay_ms(2);
     SPI1_WRITE(WTP);     //WRITE TX PACKET
     SPI1_WRITE(packetID);
     SPI1_WRITE(packetON_OFF);
     SPI1_WRITE(packetDimer);
     SPI1_WRITE(packetTemp);
      SPI1_WRITE(packetTempDec);
     SPI1_WRITE(packetLumens);
     delay_ms(2);
     CSN=1;
     while( CD == 1 ){}

     PRW_UP=1;   //TRANSMIT
     TX_EN=1;
     TRX_CE=1;

     while( DR == 0 ){}
    // delay_ms(1);

    TX_EN=0;  //rec mode
    PRW_UP=1;
    TRX_CE=1;
   }


void checkPacket(){

 if ( DR == 1 ){
 delay_ms(1);
 CSN=0;
 delay_ms(2);
 SPI1_Write(RRP);
 packet0=SPI1_Read(buffer);
 packet1=SPI1_Read(buffer);
 packet2=SPI1_Read(buffer);
 packet3=SPI1_Read(buffer);
 packet4=SPI1_Read(buffer);
 packet5=SPI1_Read(buffer);
 delay_ms(2);
 CSN=1;
 
  if ( packet1 == 2 ){
  light=0;
  TMR2IE_bit=0;
  TMR2ON_bit=0;
  IOCBP4_BIT=0;
  triac=0;
  packet1=0;
  }
 
  if(packet1 == 1){
  light=1;
  TMR2=0X00;
  IOCBF4_BIT=0; //clear zero cross flag
  TMR2IF_bit=0; //clear timer2 flag
  TMR2IE_bit=1; //enable timer2 interrupt
  IOCBP4_BIT=1; //enable zero cross  interrupt
  TMR2ON_bit=1; //enable timer2
  triac=1;
  packet1=0;
  }
  
  if(packet2 < 9){
  dimer=packet2;
  }
 txPacket(ID,light,dimer,temperatureInt,temperatureDec,lumensAverage,0x92,0xC5,0xA6,0xB0);
 }
}

void readConfigRegister(){
  short i,buffer,buffer2;

  PRW_UP=1;        //spi programming mode
  TRX_CE=0;
  TX_EN=0;
  CSN=0;
  delay_ms(2);

   SPI1_Write(RC);   //READ CONFIG REGISTER

    for(i=0;i<10;i++){
    buffer2=SPI1_Read(buffer);
    uart1_write(buffer2);
    }

  CSN=1;
  TX_EN=0;  //rec mode
  PRW_UP=1;
  TRX_CE=1;
  }

void checkButton(){

     if( timer1GateFLag == 1 ){
       TMR1GIE_bit=0;//disable timer1gate interrupt
       TMR1ON_bit=0; //disable timer1 interrupt
       TMR0IE_bit=0; //disable OVERFLOW TIMER0 INTERRUPT
       
       timerH=0;
       timerL=0;

       timerH=TMR1H;
       timerL=TMR1L;
       
       aux1=TMR1H;
       aux2=TMR1L;

       timerH=timerH<<8;
       timerH=timerH|timerL;
       
      //txPacket(0x11,aux1,aux2,dimer,0x00,0xAB,0xAC,0xAD,0xAE);
      //delay_ms(500);
     
     if(CPSCON1 == 5 ){
     cpsAverage[0]=(cpsAverage[0]*15+timerH)/16;
     
      if(timerH < cpsAverage[0]-500 ){
      cpsCounter[0]++;
      cpsCounter[0]++;
      
        if(cpsCounter[0]>3){
        buttonFlag=1;
        delay_ms(100);
        cpsCounter[0]=0;
        }
        else{cpsCounter[0]--;}
        
        if(cpsCounter[0] < 0){cpsCounter[0]=0;}
      }
     }

     if(CPSCON1 == 6){
     cpsAverage[1]=(cpsAverage[1]*15+timerH)/16;
     
       if(timerH < cpsAverage[1]-500 ){
       cpsCounter[1]++;
       cpsCounter[1]++;
       
         if(cpsCounter[1] > 3){
         buttonFlag=2;
         delay_ms(100);
         cpsCounter[1]=0;
         }
         else{cpsCounter[1]--;}
         
         if(cpsCounter[1] < 0){cpsCounter[1]=0;}
         
       } 
     }
     
     buttonCounter++;
     if( buttonCounter == 7 ) {buttonCounter = 5;}
     CPSCON1=buttonCounter;

     TMR1H=0x00; //clear timer1 counter
     TMR1L=0x00;
     TMR1GIF_bit=0;    //clear timer1gate flag bit
     TMR0IF_bit=0;    //clear timer0 flag

     TMR0=0x00;      //clear TImer0
     TMR0IE_bit=1;   //ENABLE OVERFLOW TIMER0 INTERRUPT
     TMR1GIE_bit=1; //Enable timer1gate interrupt
     TMR1ON_bit=1;  //Enable timer1 interrupt
     timer1GateFlag=0;
     }
}

void checkLight(){
  if(buttonFlag == 1 ){

   light=!light;

    if( light == 0 ){
    TMR2IE_bit=0;
    TMR2ON_bit=0;
    IOCBP4_BIT=0;
    triac=0;
    }

    if( light == 1 ){
    TMR2=0X00;
    IOCBF4_BIT=0; //clear zero cross flag
    TMR2IF_bit=0; //clear timer2 flag
    TMR2IE_bit=1; //enable timer2 interrupt
    IOCBP4_BIT=1; //enable zero cross  interrupt
    TMR2ON_bit=1; //enable timer2
    triac=1;
    }
    
   txPacket(ID,light,dimer,temperatureInt,temperatureDec,lumensAverage,0x92,0xC5,0xA6,0xB0);

   delay_ms(500);
   buttonFlag=0;
   }
}

void checkDimmer(){

  if( light == 1){
    if( buttonFlag == 2 ){
    dimer=dimer+1;
    if( dimer == 1 ){ dimer=4; }
    if( dimer > 8 ){  dimer=0;  }
    delay_ms(100);
    txPacket(ID,light,dimer,temperatureInt,temperatureDec,lumensAverage,0x92,0xC5,0xA6,0xB0);
    }

   if( buttonFlag == 3 ){
   dimer=dimer-1;
   if( dimer == 4 )  { dimer=0; }
   if( dimer < 0  )  { dimer=8; }
   delay_ms(100);
   txPacket(ID,light,dimer,temperatureInt,temperatureDec,lumensAverage,0x92,0xC5,0xA6,0xB0);
   }

   delay_ms(100);
   buttonFlag=0;

  }
}

void cpsAverageCal() {
short loopCounter=0;

 while ( loopCounter < 10 ){
 
     if( timer1GateFLag == 1 ){
       TMR1GIE_bit=0;//disable timer1gate interrupt
       TMR1ON_bit=0; //disable timer1 interrupt
       TMR0IE_bit=0; //disable OVERFLOW TIMER0 INTERRUPT

       timerH=0;
       timerL=0;

       timerH=TMR1H;
       timerL=TMR1L;

       aux1=TMR1H;
       aux2=TMR1L;

       timerH=timerH<<8;
       timerH=timerH|timerL;

       //delay_ms(1000);
      //txPacket(0xff,aux1,aux2,dimer,0x00,0xAB,0xAC,0xAD,0xAE);

     if(CPSCON1 == 5 ){
     cpsAverage[0]=cpsAverage[0]+timerH;
     delay_ms(500);
     loopCounter++;
     }


     if(CPSCON1 == 6){
     cpsAverage[1]=cpsAverage[1]+timerH;
     delay_ms(500);
     }


     buttonCounter++;
     if( buttonCounter == 7 ) { buttonCounter = 5; }
     
     CPSCON1=buttonCounter;

     TMR1H=0x00; //clear timer1 counter
     TMR1L=0x00;
     TMR1GIF_bit=0;    //clear timer1gate flag bit
     TMR0IF_bit=0;    //clear timer0 flag

     TMR0=0x00;      //clear TImer0
     TMR0IE_bit=1;   //ENABLE OVERFLOW TIMER0 INTERRUPT
     TMR1GIE_bit=1; //Enable timer1gate interrupt
     TMR1ON_bit=1;  //Enable timer1 interrupt
     timer1GateFlag=0;
     }
  }
  cpsAverage[0]=(cpsAverage[0]/10)-10000;
  cpsAverage[1]=(cpsAverage[1]/10)-10000;
  cpsAverage[2]=(cpsAverage[2]/10)-10000;
}

void sendReport(){
 if(reportFlag == 1){

 ADCON0=0b00000100; //channel 1
 ADPREF1_bit=0;
 ADPREF0_bit=0;
 ADCON0.F1=0; //A/D CONVERSION NOT IN PROGRESS
 ADON_bit=1; //ENABLE A/D CONVERTER

 Delay_ms(1);
 ADCON0.F1=1;            //START CONVERSION
 while(ADCON0.F1);      //WAIT FOR CONVERSION
 Delay_ms(1);
 adc=(ADRESH<<8)+ADRESL;
 temp=(adc*30)/1023;
 lumensAverageTable[lumensCounter]=temp;
 ADON_bit=0;          //Disable A/D CONVERTER
 ADCON0.F1=0; //A/D CONVERSION NOT IN PROGRESS
 Delay_ms(1);


 Delay_ms(5);
 ADPREF1_bit=1;       //Fix ref voltage 2048 volt
 ADPREF0_bit=1;
 ADCON0=0b00001000;  //channel 2
 ADCON0.F1=0;        //A/D CONVERSION NOT IN PROGRESS
 ADON_bit=1;         //ENABLE A/D CONVERTER
 
 Delay_ms(5);
 ADCON0.F1=1;        //START CONVERSION
 while(ADCON0.F1);   //WAIT FOR CONVERSION
 Delay_ms(5);
 adc=(ADRESH<<8)+ADRESL;
 adc=adc-150;
 temperatureInt=adc/9;
 temperatureDec=adc%9;


 ADON_bit=0;         //Disable A/D CONVERTER
 ADCON0.F1=0;        //A/D CONVERSION NOT IN PROGRESS  */

   lumensCounter++;
    if( lumensCounter > 7 ){ //8

      for(i=0;i<8;i++){
       for(k=0;k<7-i;k++){
       if(lumensAverageTable[k]>lumensAverageTable[k+1]){
       temp11=lumensAverageTable[k];
       lumensAverageTable[k]=lumensAverageTable[k+1];
       lumensAverageTable[k+1]=temp11;
       }
       
       }
      }
    
    lumensAverage=(lumensAverageTable[3]+lumensAverageTable[4])/2;
    lumensAverage=30-lumensAverage;
    txPacket(ID,light,dimer,temperatureInt,temperatureDec,lumensAverage,0x92,0xC5,0xA6,0xB0);
    delay_ms(2);
    lumensCounter=0;
    }

   reportFlag=0;
  }

}

void interrupt() {

    if( TMR1GIF_bit == 1 ){
    timer1GateFlag=1;
    TMR1ON_bit=0;    //dsiable timer1 counter
    TMR1GIF_bit=0;  //clear timer1Gate flag
    }

    if( TMR0IF_bit == 1 ){      //TIMER0 INTERRUPT
    TMR0IF_BIT=0;
    }

    if( IOCBF4_BIT == 1 ){    //ZERO_CROSS INTERRUPT
    delay_us(700);
    triac=0;
    dimerCounter=0;
    IOCBF4_BIT=0;
    TMR2=0X00;
    }

    if( TMR2IF_bit == 1 ){   //TIMER2 INTERRUPT  0.256msec
    dimerCounter++;

      if( dimerCounter >= dimer ){
      triac=1;
      dimerCounter=0;
      }

    TMR2IF_bit=0;
    TMR2=0X00;
    }

    if( TMR4IF_bit==1){     //TIMER4 INTERRUPT SEND REPORT (0.26sec interrupt period)
    timer4Counter++;
    
     if(timer4Counter > 10 ){
     reportFlag=1;
     timer4Counter=0;
     }

    TMR4IF_bit=0;
    }

 }

void main(){
 short i=0;
 portInit();
 readConfigRegister();
 initNrf(0x98,0xD5,0x1A,0xFA);
 readConfigRegister();
  
 led=1;
 delay_ms(1000);
 led=0;
 delay_ms(1000);
 triac=1;
 delay_ms(1000);
 triac=0;

cpsAverage[0]=8000;
cpsAverage[1]=8000;

   
delay_ms(5000);


TMR1H=0xFF;

 while(1){
   checkPacket();
   checkButton();
   checkLight();
   checkDimmer();
   sendReport();
   buttonFlag=0;
 }
 
}