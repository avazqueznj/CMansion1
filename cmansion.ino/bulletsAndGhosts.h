
#include <Arduboy2.h> 
#include <ArduboyTones.h>
#include "arduboy_util_ale.h"
#include "LinkedList.h"

#include "sprites.h"
#include "scheduler.h"
#include "bouncerClass.h"
#include "arduboy_util_ale.h"


#ifndef BULLETSANDGHOSTS_H
#define BULLETSANDGHOSTS_H


#define BULLET_TYPE 4
#define BULLET_LIFE 5000
#define BULLET_SPEED 3
class bulletClass:public bouncerClass{
public:
    
    bulletClass( int nameParam, int xStart, int yStart, int angleStart )
        :bouncerClass(  nameParam,  xStart,  yStart,  angleStart, NULL, BULLET_LIFE, BULLET_SPEED ){ }
      
    void virtual paint(){
        arduboy.drawLine(currentX,currentY,newCurrentX,newCurrentY);
    }
    
    int virtual type(){
        return BULLET_TYPE;
    };        
    
    bool hit = false;
    void doHit(){
        hit = true;
    }
    
    bool virtual isDone(){
        if(!hit){
            return bouncerClass::isDone();
        }else{
            return true;
        }
    }
    
    virtual ~bulletClass(){
    }
}; 


#define GUNCYCLE_TYPE 5
bool gunCycling = false;
class gunCycleClass:public eventClass{
private:    
    //uses
    const int gunCycleName;
    const unsigned long cycleTime = 0;
    //has
    unsigned long birthTime = 0;    
public:
    
    gunCycleClass( const int nameParam, const int timeToCycleParam):
        gunCycleName{nameParam},
        cycleTime{timeToCycleParam},
        eventClass(){
            // uses
            gunCycling = true;
            //has
            birthTime = millis();            
        }        
            
    int virtual name(){
        return gunCycleName;
    };
    void virtual run(){
        
    }    
    bool virtual isDone(){
        
        if( millis() > ( birthTime + cycleTime ) ){
            gunCycling = false;
            return true;
        }else{
            return false;
        }            
    }
    
    int virtual type(){
        return GUNCYCLE_TYPE;
    };        
        
    unsigned long virtual dueTime(){
        return  birthTime + cycleTime;
    }    
    
    virtual ~gunCycleClass(){
    }
}; 


#define GHOST_TYPE 3
#define GHOST_SLOW_DOWN 4
int activeGhosts = 0;
class ghostClass: public bouncerClass{ 
private:
    
    // uses
    
    // has
    bool isAlive = true;
    int skippedFrames = 0;
    
public:
    ghostClass( 
        const int nameParam, const int xStart, const int yStart, 
        const int angleStart, const byte* spriteMapParam,
        const unsigned long timeToLiveParam, unsigned long targetSpeedParam )
        :bouncerClass(  
            nameParam,  xStart,  yStart,  angleStart, spriteMapParam, 
            timeToLiveParam, targetSpeedParam ){ }

    void virtual run(){
        
        // slow down, skip frames
        skippedFrames++;
        if( skippedFrames < GHOST_SLOW_DOWN ){
            paint();
            return;
        }else{
            skippedFrames = 0;
        }
        
        bouncerClass::run();
    }
        
    int virtual type(){
        return GHOST_TYPE;
    };        
        
    bool virtual isDone(){
        
        if( !isAlive ) return true;
        
        bool itIsDone = ( millis() > ( birthTime + timeToLive ) );
        if( itIsDone ){
            activeGhosts--;
            isAlive = false;
        }
        return( itIsDone );
    }
    
    void kill(){        
        sound.tone(300, 500);  
        isAlive = false;
        activeGhosts--;
    }
};


#endif /* BULLETSANDGHOSTS_H */

