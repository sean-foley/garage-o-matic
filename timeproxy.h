#ifndef _GARAGEOMATIC_TIMEPROXY_H_
#define _GARAGEOMATIC_TIMEPROXY_H_

/*======================================================================
FILE:
timeproxy.h

CREATOR:
Sean Foley

DESCRIPTION:
Uses Network Time Protocol (NTP) to seed the timing routines.  This code is 
based on the TimeNTP_ESP8266WIFI example (there is no copyright/author info
in the file to give credit to - thanks and you rock!)

PUBLIC CLASSES AND FUNCTIONS:
TimeProxy

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

#include <WiFiUdp.h>
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
TimeProxy

DESCRIPTION:
Uses Network Time Protocol (NTP) to seed the timing routines and provides
some helper methods to abstract the use of TimeLib calls.

HOW TO USE:
1. Construct with the ntp server to use. 
2. Call Begin() to initialze and start everything
3. Call the helper methods to get the time.

======================================================================*/
class TimeProxy
{
    public:

    //=================================================================
    // TYPE DECLARATIONS AND CONSTANTS    
    //=================================================================

    // Some timezone preset offsets
    enum TimeZones
    {
        UTC = 0,
        EST = -5,
        EDT = -4,
        PST = -8,
        PDT = -7
    };

    //=================================================================
    // CLIENT INTERFACE
    //=================================================================

    TimeProxy( const String &ntpServer,
               unsigned int syncIntervalS = 300 );

    void Begin();

    time_t GetCurrentTimeUTC();

    String GetTimeStringUTC();

    protected:

    //=================================================================
    // SUBCLASS INTERFACE   
    //=================================================================

    // None.

    private:

    static time_t getNtpTime();

    static void sendNTPpacket( IPAddress &address );

    //=================================================================
    // CUSTOMIZATION INTERFACE    
    //=================================================================

    // None.

    //=================================================================
    // IMPLEMENTATION INTERFACE    
    //=================================================================

    // No copying. Leaving the implementation undefined to cause a link
    // error
    TimeProxy( const TimeProxy &rhs );

    //=================================================================
    // DATA MEMBERS    
    //=================================================================

    static int _timezone;

    static String _ntpServer;

    // How often should we resync time (seconds) with our 
    // NTP time source?
    unsigned int _syncIntervalS;

    static WiFiUDP _udp;
    unsigned int _localport = 8888;
        
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

#endif	// #ifendif _GARAGEOMATIC_TIMEPROXY_H_
