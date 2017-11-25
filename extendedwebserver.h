#ifndef _GARAGEOMATIC_EXTENDEDWEBSERVER_H_
#define _GARAGEOMATIC_EXTENDEDWEBSERVER_H_

/*======================================================================
FILE:
extendedwebserver.h

CREATOR:
Sean Foley

DESCRIPTION:
The ESP8266WebServer has been extended to support digest authentication. 
However, when I pulled the esp8266 core down from git, there were a lot
of side effects that broke things.  The ExtendedWebServer subclasses the
ESP8266WebServer and implements the exact code that the ESP8266WebServer 
used to implement digest authentication. A big thanks to the original 
authors - author details and changesets in the links below.

Once the new esp8266 core becomes an official release, it should be 
trivial to remove this code and use the release code.

Implements digest auth that was added to ESP8266WebServer
https://github.com/esp8266/Arduino/commit/eebc5ec593a8c739f8da2612a0e25ff4dab982fb#diff-b78105e1729cc548e062483fec020a9b

Implements the constant time string compare from WString
https://github.com/esp8266/Arduino/commit/03f1a540caa5af96a686db81fc3a21b9936dd4a7#diff-3d1eaec7ee8f9cdadc75a401477867a0


PUBLIC CLASSES AND FUNCTIONS:
ExtendedWebServer

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

#include <ESP8266WebServer.h>
#include <WString.h>

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
ExtendedWebServer

DESCRIPTION:
Subclasses the ESP8266WebServer to implement digest authentication.
Digest authentication, while not strong (MD5), at least doesn't
pass the credentials in the clear.

HOW TO USE:
This class is a drop-in replacement for the ESP8266WebServer, so 
follow its usage pattern.  When you want to authenticate:
1. call authenticate()
2. if that fails, call requestAuthentication() with the auth method you
want to use.

======================================================================*/
class ExtendedWebServer : public ESP8266WebServer
{
    public:

    //=================================================================
    // TYPE DECLARATIONS AND CONSTANTS    
    //=================================================================

    enum HTTPAuthMethod { BASIC_AUTH, DIGEST_AUTH };

    //=================================================================
    // CLIENT INTERFACE
    //=================================================================

    ExtendedWebServer( IPAddress addr, int port ) 
        : ESP8266WebServer( addr, port ) {}

    ExtendedWebServer( int port) : ESP8266WebServer( port ){ }

    void requestAuthentication( HTTPAuthMethod mode = BASIC_AUTH, 
                                const char* realm = NULL, 
                                const String& authFailMsg = String( "" ) );

    bool authenticate( const char * username, const char * password );

    protected:

    //=================================================================
    // SUBCLASS INTERFACE   
    //=================================================================

    String _getRandomHexString();
    
    // for extracting Auth parameters
    String _exractParam( String& authReq, const String& param, const char delimit = '"' );

    // Hack... this was added to WString, code ported and implemented here
    unsigned char equalsConstantTime( const String &lhs, const String &rhs );

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

    String _snonce;  // Store noance and opaque for future comparison
    String _sopaque;
    String _srealm;  // Store the Auth realm between Calls

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

#endif	// #ifendif _GARAGEOMATIC_EXTENDEDWEBSERVER_H_
