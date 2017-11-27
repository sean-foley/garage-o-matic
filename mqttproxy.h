#ifndef _GARAGEOMATIC_MQTTPROXY_H_
#define _GARAGEOMATIC_MQTTPROXY_H_

/*======================================================================
FILE:
mqttproxy.h

CREATOR:
Sean Foley

DESCRIPTION:
Provides functionality that hides the details of using
MQTT on a micro/SoC platform.

PUBLIC CLASSES AND FUNCTIONS:
MqttProxy

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

// For unique_ptr support
#include <memory>

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

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
MqttProxy

DESCRIPTION:
Proxy class that hides the details of using MQTT on a micro/SoC 
platform.

HOW TO USE:
1. Construct with the required info
2. Call Connect() to connect to the MQTT broker (server).
3. Call Publish() to publish data. 

A good usage pattern would be to pair Connect() then a Publish() call.

======================================================================*/
class MqttProxy
{
    public:

    //=================================================================
    // TYPE DECLARATIONS AND CONSTANTS    
    //=================================================================

    // None.

    //=================================================================
    // CLIENT INTERFACE
    //=================================================================

    MqttProxy( WiFiClient &client, 
               const String &mqttserver, 
               const int mqttport,
               const String &mqttPublishFeed);

    // Call to connect to the MQTT broker (server).  This method is
    // safe to call even if already connected.  
    bool Connect();

    // Publishes that message to the feed that was specified
    // when this object was constructed
    bool Publish( const String &message );

    // If you don't have data to publish, you should periodically call
    // this method to keep the connection alive.
    bool Ping();

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

    // None.

    //=================================================================
    // DATA MEMBERS    
    //=================================================================

    WiFiClient _wifiClient;

    std::unique_ptr<Adafruit_MQTT_Client> _mqttClient;

    std::unique_ptr<Adafruit_MQTT_Publish> _mqttPublisher;

    int _mqttport;

    // After a bit of debugging, the AdaFruit MQTT libraries will only
    // cache a char* to the memory location given.  For example, if you
    // allocate the server name on the stack while constructing this object, 
    // then later try to call connect(), the server name is POOF.  Rather than
    // follow most Arduino dev patterns of declaring globals, we'll make
    // this object properly manage the state (buffers)
    String _mqttserver;
    String _mqttfeedPubName;

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

#endif	// #ifendif _GARAGEOMATIC_MQTTPROXY_H_
