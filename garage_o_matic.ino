/*======================================================================
FILE:
garage-o-matic.ino

CREATOR:
Sean Foley

GENERAL DESCRIPTION:
Do you ever leave your house and forget if you closed your garage 
door?  If so, you need garage-o-matic.

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

#define PIN_RELAY_DOOR_0    15
#define PIN_RELAY_DOOR_1    13

#define PIN_SENSOR_DOOR_0   12
#define PIN_SENSOR_DOOR_1   14

#define PIN_ACTIVITY_LED    0
#define PIN_NETWORK_LED     2

#define PIN_FACTORY_RESET   16

//----------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------

#include <TimeLib.h>

// For the unique_ptr support
#include <memory>

#include <ESP8266WiFi.h>

#include "WiFiManager.h"

#include "webserverproxy.h"

#include "mqttproxy.h"

#include "timeproxy.h"

// Configuration and persistence used
// for the garage-o-matic
#include "configuration.h";
#include "configurationmanager.h";

// OTA firmware updating
#include "firmwareupdater.h"

#include "ledhelper.h"

#include "extendedwebserver.h"

//----------------------------------------------------------------------
// Type Declarations
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Global Constant Definitions
//----------------------------------------------------------------------

const int STATE_INITIALIZING = 0;
const int STATE_INITIALIZED = 1;
const int STATE_CHECK_STORED_CONFIG = 2;
const int STATE_WIFI_AP_CONFIG_MODE = 3;
const int STATE_WIFI_STA_DISCONNECTED = 4;
const int STATE_WIFI_STA_CONNECTED = 5;
const int STATE_READY = 6;

//----------------------------------------------------------------------
// Global Data Definitions
//----------------------------------------------------------------------

LedHelper activityLed( PIN_ACTIVITY_LED );
LedHelper networkLed( PIN_NETWORK_LED );

std::unique_ptr<WebserverProxy> webserverProxy;

std::unique_ptr<MqttProxy> mqttProxy;

std::unique_ptr<FirmwareUpdater> firmwareUpdater;

WiFiClient wifiClient;

GarageDoor::GarageDoorCollection garagedoors;

volatile bool saveConfigFlag = false;

std::unique_ptr<TimeProxy> timeProxy;

Configuration config;

//----------------------------------------------------------------------
// Static Variable Definitions 
//----------------------------------------------------------------------

volatile int activeState = STATE_INITIALIZED;

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
setup()

DESCRIPTION:
Put your 1-time initialization code in this function

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void setup()
{
    activityLed.TurnOff();
    networkLed.TurnOff();
    
    activityLed.Flash();
    networkLed.Flash();

    GarageDoor door0( PIN_SENSOR_DOOR_0, PIN_RELAY_DOOR_0 );
    GarageDoor door1( PIN_SENSOR_DOOR_1, PIN_RELAY_DOOR_1 );

    // We are supporting 2 doors
    garagedoors.reserve( 2 );

    garagedoors.push_back( door0 );
    garagedoors.push_back( door1 );

    pinMode( PIN_FACTORY_RESET, INPUT_PULLDOWN_16 );
}

/*======================================================================
FUNCTION:
saveConfigCallback()

DESCRIPTION:
Callback from WifiManager to indicate the user indicated to save the
configuration data.

RETURN VALUE:
none

SIDE EFFECTS:
Sets a global flag to indicate if saving should happen

======================================================================*/
void saveConfigCallback()
{
    // Set our global to indicate we should save
    saveConfigFlag = true;
}

/*======================================================================
FUNCTION:
wifiDisconnected()

DESCRIPTION:
Callback when we are disconnected from the wifi network.

RETURN VALUE:
none

SIDE EFFECTS:
none.

======================================================================*/
void wifiDisconnected( WiFiEventStationModeDisconnected event_info )
{
    Serial.println( "**WIFI disconnected**" );

    // Set the state machine
    activeState = STATE_WIFI_STA_DISCONNECTED;
}

