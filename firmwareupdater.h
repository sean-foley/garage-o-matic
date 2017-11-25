#ifndef _GARAGEOMATIC_FIRMWAREUPDATER_H_
#define _GARAGEOMATIC_FIRMWAREUPDATER_H_

/*======================================================================
FILE:
firmwareupdater.h

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
// INCLUDES AND PUBLIC DATA DECLARATIONS
//======================================================================

//----------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------

#include "WString.h"

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
FirmwareUpdater

DESCRIPTION:
This class provides over-the-air firmware update functionality. 

HOW TO USE:
1. Call the appropriate c-tor
2. Call Begin() to initialize/start things up
3. Periodically call Process() so all the internal handling/magic happens

======================================================================*/
class FirmwareUpdater
{
    public:

    //=================================================================
    // TYPE DECLARATIONS AND CONSTANTS    
    //=================================================================

    // None.

    //=================================================================
    // CLIENT INTERFACE
    //=================================================================

    FirmwareUpdater( const String &password, int port = 8266 );

    void Begin();

    void Process();

    protected:

    //=================================================================
    // SUBCLASS INTERFACE   
    //=================================================================

    // Callbacks
    void handleUpdateStart();
    void handleUpdateComplete();

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

    String _password;
    int _port;

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

#endif	// #ifendif _GARAGEOMATIC_FIRMWAREUPDATER_H_
