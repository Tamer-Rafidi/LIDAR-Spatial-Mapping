//Tamer Rafidi - rafidit - 400333527

#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "vl53l1x_api.h"
#include "PLL.h"
#include "SysTick.h"
#include "uart.h"
#include "onboardLEDs.h"




#define I2C_MCS_ACK             0x00000008  // Data Acknowledge Enable
#define I2C_MCS_DATACK          0x00000008  // Acknowledge Data
#define I2C_MCS_ADRACK          0x00000004  // Acknowledge Address
#define I2C_MCS_STOP            0x00000004  // Generate STOP
#define I2C_MCS_START           0x00000002  // Generate START
#define I2C_MCS_ERROR           0x00000002  // Error
#define I2C_MCS_RUN             0x00000001  // I2C Master Enable
#define I2C_MCS_BUSY            0x00000001  // I2C Busy
#define I2C_MCR_MFE             0x00000010  // I2C Master Function Enable

#define MAXRETRIES              5           // number of receive attempts before giving up
void I2C_Init(void){
  SYSCTL_RCGCI2C_R |= SYSCTL_RCGCI2C_R0;           													// activate I2C0
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;          												// activate port B
  while((SYSCTL_PRGPIO_R&0x0002) == 0){};																		// ready?
    GPIO_PORTB_AFSEL_R |= 0x0C;           																	// 3) enable alt funct on PB2,3       0b00001100
    GPIO_PORTB_ODR_R |= 0x08;             																	// 4) enable open drain on PB3 only
    GPIO_PORTB_DEN_R |= 0x0C;             																	// 5) enable digital I/O on PB2,3
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFFF00FF)+0x00002200;    //TED
    I2C0_MCR_R = I2C_MCR_MFE;                      													// 9) master function enable
    I2C0_MTPR_R = 0b0000000000000101000000000111011;                       	// 8) configure for 100 kbps clock (added 8 clocks of glitch suppression ~50ns)     
}

//The VL53L1X needs to be reset using XSHUT.  We will use PG0
void PortG_Init(void){
    //Use PortG0
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R6;                // activate clock for Port N
    while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R6) == 0){};    // allow time for clock to stabilize
    GPIO_PORTG_DIR_R &= 0x00;                                        // make PG0 in (HiZ)
		GPIO_PORTG_AFSEL_R &= ~0x01;                                     // disable alt funct on PG0
		GPIO_PORTG_DEN_R |= 0x01;                                        // enable digital I/O on PG0
		GPIO_PORTG_AMSEL_R &= ~0x01;                                     // disable analog functionality on PN0

    return;
}

//XSHUT     This pin is an active-low shutdown input; 
//					the board pulls it up to VDD to enable the sensor by default. 
//					Driving this pin low puts the sensor into hardware standby. This input is not level-shifted.
void VL53L1X_XSHUT(void){
    GPIO_PORTG_DIR_R |= 0x01;                                        // make PG0 out
    GPIO_PORTG_DATA_R &= 0b11111110;                                 //PG0 = 0
    FlashAllLEDs();
    SysTick_Wait10ms(10);
    GPIO_PORTG_DIR_R &= ~0x01;                                       // make PG0 input (HiZ)    
}

void PortJ_Init(void){
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R8;                    // activate clock for Port J
    while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R8) == 0){};           // allow time for clock to stabilize
    GPIO_PORTJ_DIR_R &= ~0x02;                                  // make PJ1 in 
    GPIO_PORTJ_DEN_R |= 0x02;                                   // enable digital I/O on PJ1
    GPIO_PORTJ_PCTL_R &= ~0x000000F0;                           //  configure PJ1 as GPIO 
    GPIO_PORTJ_AMSEL_R &= ~0x02;                                //  disable analog functionality on PJ1        
    GPIO_PORTJ_PUR_R |= 0x02;                                   //  enable weak pull up resistor
}

void PortN1_Init(void){				                            //FROM MS1
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;               //activate the clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};      //allow time for clock to stabilize
	GPIO_PORTN_DIR_R=0b00000010;                            //Make PN1 output, to turn on LED's
	GPIO_PORTN_DEN_R=0b00000010;                            //Enable PN1
	return;
}
void PortM_Init(void){				                    
	                                                     //Use PortM pins for output
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11;				     // activate clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R11) == 0){};	 // allow time for clock to stabilize
	GPIO_PORTM_DIR_R |= 0xFF;        								     // make PN0 out (PN0 built-in LED1)
  GPIO_PORTM_AFSEL_R &= ~0xFF;     								     // disable alt funct on PN0
  GPIO_PORTM_DEN_R |= 0xFF;        								     // enable digital I/O on PN0
  GPIO_PORTM_AMSEL_R &= ~0xFF;     								     // disable analog functionality on PN0		
	return;
}