/*======================================================================
FUNCTION:
wifiConnectedWithIP()

DESCRIPTION:
Callback when we are associated with a wifi network and have 
and IP address.

RETURN VALUE:
none

SIDE EFFECTS:
none.

======================================================================*/
void wifiConnectedWithIP( WiFiEventStationModeGotIP ipInfo )
{
    Serial.printf( "**WIFI connected and assigned ip %s\n", ipInfo.ip.toString().c_str() );
}

/*======================================================================
FUNCTION:
wifiConnected()

DESCRIPTION:
Callback when we are associated with a wifi network but do not 
have an ip address

RETURN VALUE:
none

SIDE EFFECTS:
none.

======================================================================*/
void wifiConnected( const WiFiEventStationModeConnected &connectInfo )
{
    Serial.printf( "**Wifi connected\n" );
}

/*======================================================================
FUNCTION:
doFactoryReset()

DESCRIPTION:
Reads the GPIO pin that is used to indicate we should perform a factory
reset.

RETURN VALUE:
true if the GPIO pin is HIGH

SIDE EFFECTS:
none.

======================================================================*/
bool doFactoryReset()
{
    return digitalRead( PIN_FACTORY_RESET ) == 1;
}

/*======================================================================
FUNCTION:
wifiApConfigMode()

DESCRIPTION:
Reads the GPIO pin that is used to indicate we should perform a factory
reset.

RETURN VALUE:
true if the GPIO pin is HIGH

SIDE EFFECTS:
none.

======================================================================*/
bool connectWifi()
{
    bool connected = false;

    config = ConfigurationManager::Load();

    Serial.printf( "Found configuration, attempting to connect to WLAN %s\n", config.GetWlanSSID().c_str() );

    WiFi.begin( config.GetWlanSSID().c_str(), config.GetWlanPassword().c_str() );

    // Handlers seem to need to be set after the begin() call
    WiFi.onStationModeDisconnected( wifiDisconnected );
    WiFi.onStationModeGotIP( wifiConnectedWithIP );
    WiFi.onStationModeConnected( wifiConnected );

    WiFi.persistent( false );
    WiFi.mode( WIFI_OFF );
    WiFi.mode( WIFI_STA );

    WiFi.begin( config.GetWlanSSID().c_str(), config.GetWlanPassword().c_str() );

    while ( WiFi.status() != WL_CONNECTED )
    {
        // TODO... break out of  loop after some N amount of time
        delay( 500 );
        Serial.print( "." );
    }

    connected = true;

    Serial.printf( "\nConnected to SSID %s, ip address %s\n",
                   config.GetWlanSSID().c_str(),
                   WiFi.localIP().toString().c_str() );

    return connected;
}

