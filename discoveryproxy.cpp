/*======================================================================
FILE:
discoveryproxy.cpp

CREATOR:
Sean Foley

SERVICES:
mDNS support

DESCRIPTION:
Network and service discovery support.

PUBLIC CLASSES AND FUNCTIONS:
DiscoveryProxy

INITIALIZATION AND SEQUENCING REQUIREMENTS:
A valid network connection must exist

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

#include "discoveryproxy.h"

// Multicast DNS
#include <ESP8266mDNS.h>

// For SSDP support
//#include <ESP8266SSDP.h>

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
DiscoveryProxy()

DESCRIPTION:
C-tor

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
DiscoveryProxy::DiscoveryProxy(const String &hostname) : _hostname(hostname)
{

}

/*======================================================================
FUNCTION:
Begin()

DESCRIPTION:
Starts the network discovery processes (mDNS, etc.)

RETURN VALUE:
true if started successfully.

SIDE EFFECTS:
none

======================================================================*/
bool DiscoveryProxy::Begin()
{
    bool mdnsResult = MDNS.begin( _hostname.c_str() );

    //
    // TODO - Add SSDP support.
    //

    return mdnsResult;
}

void DiscoveryProxy::AddService( const String &service, const String &protocol, int port )
{
    MDNS.addService( service, protocol, port );
}

/*=====================================================================
// IMPLEMENTATION NOTES
//=====================================================================

None

=====================================================================*/