void spin(int direction){                             //Spin function that decides when and how the stepper motor will spin
    for(int i=0; i<8; i++){
			if(direction == 1) { //Counter-clockwise 
				GPIO_PORTM_DATA_R = 0b00001100;
        SysTick_Wait10ms(1);
        GPIO_PORTM_DATA_R = 0b00000110;
        SysTick_Wait10ms(1);
        GPIO_PORTM_DATA_R = 0b00000011;
        SysTick_Wait10ms(1);
        GPIO_PORTM_DATA_R = 0b00001001;
        SysTick_Wait10ms(1);
      }
      else if(direction == 0) { //Clockwise
        GPIO_PORTM_DATA_R = 0b00001001;
        SysTick_Wait10ms(1);
        GPIO_PORTM_DATA_R = 0b00000011;
        SysTick_Wait10ms(1);
				GPIO_PORTM_DATA_R = 0b00000110;
        SysTick_Wait10ms(1);
				GPIO_PORTM_DATA_R = 0b00001100;
        SysTick_Wait10ms(1);
      }
      else {
        GPIO_PORTH_DATA_R = 0b00000000;
        SysTick_Wait10ms(1);
			}        
    }
        GPIO_PORTM_DATA_R = 0b00000000;
}
//*********************************************************************************************************
//*********************************************************************************************************
//***********					MAIN Function				*****************************************************************
//*********************************************************************************************************
//*********************************************************************************************************
uint16_t	dev = 0x29;			//address of the ToF sensor as an I2C slave peripheral
int status=0;

int main(void) {
  uint8_t byteData, sensorState=0, myByteArray[10] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF} , i=0;
  uint16_t wordData;
  uint16_t Distance;
  uint16_t SignalRate;
  uint16_t AmbientRate;
  uint16_t SpadNum; 
  uint8_t RangeStatus;
  uint8_t dataReady;

	//initialize function
	PLL_Init();	
	SysTick_Init();
	onboardLEDs_Init();
	I2C_Init();
	UART_Init();
	
	// hello world!
	UART_printf("Program Begins\r\n");
	int mynumber = 1;
	sprintf(printf_buffer,"2DX4 Program Studio Code %d\r\n",mynumber);
	UART_printf(printf_buffer);


/* Those basic I2C read functions can be used to check your own I2C functions */
	status = VL53L1X_GetSensorId(dev, &wordData);

	sprintf(printf_buffer,"(Model_ID, Module_Type)=0x%x\r\n",wordData);
	UART_printf(printf_buffer);

	// Booting ToF chip
	while(sensorState==0){
		status = VL53L1X_BootState(dev, &sensorState);
		SysTick_Wait10ms(10);
  }
	FlashAllLEDs();
	UART_printf("ToF Chip Booted!\r\n Please Wait...\r\n");
	
	status = VL53L1X_ClearInterrupt(dev); /* clear interrupt has to be called to enable next interrupt*/
	
  /* This function must to be called to initialize the sensor with the default setting  */
  status = VL53L1X_SensorInit(dev);                                     //ToF API function "SensorInit" which initializes teh sensor 
	Status_Check("SensorInit", status);

		while(1){
			status = VL53L1X_StartRanging(dev);   														//ToF API function is called to begin ranging
			if((GPIO_PORTJ_DATA_R&0b00000010) == 0){                          //Data port for button is cleared to initiate system
				int j = 64;
				Restart:
					for(int i = 0; i < j; i++) {																		//For loop that iterates 64 times to measure 64 distances
						int degrees = 5.625*i;                                        //
						while (dataReady == 0){																				//Holds until ToF sensor is ready to gather data
							
							status = VL53L1X_CheckForDataReady(dev, &dataReady);        //ToF API function "CheckForDataReady" which checks if the ToF sensor is ready to collect data
							VL53L1_WaitMs(dev, 5);
						}
						dataReady = 0;
						status = VL53L1X_GetDistance(dev, &Distance);								//Reads the measured distance values from the ToF sensor
						FlashLED4(1);
						status = VL53L1X_ClearInterrupt(dev); 											//ToF API function "ClearInterrupt" is called to enable the interrupt
						double distance = Distance;
																																				
						sprintf(printf_buffer,"%f\r\n", distance/1000);             //Prints the measured data to UART
						UART_printf(printf_buffer);
						spin(0);                                                    //Calls spin(0) to spin the motor 360 degrees backwards to return back to neutral position
						SysTick_Wait10ms(1);
					}
				for(int i = 0; i<64;i++){                                       //For loop to interate until the motor turns 360 degrees
				spin(1);
			  }
			  VL53L1X_StopRanging(dev);                                       //ToF API function "StopRanging" which stops the ToF sensor from gathering data
			}
		}
}

