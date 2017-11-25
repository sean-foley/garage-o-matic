#ifndef _GARAGEOMATIC_CONFIGURATION_H_
#define _GARAGEOMATIC_CONFIGURATION_H_

/*======================================================================
FILE:
configuration.h

CREATOR:
Sean Foley

DESCRIPTION:
This file/class provides all the configuration values needed by the
garage-o-matic.

PUBLIC CLASSES AND FUNCTIONS:
Configuration

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
Configuration

DESCRIPTION:
This is a propery class that has the various configuration values
used by this subsystem.

HOW TO USE:
Use set/get for the various property you are interested in. The
Serialize/Deserialize methods will turn this object's state to/from
a string representation.

======================================================================*/
class Configuration
{
    public:

    //=================================================================
    // TYPE DECLARATIONS AND CONSTANTS    
    //=================================================================

    typedef struct KVPair
    {
        int tokenKey;
        String value;
    };

    //=================================================================
    // CLIENT INTERFACE
    //=================================================================

    // Accessors/mutators
    void SetWlanSSID( const String &value ) { _wlanssid = value; }
    String GetWlanSSID() const { return _wlanssid; }

    void SetWlanPassword( const String &value ) { _wlanpassword = value; }
    String GetWlanPassword() const { return _wlanpassword; }

    void SetMqttServer( const String &value ) { _mqttServer = value; }
    String GetMqttServer() const { return _mqttServer; }

    void SetMqttPort( const String &value ) 
    {   
        _mqttPort = atoi( value.c_str() ); 
        if ( 0 == _mqttPort ){ _mqttPort = 1883; } 
    }
    void SetMqttPort( int value ) { _mqttPort = value; }
    int GetMqttPort() const { return _mqttPort; }

    void SetMqttPubFeed( const String &value ) { _mqttPubFeed = value; }
    String GetMqttPubFeed() const { return _mqttPubFeed; }

    void SetNtpServer( const String &value ) { _ntpServer = value; }
    String GetNtpServer() const { return _ntpServer; }

    void SetDeviceUsername( const String &value ) { _deviceUsername = value; }
    String GetDeviceUsername() const { return _deviceUsername; }

    void SetDevicePassword( const String &value ) { _devicePassword = value; }
    String GetDevicePassword() const { return _devicePassword; }

    String Serialize() const;
    Configuration Deserialize( const String &content ) const;

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

    String makeKeyValue( const String &key, const String &value ) const;

    KVPair findTokenFromKey( const String &kv ) const;

    //=================================================================
    // DATA MEMBERS    
    //=================================================================

    String _wlanssid;
    String _wlanpassword;

    String _mqttServer;
    int    _mqttPort;
    String _mqttPubFeed;
    String _ntpServer;

    String _deviceUsername;
    String _devicePassword;
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

#endif	// #ifendif _GARAGEOMATIC_CONFIGURATION_H_

