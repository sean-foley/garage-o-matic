/*======================================================================
FILE:
configurationmanager.cpp

CREATOR:
Sean Foley

GENERAL DESCRIPTION:
This class abstracts the flash file system and provides simple
methods to save/load configuraion data.

PUBLIC CLASSES AND FUNCTIONS:
ConfigurationManager

INITIALIZATION AND SEQUENCING REQUIREMENTS:
The flash file system must be started before any data reads/writes, 
which is handled by the object accordingly.

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

#include "configurationmanager.h"

// Flash file system support
#include <FS.h>

//----------------------------------------------------------------------
// Type Declarations
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Global Constant Definitions
//----------------------------------------------------------------------

// This is the name of the file we use to store/read config data
const char* FILENAME = "/config.txt";

//----------------------------------------------------------------------
// Global Data Definitions
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Static Variable Definitions 
//----------------------------------------------------------------------

// Default case is we are not initialized yet
bool ConfigurationManager::_initialized = false;

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
Save()

DESCRIPTION:
Saves the cofiguration data to the flash file system

RETURN VALUE:
true if successful

SIDE EFFECTS:
data in the flash file system is modified

======================================================================*/
bool ConfigurationManager::Save( const Configuration &config )
{
    initialze();

    // Open config file for writing.
    File configFile = SPIFFS.open( FILENAME, "w" );
    if ( !configFile )
    {
        Serial.printf( "Failed to open %s for writing\n", FILENAME );

        return false;
    }

    String content = config.Serialize();

    // Save the configuration data
    configFile.println( content );

    Serial.println( content.c_str() );
    
    configFile.close();

    return true;
}

/*======================================================================
FUNCTION:
Load()

DESCRIPTION:
Reads the cofiguration data from the flash file system

RETURN VALUE:
Configuration object

SIDE EFFECTS:
None

======================================================================*/
Configuration ConfigurationManager::Load()
{
    initialze();

    Configuration config;

    // open file for reading.
    File configFile = SPIFFS.open( FILENAME, "r" );
    if ( !configFile )
    {
        Serial.printf( "Failed to open %s\n", FILENAME );

        return config;
    }

    // Read content from config file.
    String content = configFile.readString();
    
    configFile.close();

    config = config.Deserialize( content );

    return config;
}
 
/*======================================================================
FUNCTION:
Reset()

DESCRIPTION:
Removes/deletes the configuration file from the flash file system

RETURN VALUE:
true if successful

SIDE EFFECTS:
None

======================================================================*/
bool ConfigurationManager::Reset()
{
    initialze();

    return SPIFFS.remove( FILENAME );
}

/*======================================================================
FUNCTION:
initialize()

DESCRIPTION:
Starts the flash file system if it hasn't been started

RETURN VALUE:
true if successful

SIDE EFFECTS:
None

======================================================================*/
inline bool ConfigurationManager::initialze()
{
    if ( false == _initialized )
    {
        // The SPIFFS file system will not be available until
        // we call begin()
        _initialized = SPIFFS.begin();
    }
    else
    {
        return _initialized;
    }
}

/*=====================================================================
// IMPLEMENTATION NOTES
//=====================================================================

None

=====================================================================*/

