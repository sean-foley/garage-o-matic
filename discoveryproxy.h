#ifndef _GARAGEOMATIC_DISCOVERYPROXY_H_
#define _GARAGEOMATIC_DISCOVERYPROXY_H_

/*======================================================================
FILE:
discoveryproxy.h

CREATOR:
Sean Foley

DESCRIPTION:
Network and service discovery support.

PUBLIC CLASSES AND FUNCTIONS:
DiscoveryProxy

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
DiscoveryProxy

DESCRIPTION:
Hide all the network/service discovery stuff behind this proxy
class.

HOW TO USE:
1. Construct the object with the hostanme 
2. Call Begin() to start everything

If your OS supports mDNS, you should now be able to resolve the 
ip address of your device by typing "ping hostname.local" and
you should be able to access the REST enpoints in a browser
by adding .local to the hostname.

======================================================================*/
class DiscoveryProxy
{
    public:

    //=================================================================
    // TYPE DECLARATIONS AND CONSTANTS    
    //=================================================================

    // None.

    //=================================================================
    // CLIENT INTERFACE
    //=================================================================

    DiscoveryProxy( const String &hostname );

    // Call this to start network discovery
    bool Begin();

    void AddService( const String &service, const String &protocol, int port );

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

    // No copying
    DiscoveryProxy( const DiscoveryProxy &rhs );

    //=================================================================
    // DATA MEMBERS    
    //=================================================================

    String _hostname;

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

#endif	// #ifendif _GARAGEOMATIC_DISCOVERYPROXY_H_
