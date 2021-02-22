#include "main.h"
// Noi khai bao hang so
#define     LED     PORTD
#define     ON      1
#define     OFF     0

#define PORTD_OUT   PORTD
#define TRISD_OUT    TRISD

#define     data_led    6
#define     data_led1  2
#define     data_led2  3
#define     clk            4   
#define     clk_1         0    
#define     latch         5
#define     latch_1      1
unsigned char binary_pattern[]={0b00000011, //0
                      0b10011111, //1 
                      0b00100101, //2
                      0b00001101, //3 
                      0b10011001, //4 
                      0b01001001, //5
                      0b01000001, //6  
                      0b00011111, //7
                      0b00000001, //8
                      0b00001001 //9 
                     };
void init_output(void);
// Noi khai bao bien toan cuc
unsigned char arrayMapOfOutput [8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char statusOutput[8] = {0,0,0,0,0,0,0,0};
// Khai bao cac ham co ban IO
void init_system(void);
void delay_ms(int value);
void OpenOutput(int index);
void CloseOutput(int index);
void TestOutput(void);
void ReverseOutput(int index);


void Phase1_GreenOn();
void Phase1_GreenOff();
void Phase1_YellowOn();
void Phase1_YellowOff();
void Phase1_RedOn();
void Phase1_RedOff();

void Phase2_GreenOn();
void Phase2_GreenOff();
void Phase2_YellowOn();
void Phase2_YellowOff();
void Phase2_RedOn();
void Phase2_RedOff();

#define     INIT_SYSTEM            255
#define     PHASE1_GREEN         0
#define     PHASE1_YELLOW       1
#define     PHASE2_GREEN         2
#define     PHASE2_YELLOW       3
#define     WAIT                        4
#define     CONSTANT_YELLOW 5
unsigned char statusOfLight = INIT_SYSTEM;
unsigned char statusOfAdj = INIT_SYSTEM;
unsigned char timeOfGreenPhase1 = 25;
unsigned char timeOfYellowPhase1 = 3;
unsigned char timeOfGreenPhase2 = 20;
unsigned char timeOfYellowPhase2 = 3;
unsigned char timeOfLight = 0;
unsigned char cntOfLight = 0;
unsigned char temp=25;
unsigned char aux=0;
unsigned char mode = 0;
unsigned char pre=10;
unsigned char tessst=0;
unsigned char second=0;
unsigned char sw=1;
void TestButton();
void AutTrafficLight();
void UartDataReceiveProcess();
void UartDataReceiveProcess_ElectronicDeviceControl();


void send_number_1(unsigned int data_out1,unsigned int data_out2);
//Send number to the two 7-segment LED modules
void send_led(unsigned int data_out);
//send 6 control bits to the "traffic lights"
void AppTrafficLight();
//main function to operate the "traffic lights"
void AdjTrafficLight();
//function to adjust the time of each phase.



////////////////////////////////////////////////////////////////////
//Hien thuc cac chuong trinh con, ham, module, function duoi cho nay
////////////////////////////////////////////////////////////////////
void main(void)
{
        unsigned int k = 0;
        init_system();
        delay_ms(1000);
        while (1)
          {
            while (!flag_timer3);
            flag_timer3 = 0;
            scan_key_matrix(); // 8 button
            AppTrafficLight();
            AdjTrafficLight();
            //AutTrafficLight();
            //TestButton();
           // send_number_2(45);
            //send_number_1(binary_pattern[1]);
            //send_number_2(binary_pattern[2]);
            //send_number_2(binary_pattern[3]);
            DisplayLcdScreenOld(); //Output process 14ms vs 10ms with no timer
          }
}
// Hien thuc cac module co ban cua chuong trinh
void delay_ms(int value)
{
	int i,j;
	for(i=0;i<value;i++)
		for(j=0;j<238;j++);
}

void init_output(void)
{
   TRISD_OUT = 0x00;
   PORTD_OUT = 0x00;
}

void init_system(void)
{
    init_output();
    lcd_init();
    //init_i2c();
    init_key_matrix();
    init_interrupt();
    lcd_clear();
    LcdClearS();
    //init_i2c();
    //write_ds1307(0,9);
    delay_ms(500);
    init_timer0(4695);//dinh thoi 1ms sai so 1%
    init_timer3(46950);//dinh thoi 10ms
    SetTimer0_ms(2);
    SetTimer3_ms(50); //Chu ky thuc hien viec xu ly input,proccess,output
    //PORTAbits.RA0 = 1;
}

void OpenOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTD_OUT = PORTD_OUT | arrayMapOfOutput[index];
	}


}

void CloseOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTD_OUT = PORTD_OUT & ~arrayMapOfOutput[index];
	}
}

