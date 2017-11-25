/*======================================================================
FILE:
ledhelper.cpp

CREATOR:
Sean Foley

GENERAL DESCRIPTION:
Helper class that does some simple animation for an LED hooked
up to a micro GPIO pin.

PUBLIC CLASSES AND FUNCTIONS:
LedHelper

INITIALIZATION AND SEQUENCING REQUIREMENTS:
The hardware must be properly wired.

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
// INCLUDES AND VARIABLE DEFINITIONS
//======================================================================

//----------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------

#include "ledhelper.h"

//----------------------------------------------------------------------
// Type Declarations
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Global Constant Definitions
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Global Data Definitions
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Static Variable Definitions 
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Function Prototypes
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Required Libraries
//----------------------------------------------------------------------

// None. (Where supported these should be in the form
// of C++ pragmas).

//======================================================================
// FUNCTION IMPLEMENTATIONS
//======================================================================

/*======================================================================
FUNCTION:
TurnOn()

DESCRIPTION:
Turns the LED on

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void LedHelper::TurnOn() 
{
    uint8_t value = HIGH;

    if ( true == _invertLogicLevel )
    {
        value = !value;
    }

    digitalWrite( _gpio, value );
}

/*======================================================================
FUNCTION:
TurnOff()

DESCRIPTION:
Turns the LED off.

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void LedHelper::TurnOff() 
{
    uint8_t value = LOW;

    if ( true == _invertLogicLevel )
    {
        value = !value;
    }

    digitalWrite( _gpio, value );
}

/*======================================================================
FUNCTION:
Toggle()

DESCRIPTION:
This will either turn the LED on/off based on it's previous state

RETURN VALUE:
The resulting on or off state after the toggle

SIDE EFFECTS:
none

======================================================================*/
int LedHelper::Toggle() 
{
    // Read the current state and flip it
    int state = !digitalRead( _gpio );

    digitalWrite( _gpio, state );

    return state;
}

/*======================================================================
FUNCTION:
Flash()

DESCRIPTION:
Flashes the LED. The LED will be ON for durationMS/2, and off 
for the other half of the duration.

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void LedHelper::Flash( const int durationMS ) 
{
    unsigned int waitcycle = durationMS / 2;

    // Flip/flop for 2 cycles
    for( int i = 0; i < 2; i++ )
    {
        unsigned long start = millis();
        Toggle();

        while ( true )
        {
            unsigned long stop = millis();

            unsigned long elapsed = stop - start;

            if ( elapsed >= waitcycle ) { break; }

            yield();
        }
    }
}

/*=====================================================================
// IMPLEMENTATION NOTES
//=====================================================================

None

=====================================================================*/

