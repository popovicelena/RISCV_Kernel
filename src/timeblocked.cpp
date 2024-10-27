#include "../h/timeblocked.hpp"

class Timeblocked;

Timeblocked* Timeblocked::instancePtr = nullptr;

Timeblocked *Timeblocked::getInstance(){

    if(instancePtr == nullptr){
        instancePtr = new Timeblocked();
        return instancePtr;
    }else{
        return instancePtr;
    }
}

