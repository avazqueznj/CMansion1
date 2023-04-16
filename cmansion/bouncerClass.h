
#include <Arduboy2.h> 
#include <ArduboyTones.h>
#include "arduboy_util_ale.h"
#include "LinkedList.h"
#include "scheduler.h"
#include "sprites.h"

#ifndef BOUNCERCLASS_H
#define BOUNCERCLASS_H

const float PROGMEM radiansPerAngle = 0.01745329252;
class bouncerClass:public screenEventClass{
public:
    //uses
    const int targetName;
    
    //has     
    bool alive = true;
    unsigned long birthTime = 0;
    unsigned long timeToLive = 0;
    unsigned long targetSpeed = 0;    
    const byte*   spriteMap;    
    
public:
    
    bouncerClass( const int nameParam, const int xStart, const int yStart, const int angleStart, 
                const byte* spriteMapParam, const unsigned long timeToLiveParam, const unsigned long targetSpeedParam ): 
        targetName{nameParam},
        timeToLive{timeToLiveParam},
        targetSpeed{targetSpeedParam},
        spriteMap{spriteMapParam},
        screenEventClass(xStart, yStart, angleStart)
        {
            //has
            birthTime = millis();
        }
    
    void virtual bouceHorizontally(){
        if( angle < 180 ){
            int dif = angle - 90;
            angle = 90 - dif;
        }else{
            int dif = 270 - angle;
            angle = 270 + dif;
            }          
    }
    
    void virtual bounceVertically(){
        if( angle < 90 ){
            int dif = 90 - angle;
            angle = 270 + dif;
        }else{
            int dif = angle - 90;
            angle = 270 - dif;
        }        
    }
    
    void virtual render(){                
                                                                             
        // bounce side left
        if( startX < 0 || startX >128 ){            
            bouceHorizontally();
        }
            
        // bounce top & bottom
        if( startY < 0 || startY >64 ){
            bounceVertically();            
        }
                        
        // x component        
        endX = roundf( startX + (targetSpeed * cos(angle * radiansPerAngle)));   
        endY = roundf( startY - (targetSpeed * sin(angle * radiansPerAngle)));                       
 
        // draw player
        paint();        
        
        startX = endX;
        startY = endY;   
                
    }
    
    void virtual paint(){
        // draw player
        sprites.drawOverwrite( startX, startY, spriteMap, 0);         
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

