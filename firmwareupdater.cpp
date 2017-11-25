/*======================================================================
FILE:
firmwareupdater.cpp

CREATOR:
Sean Foley

DESCRIPTION:
This is a proxy class that hides the ArduinoOTA over-the-air
firmware update functionality

PUBLIC CLASSES AND FUNCTIONS:
FirmwareUpdater

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

#include "firmwareupdater.h"

#include <ArduinoOTA.h>

// std::bind support
#include <functional>

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
FirmwareUpdater()

DESCRIPTION:
C-tor

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
FirmwareUpdater::FirmwareUpdater(const String &password, int port)
    :_password(password), _port(port)
{

}

/*======================================================================
FUNCTION:
Begin()

DESCRIPTION:
This method starts the ArduinoOTA functionality.

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void FirmwareUpdater::Begin()
{
    ArduinoOTA.setHostname( "garage-o-matic" );

    ArduinoOTA.setPort( _port );

    if ( _password.length() > 0 )
    {
        ArduinoOTA.setPassword( _password.c_str() );
    }

    // Set the callbacks
    ArduinoOTA.onStart( std::bind( &FirmwareUpdater::handleUpdateStart, this ) );
    ArduinoOTA.onEnd( std::bind( &FirmwareUpdater::handleUpdateComplete, this ) );

    Serial.printf("FirmwareUpdater: starting OTA update support on port %d\n", _port);

    ArduinoOTA.begin();
}

/*======================================================================
FUNCTION:
Process()

DESCRIPTION:
Wrapper for the ArduinoOTA.handle().  You must call this periodically
otherwise the device will not respond to OTA updates

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void FirmwareUpdater::Process()
{
    ArduinoOTA.handle();
}

/*======================================================================
FUNCTION:
handleUpdateStart()

DESCRIPTION:
Callback when the OTA process starts

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void FirmwareUpdater::handleUpdateStart()
{
    Serial.println( "FirmwareUpdater: firmware OTA update starting" );
}

/*======================================================================
FUNCTION:
handleUpdateComplete()

DESCRIPTION:
Callback that is called when the OTA process is complete

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void FirmwareUpdater::handleUpdateComplete()
{
    Serial.println( "FirmwareUpdater: update complete, restarting device" );
    
    // Sometimes the device doesn't seem to automagically restart
    // so maybe we need to help it along???
    //ESP.reset();
}

/*=====================================================================
// IMPLEMENTATION NOTES
//=====================================================================

None

=====================================================================*/

