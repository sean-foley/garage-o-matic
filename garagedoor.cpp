/*======================================================================
FILE:
garagedoor.cpp

CREATOR:
Sean Foley

GENERAL DESCRIPTION:
Provides functionality to determine if a Garage Door is open/closed
and "switch" functionality to engage the garage door opener.

PUBLIC CLASSES AND FUNCTIONS:
GarageDoor

INITIALIZATION AND SEQUENCING REQUIREMENTS:
The hardware must be setup correctly.  Specifically, this code implementation
expects that the GPIO sensor pin will be connected to a magnetic-reed type
switch to determine if the door is open/closed, and the other GPIO pin
connected to a circuit that will drive a relay to engage the garage
door opener.

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

#include "garagedoor.h"

#include "Arduino.h"

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
C-tor()

DESCRIPTION:
Constructs the object

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
GarageDoor::GarageDoor( int doorSensorGPIO, int relaySensorGPIO )
    : _doorSensorPin( doorSensorGPIO), _doorRelayPin(relaySensorGPIO)
{
    setupGPIO();
}

/*======================================================================
FUNCTION:
GarageDoor()

DESCRIPTION:
Copy constructor

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
GarageDoor::GarageDoor( const GarageDoor &rhs )
{
    // Copy state. Note that since the gpio is setup as part of the
    // original object construction, we shouldn't need to call
    // setupgpio() since it's already set the way we want.
    _doorRelayPin  = rhs._doorRelayPin;
    _doorSensorPin = rhs._doorSensorPin;
}

/*======================================================================
FUNCTION:
operator=()

DESCRIPTION:
Overloaded assignment operator

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void GarageDoor::operator=( const GarageDoor &rhs )
{
    _doorRelayPin  = rhs._doorRelayPin;
    _doorSensorPin = rhs._doorSensorPin;
}

/*======================================================================
FUNCTION:
setupGPIO()

DESCRIPTION:
Configures the GPIO pins for use

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void GarageDoor::setupGPIO()
{
   pinMode( _doorRelayPin, OUTPUT );

   // Note - we are using the internal pullups to keep
   // the value from floating. If your micro doesn't have
   // internal pullout support, make sure to add an external
   // pullup into the sensor circuit.
   pinMode( _doorSensorPin, INPUT_PULLUP );
}

/*======================================================================
FUNCTION:
Status()

DESCRIPTION:
Reads the GPIO pin and returns the corresponding value to indicate
if the door is open/closed

RETURN VALUE:
GarageDoor::DoorStatus enumeration indicating if the door is open/closed

SIDE EFFECTS:
none

======================================================================*/
GarageDoor::DoorStatus GarageDoor::Status() const
{
    int state = digitalRead( _doorSensorPin );

    if ( state == 0 )
    {
        return DoorStatus::CLOSED;
    }
    else
    {
        return DoorStatus::OPEN;
    }
}

/*======================================================================
FUNCTION:
ToggleRelay()

DESCRIPTION:
This method will turn the GPIO pin high for the specified duration.
Since the GPIO is expected to be tied to a relay, this essentially 
closes the relay for that about of time.  You will need to set the 
duration accordingly for whatever min/avg time your garage door 
opener needs to recognize a button press.

RETURN VALUE:
True if successful.

SIDE EFFECTS:
none

======================================================================*/
bool GarageDoor::ToogleRelay(int durationMS )
{
    // Turn the relay on...
    digitalWrite( _doorRelayPin, HIGH );

    unsigned long start = millis();
    unsigned long elapsed = 0;

    // Spin until we have elapsed past the duration.
    do
    {
        elapsed = millis() - start;

        // Be nice and let others do things while we're doing nothing
        // but killing time.
        yield();

    } while ( elapsed <= durationMS );
 
    digitalWrite( _doorRelayPin, LOW );

    return true;
}

/*=====================================================================
// IMPLEMENTATION NOTES
//=====================================================================

None

=====================================================================*/