void ReverseOutput(int index)
{
    if (statusOutput[index]  == ON)
    {
        CloseOutput(index);
        statusOutput[index] = OFF;
    }
    else
    {
        OpenOutput(index);
        statusOutput[index] = ON;
    }
}
void TestButton(){
  if (key_code[0]) tessst=tessst+1;
  LcdPrintNumS(0,0,tessst);
  LcdPrintNumS(1,0,key_code[0]);
}
void TestOutput(void)
{
	int k;
	for (k=0;k<14 ;k++ )
	{
		OpenOutput(k);
		delay_ms(500);
		CloseOutput(k);
		delay_ms(500);
	}
}

void Phase1_GreenOn()
{
    OpenOutput(0);
}
void Phase1_GreenOff()
{
    CloseOutput(0);
}

void Phase1_YellowOn()
{
    OpenOutput(4);
}
void Phase1_YellowOff()
{
    CloseOutput(4);
}

void Phase1_RedOn()
{
    OpenOutput(6);
}
void Phase1_RedOff()
{
    CloseOutput(6);
}

void Phase2_GreenOn()
{
    OpenOutput(1);
}
void Phase2_GreenOff()
{
    CloseOutput(1);
}

void Phase2_YellowOn()
{
    OpenOutput(5);
}
void Phase2_YellowOff()
{
    CloseOutput(5);
}

void Phase2_RedOn()
{
    OpenOutput(7);
}
void Phase2_RedOff()
{
    CloseOutput(7);
}

unsigned char isMode()
{
    if (key_code[12] == 1)
        return 1;
    else
        return 0;
}

unsigned char isUp()
{
    if (key_code[1] == 1)
        return 1;
    else
        return 0;
}

unsigned char isUpHold()
{
    if (key_code[1] > 10)
        return 1;
    else
        return 0;
}

unsigned char isDown()
{
    if (key_code[9] == 1)
        return 1;
    else
        return 0;
}

unsigned char isDownHold()
{
    if (key_code[9] > 10)
        return 1;
    else
        return 0;
}

unsigned char isReturn()
{
    if (key_code[14] == 1)
        return 1;
    else
        return 0;
}

unsigned char isConfirm()
{
    if (key_code[5] == 1)
        return 1;
    else
        return 0;
}

unsigned char isChange()
{
    if (key_code[6] == 1)
        return 1;
    else
        return 0;
}

unsigned char isWait()
{
    if (key_code[7] == 1)
        return 1;
    else
        return 0;
}

unsigned char isYellow()
{
    if (key_code[11] == 1)
        return 1;
    else
        return 0;
}

unsigned char isCC()
{
    if (key_code[15] == 1)
        return 1;
    else
        return 0;
}

unsigned char isBack()
{
    if (key_code[6] == 1)
        return 1;
    else
        return 0;
}

unsigned char keyNumber()
{
    if (key_code[0]==1) return 1;
    if (key_code[1]==1) return 2;
    if (key_code[2]==1) return 3;
    if (key_code[4]==1) return 4;
    if (key_code[5]==1) return 5;
    if (key_code[6]==1) return 6;
    if (key_code[8]==1) return 7;
    if (key_code[9]==1) return 8;
    if (key_code[10]==1) return 9;
    if (key_code[13]==1) return 0;
    return 10;
}


