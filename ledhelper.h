#ifndef _GARAGEOMATIC_LEDHELPER_H_
#define _GARAGEOMATIC_LEDHELPER_H_

/*======================================================================
FILE:
ledhelper.h

CREATOR:
Sean Foley

DESCRIPTION:
Helper class that does some simple animation for an LED hooked 
up to a micro GPIO pin.

PUBLIC CLASSES AND FUNCTIONS:
LedHelper

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

#include <stdint.h>

#include <Arduino.h>

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
LedHelper

DESCRIPTION:
Helper class that does some simple animation for an LED hooked
up to a micro GPIO pin.

HOW TO USE:
Construct the object with the GPIO pin and logic level.   

Then call the various methods to control the Led.

======================================================================*/
class LedHelper
{
    public:

    //=================================================================
    // TYPE DECLARATIONS AND CONSTANTS    
    //=================================================================

    // None.

    //=================================================================
    // CLIENT INTERFACE
    //=================================================================

    LedHelper( const uint8_t gpio, bool invertLogicLevel = true );

    void TurnOn();
    void TurnOff();

    // Toggle will flip/flop the LED - if it is off, it turns on,
    // and if it is on, it turns off.
    int Toggle();

    // This will flash the LED for the given duration in milliseconds
    void Flash( const int durationMS = 500 );

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

    // None.

    //=================================================================
    // DATA MEMBERS    
    //=================================================================

    uint8_t _gpio;
    bool _invertLogicLevel;

};

//======================================================================
// INLINE FUNCTION DEFINITIONS
//======================================================================

// C-tor
inline LedHelper::LedHelper( const uint8_t gpio, bool invertLogicLevel ) 
    : _gpio( gpio ), _invertLogicLevel( invertLogicLevel )
{ 
    pinMode( _gpio, OUTPUT );
}

/*======================================================================
// DOCUMENTATION
========================================================================

None.

======================================================================*/

#endif	// #ifendif _GARAGEOMATIC_LEDHELPER_H_