/*======================================================================
FUNCTION:
wifiApConfigMode()

DESCRIPTION:
Puts the AP into stand-along access point (AP) mode.  The user then connects
to this SSID and can use the web UI to set the garage-o-matic configuration
parameters, such as the wlan ssid/password, etc.

RETURN VALUE:
true if successfully configured

SIDE EFFECTS:
none.

======================================================================*/
bool wifiApConfigMode()
{
    const char* SSID = "garage-o-matic";

    bool connected = false;

    config = ConfigurationManager::Load();

        // Let's set some visual indicators to show we
        // are going into AP/setup mode
    activityLed.TurnOn();
    networkLed.TurnOff();

    Serial.println( "No configuration found, going into initial setup mode" );
    WiFiManager wifiManager;

    wifiManager.resetSettings();

    const int BUF_SIZE = 50;;
    char mqttServer [BUF_SIZE + 1] = { 0 };
    char mqttPubFeed[BUF_SIZE + 1] = { 0 };
    char ntpServer  [BUF_SIZE + 1] = { 0 };
    char deviceUser [BUF_SIZE + 1] = { 0 };
    char devicePass [BUF_SIZE + 1] = { 0 };

    strncpy( mqttPubFeed, "garage/doors", BUF_SIZE );
    strncpy( ntpServer, "us.pool.ntp.org", BUF_SIZE );

    strncpy( deviceUser, "admin", BUF_SIZE );
    strncpy( devicePass, "password", BUF_SIZE );

    char mqttPort[6] = { 0 };

    // The extra parameters to be configured (can be either global or just in the setup)
    // After connecting, parameter.getValue() will get you the configured value
    // id/name placeholder/prompt default length
    WiFiManagerParameter mqttServerParam( "mqtt_server", "mqtt server", mqttServer, BUF_SIZE );
    WiFiManagerParameter mqttPortParam( "mqtt_port", "1883", mqttPort, 6 );
    WiFiManagerParameter mqttPubFeedParam( "mqtt_pubfeed", "garage/doors", mqttPubFeed, BUF_SIZE );
    WiFiManagerParameter ntpServerParam( "ntp_server", "ntp server", ntpServer, BUF_SIZE );
    WiFiManagerParameter deviceUserParam( "device_user", "device username", deviceUser, BUF_SIZE );
    WiFiManagerParameter devicePassParam( "device_pass", "device password", devicePass, BUF_SIZE );

    wifiManager.addParameter( &mqttServerParam );
    wifiManager.addParameter( &mqttPortParam );
    wifiManager.addParameter( &mqttPubFeedParam );
    wifiManager.addParameter( &ntpServerParam );
    wifiManager.addParameter( &deviceUserParam );
    wifiManager.addParameter( &devicePassParam );

    wifiManager.setSaveConfigCallback( saveConfigCallback );

    // Set the ip subnet info for our AP
    wifiManager.setAPStaticIPConfig(
        IPAddress( 192, 168, 99, 1 ),
        IPAddress( 192, 168, 99, 1 ),
        IPAddress( 255, 255, 255, 0 ) );

    connected = wifiManager.autoConnect( SSID );

    if ( saveConfigFlag == true )
    {
        config.SetWlanSSID( wifiManager.getWlanSSID() );
        config.SetWlanPassword( wifiManager.getWlanPass() );

        config.SetMqttServer( mqttServerParam.getValue() );
        config.SetMqttPort( mqttPortParam.getValue() );
        config.SetMqttPubFeed( mqttPubFeedParam.getValue() );
        config.SetNtpServer( ntpServerParam.getValue() );
        config.SetDeviceUsername( deviceUserParam.getValue() );
        config.SetDevicePassword( devicePassParam.getValue() );

        ConfigurationManager::Save( config );
    }

    return connected;
}

/*======================================================================
FUNCTION:
serializeJSONPayload()

DESCRIPTION:
Creates a JSON string that represents the state of the garage door(s).

RETURN VALUE:
String with the json data

SIDE EFFECTS:
none

======================================================================*/
String serializeJSONPayload( const GarageDoor::GarageDoorCollection & garageDoors )
{
    const int DOOR_BUF_SIZE = 50;
    
    // We use about ~120 bytes for 2 doors.  This
    // buffer size is stoopid big.
    const int JSON_BUF_SIZE = 512;
    char json[JSON_BUF_SIZE] = { 0 };

    snprintf( json, 
              JSON_BUF_SIZE - 1, 
              "{\"garageomatic\":{\"version\":\"1.0.0\",\"timeUTC\":\"%s\",\"garagedoors\":[",
              timeProxy->GetTimeStringUTC().c_str() );

    for ( int i = 0; i < garageDoors.size(); i++ )
    {
        char doorjson[DOOR_BUF_SIZE] = { 0 };

        GarageDoor door = garageDoors[i];

        const char *EMPTY = "";
        const char *COMMA = ",";

        const char *SEPARATOR = COMMA;

        if ( ( i + 1 ) == garageDoors.size() )
        {
            SEPARATOR = EMPTY;
        }

        // Format the door json
        snprintf( doorjson, DOOR_BUF_SIZE - 1, "{\"door\": %d,\"status\":\"%s\"}%s",
                  i,
                  door.Status() == GarageDoor::OPEN ? "open" : "closed",
                  SEPARATOR
        );

        strncat( json, doorjson, JSON_BUF_SIZE - strlen( json ) );
    }

    strncat( json, "]}}", JSON_BUF_SIZE - strlen( json ) );
    
    return json;
}

