#ifndef _GARAGEOMATIC_WEBSERVERPROXY_H_
#define _GARAGEOMATIC_WEBSERVERPROXY_H_

/*======================================================================
FILE:
webserverproxy.h

CREATOR:
Sean Foley

DESCRIPTION:
Proxy class to hide the details of using a web server on your device.

PUBLIC CLASSES AND FUNCTIONS:
WebserverProxy

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

//#include <ESP8266WebServer.h>
#include "extendedwebserver.h"

#include "configuration.h"

#include "garagedoor.h"

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
WebserverProxy

DESCRIPTION:
Proxy class to hide the complexity of using a webserver.  This class
sets up our REST endpoints specific to the needs of the garage-o-matic.

HOW TO USE:
1. Construct with the garage door collection.  This class uses the
number of doors to dynamically build the REST endpoints.
2. Call Begin() to start everything
3. Periodically call Process() to allow the web server handlers to
do their thing.

======================================================================*/
class WebserverProxy
{
    public:

    //=================================================================
    // TYPE DECLARATIONS AND CONSTANTS    
    //=================================================================

    // None.

    //=================================================================
    // CLIENT INTERFACE
    //=================================================================

    WebserverProxy( 
        const Configuration &config,
        GarageDoor::GarageDoorCollection &garageDoors, 
        int port = 80 );

    void Begin();

    void Process();

    protected:

    //=================================================================
    // SUBCLASS INTERFACE   
    //=================================================================

    void handleRoot();
    
    void handleNotFound();

    void handleDoorStatus();

    void handleDoorOpen();

    void handleDoorClose();

    void handleCalibrate();

    void handleCalibrateRunTest();

    bool authenticate();

    // Sets the HTTP response headers to 
    // tell the client to not cache the response
    void setNoCacheHeaders();

    int getDoorNumberFromUri( const String &uri ) const;

    private:

    //=================================================================
    // CUSTOMIZATION INTERFACE    
    //=================================================================

    // None.

    //=================================================================
    // IMPLEMENTATION INTERFACE    
    //=================================================================

    void init();

    //=================================================================
    // DATA MEMBERS    
    //=================================================================

    //ESP8266WebServer _server;
    ExtendedWebServer _server;

    GarageDoor::GarageDoorCollection _garagedoors;

    const Configuration _config;
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

#endif	// #ifendif _GARAGEOMATIC_WEBSERVERPROXY_H_
