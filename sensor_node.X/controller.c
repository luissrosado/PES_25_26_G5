/*
 * File:   controller.c
 * Author: Fracisco Jesus
 *
 * Created on 10 de Outubro de 2025, 16:13
 */


#include "sensor_node.h"

void controller(uint16_t target, uint16_t current_read){
    uint16_t dcyc = 0;
    
    if(target <= 0){                                // 'negative' current
        PWM_init(1, 0);                             // disable all PWM
        PWM_init(2, 0);                             // until error
        PWM_init(3, 0);                             // is solved
    }else if(target <= 100){                        // 22R branch 
        if(current_read <= target){                 // increases current until  
            dcyc += 1;                              // target reached
            PWM_updDcyc(1, dcyc);
        }else if(current_read > target){            // decreases current until  
            dcyc -= 1;                              // target reached
            PWM_updDcyc(1, dcyc);
        }
    }else if((target > 100) && (target <= 200)){    // 10R branch
        if(current_read < target){               
            dcyc += 1;
            PWM_updDcyc(2, dcyc);
        }else if(current_read > target){            // decreases current until  
            dcyc -= 1;                              // target reached
            PWM_updDcyc(2, dcyc);
        }
    }else if((target > 200) && (target <= 400)){   // 4.7R branch 
        if(current_read < target){
            dcyc += 1;
            PWM_updDcyc(3, dcyc);
        }else if(current_read > target){            // decreases current until  
            dcyc -= 1;                              // target reached
            PWM_updDcyc(3, dcyc);
        }
    }else{
        target = 410;                               // maximum current
        
        if(current_read < target){
            dcyc += 1;
            PWM_updDcyc(3, dcyc);
        }else if(current_read > target){            // decreases current until  
            PWM_init(1, 0);                         // disable all PWM
            PWM_init(2, 0);                         // until error
            PWM_init(3, 0);                         // is solved
        }
    }
}