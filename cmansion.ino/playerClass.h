#include <Arduboy2.h> 
#include <ArduboyTones.h>
#include "arduboy_util_ale.h"
#include "LinkedList.h"

#include "sprites.h"
#include "scheduler.h"
#include "bouncerClass.h"
#include "arduboy_util_ale.h"



#ifndef PLAYERCLASS_H
#define PLAYERCLASS_H


#define PLAYER_TYPE 2
#define STARTX 64;
#define STARTY 56;
#define ANGLE_CHANGE_SPEED 4;
#define SIZE_GUN 4
#define GUN_CYCLE_DELAY 200
class playerClass:public eventClass{
public: 
        
    //uses
    int playerName;
    
    //has -  defaulted
    int playerX = STARTX; 
    int playerY = STARTY;
    int aimAngle = 90;
    int realAimlAngle = 0;
    int aimX = 0;              
    int aimY = 0;    
public:         
        
    playerClass( int nameParam ):eventClass(){
        
        playerName = nameParam;        
    }    
    
    void drawAimer(){
             
        int aimBaseX = playerX + 5;  // middle
        int aimBasey = playerY ;  // gap on top     
        
        if( arduboy.pressed(DOWN_BUTTON) && (aimAngle > 0) ) {
            aimAngle = aimAngle - ANGLE_CHANGE_SPEED;                    
        }

        if( arduboy.pressed(UP_BUTTON ) && (aimAngle < 180) ) {
            aimAngle = aimAngle + ANGLE_CHANGE_SPEED;                                
        }    
          
        // figure angle
        realAimlAngle = 180 - aimAngle;    
        
        // get angle in radians          
        float angleRadians = realAimlAngle * radiansPerAngle;     
        
        // x component
        aimX = roundf( aimBaseX + (SIZE_GUN * cos(angleRadians)));               
        aimY = roundf( aimBasey - (SIZE_GUN * sin(angleRadians)));        
        
        arduboy.drawLine( aimBaseX, aimBasey, aimX, aimY );
        
    }
    
    void drawPlayer(){
        
        if( arduboy.pressed(LEFT_BUTTON) ) {
            playerX--;
            if(playerX < 0){
                playerX = (WIDTH -8);
            }
        }

        if( arduboy.pressed(RIGHT_BUTTON )  ) {
            playerX++;   
            if(playerX > (WIDTH -8)){
                playerX = 0;
            }
        }        
        
        // draw player
        sprites.drawOverwrite( playerX, playerY, playerSprite, 0); 
        
        // shoot!! event
        if( arduboy.pressed(B_BUTTON) && (!gunCycling) && (freeMemory() > 100) ){
                
            sound.tone(700, 100);            
            
            pk++;
            bulletClass* bullet =  new bulletClass( pk, getRealAimX(), getRealAimY(), getRealAimlAngle() );    
            scheduler.Append( bullet );  
            
            gunCycleClass* cycleDelay = new gunCycleClass( pk, GUN_CYCLE_DELAY );
            scheduler.Append( cycleDelay );  
        }           
    }    
    
    int virtual name(){
        return playerName;
    };
    void virtual run(){

        drawPlayer();    
        drawAimer();
               
    }    
    bool virtual isDone(){
        return false;
    }
    unsigned long virtual dueTime(){
        return 0;
    }
    int virtual type(){
        return PLAYER_TYPE;
    };        
    
    
    int getRealAimlAngle(){
        return realAimlAngle;
    }
    int getRealAimX(){
        return aimX;
    }
    int getRealAimY(){
        return aimY;
    }
        
    virtual ~playerClass(){                                
    }
    
};

#endif /* PLAYERCLASS_H */

