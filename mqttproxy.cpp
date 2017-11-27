/*======================================================================
FILE:
mqttproxt.cpp

CREATOR:
Sean Foley

DESCRIPTION:
Provides functionality that hides the details of using
MQTT on a micro/SoC platform.

PUBLIC CLASSES AND FUNCTIONS:
MqttProxy

INITIALIZATION AND SEQUENCING REQUIREMENTS:
The device must have a good network connection and have
an ip address before this class/object is used.

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

#include "mqttproxy.h"

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
MqttProxy()

DESCRIPTION:
C-tor

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
MqttProxy::MqttProxy( WiFiClient &client,
                      const String &mqttserver,
                      const int mqttport,
                      const String &mqttPublishFeed )
    : _wifiClient( client )
    , _mqttserver(mqttserver)
    , _mqttport( mqttport )
    , _mqttfeedPubName(mqttPublishFeed)
{
    _mqttClient.reset(
        new Adafruit_MQTT_Client( &_wifiClient, _mqttserver.c_str(), mqttport )
    );

    _mqttPublisher.reset(
        new Adafruit_MQTT_Publish( _mqttClient.get(), _mqttfeedPubName.c_str() )
    );
}

/*======================================================================
FUNCTION:
Connect()

DESCRIPTION:
This method will connect to the MQTT broker (server).  It is safe
to call even if you are already connected.

RETURN VALUE:
true if connected.

SIDE EFFECTS:
none

======================================================================*/
bool MqttProxy::Connect()
{    
    // Stop if already connected.
    if ( _mqttClient->connected() )
    {
        return true;
    }

    Serial.printf( "Attempting to connect to MQTT server %s on port %d...", 
                  _mqttserver.c_str(),
                  _mqttport );

    int8_t result = _mqttClient->connect();
    
    const int8_t CONNECTED = 0;

    if ( result == CONNECTED )
    {
        Serial.println( "MQTT Connected!" );
    }
    else
    {
        Serial.println( "MQTT no connection" );
    }

    return result == CONNECTED;
}

/*======================================================================
FUNCTION:
Publish()

DESCRIPTION:
Publishes the message to the MQTT broker (server).

RETURN VALUE:
true if successful

SIDE EFFECTS:
none

======================================================================*/
bool MqttProxy::Publish( const String &message )
{
    return _mqttPublisher->publish( message.c_str() );
}

/*======================================================================
FUNCTION:
Ping()

DESCRIPTION:
Pings the MQTT server to keep the connection alive

RETURN VALUE:
true if successful

SIDE EFFECTS:
none

======================================================================*/
bool MqttProxy::Ping()
{
    return _mqttClient->ping();
}

/*=====================================================================
// IMPLEMENTATION NOTES
//=====================================================================

None

=====================================================================*/

