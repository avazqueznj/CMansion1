
#include <Arduboy2.h> 
#include <ArduboyTones.h>
#include "arduboy_util_ale.h"
#include "LinkedList.h"
#include "scheduler.h"
#include "sprites.h"

#ifndef BOUNCERCLASS_H
#define BOUNCERCLASS_H

const float PROGMEM radiansPerAngle = 0.01745329252;
class bouncerClass:public eventClass{
public:
    //uses
    int targetName;
    int currentX = 0;
    int currentY = 0;
    int newCurrentX = 0;   
    int newCurrentY = 0;
    int currentAngle = 0;    
    //has 
    bool alive = true;
    unsigned long birthTime = 0;
    unsigned long timeToLive = 0;
    unsigned long targetSpeed = 0;    
    const byte*   spriteMap;
public:
    
    bouncerClass( int nameParam, int xStart, int yStart, int angleStart, const byte* spriteMapParam,
            unsigned long timeToLiveParam, unsigned long targetSpeedParam ):eventClass(){
        //uses
        currentX = xStart;
        currentY = yStart;
        currentAngle = angleStart;                  
        targetName = nameParam;
        timeToLive = timeToLiveParam;
        targetSpeed = targetSpeedParam;
        spriteMap = spriteMapParam;
        
        //has
        birthTime = millis();
    }
    
    void virtual bouceHorizontally(){
        if( currentAngle < 180 ){
            int dif = currentAngle - 90;
            currentAngle = 90 - dif;
        }else{
            int dif = 270 - currentAngle;
            currentAngle = 270 + dif;
            }          
    }
    
    void virtual bounceVertically(){
        if( currentAngle < 90 ){
            int dif = 90 - currentAngle;
            currentAngle = 270 + dif;
        }else{
            int dif = currentAngle - 90;
            currentAngle = 270 - dif;
        }        
    }
    
    void virtual render(){                
                                                              
        // bounce side left
        if( currentX < 0 || currentX >128 ){            
            bouceHorizontally();
        }
            
        // bounce top & bottom
        if( currentY < 0 || currentY >64 ){
            bounceVertically();            
        }
                
        float angleRadians = currentAngle * radiansPerAngle;                                  
        
        // x component        
        newCurrentX = roundf( currentX + (targetSpeed * cos(angleRadians)));   
        newCurrentY = roundf( currentY - (targetSpeed * sin(angleRadians)));                       
 
        // draw player
        paint();        
        
        currentX = newCurrentX;
        currentY = newCurrentY;   
                
    }
    
    void virtual paint(){
        // draw player
        sprites.drawOverwrite( currentX, currentY, spriteMap, 0);         
    }
            
    int virtual name(){
        return targetName;
    };
    void virtual run(){
        render();
    }    
    bool virtual isDone(){
        return ( millis() > ( birthTime + timeToLive ) );
    }
    unsigned long virtual dueTime(){
        return 0;
    }    
    
    void virtual collision(eventClass * collisionSource){        
    }    
    
    virtual ~bouncerClass(){
    }
    
    
}; 


#endif /* BOUNCERCLASS_H */

