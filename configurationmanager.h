#ifndef _GARAGEOMATIC_CONFIGURATIONMANAGER_H_
#define _GARAGEOMATIC_CONFIGURATIONMANAGER_H_

/*======================================================================
FILE:
configurationmanager.h

CREATOR:
Sean Foley

GENERAL DESCRIPTION:
This class abstracts the flash file system and provides simple
methods to save/load configuraion data.

PUBLIC CLASSES AND FUNCTIONS:
ConfigurationManager

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

#include "configuration.h"

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
ConfigurationManager

DESCRIPTION:
This is a static utility class that abstracts how/where the configuration
data is stored.  In this implementation, the data is stored on the
flash file system.

HOW TO USE:
Simply call the method you want to use.  There is no sequencing 
requirements.

======================================================================*/
class ConfigurationManager
{
    public:

    //=================================================================
    // TYPE DECLARATIONS AND CONSTANTS    
    //=================================================================

    // None.

    //=================================================================
    // CLIENT INTERFACE
    //=================================================================

    // This will save the configuration data
    static bool Save( const Configuration &config );

    // Reads the configuration data and returns a config object
    static Configuration Load();

    // Removes any previously saved configuration (i.e. like a 
    // factory reset)
    static bool Reset();

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

    // No direct construction by callers. We won't define an
    // implementation to throw a link error in case someone magically
    // finds a way to try to directly instantiate this object
    ConfigurationManager();

    // Performs any one time initialization
    static bool initialze();

    //=================================================================
    // DATA MEMBERS    
    //=================================================================

    static bool _initialized;
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

#endif	// #ifendif _GARAGEOMATIC_CONFIGURATIONMANAGER_H_

