
#ifndef SCHEDULER_H
#define SCHEDULER_H

#define EVENT_TYPE 1
class eventClass{
public:    
    int virtual name(){
        return 0;
    };

    int virtual type(){
        return EVENT_TYPE;
    };
    
    void virtual run(){};
    
    bool virtual isDone(){
        return false;
    };
    
    unsigned long virtual dueTime(){
        return 0;
    };    
    
    bool operator == (const eventClass &other){
       if( name() == other.name()  )
         return true;
       else
         return false;
    }    
    
    // base destructor
    virtual ~eventClass(){
        
    }
};


class screenEventClass: public eventClass{
public:
    int startX;
    int startY;
    int endX;
    int endY;   
    int angle;
    screenEventClass( int Xparam, int YParam, int angleParam ):
        startX{ Xparam },startY{ YParam },        
        endX{ Xparam },endY{ YParam }, angle{angleParam},
        eventClass(){
    }
};

int pk = 0;
LinkedList< eventClass* > scheduler;

#endif /* SCHEDULER_H */

