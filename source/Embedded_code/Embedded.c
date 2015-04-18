/**********************************************************
CS101 : LIBRARY HELPMATE BOT
Team ID : 104
Team Members : (I) Saurabh Garg
(II)Shaurya Arya
(III)Pratik Brahma
(IV)Abhin Shah
Filename : Embedded C Code
Reference : E-Yantra Fire Bird Bot Tutorials
**********************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h> /**included to support power function*/
#include "lcd.c"
void port_init(); /**Function to initialize PORTS void timer5_init(); /**timer to initialize velocity control */
void velocity(unsigned char, unsigned char); /** Function for velocity control*/
unsigned char ADC_Conversion(unsigned char);
 unsigned char ADC_Value;
 unsigned char flag = 0;
 unsigned char Left_white_line = 0;
 unsigned char Center_white_line = 0;
 unsigned char Right_white_line = 0;
 unsigned char sharp1,sharp2,sharp3, distance, adc_reading;
 unsigned int value1,value2,value3;
unsigned char data; /**to store received data from UDR1*/
unsigned char i1=0;
/**Function to configure LCD port*/
void lcd_port_config (void)
{
DDRC = DDRC | 0xF7; /**all the LCD pin's direction set as output*/
PORTC = PORTC & 0x80; /** all the LCD pins are set to logic 0 except PORTC 7*/
}
/**Configure PORTB 5 pin for servo motor 1 operation*/
void servo1_pin_config (void)
{
DDRB = DDRB | 0x20; /**making PORTB 5 pin output*/
PORTB = PORTB | 0x20; /**setting PORTB 5 pin to logic 1*/
}
/**ADC pin configuration*/
void adc_pin_config (void)
{
DDRF = 0x00;
PORTF = 0x00;
DDRK = 0x00;
PORTK = 0x00;
}
/**Function to configure ports to enable robot's motion*/
void motion_pin_config (void)
{
DDRA = DDRA | 0x0F;
PORTA = PORTA & 0xF0;
DDRL = DDRL | 0x18; /**Setting PL3 and PL4 pins as output for PWM generation*/
PORTL = PORTL | 0x18; /**PL3 and PL4 pins are for velocity control using PWM.*/
}
/**Function to Initialize PORTS*/
void port_init()
{
servo1_pin_config(); /**Configure PORTB 5 pin for servo motor 1 operation*/
lcd_port_config();
 adc_pin_config();
  motion_pin_config();
}
/** Timer 5 initialized in PWM mode for velocity control*/
/** Prescale:256*/
/** PWM 8bit fast, TOP=0x00FF /** Timer Frequency:225.000Hz*/
void timer5_init()
{
TCCR5B = 0x00;
/**Stop with*/
TCNT5H = 0xFF;
/**Counter higher 8-bit value to which OCR5xH value is compared with*/
TCNT5L = 0x01;
/**Counter lower 8-bit value to which OCR5xH value is compared*/
OCR5AH = 0x00;
/**Output compare register high value for Left Motor*/
OCR5AL = 0xFF;
/**Output compare register low value for Left Motor*/
OCR5BH = 0x00;
/**Output compare register high value for Right Motor*/
OCR5BL = 0xFF;
/**Output compare register low value for Right Motor*/
OCR5CH = 0x00;
/**Output compare register high value for Motor C1*/
OCR5CL = 0xFF;
/**Output compare register low value for Motor C1*/
TCCR5A = 0xA9;  /**{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}*/
                /**For Overriding normal port functionality to OCRnA outputs.*/
                /**{WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/
TCCR5B = 0x0B;  /**WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)*/
}
/**TIMER1 initialization in 10 bit fast PWM mode*/
/**prescale:256*/
/** WGM: 7) PWM 10bit fast, TOP=0x03FF*/
/** actual value: 52.25Hz*/
void timer1_init(void)
{
TCCR1B = 0x00; /**stop*/
TCNT1H = 0xFC; /**Counter high value to which OCR1xH value is to be compared with*/
TCNT1L = 0x01;
/**Counter low value to which OCR1xH value is to be compared with*/
OCR1AH = 0x03;
/**Output compare Register high value for servo 1*/
OCR1AL = 0xFF;
/**Output Compare Register low Value For servo 1*/
ICR1H = 0x03;
ICR1L = 0xFF;
TCCR1A = 0xAB; /**{COM1A1=1, COM1A0=0; COM1B1=1, COM1B0=0; COM1C1=1 COM1C0=0} For Overriding normal port functionality to OCRnA outputs.*/
               /**{WGM11=1, WGM10=1} Along With WGM12 in TCCR1B for Selecting FAST PWM Mode */
TCCR1C = 0x00;
TCCR1B = 0x0C; /**WGM12=1; CS12=1, CS11=0, CS10=0 (Prescaler=256)*/
}
void adc_init()
{
ADCSRA = 0x00;
ADCSRB = 0x00; /**MUX5 = 0*/
ADMUX = 0x20;  /**Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000*/
ACSR = 0x80;
ADCSRA = 0x86; /**ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0*/
}
/**Function For ADC Conversion*/
unsigned char ADC_Conversion(unsigned char Ch)
{
unsigned char a; if(Ch>7)
{
ADCSRB = 0x08;
}
Ch = Ch & 0x07;
ADMUX= 0x20| Ch;
ADCSRA = ADCSRA | 0x40;  /**Set start conversion bit*/
while((ADCSRA&0x10)==0); /**Wait for conversion to complete*/
a=ADCH;
ADCSRA = ADCSRA|0x10;    /**clear ADIF (ADC Interrupt Flag) by writing 1 to it*/
ADCSRB = 0x00; return a;
}
/**Function To Print Sesor Values At Desired Row And Coloumn Location on LCD*/
void print_sensor(char row, char coloumn,unsigned char channel)
{
ADC_Value = ADC_Conversion(channel); lcd_print(row, coloumn, ADC_Value, 3);
}
/** This Function calculates the actual distance in millimeters(mm) from the input*/
/** analog value of Sharp Sensor.*/
unsigned int Sharp_GP2D12_estimation(unsigned char adc_reading)
{
float distance;
unsigned int distanceInt;
 distance = (int)(10.00*(2799.6*(1.00/(pow(adc_reading,1.1546)))));
distanceInt = (int)distance;
 if(distanceInt>800)
{
distanceInt=800;
}
return distanceInt;
}
/**Function to rotate Servo by specified angle*/
void servo_1(unsigned char degrees)
{
float PositionPanServo = 0;
PositionPanServo = ((float)degrees / 3) + 35.0;
OCR1AH = 0x00;
OCR1AL = (unsigned char) PositionPanServo;
}
/**Function for velocity control*/
void velocity (unsigned char left_motor, unsigned char right_motor) {
OCR5AL = (unsigned char)left_motor;
OCR5BL = (unsigned char)right_motor;
}
/**Function used for setting motor's direction*/
void motion_set (unsigned char Direction)
{
unsigned char PortARestore = 0;
Direction &= 0x0F; /** removing upper nibbel for the protection*/
PortARestore = PORTA; /** reading the PORTA original status*/
PortARestore &= 0xF0; /** making lower direction nibbel to 0*/
PortARestore |= Direction; /** adding lower nibbel for forward command and restoring the PORTA status*/
PORTA = PortARestore; /** executing the command*/
}
void forward (void) /**both wheels forward*/
{
motion_set(0x06);
}
void back (void) /**both wheels backward*/
{
motion_set(0x09);
}
void left (void) /**Left wheel backward, Right wheel forward*/
{
motion_set(0x05);
}
void right (void) /**Left wheel forward, Right wheel backward*/
{
motion_set(0x0A);
}
void soft_left (void) /**Left wheel stationary, Right wheel forward*/
{
motion_set(0x04);
}
void soft_right (void) /**Left wheel forward, Right wheel is stationary*/
{
motion_set(0x02);
}
void soft_left_2 (void) /**Left wheel backward, right wheel stationary*/
{
motion_set(0x01);
}
void soft_right_2 (void) /**Left wheel stationary, Right wheel backward*/
{
motion_set(0x08);
}
void stop (void) /**hard stop*/
{
motion_set(0x00);
}
/**Function To Initialize UART0*/
/** desired baud rate:9600*/
/** actual baud rate:9600 (error 0.0%)*/
/** char size: 8 bit /** parity: Disabled*/
void uart0_init(void)
{
UCSR0B = 0x00; /**disable while setting baud rate*/
UCSR0A = 0x00;
UCSR0C = 0x06;
UBRR0L = 0x5F; /**set baud rate lo*/
UBRR0H = 0x00; /**set baud rate hi*/
UCSR0B = 0x98;
}
/**Function To Initialize all The Devices*/
void init_devices (void)
{
cli(); /**Clears the global interrupts*/
port_init();
uart0_init();
timer1_init();
adc_init();
 timer5_init();
sei(); /**Enables the global interrupts*/
}
/**To assign direction on check 1 according to data received from XBEE*/
void direction1(char a)
{
if(a==0x38)
{
forward();
_delay_ms(100);
}
if(a==0x34)
{ left();
_delay_ms(600);
}
if(a==0x36)
{ right();
_delay_ms(600);
}
}
/** To assign direction on check 3 according to data received from XBEE*/
 void direction2(char a)
{
if(a==0x38)
{
forward();
_delay_ms(100);
}
if(a==0x36)
{ left();
_delay_ms(800);
}
if(a==0x34)
{ right();
_delay_ms(800);
}
}
/** ISR for receive complete interrupt*/
SIGNAL(SIG_USART0_RECV)
{
data = UDR0; /**making copy of data from UDR0 in 'data' variable*/
UDR0 = data; /**echo data back to PC*/
}
/**Main Function*/
 int main()
{
init_devices();
 lcd_set_4bit();
 lcd_init();
int counter =1,flag2=0,flag3=0,start=0,decide=0; /**local variables for checks*/
while(1)
{
if(start==0)
{ _delay_ms(2000);
for (i1 = 0; i1 <90; i1++) /**To initiate servo motors to hold a book*/
{ servo_1(i1); _delay_ms(100); start=1;
}
}
Left_white_line = ADC_Conversion(3); /**Getting data of Left WL Sensor*/
Center_white_line = ADC_Conversion(2); /**Getting data of Center WL Sensor*/
Right_white_line = ADC_Conversion(1); /**Getting data of Right WL*/
flag=0;
print_sensor(1,1,3);
/**Prints value of White Line Sensor1*/
print_sensor(1,5,2);
/**Prints Value of White Line Sensor2*/
print_sensor(1,9,1);
/**Prints Value of White Line Sensor3*/
if((flag2==0)&&Center_white_line>0x28 && Left_white_line<0x28 && Right_white_line<0x28 && (flag==0)) /** to move forward when only middle sensor is on black line*/
{
flag=1; forward();
_delay_ms(0.2);
decide=0;
}
if((flag2==0)&&(Left_white_line<0x28 && Right_white_line>0x28 && Center_white_line>0x28 ) && (flag==0)) /** to turn right when middle and right sensor are on black line*/
{
flag=1;
soft_right();
_delay_ms(40);
decide=2;
}
if((flag2==0)&&(Right_white_line<0x28 && Center_white_line>0x28 && Left_white_line>0x28) && (flag==0)) /** to turn letf when middle and left sensor are on black line*/
{
flag=1;
soft_left();
_delay_ms(40);
decide=1;
}
if((flag2==0)&&(Left_white_line<0x28 && Right_white_line>0x28 && Center_white_line<0x28 ) && (flag==0)) /** to turn right even when only right sensor is on black line*/
{
flag=1;
soft_right();
_delay_ms(40);
decide=2;
}
if((flag2==0)&&(Right_white_line<0x28 && Center_white_line<0x28 && Left_white_line>0x28) && (flag==0)) /** to turn left even when only left sensor is on black line*/
{
flag=1;
soft_left();
_delay_ms(40);
decide=1;
}
if((flag2==0) && Center_white_line<0x28 && Left_white_line<0x28 && Right_white_line<0x28 && (flag==0)) /** to go back when each sensor somehow goes out of black line*/
{
flag=1; stop();
_delay_ms(100);
if(decide==1)
{
soft_left();
_delay_ms(1);
}
else if(decide==2)
{
soft_right();
_delay_ms(1);
}
else if(decide==0)
{stop();
_delay_ms(100);
back();
_delay_ms(1);
}
decide=0;

}
if((flag2==0)&&(Center_white_line>0x28 && Left_white_line>0x28 && Right_white_line>0x28) && (flag==0)&&(counter==1)) /** to take turn on check 1 according to data receuved from xbee*/
{
flag=1; forward(); _delay_ms(400); direction1(data); counter=2;
forward();
_delay_ms(300);
decide=0;
}
if((flag2==0)&&(Center_white_line>0x28 && Left_white_line>0x28 && Right_white_line>0x28) && (flag==0)&&(counter==2)) /** to turn 180 degrees and release book at end of path on check 2*/
{ flag=1;
decide=0;
forward();
_delay_ms(300);
left();
_delay_ms(1400);
counter=3;

stop();
_delay_ms(1000);
for (i1 = 0; i1 <90; i1++)
{
servo_1(i1);
_delay_ms(100);
}
forward();
_delay_ms(300);
}
if((flag2==0)&&(Center_white_line>0x28 && Left_white_line>0x28 && Right_white_line>0x28) && (flag==0)&&(counter==3)) /** to take exaclty opposite turn as that of check 1*/
{ flag=1;
decide=0;
forward(); _delay_ms(400); direction2(data);
counter=4;
forward();
_delay_ms(300);
decide=0;
}
if((flag2==0)&&(Center_white_line>0x28 && Left_white_line>0x28 && Right_white_line>0x28) && (flag==0)&&(counter==4)) /** to turn 180 degrees and then to stop for next 10 sec*/
{
flag=1; forward(); _delay_ms(50); left();
_delay_ms(1400);
stop();
_delay_ms(10000);
decide=0;
}
/** Obstacle detection*/
sharp1 = ADC_Conversion(11); /**Stores the Analog value of front sharp connected to ADC channel 11 into variable "sharp"*/
sharp2 = ADC_Conversion(13);
sharp3= ADC_Conversion(12);
value1 = Sharp_GP2D12_estimation(sharp1); /**Stores Distance calsulated in a variable "value".*/
value3 = Sharp_GP2D12_estimation(sharp3); /**Stores Distance calsulated in a variable "value".*/
value2 = Sharp_GP2D12_estimation(sharp2); /**Stores Distance calsulated in a variable "value".*/
if(value1<=100 && value1>=80&&flag2==0)
{ flag2=1; left();
_delay_ms(500);
Center_white_line=0x00 ; Left_white_line=0x00; Right_white_line=0x00;
}
lcd_print(2,2,value1,3); /**Prints Value Of Distanc in MM measured by Sharp Sensor.*/
lcd_print(2,6,value2,3); /**Prints Value Of Distanc in MM measured by Sharp Sensor.*/
lcd_print(2,10,value3,3); /**Prints Value Of Distanc in MM measured by Sharp Sensor.*/
if((flag2==1) && (Center_white_line>0x28 || Left_white_line>0x28 || Right_white_line>0x28)) /** come back on path after all three sensors comes on black line*/
{
forward(); _delay_ms(350);
left();
_delay_ms(500); flag2=0;
}
if(value2<=200 && value2>=100 && value1>=100 && flag2==1)
{
forward();
_delay_ms(1);
}
if(value2>=200 &&value1>=100 && flag2==1)
{
soft_right(); _delay_ms(1); flag3=1;
}
if((value3<200||value1<100)&& value2>200 && flag2==1 && flag3==1)
{
soft_left();
_delay_ms(1);
}
}
}
