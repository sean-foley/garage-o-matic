/*======================================================================
FILE:
webserverproxy.h

CREATOR:
Sean Foley

DESCRIPTION:
Proxy class to hide the details of using a web server on your device.

PUBLIC CLASSES AND FUNCTIONS:
WebserverProxy

INITIALIZATION AND SEQUENCING REQUIREMENTS:
The device must be on the network and have a valid 
ip address assigned before using this class/object.

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

#include "webserverproxy.h"

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

// None

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
WebserverProxy()

DESCRIPTION:
C-tor

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
WebserverProxy::WebserverProxy( 
    const Configuration &config,
    GarageDoor::GarageDoorCollection &garageDoors,
    int port)
    : _config(config), _garagedoors( garageDoors), _server( port )
{
    init();
}

/*======================================================================
FUNCTION:
Begin()

DESCRIPTION:
Starts the webserver so it listens for requests

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void WebserverProxy::Begin()
{
    _server.begin();
}

/*======================================================================
FUNCTION:
Process()

DESCRIPTION:
Let's the web server do its thing. If you don't call this periodically
(like in a tight loop) the web server will be unresponsive to clients.

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void WebserverProxy::Process()
{
    // Pump the server so it can do things
    _server.handleClient();

    // Just in case the caller is calling this in a tight loop
    yield();
}

/*======================================================================
FUNCTION:
init()

DESCRIPTION:
Sets up all the callback handlers for the webserver.  This is also where
the REST endpoints are dynamically created for each door in the collection.

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void WebserverProxy::init()
{
    _server.on( "/", std::bind( &WebserverProxy::handleRoot, this ) );
    _server.on( "/", std::bind( &WebserverProxy::handleNotFound, this ) );

    // Dynamically build our REST endpoints based on the 
    // number of garage doors we are supporting
    for ( int i = 0; i < _garagedoors.size(); i++ )
    {
        String urlStatus = "/garage/door/status/";
        String urlOpen   = "/garage/door/command/open/";
        String urlClose  = "/garage/door/command/close/";
        String urlCalibrate = "/garage/door/calibrate/";
        String urlCalibrateRun = "/garage/door/calibrate/test/";

        urlStatus    += i;
        urlOpen      += i;
        urlClose     += i;
        urlCalibrate += i;
        urlCalibrateRun += i;

        _server.on( urlStatus.c_str(),    std::bind( &WebserverProxy::handleDoorStatus, this ) );
        _server.on( urlOpen.c_str(),      std::bind( &WebserverProxy::handleDoorOpen,   this ) );
        _server.on( urlClose.c_str(),     std::bind( &WebserverProxy::handleDoorClose,  this ) );
        _server.on( urlCalibrate.c_str(), std::bind( &WebserverProxy::handleCalibrate,  this ) );

        _server.on( urlCalibrateRun.c_str(), std::bind( &WebserverProxy::handleCalibrateRunTest, this ) );
    }
}

/*======================================================================
FUNCTION:
handleCalibrateRunTest()

DESCRIPTION:
This method will time how long it takes for the garage door to close
from the fully opened position.  Since there is only one sensor being
used to detect if the door is open/closed, we don't really know if the door
is partially open, etc.  The idea was that once we have a calibration 
value, we should always expect the door to close within the calibration 
timeframe (+/- some tolerance).  Therefore this calibration value
can be later incorportated into door status monitoring, etc.

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void WebserverProxy::handleCalibrateRunTest()
{
    if ( authenticate() == false )
    {
        return;
    }

    int doornum = getDoorNumberFromUri( _server.uri() );

    GarageDoor door = _garagedoors[doornum];

    GarageDoor::DoorStatus status = door.Status();

    int httpcode = 0;

    String message;
    String redirectUrl;

    Serial.printf( "Running calibration test for garage door %d\n", doornum );

    switch ( status )
    {
        case GarageDoor::DoorStatus::OPEN:
            // Run the test
        {
            unsigned long start = millis();

            door.ToogleRelay();

            while ( true )
            {
                ESP.wdtFeed();

                if ( door.Status() == GarageDoor::CLOSED )
                {
                    unsigned long stop = millis();

                    unsigned long elapsed = stop - start;

                    message += "door takes ";
                    message += elapsed;
                    message += " ms to close.";

                    Serial.printf( "Garage door %d takes %lu ms to close\n", doornum, elapsed );

                    httpcode = 200;
                    break;
                }

                yield();

            }    
        }

            break;

        case GarageDoor::DoorStatus::CLOSED:

            message = "garage door must be completely open to calibrate.";
            redirectUrl = "/garage/door/calibrate/";
            redirectUrl += doornum;
            _server.sendHeader( "Location", redirectUrl );
            httpcode = 302;
            break;

        default:
            message = "cannot determine if door is closed or open. check sensor(s)";

            httpcode = 500;
            break;
    }

    setNoCacheHeaders();

    _server.sendHeader( "Content-Length", String( message.length() ) );
    _server.send( httpcode, "text/plain", message );
}

/*======================================================================
FUNCTION:
handleCalibrate()

DESCRIPTION:
This handler provides an HTML browser interface to for the calibration
tests.

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void WebserverProxy::handleCalibrate()
{
    if ( authenticate() == false )
    {
        return;
    }

    int doornum = getDoorNumberFromUri( _server.uri() );

    GarageDoor door = _garagedoors[doornum];

    GarageDoor::DoorStatus status = door.Status();

    String message, href;

    message = "<html><head><title>Garage Door Calibration</title></head><body>";
    message += "<p>Use the calibration to measure how long it takes the garage door to close from a ";
    message += "fully opended position.</p>";

    int httpcode = 0;
   
    switch ( status )
    {
        case GarageDoor::DoorStatus::OPEN:
            message +="<p>When you click on the CALIBRATE link below, the garage door will ";
            message += "close. Please do not press any other wireless door remotes or the open/close ";
            message += "garage door button while the test is in progress.</p>";
            
            href = "<a href=\"";
            href += "/garage/door/calibrate/test/";
            href += doornum;
            href += "\">CALIBRATE</a>";
            
            message += href;
            httpcode = 200;

            break;

        case GarageDoor::DoorStatus::CLOSED:
            
            message += "<p>Cannot run test because the door is closed. Please fully open ";
            message += "the garage door, then reload this page.</p>";

            // Let's use the conflict code because the caller
            // is requesting us to close an already closed door
            httpcode = 200;
            break;

        default:
            message = "cannot determine if door is closed or open. check sensor(s)";

            httpcode = 500;
            break;
    }

    message += "</body></html>";

    setNoCacheHeaders();

    _server.sendHeader( "Content-Length", String( message.length() ) );
    _server.send( httpcode, "text/html", message );

}

/*======================================================================
FUNCTION:
handleDoorOpen()

DESCRIPTION:
Provides a REST endpoint to handle opening the garage door.  If the 
door is closed, this will Toggle() the door, which will invoke the 
garage door opener.

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void WebserverProxy::handleDoorOpen()
{
    if ( authenticate() == false )
    {
        return;
    }

    int doornum = getDoorNumberFromUri( _server.uri() );

    GarageDoor door = _garagedoors[doornum];

    GarageDoor::DoorStatus status = door.Status();
    
    String message;
    int httpcode = 0;

    switch ( status )
    {
        case GarageDoor::DoorStatus::OPEN:
            message = "door already open";
            httpcode = 409;

            break;

        case GarageDoor::DoorStatus::CLOSED:
            // The door is closed, so toggle the relay to open it
            door.ToogleRelay();

            message = "opening";

            // Let's use the conflict code because the caller
            // is requesting us to close an already closed door
            httpcode = 409;
            break;

        default:
            message = "cannot determine if door is closed or open. check sensor(s)";

            httpcode = 500;
            break;
    }

    setNoCacheHeaders();

    _server.sendHeader( "Content-Length", String( message.length() ) );
    _server.send( httpcode, "text/plain", message );
}

/*======================================================================
FUNCTION:
handleDoorClose()

DESCRIPTION:
Provides a REST endpoint to handle closing the garage door.  If the
door is open, this will Toggle() the door, which will invoke the
garage door opener.

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void WebserverProxy::handleDoorClose()
{
    if ( authenticate() == false )
    {
        return;
    }

    int doornum = getDoorNumberFromUri( _server.uri() );

    GarageDoor door = _garagedoors[doornum];

    GarageDoor::DoorStatus status = door.Status();
    
    String message;
    int httpcode = 0;

    switch ( status )
    {
        case GarageDoor::DoorStatus::OPEN:
            // The door is open, so toggle the relay to close it
            door.ToogleRelay();

            // TODO: Should we wait for the door to close?

            message = "closing";

            // Use a 200 ok for the REST result
            httpcode = 200; 

            break;

        case GarageDoor::DoorStatus::CLOSED:
            message = "door already closed";

            // Let's use the conflict code because the caller
            // is requesting us to close an already closed door
            httpcode = 409;
            break;

        default:
            message = "cannot determine if door is closed or open. check sensor(s)";

            httpcode = 500;
            break;
    }

    setNoCacheHeaders();

    _server.sendHeader( "Content-Length", String( message.length() ) );
    _server.send( httpcode, "text/plain", message );

}

/*======================================================================
FUNCTION:
handleDoorStatus()

DESCRIPTION:
Provides a REST endpoint to handle returning the garage door status
(opened or closed).

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void WebserverProxy::handleDoorStatus()
{
    if ( authenticate() == false )
    {
        return;
    }

    int doornum = getDoorNumberFromUri( _server.uri() );

    GarageDoor::DoorStatus status = _garagedoors[doornum].Status();

    String message;

    switch ( status )
    {
        case GarageDoor::DoorStatus::OPEN:
            message = "open";
            break;

        case GarageDoor::DoorStatus::CLOSED:
            message = "closed";
            break;

        default:
            message = "unknown";
            break;
    }

    setNoCacheHeaders();
    
    _server.sendHeader( "Content-Length", String( message.length() ) );
    _server.send( 404, "text/plain", message );
}

/*======================================================================
FUNCTION:
handleNotFound()

DESCRIPTION:
This is the 404 not found handler for any URI that we don't know 
what to do with.  Source code from the web server example.

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void WebserverProxy::handleNotFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += _server.uri();
    message += "\nMethod: ";
    message += ( _server.method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += _server.args();
    message += "\n";

    for ( uint8_t i = 0; i < _server.args(); i++ )
    {
        message += " " + _server.argName( i ) + ": " + _server.arg( i ) + "\n";
    }

    setNoCacheHeaders();
    _server.sendHeader( "Content-Length", String( message.length() ) );
    _server.send( 404, "text/plain", message );
}

/*======================================================================
FUNCTION:
handleRoot()

DESCRIPTION:
This handler handles root ("/") requests.

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void WebserverProxy::handleRoot()
{
    if ( authenticate() == false )
    {
        return;
    }

    String message = "Hi there from the Garage-o-Matic!";

    setNoCacheHeaders();
    _server.sendHeader( "Content-Length", String( message.length() ) );
    _server.send( 200, "text/plain", message );
}

/*======================================================================
FUNCTION:
authenticate()

DESCRIPTION:
Call this at the begining of a request handler if you want to require
authentication.

RETURN VALUE:
true if authenticated

SIDE EFFECTS:
none

======================================================================*/
bool WebserverProxy::authenticate()
{
    const char *REALM = "garage-o-matic";

    bool result = _server.authenticate( _config.GetDeviceUsername().c_str(), _config.GetDevicePassword().c_str() );

    if ( false == result )
    {
        _server.requestAuthentication( ExtendedWebServer::DIGEST_AUTH, REALM, "Authentication Failed" );
    }

    return result;
}

/*======================================================================
FUNCTION:
setNoCacheHeaders()

DESCRIPTION:
Helper to set HTTP headers to direct the client to not cache
the response.

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void WebserverProxy::setNoCacheHeaders()
{
    _server.sendHeader( "Cache-Control", "no-cache, no-store, must-revalidate" );
    _server.sendHeader( "Pragma", "no-cache" );
    _server.sendHeader( "Expires", "-1" );
}

/*======================================================================
FUNCTION:
getDoorNumberFromUri()

DESCRIPTION:
Helper that extracts the integer value from the uri.  We will use
this value later to offset into our door collection.

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
int WebserverProxy::getDoorNumberFromUri( const String &uri ) const
{
    int lastslash = uri.lastIndexOf( '/' );

    // Add 1 to offset from the last / character
    String uridoor = uri.substring( lastslash + 1 );

    // Note - we don't need to worry about trailing junk at 
    // this point because we didn't map a correspoding url.  
    // So if someone passes some extra characters, it will 
    // result in the not found handler
    int doornum = uridoor.toInt();

    return doornum;
}

/*=====================================================================
// IMPLEMENTATION NOTES
//=====================================================================

None

=====================================================================*/