void AppTrafficLight()
{
    LcdClearS();
    cntOfLight = (cntOfLight + 1)%20;
    if (cntOfLight == 0 && sw) 
        timeOfLight ++;
    switch (statusOfLight)
    {
      case INIT_SYSTEM :
        LcdPrintStringS(0,0,"INIT_SYSTEM");
        statusOfLight=PHASE1_GREEN;
        timeOfLight=0;
        break;
      case PHASE1_GREEN :
        LcdPrintStringS(0,0,"P1 GREEN");
        LcdPrintStringS(1,0,"P2 RED");
        send_led(0b00011000);
        if ((((timeOfLight>=timeOfGreenPhase1)||isChange())&&sw)||(!sw&&isChange())){
          statusOfLight=PHASE1_YELLOW;
          timeOfLight=0;
        }
        if(isBack()){
            statusOfLight=PHASE2_YELLOW;
             timeOfLight=0;
          }
        if (isYellow()) statusOfLight=CONSTANT_YELLOW;
        if (sw){
            send_number_1(timeOfGreenPhase1-timeOfLight,timeOfGreenPhase1-timeOfLight+timeOfYellowPhase1);
            LcdPrintNumS(0,14,timeOfGreenPhase1-timeOfLight);
            //LcdPrintNumS(0,14,timeOfGreenPhase1-timeOfLight);
            LcdPrintNumS(1,14,timeOfGreenPhase1-timeOfLight+timeOfYellowPhase1);
          }
        break;
      case PHASE1_YELLOW :
        LcdPrintStringS(0,0,"P1 YELLOW");
        LcdPrintStringS(1,0,"P2 RED");
        send_led(0b00101000);
        if ((((timeOfLight>=timeOfYellowPhase1)||(isChange()))&&sw)||(!sw&&isChange())){
          statusOfLight=PHASE2_GREEN;
          timeOfLight=0;
          }
        if (isYellow()) statusOfLight=CONSTANT_YELLOW;
        if (sw){
            send_number_1(timeOfYellowPhase1-timeOfLight,-timeOfLight+timeOfYellowPhase1);
            LcdPrintNumS(0,14,timeOfYellowPhase1-timeOfLight);
            LcdPrintNumS(1,14,-timeOfLight+timeOfYellowPhase1);
        }
        if(isBack()){
            statusOfLight=PHASE1_GREEN;
             timeOfLight=0;
          }
        break;
      case PHASE2_GREEN :
        LcdPrintStringS(1,0,"P2 GREEN");
        LcdPrintStringS(0,0,"P1 RED");
        send_led(0b01000010);
        if ((((timeOfLight>=timeOfGreenPhase2)||(isChange()))&&sw)||(!sw&&isChange())){
          statusOfLight=PHASE2_YELLOW;  
          timeOfLight=0;
          }
        if (isYellow()) statusOfLight=CONSTANT_YELLOW;
        if(isBack()){
            statusOfLight=PHASE1_YELLOW;
             timeOfLight=0;
          }
        
        if (sw){
            send_number_1(timeOfGreenPhase2-timeOfLight+timeOfYellowPhase2,timeOfGreenPhase2-timeOfLight);
            LcdPrintNumS(0,14,timeOfGreenPhase2-timeOfLight);
            LcdPrintNumS(1,14,timeOfGreenPhase2-timeOfLight+timeOfYellowPhase2);
          }  
      break;
      case PHASE2_YELLOW :
        LcdPrintStringS(1,0,"P2 YELLOW");
        LcdPrintStringS(0,0,"P1 RED");
        send_led(0b01000100);
        if ((((timeOfLight>=timeOfYellowPhase2)||(isChange()))&&sw)||(!sw&&isChange())){
          statusOfLight=PHASE1_GREEN;
          timeOfLight=0;
          }
        if (isYellow()) statusOfLight=CONSTANT_YELLOW;
        if(isBack()){
            statusOfLight=PHASE2_GREEN;
             timeOfLight=0;
          }
        if (sw){
            send_number_1(timeOfYellowPhase2-timeOfLight,-timeOfLight+timeOfYellowPhase2);
            LcdPrintNumS(0,14,timeOfYellowPhase2-timeOfLight);
            LcdPrintNumS(1,14,-timeOfLight+timeOfYellowPhase2);
          }
        break;
        case CONSTANT_YELLOW:
        LcdPrintStringS(0,0,"P1 Yellow Blink");
        LcdPrintStringS(1,0,"P2 Yellow Blink");
        if (isYellow()) {
            statusOfLight=INIT_SYSTEM;
            timeOfLight=0;
          }
        if (cntOfLight>10){
            send_led(0b00100100);
          }
        else {
            send_led(0b00000000);
          }
        send_number_1(0,0);
        break;
      default:statusOfLight = INIT_SYSTEM;
    }
    if (isWait()) sw=!sw;
    if (isMode()) {
        mode=1;
        statusOfAdj=INIT_SYSTEM;
      }
}
void AdjTrafficLight()
{
  if (mode){
      LcdClearS();
      switch (statusOfAdj){
        case INIT_SYSTEM :
            statusOfAdj=PHASE1_GREEN;
            temp=timeOfGreenPhase1;
            break;
        case PHASE1_GREEN :
            if (isReturn()) mode=0;
            LcdPrintStringS(0,0,"PHASE1_GREEN");
            if(cntOfLight%7) LcdPrintNumS(1,0,temp);
            if (isUp()){
                temp=temp+1;
                if (temp==100) temp=1;
              }
            if (isDown()){
                temp=temp-1;
                if (temp==0) temp=99;
              }
            if (isUpHold()){
                temp=temp+1;
                if (temp==100) temp=1;
              }
            if (isDownHold()){
                temp=temp-1;
                if (temp==0) temp=99;
              }
            if (isConfirm()) {
                timeOfGreenPhase1=temp;
                temp=timeOfYellowPhase1;
                statusOfAdj=PHASE1_YELLOW;
              }
            break;
        
        case PHASE1_YELLOW :
            if (isReturn()) statusOfAdj=PHASE1_GREEN;
            LcdPrintStringS(0,0,"PHASE1_YELLOW");
            if(cntOfLight%7==0) LcdPrintNumS(1,0,temp);
            if (isUp()){
                temp=temp+1;
                if (temp==11) temp=1;
              }
            if (isDown()){
                temp=temp-1;
                if (temp==0) temp=10;
              }
            if (isUpHold()){
                temp=temp+1;
                if (temp==11) temp=1;
              }
            if (isDownHold()){
                temp=temp-1;
                if (temp==0) temp=10;
              }
            if (isConfirm()) {
                timeOfYellowPhase1=temp;
                temp=timeOfGreenPhase2;
                statusOfAdj=PHASE2_GREEN;
              }
            break;
  
        case PHASE2_GREEN :
            if (isReturn()) statusOfAdj=PHASE1_YELLOW;
            LcdPrintStringS(0,0,"PHASE2_GREEN");
            if(cntOfLight%7==0) LcdPrintNumS(1,0,temp);
            if (isUp()){
                temp=temp+1;
                if (temp==100) temp=1;
                
              }
            if (isDown()){
                temp=temp-1;
                if (temp==0) temp=99;
              }
            if (isUpHold()){
                temp=temp+1;
                if (temp==100) temp=1;
              }
            if (isDownHold()){
                temp=temp-1;
                if (temp==0) temp=99;
              }
            if (isConfirm()) {
                timeOfGreenPhase2=temp;
                temp=timeOfYellowPhase2;
                statusOfAdj=PHASE2_YELLOW;
              }
            break;
        
        case PHASE2_YELLOW :
            if (isReturn()) statusOfAdj=PHASE2_GREEN; 
            LcdPrintStringS(0,0,"PHASE2_YELLOW");
            if(cntOfLight%7==0) LcdPrintNumS(1,0,temp);
            if (isUp()){
                temp=temp+1;
                if (temp==11) temp=1;
              }
            if (isDown()){
                temp=temp-1;
                if (temp==0) temp=10;
              }
            if (isUpHold()){
                temp=temp+1;
                if (temp==11) temp=1;
              }
            if (isDownHold()){
                temp=temp-1;
                if (temp==0) temp=10;
              }
            if (isConfirm()) {
                timeOfYellowPhase2=temp;
                temp=timeOfGreenPhase1;
                mode=0;
              }
            break;
        }
    }
}


