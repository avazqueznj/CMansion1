
/***********************************************************************
 
    Chato's Mansion
    Copyright (C) 2019  Alejandro Vazquez

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
  
 
 * Compile
 ./arduino-cli.exe compile -b arduino:avr:leonardo ./cmansion.ino
 arduino-cli.exe upload -p COM4 -b arduino:avr:leonardo cmansion.ino
 
 ***********************************************************************/

#include <Arduboy2.h> 
#include <ArduboyTones.h>
#include "arduboy_util_ale.h"
#include "LinkedList.h"

#include "sprites.h"
#include "scheduler.h"
#include "bouncerClass.h"
#include "bulletsAndGhosts.h"
#include "playerClass.h"


int playerScore = 0;
playerClass* player1 = NULL;
void setup() {
 
    Serial.begin(9600);    
    arduboy.begin();
    arduboy.setFrameRate(50);
        
    // start a player
    pk++;
    player1 = new playerClass(pk);
    scheduler.Append( player1 );                      
}


bool spashScreen = true;
void showSplash(){
       
    sprites.drawOverwrite( 0, 0, splashScreen,0);
    arduboy.setCursor( 0, 0);
    arduboy.print( "press [A]\n" );    
    arduboy.print( "Up/Dwn aim\n" );      
    arduboy.print( "v1.1n" );    
    
    arduboy.display();
    
    while(!arduboy.pressed(A_BUTTON)){ 
        delay( 100 );
    }       
    spashScreen = false;
}


void loop() {
    
    // misc    
    if (!(arduboy.nextFrame())) return;    
    
    // setup
    arduboy.clear();   
    
    if( spashScreen ){
        showSplash();
    };
    
    sprites.drawOverwrite( 0, 0, background,0);        
    arduboy.setCursor( 110, 0 );           
    arduboy.print( playerScore );   
    debug();            
    
    // execute timeline    
    if( scheduler.moveToStart())
        while( true ){                  
            
            eventClass* currentEvent = scheduler.getCurrent();
            
            if( !currentEvent->isDone() && ( millis() > currentEvent->dueTime() ) ){   
                currentEvent->run();
            }

            // expired ?
            if( currentEvent->isDone() ){
                if(!scheduler.hasNext()){
                    scheduler.Delete(currentEvent);
                    delete currentEvent;
                    break;
                }else{
                    scheduler.Delete(currentEvent);
                    delete currentEvent;
                    continue;
                }
            }
                
            // next
            if( scheduler.hasNext() ){
                scheduler.next();
            }else{
                break;
            }            
        }                        

    makeGhosts();    
    doCollisions();    
    arduboy.display();    
}

int totalGhostsForLevel = 1;    
void makeGhosts(){
    
    
    totalGhostsForLevel++;
            
    if( (activeGhosts < totalGhostsForLevel) && (freeMemory() > 100) ){
        
        activeGhosts++;        
        pk++;
        ghostClass* gohst = new ghostClass( 
                pk, 
                random( 10, 120),  //x
                random( 10, 20),  //y
                random( 0, 360),  //angle
                ghostSprite, 
                GHOST_LIFE, 
                GHOST_SPEED ); 
        scheduler.Append( gohst );                      
    }    
}

void doCollisions(){

    // TODO: redo all these ....
    const LinkedList< eventClass* > tempList;    
    if( scheduler.moveToStart()) while( true ){                              
        tempList.Append( scheduler.getCurrent() );
        if( scheduler.hasNext() ){
            scheduler.next();
        }else{
            break;
        }            
    }

    // Check collisions
    if( tempList.moveToStart()) while( true ){                                             
        const eventClass* collisionSource = tempList.getCurrent();
        
        // BULLET COLLISION
        if( collisionSource->type() == BULLET_TYPE ){              
            const Point bulletPoint = Point( ((bulletClass*)collisionSource)->startX, ((bulletClass*)collisionSource)->startY );
            
            // scan possible targets            
            if( scheduler.moveToStart()) while( true ){                    
                const eventClass* collisionTarget = scheduler.getCurrent();
                const Rect targetRect = Rect( ((screenEventClass*)collisionTarget)->startX, ((screenEventClass*)collisionTarget)->startY, 8,8 );                                                           
                
                // .. with GHOST
                if(collisionTarget->type() == GHOST_TYPE){                                        
                    if(arduboy.collide(bulletPoint, targetRect )){
                        ((ghostClass*)collisionTarget)->kill();
                        ((bulletClass*)collisionSource)->doHit();
                        playerScore++;
                    }
                }

                // .. with PLAYER
                if(collisionTarget->type() == PLAYER_TYPE){                    
                    if(arduboy.collide(bulletPoint, targetRect )){                        
                        ((bulletClass*)collisionSource)->doHit();                           
                         playerScore =  playerScore - 2;                              
                         sound.tone(5000, 100);                                                                           
                    }
                }
                
                if( scheduler.hasNext() ){
                    scheduler.next();
                }else{
                    break;
                }            
            }         
        } 
          
        // GHOST COLLISION
        if( collisionSource->type() == GHOST_TYPE ){    
            
            // .. with PLAYER
            const Rect ghostRect = Rect( ((ghostClass*)collisionSource)->startX, ((ghostClass*)collisionSource)->startY, 8, 8 );            
            const Rect playerRect = Rect( player1->startX, player1->startY,8,8 );                                                                       
            if(arduboy.collide(ghostRect, playerRect )){
                doGameOver();
                return;
            }                
        }       
        
        if( tempList.hasNext() ){
            tempList.next();
        }else{
            break;
        }            
    }    
}


void doGameOver(){
    
    // clear 
    while( scheduler.moveToStart() ){
        delete( scheduler.getCurrent() );
        scheduler.DeleteCurrent();
    }

    arduboy.clear();
    arduboy.setCursor( 0, 30 );                              
    arduboy.print( "[[ GAME OVER CHATO!]]\n" );
    arduboy.print( "   final score "  );
    arduboy.print( playerScore );                                
    arduboy.print( "\n"  );                
    arduboy.print( "      press [A]"  );    

    activeGhosts = 0;
    playerScore = 0;                 
    gunCycling = false;                

    arduboy.display();
    sound.tones(song1);  

    pk++;
    player1 = new playerClass(pk);
    scheduler.Append( player1 );  

    while(!arduboy.pressed(A_BUTTON)) delay( 100 );

    spashScreen = true;
    delay( 500 );
    
    return;    
}


bool freeze = false;
void debug(){
 
//    arduboy.setCursor( 0,0 );
//    arduboy.print(freeMemory()); 
}


