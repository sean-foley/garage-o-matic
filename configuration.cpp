/*======================================================================
FILE:
configuration.cpp

CREATOR:
Sean Foley

DESCRIPTION:
This file/class provides all the configuration values needed by the
garage-o-matic.

PUBLIC CLASSES AND FUNCTIONS:
Configuration

INITIALIZATION AND SEQUENCING REQUIREMENTS:
None.

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

#include "configuration.h"

#include "Arduino.h"

//----------------------------------------------------------------------
// Type Declarations
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Global Constant Definitions
//----------------------------------------------------------------------

// Key definitions used for building the key-value strings
const char* KEY_WLAN_SSID    = "wlanssid";
const char* KEY_WLAN_PASS    = "wlanpass";
const char* KEY_MQTT_SERVER  = "mqttserver";
const char* KEY_MQTT_PORT    = "mqttport";
const char* KEY_MQTT_PUBFEED = "mqttpubfeed";
const char* KEY_NTP_SERVER   = "ntpserver";
const char* KEY_DEVICE_USERNAME = "deviceusername";
const char* KEY_DEVICE_PASSWORD = "devicepassword";
const char* KEY_UNKNOWN = NULL;

// The token that corresponds to the key above. We use the tokens
// to know how to properly assign the value to the correct property
const int TOKEN_WLAN_SSID    = 0;
const int TOKEN_WLAN_PASS    = 1;
const int TOKEN_MQTT_SERVER  = 2;
const int TOKEN_MQTT_PORT    = 3;
const int TOKEN_MQTT_PUBFEED = 4;
const int TOKEN_NTP_SERVER   = 5;
const int TOKEN_DEVICE_USERNAME = 6;
const int TOKEN_DEVICE_PASSWORD = 7;
const int TOKEN_UNKNOWN      = 99;

const char* DELIMITER  = ":";
const char* TERMINATOR = "\n";

//----------------------------------------------------------------------
// Global Data Definitions
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Static Variable Definitions 
//----------------------------------------------------------------------

// This is the list of keys.  Add any new key consts here.
static const char* KEYS[] = {
    KEY_WLAN_SSID,
    KEY_WLAN_PASS,
    KEY_MQTT_SERVER,
    KEY_MQTT_PORT,
    KEY_MQTT_PUBFEED,
    KEY_NTP_SERVER,
    KEY_DEVICE_USERNAME,
    KEY_DEVICE_PASSWORD,
    KEY_UNKNOWN
};

// This is the corresponding list of key tokens.  Add any new tokens
// here and make sure *THIS* list orders correctly to the KEYS[] list
static const int TOKENS[] = {
    TOKEN_WLAN_SSID,
    TOKEN_WLAN_PASS,
    TOKEN_MQTT_SERVER,
    TOKEN_MQTT_PORT,
    TOKEN_MQTT_PUBFEED,
    TOKEN_NTP_SERVER,
    TOKEN_DEVICE_USERNAME,
    TOKEN_DEVICE_PASSWORD,
    TOKEN_UNKNOWN
};

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
Serialize()

DESCRIPTION:
Serializes the object data into a key:value pair terminated by 
a newline (\n) character.

RETURN VALUE:
String object representing the encoded object value

SIDE EFFECTS:
none

======================================================================*/
String Configuration::Serialize() const
{
    String content;

    content.reserve( 200 );

    content += makeKeyValue( KEY_WLAN_SSID, _wlanssid );
    content += makeKeyValue( KEY_WLAN_PASS, _wlanpassword );
    content += makeKeyValue( KEY_MQTT_SERVER, _mqttServer );
    content += makeKeyValue( KEY_MQTT_PORT, String( _mqttPort ) );
    content += makeKeyValue( KEY_MQTT_PUBFEED, _mqttPubFeed );
    content += makeKeyValue( KEY_NTP_SERVER, _ntpServer );
    content += makeKeyValue( KEY_DEVICE_USERNAME, _deviceUsername );
    content += makeKeyValue( KEY_DEVICE_PASSWORD, _devicePassword );

    Serial.printf( "Config serialization content len: %d\n", content.length() );

    return content;
}