void AutTrafficLight(){
  LcdPrintNumS(0,0,read_ds1307(0x00));
  LcdPrintNumS(1,0,121);
}

void clock_signal(unsigned char p ){
    OpenOutput(p);
    //delay_ms(5);
    CloseOutput(p);
}
void latch_enable(unsigned char p){
    OpenOutput(p);
    //delay_ms(5);
    CloseOutput(p);
}
void send_number_1(unsigned int data_out1,unsigned int data_out2)
{
    int i;
    unsigned char dig1=binary_pattern[data_out1/10];
    unsigned char dig2=binary_pattern[data_out1%10];
    unsigned char num1=binary_pattern[data_out2/10];
    unsigned char num2=binary_pattern[data_out2%10];
    //unsigned hold;
    for (i=0 ; i<8 ; i++)
    {
        unsigned char DATA_pin = (dig2 >> i) & (0x01);
        if (DATA_pin) OpenOutput(data_led1);
        else CloseOutput(data_led1);
        
        //dig2=binary_pattern[data_out2%10];
        DATA_pin = (num2 >> i) & (0x01);
        if (DATA_pin) OpenOutput(data_led2);
        else CloseOutput(data_led2);
        
        clock_signal(clk_1);
    }
    for (i=0 ; i<8 ; i++)
    {
        unsigned char DATA_pin = (dig1 >> i) & (0x01);
        if (DATA_pin) OpenOutput(data_led1);
        else CloseOutput(data_led1);
        
        //dig1=binary_pattern[data_out2/10];
        DATA_pin = (num1 >> i) & (0x01);
        if (DATA_pin) OpenOutput(data_led2);
        else CloseOutput(data_led2);
        
        clock_signal(clk_1);
    }
    
    /*
    num1=data_out2/10;
    num2=data_out2%10;
    num1=binary_pattern[num1];
    num2=binary_pattern[dig2];
    //unsigned hold;
    for (i=0 ; i<8 ; i++)
    {
        unsigned char DATA_pin = (dig2 >> i) & (0x01);
        if (DATA_pin) OpenOutput(data_led2);
        else CloseOutput(data_led2);
        clock_signal(clk_1);
    }
    for (i=0 ; i<8 ; i++)
    {
        unsigned char DATA_pin = (dig1 >> i) & (0x01);
        if (DATA_pin) OpenOutput(data_led2);
        else CloseOutput(data_led2);
        clock_signal(clk_1);
    }*/
    latch_enable(latch_1); // Data finally submitted
}

void send_led(unsigned int data_out)
{
    int i;
    //unsigned hold;
    for (i=0 ; i<8 ; i++)
    {
        unsigned char DATA_pin = (data_out >> i) & (0x01);
        if (DATA_pin) OpenOutput(data_led);
        else CloseOutput(data_led);
        clock_signal(clk);
    }
     latch_enable(latch);
}



