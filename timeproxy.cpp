/*======================================================================
FILE:
timeproxy.cpp

CREATOR:
Sean Foley

GENERAL DESCRIPTION:
Uses Network Time Protocol (NTP) to seed the timing routines.  This code is
based on the TimeNTP_ESP8266WIFI example (there is no copyright/author info
in the file to give credit to - thanks and you rock!)

PUBLIC CLASSES AND FUNCTIONS:
TimeProxy

INITIALIZATION AND SEQUENCING REQUIREMENTS:
Must have a valid network connection

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

#include "timeproxy.h"

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


//----------------------------------------------------------------------
// Type Declarations
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Global Constant Definitions
//----------------------------------------------------------------------

// NTP time is in the first 48 bytes of message
const int NTP_PACKET_SIZE = 48; 

//----------------------------------------------------------------------
// Global Data Definitions
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Static Variable Definitions 
//----------------------------------------------------------------------

WiFiUDP TimeProxy::_udp;

String TimeProxy::_ntpServer;

// We haven't built functionality to reference back to 
// UTC yet, so we need to start at UTC time for now.
int TimeProxy::_timezone = TimeProxy::TimeZones::UTC;

// NTP Servers:
//static const char ntpServerName[] = "us.pool.ntp.org";
//static const char ntpServerName[] = "time.nist.gov";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";

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
TimeProxy()

DESCRIPTION:
C-tor

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
TimeProxy::TimeProxy( const String &ntpServer, unsigned int syncIntervalS )
    :_syncIntervalS( syncIntervalS )
{
    _ntpServer = ntpServer;
}

/*======================================================================
FUNCTION:
Begin()

DESCRIPTION:
Starts the udp subsystem and sets the callback handlers needed by
the TimeLib routines
RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void TimeProxy::Begin()
{
    if( _udp.begin( _localport ) == 0 )
    {
        Serial.printf( "starting udp on port %d failed\n", _localport );
    }

    setSyncProvider( &TimeProxy::getNtpTime );
    setSyncInterval( _syncIntervalS );
}

/*======================================================================
FUNCTION:
GetCurrentTimeUTC()

DESCRIPTION:
Returns the current time relative to UTC in a c-style time_t dude

RETURN VALUE:
UTC offset time_t value 

SIDE EFFECTS:
none

======================================================================*/
time_t TimeProxy::GetCurrentTimeUTC()
{
    // TODO - handle fixups in case we are not using a UTC referenced 
    // timezone
    return now();
}

/*======================================================================
FUNCTION:
GetTimeStringUTC()

DESCRIPTION:
Returns an ISO 8601 date/time string relative to UTC time.
https://en.wikipedia.org/wiki/ISO_8601
Example time string: 2017-11-10T01:28:49Z

RETURN VALUE:
ISO 8601 date/time string

SIDE EFFECTS:
none

======================================================================*/
String TimeProxy::GetTimeStringUTC()
{
    time_t t = GetCurrentTimeUTC();

    const int BUF_SIZE = 30;
    char buffer[BUF_SIZE + 1] = { 0 };

    // TODO - handle fixups in case we are not using a UTC referenced 
    // timezone
    snprintf( buffer, BUF_SIZE, "%d-%d-%dT%d:%d:%dZ",
              year( t ),
              month( t ),
              day( t ),
              hour( t ),
              minute( t ),
              second( t )
    );

    // Note - a String object will implicitly be
    // constructed and returned
    return buffer;
}

/*======================================================================
FUNCTION:
getNtpTime()

DESCRIPTION:
This is the callback handler that is called from the TimeLib
routines.  This calls out to the NTP server and returns the 
time_t value used by the TimeLib routines to manage time.  This code is 
based on the TimeNTP_ESP8266WIFI example (there is no copyright/author 
info in the file to give credit to - thanks and you rock!)

RETURN VALUE:
time_t value 

SIDE EFFECTS:
none

======================================================================*/
time_t TimeProxy::getNtpTime()
{
    IPAddress ntpServerIP; // NTP server's ip address

    while ( _udp.parsePacket() > 0 ); // discard any previously received packets
    Serial.println( "Transmit NTP Request" );
    
    // get a random server from the pool
    WiFi.hostByName( _ntpServer.c_str(), ntpServerIP );
    Serial.print( _ntpServer.c_str() );
    Serial.print( ": " );
    Serial.println( ntpServerIP );
    sendNTPpacket( ntpServerIP );
    uint32_t beginWait = millis();

    //buffer to hold incoming & outgoing packets
    byte packetBuffer[NTP_PACKET_SIZE]; 

    while ( millis() - beginWait < 1500 )
    {
        int size = _udp.parsePacket();
        if ( size >= NTP_PACKET_SIZE )
        {
            Serial.println( "Receive NTP Response" );
            _udp.read( packetBuffer, NTP_PACKET_SIZE );  // read packet into the buffer
            unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            secsSince1900 = (unsigned long) packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long) packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long) packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long) packetBuffer[43];
            return secsSince1900 - 2208988800UL + _timezone * SECS_PER_HOUR;
        }
    }
    Serial.println( "No NTP Response :-(" );
    return 0; // return 0 if unable to get the time
}

/*======================================================================
FUNCTION:
sendNTPpacket()

DESCRIPTION:
This method formats an NTP request and stuffs it into a UDP packet
that gets sent to the NTP server.  This code is  based on the 
TimeNTP_ESP8266WIFI example (there is no copyright/author
info in the file to give credit to - thanks and you rock!)

RETURN VALUE:
none

SIDE EFFECTS:
none

======================================================================*/
void TimeProxy::sendNTPpacket( IPAddress &address )
{
    //buffer to hold incoming & outgoing packets
    byte packetBuffer[NTP_PACKET_SIZE];

    // set all bytes in the buffer to 0
    memset( packetBuffer, 0, NTP_PACKET_SIZE );
    
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
                             // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    
    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    // NTP requests are to port 123
    const int NTP_PORT = 123;
    _udp.beginPacket( address, NTP_PORT );
    _udp.write( packetBuffer, NTP_PACKET_SIZE );
    _udp.endPacket();
}

/*=====================================================================
// IMPLEMENTATION NOTES
//=====================================================================

None

=====================================================================*/

