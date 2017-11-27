#ifndef _GARAGEOMATIC_GARAGEDOOR_H_
#define _GARAGEOMATIC_GARAGEDOOR_H_

/*======================================================================
FILE:
garagedoor.h

CREATOR:
Sean Foley

DESCRIPTION:
Provides functionality to determine if a Garage Door is open/closed
and "switch" functionality to engage the garage door opener.

PUBLIC CLASSES AND FUNCTIONS:
GarageDoor

Copyright (C) 2017 Sean Foley  All Rights Reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted.  Enjoy.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

======================================================================*/

//======================================================================
// INCLUDES AND PUBLIC DATA DECLARATIONS
//======================================================================

//----------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------

// Hack to deal with a collision between the Arduino.h definitions and
// what is in the std (stlvector.h) definition
// https://github.com/platformio/platformio-core/issues/646
#undef min
#undef max

#include <vector>

//----------------------------------------------------------------------
// Type Declarations
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Global Constant Declarations
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Global Data Declarations
//----------------------------------------------------------------------

// None.

//======================================================================
// WARNINGS!!!
//======================================================================

// None.

//======================================================================
// FUNCTION DECLARATIONS
//======================================================================

// None.

//=====================================================================
// EXCEPTION CLASS DEFINITIONS
//=====================================================================

// None.

//======================================================================
// CLASS DEFINITIONS
//======================================================================

/*======================================================================
CLASS:
GarageDoor

DESCRIPTION:
The GarageDoor class provides functionality to determine if the door
is open/closed, along with engaging the garage door opener.

HOW TO USE:
Construct the class with the GPIO pins that are used for the door
open/closed sensor, and the pin used to toggle the relay to engage
the garage door opener.

After that, call Status() to determine if the door is open/closed
and ToggleRelay() to engage the garage door opener.

======================================================================*/
class GarageDoor
{
    public:

    //=================================================================
    // TYPE DECLARATIONS AND CONSTANTS    
    //=================================================================

    enum DoorStatus
    {
        CLOSED = 0,
        OPEN = 1
    };

    typedef std::vector<GarageDoor> GarageDoorCollection;

    typedef std::vector<DoorStatus> DoorStatusCollection;

    //=================================================================
    // CLIENT INTERFACE
    //=================================================================

    GarageDoor( int doorSensorGPIO, int relaySensorGPIO );

    GarageDoor( const GarageDoor &rhs );

    void operator=( const GarageDoor &rhs );

    // Checks the sensor to determine if the door is
    // open or closed. 
    DoorStatus Status() const;

    // This will toggle the relay on for the given timeframe
    // then the relay will toogle off
    bool ToogleRelay(int durationMS = 500);

    protected:

    //=================================================================
    // SUBCLASS INTERFACE   
    //=================================================================

    // None.

    private:

    //=================================================================
    // CUSTOMIZATION INTERFACE    
    //=================================================================

    // None.

    //=================================================================
    // IMPLEMENTATION INTERFACE    
    //=================================================================

    void setupGPIO();

    //=================================================================
    // DATA MEMBERS    
    //=================================================================

    int _doorSensorPin;
    int _doorRelayPin;
};

//======================================================================
// INLINE FUNCTION DEFINITIONS
//======================================================================

// None.


/*======================================================================
// DOCUMENTATION
========================================================================

None.

======================================================================*/

#endif	// #ifendif _GARAGEOMATIC_GARAGEDOOR_H_