/*======================================================================
FUNCTION:
Deserialize()

DESCRIPTION:
Takes the string input and parses it into a Configuration object

RETURN VALUE:
Configuration object

SIDE EFFECTS:
none

======================================================================*/
Configuration Configuration::Deserialize( const String &content ) const
{
    Configuration config;

    // Initial garage checking
    if ( content.length() == 5 )
    {
        return config;
    }

    int pos = 0;

    while ( true )
    {
        int terminator = content.indexOf( TERMINATOR, pos );

        if ( terminator == -1 )
        {
            break;
        }

        String line = content.substring( pos, terminator );

        // Update the pointer to point to the next character past
        // the most recent terminator we've founf
        pos = terminator + 1;

        KVPair pair = findTokenFromKey( line );

        switch ( pair.tokenKey )
        {
            case TOKEN_WLAN_PASS:
            {
                String value = pair.value;
                //value.replace( TERMINATOR, "\0" );
                config.SetWlanPassword( value );
            }
                break;

            case TOKEN_WLAN_SSID:
            {
                String value = pair.value;
                //value.replace( TERMINATOR, "\0" );
                config.SetWlanSSID( value );
            }
                break;

            case TOKEN_MQTT_SERVER:
                //Serial.printf( "TOKEN_MQTT_SERVER: value is %s\n", pair.value.c_str() );
                config.SetMqttServer( pair.value );
                break;

            case TOKEN_MQTT_PORT:
                //Serial.printf( "TOKEN_MQTT_PORT: value is %s\n", pair.value.c_str() );
                config.SetMqttPort( pair.value );
                break;

            case TOKEN_MQTT_PUBFEED:
                //Serial.printf( "TOKEN_MQTT_PUBFEED: value is %s\n", pair.value.c_str() );
                config.SetMqttPubFeed( pair.value );
                break;

            case TOKEN_NTP_SERVER:
                config.SetNtpServer( pair.value );
                break;

            case TOKEN_DEVICE_USERNAME:
                //Serial.printf( "TOKEN_DEVICE_USERNAME: value is %s\n", pair.value.c_str() );
                config.SetDeviceUsername( pair.value );
                break;

            case TOKEN_DEVICE_PASSWORD:
                //Serial.printf( "TOKEN_DEVICE_PASSWORD: value is %s\n", pair.value.c_str() );
                config.SetDevicePassword( pair.value );
                break;

            case TOKEN_UNKNOWN:

                // Might be at the end
                break;
        }
    }

    return config;
}

/*======================================================================
FUNCTION:
findTokenFromKey()

DESCRIPTION:
Scans the string looking for a key prefix.  If found, it sets the 
key token and extracts the value.  Both are set in the kvpair.

RETURN VALUE:
Configuration::kvpair object. 

SIDE EFFECTS:
none

======================================================================*/
Configuration::KVPair Configuration::findTokenFromKey( const String &kv ) const
{
    KVPair pair;

    pair.tokenKey = TOKEN_UNKNOWN;

    //Serial.printf( "raw line: %s\n", kv.c_str() );

    // Note this is a brute-force search, so worst case
    // it will be O(n).  There are not a lot of keys defined
    // so this is intentionally sloppy.  If your keyspace 
    // increases then consider a different data structure that is 
    // more search-friendly (such as a tree)
    for ( int i = 0; KEYS[i] != KEY_UNKNOWN; i++ )
    {
        //Serial.printf( "checking for key: %s\n", KEYS[i] );

        if ( kv.startsWith( KEYS[i] ) == 1 )
        {
            //Serial.printf( "Found key: %s\n", KEYS[i] );
            pair.tokenKey = TOKENS[i];

            // Find the offset for the delimiter
            int offset = kv.indexOf( DELIMITER );

            // Move +1 off the delimiter and -1 to exclude the terminator
            pair.value = kv.substring( offset + 1, kv.length() );

            // Sloppy but simple
            pair.value.replace( "\n", "" );
        }
    }

    return pair;
}

/*======================================================================
FUNCTION:
makeKeyValue()

DESCRIPTION:
Simple method that concantentates the key/value into a known string that
follows the pattern key[delimeter]value[terminator]

RETURN VALUE:
Formatted String object

SIDE EFFECTS:
none

======================================================================*/
String Configuration::makeKeyValue( const String &key, const String &value ) const
{
    // Let's calc the total length we need and then use that to reserve that space
    // for the String.  This should help with heap fragmentation over time.
    int len = key.length() + value.length() + strlen( DELIMITER ) + strlen( TERMINATOR );
    
    String kv;
    kv.reserve( len );

    kv = key;
    kv += DELIMITER;
    kv += value;
    kv += TERMINATOR;

    return kv;
}

/*=====================================================================
// IMPLEMENTATION NOTES
//=====================================================================

None

=====================================================================*/

