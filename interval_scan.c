/**************************************************
* Penn State University
* On 11/11/2019
* By Geesun Jang
* 
***************************************************/


#include "starter.h"

#define PWM_RANGE 50 // Range of motor speed
#define MOTOR_SPEED 50 //Speed of motor

#define TRIG	25 //us
#define GROUND  9950 //us
#define SET_INTER  10000 //us Trigger Interval = signal trigger(52us) + usleep execute(58us) + set time(us), at this time Trigger interval = 10,000us
#define POINTS 77 // Number of points that we measure distance
#define DELAY_S   2333 // delay for sensor trigger in us
#define DELAY_T_1   62400 // delay for motor in s
#define DELAY_T_2   400000 // delay for motor in s
#define TIMES 1 // Number of times that we measure distance from sensor to one point





int main( void )
{
    volatile struct io_peripherals *io;
    uint32_t   DLevel;
    bool done = false;
    
    io = import_registers();
    
  if (io != NULL)
  {
    /* print where the I/O memory was actually mapped to */
    printf( "mem at 0x%8.8X\n", (unsigned int)io );

    enable_pwm_clock( io );
    
    
    /*  GPIO settings for motor control  */

    io->gpio.GPFSEL1.field.FSEL2 = GPFSEL_ALTERNATE_FUNCTION0;
    io->gpio.GPFSEL1.field.FSEL3 = GPFSEL_ALTERNATE_FUNCTION0;
    
    io->gpio.GPFSEL1.field.FSEL8 = GPFSEL_OUTPUT;    //set GPIO18 as AI1 (left motor direction control)
    io->gpio.GPFSEL1.field.FSEL9 = GPFSEL_OUTPUT;    //set GPIO19 as AI2 (left motor direction control)
    io->gpio.GPFSEL2.field.FSEL2 = GPFSEL_OUTPUT;    //set GPIO22 as BI1 (right motor direction control)
    io->gpio.GPFSEL2.field.FSEL3 = GPFSEL_OUTPUT;    //set GPIO27 as BI2 (right motor direction control)
    
    io->gpio.GPFSEL2.field.FSEL4 = GPFSEL_OUTPUT;
    io->gpio.GPFSEL2.field.FSEL5 = GPFSEL_OUTPUT;
    

    /*  configure the PWM channels  */
    io->pwm.RNG1 = PWM_RANGE;     /* the range value, 32 level steps */
    io->pwm.RNG2 = PWM_RANGE;     /* the range value, 32 level steps */
    io->pwm.CTL.field.MODE1 = 0;  /* PWM mode */
    io->pwm.CTL.field.MODE2 = 0;  /* PWM mode */
    io->pwm.CTL.field.RPTL1 = 1;  /* not using FIFO, but repeat the last byte anyway */
    io->pwm.CTL.field.RPTL2 = 1;  /* not using FIFO, but repeat the last byte anyway */
    io->pwm.CTL.field.SBIT1 = 0;  /* idle low */
    io->pwm.CTL.field.SBIT2 = 0;  /* idle low */
    io->pwm.CTL.field.POLA1 = 0;  /* non-inverted polarity */
    io->pwm.CTL.field.POLA2 = 0;  /* non-inverted polarity */
    io->pwm.CTL.field.USEF1 = 0;  /* do not use FIFO */
    io->pwm.CTL.field.USEF2 = 0;  /* do not use FIFO */
    io->pwm.CTL.field.MSEN1 = 1;  /* use M/S algorithm */
    io->pwm.CTL.field.MSEN2 = 1;  /* use M/S algorithm */
    io->pwm.CTL.field.CLRF1 = 1;  /* clear the FIFO, even though it is not used */
    io->pwm.CTL.field.PWEN1 = 1;  /* enable the PWM channel */
    io->pwm.CTL.field.PWEN2 = 1;  /* enable the PWM channel */    
    
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    
   int _interval_time = SET_INTER - 52 - 58;
    
    
    /* Set initial state*/
    
    GPIO_CLR( &(io->gpio), 18);  
    GPIO_CLR( &(io->gpio), 19);  
    GPIO_CLR( &(io->gpio), 22);  
    GPIO_CLR( &(io->gpio), 23);  
    GPIO_CLR( &(io->gpio), 24);
    GPIO_CLR( &(io->gpio), 25);
    
    int _DLevel = MOTOR_SPEED;
    io->pwm.DAT1 = _DLevel;            
    io->pwm.DAT2 = _DLevel;

    printf("Commands : s(scan) / m(move forward)/ / q(stop) / b(go backward) \n");  // s: start to scan objects while moving motors   m,q,b : motor command
    printf("press command...\n");

      
       
    
      while (!done){
          switch (get_pressed_key())
          {
            
            case 's':   // Scan process automation
                 
                for (int i = 0; i< POINTS; i++){

                    
                    /*--------------------motor instructions----------------------*/

                    GPIO_SET( &(io->gpio), 19);
                    GPIO_SET( &(io->gpio), 23); //Make the car move

                    usleep(DELAY_T_1);

                    GPIO_CLR( &(io->gpio), 19);
                    GPIO_CLR( &(io->gpio), 23); //Make the car stop
                    usleep(DELAY_T_2);
                    
                    
                    
                    /*--------------------sensor instructions----------------------*/
                    // trigger signal : 2 pulse 40kHz burst

                    for(int a = 0; a< TIMES; a++){
                        
                        GPIO_SET( &(io->gpio), 24);
                        for (int j = 0; j<DELAY_S; j++){  //Become 12.5 us -> 40kHz
                            
                        }
                          GPIO_CLR( &(io->gpio), 24);
                          GPIO_SET( &(io->gpio), 26);
                        for (int k = 0; k<DELAY_S; k++){
                            
                        }
                          GPIO_SET( &(io->gpio), 24);
                          GPIO_CLR( &(io->gpio), 26);
                        for (int k = 0; k<DELAY_S; k++){
                            
                        }
                          GPIO_CLR( &(io->gpio), 24);
                          GPIO_SET( &(io->gpio), 26);
                        for (int k = 0; k<DELAY_S; k++){
                            
                        }
                          GPIO_CLR( &(io->gpio), 26);
                          
                          usleep(_interval_time);
                        
                        }
                      
                     
                     //printf("POINTS :%d\n", i);
                     sleep(1.0);
                     
                }     
                    done = true;
              break;
            
            /*  Debug modes for debugging motor process

            case 'm':   // start moving : move forward

                                                
                GPIO_SET( &(io->gpio), 19);
                GPIO_SET( &(io->gpio), 23);
            
                
                a = GPIO_READ(&(io->gpio), 18);
                b = GPIO_READ(&(io->gpio), 19);
                c = GPIO_READ(&(io->gpio), 22);
                d = GPIO_READ(&(io->gpio), 23);
                
                printf("%d %d %d %d\n", a,b,c,d);

              break;

            case 'q':   // stop moving
                
                 GPIO_CLR( &(io->gpio), 18);
                 GPIO_CLR( &(io->gpio), 19);
       
                GPIO_CLR( &(io->gpio), 22);
                GPIO_CLR( &(io->gpio), 23);

                a = GPIO_READ(&(io->gpio), 18);
                b = GPIO_READ(&(io->gpio), 19);
                c = GPIO_READ(&(io->gpio), 22);
                d = GPIO_READ(&(io->gpio), 23);
                printf("%d %d %d %d\n", a,b,c,d);
              
              break;
            
            case 'b': // move backward
                GPIO_SET( &(io->gpio), 18);
                GPIO_CLR( &(io->gpio), 19);
       
                GPIO_SET( &(io->gpio), 22);
                GPIO_CLR( &(io->gpio), 23);
                break;

            */

            default:
              done = true;
              printf("DLevel: %d\n",_DLevel);
              break;
          }
    }

    GPIO_CLR( &(io->gpio), 23);
    GPIO_CLR( &(io->gpio), 24);
}
  else
  {
    ; /* warning message already issued */
  }

  return 0;
}