/*======================================================================
FUNCTION:
loop()

DESCRIPTION:
For arduinos this is your main function loop.

RETURN VALUE:
none.

SIDE EFFECTS:
none

======================================================================*/
void loop()
{
    activityLed.Flash();
   
    switch ( activeState )
    {
        case STATE_INITIALIZING:

            activeState = STATE_INITIALIZED;
            break;

        case STATE_INITIALIZED:

            if ( doFactoryReset() == true )
            {
                Serial.println( "Initiating factory reset. All configuration data is erased." );

                // Nuke the configuration data
                ConfigurationManager::Reset();

                // Let's do some LED animation to show we 
                // successfully ran the factory reset.  
                // We are going to wig-wag the 2 leds
                for ( int x = 0; x < 100; x++ )
                {
                    int state = activityLed.Toggle();

                    if ( 0 == state )
                    {
                        networkLed.TurnOn();
                    }
                    else
                    {
                        networkLed.TurnOff();
                    }

                    delay( 50 );
                }
            }

            activeState = STATE_CHECK_STORED_CONFIG;
            break;

        case STATE_CHECK_STORED_CONFIG:

            config = ConfigurationManager::Load();

            if ( config.GetWlanSSID().length() == 0 )
            {
                // Assume if we don't have stored credentials
                // then this is the first time configuration
                activeState = STATE_WIFI_AP_CONFIG_MODE;
            }
            else
            {
                activeState = STATE_WIFI_STA_DISCONNECTED;
            }

            break;

        case STATE_WIFI_AP_CONFIG_MODE:
            {
                bool success = wifiApConfigMode();

                if ( true == success )
                {
                    activeState = STATE_WIFI_STA_DISCONNECTED;
                }
                else
                {
                    // do nothing and we'll keep looping on 
                    // this state
                }
            }

            break;

        case STATE_WIFI_STA_DISCONNECTED:
        {
            networkLed.TurnOff();
            bool connected = connectWifi();
            if ( connected == true )
            {
                Serial.printf( "Setting state to WIFI STA connected\n" );
                activeState = STATE_WIFI_STA_CONNECTED;
            }
            else
            {
                Serial.printf( "connectWifi() returned false\n" );
            }
        }

            break;

        case STATE_WIFI_STA_CONNECTED:
            
            networkLed.TurnOn();
            if ( firmwareUpdater == false )
            {
                Serial.println( "Starting firmware OTA support" );

                firmwareUpdater.reset( new FirmwareUpdater( config.GetDevicePassword().c_str() ) );

                firmwareUpdater->Begin();
            }

            if ( mqttProxy == false )
            {
                // Let's only use MQTT if a server was configured
                if ( config.GetMqttServer().length() != 0 )
                {
                    Serial.printf( "Starting mqtt proxy. Server %s, port %d, feed %s\n",
                                   config.GetMqttServer().c_str(),
                                   config.GetMqttPort(),
                                   config.GetMqttPubFeed().c_str()
                    );

                    mqttProxy.reset( new MqttProxy(
                        wifiClient,
                        config.GetMqttServer(),
                        config.GetMqttPort(),
                        config.GetMqttPubFeed() ) );
                }
            }

                // Do we have a webserver yet?
            if ( webserverProxy == false )
            {
                Serial.println( "Starting webserver" );
                // Allocate and start up
                webserverProxy.reset( new WebserverProxy( config, garagedoors ) );

                webserverProxy->Begin();
            }

            if ( timeProxy == false )
            {
                timeProxy.reset( new TimeProxy( config.GetNtpServer()) );

                timeProxy->Begin();
            }

            activeState = STATE_READY;
            break;

        case STATE_READY:

            if ( timeStatus() != timeNotSet )
            {
                //Serial.printf( "current time: %s\n", timeProxy->GetAscTime().c_str() );
            }

            //Serial.printf( "current time: %s\n", timeProxy->GetTimeStringUTC().c_str() );

            // Call of the handlers so they can do their thing
            webserverProxy->Process();
            firmwareUpdater->Process();
            
            if ( mqttProxy != false )
            {
                // Only publish if we have a mqtt proxy object
                mqttProxy->Connect();
                bool ok = mqttProxy->Publish( serializeJSONPayload( garagedoors ) );
            }

            serializeJSONPayload( garagedoors );

            delay( 100 );
            break;
    }
}


/*=====================================================================
// IMPLEMENTATION NOTES
//=====================================================================

None

=====================================================================*/