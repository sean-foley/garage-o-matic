//======================================================================
// PRE-COMPILED HEADER
//======================================================================

/*======================================================================
FILE:
extendedwebserver.cpp

CREATOR:
Sean Foley

GENERAL DESCRIPTION:
The ESP8266WebServer has been extended to support digest authentication.
However, when I pulled the esp8266 core down from git, there were a lot
of side effects that broke things.  The ExtendedWebServer subclasses the
ESP8266WebServer and implements the exact code that the ESPWebServer used
to implement digest authentication.  Once the new esp8266 core becomes an
official release, it should be trivial to remove this code and use the
release code.

Implements digest auth that was added to ESP8266WebServer
https://github.com/esp8266/Arduino/commit/eebc5ec593a8c739f8da2612a0e25ff4dab982fb#diff-b78105e1729cc548e062483fec020a9b

Implements the constant time string compare from WString 
https://github.com/esp8266/Arduino/commit/03f1a540caa5af96a686db81fc3a21b9936dd4a7#diff-3d1eaec7ee8f9cdadc75a401477867a0

PUBLIC CLASSES AND FUNCTIONS:
ExtendedWebServer

INITIALIZATION AND SEQUENCING REQUIREMENTS:
None

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

#include "extendedwebserver.h"
#include <libb64/cencode.h>

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

String ExtendedWebServer::_getRandomHexString()
{
    char buffer[33];  // buffer to hold 32 Hex Digit + /0
    int i;
    for ( i = 0; i<4; i++ )
    {
        sprintf( buffer + ( i * 8 ), "%08x", RANDOM_REG32 );
    }
    return String( buffer );
}

String ExtendedWebServer::_exractParam( String& authReq, const String& param, const char delimit )
{
    int _begin = authReq.indexOf( param );
    if ( _begin == -1 ) return "";
    return authReq.substring( _begin + param.length(), authReq.indexOf( delimit, _begin + param.length() ) );
}

void ExtendedWebServer::requestAuthentication( HTTPAuthMethod mode, const char* realm, const String& authFailMsg )
{
    if ( realm == NULL )
    {
        _srealm = "Login Required";
    }
    else
    {
        _srealm = String( realm );
    }
    if ( mode == BASIC_AUTH )
    {
        sendHeader( "WWW-Authenticate", "Basic realm=\"" + _srealm + "\"" );
    }
    else
    {
        _snonce = _getRandomHexString();
        _sopaque = _getRandomHexString();
        sendHeader( "WWW-Authenticate", "Digest realm=\"" + _srealm + "\", qop=\"auth\", nonce=\"" + _snonce + "\", opaque=\"" + _sopaque + "\"" );
    }
    send( 401, "text/html", authFailMsg );
}

bool ExtendedWebServer::authenticate( const char * username, const char * password )
{
    const char * AUTHORIZATION_HEADER = "Authorization";

    if ( hasHeader( AUTHORIZATION_HEADER ) )
    {
        String authReq = header( AUTHORIZATION_HEADER );
        if ( authReq.startsWith( "Basic" ) )
        {
            authReq = authReq.substring( 6 );
            authReq.trim();
            char toencodeLen = strlen( username ) + strlen( password ) + 1;
            char *toencode = new char[toencodeLen + 1];
            if ( toencode == NULL )
            {
                authReq = String();
                return false;
            }
            char *encoded = new char[base64_encode_expected_len( toencodeLen ) + 1];
            if ( encoded == NULL )
            {
                authReq = String();
                delete[] toencode;
                return false;
            }
            sprintf( toencode, "%s:%s", username, password );
            
            //if ( base64_encode_chars( toencode, toencodeLen, encoded ) > 0 && authReq.equalsConstantTime( encoded ) )
            if ( base64_encode_chars( toencode, toencodeLen, encoded ) > 0 && equalsConstantTime( authReq, encoded ) )
            {
                authReq = String();
                delete[] toencode;
                delete[] encoded;
                return true;
            }
            delete[] toencode;
            delete[] encoded;
        }
        else if ( authReq.startsWith( "Digest" ) )
        {
            authReq = authReq.substring( 7 );
            #ifdef DEBUG_ESP_HTTP_SERVER
            DEBUG_OUTPUT.println( authReq );
            #endif
            String _username = _exractParam( authReq, "username=\"" );
            if ( ( !_username.length() ) || _username != String( username ) )
            {
                authReq = String();
                return false;
            }
            // extracting required parameters for RFC 2069 simpler Digest
            String _realm = _exractParam( authReq, "realm=\"" );
            String _nonce = _exractParam( authReq, "nonce=\"" );
            String _uri = _exractParam( authReq, "uri=\"" );
            String _response = _exractParam( authReq, "response=\"" );
            String _opaque = _exractParam( authReq, "opaque=\"" );

            if ( ( !_realm.length() ) || ( !_nonce.length() ) || ( !_uri.length() ) || ( !_response.length() ) || ( !_opaque.length() ) )
            {
                authReq = String();
                return false;
            }
            if ( ( _opaque != _sopaque ) || ( _nonce != _snonce ) || ( _realm != _srealm ) )
            {
                authReq = String();
                return false;
            }
            // parameters for the RFC 2617 newer Digest
            String _nc, _cnonce;
            if ( authReq.indexOf( "qop=auth" ) != -1 )
            {
                _nc = _exractParam( authReq, "nc=", ',' );
                _cnonce = _exractParam( authReq, "cnonce=\"" );
            }
            MD5Builder md5;
            md5.begin();
            md5.add( String( username ) + ":" + _realm + ":" + String( password ) );  // md5 of the user:realm:user
            md5.calculate();
            String _H1 = md5.toString();
            #ifdef DEBUG_ESP_HTTP_SERVER
            DEBUG_OUTPUT.println( "Hash of user:realm:pass=" + _H1 );
            #endif
            md5.begin();
            if ( _currentMethod == HTTP_GET )
            {
                md5.add( "GET:" + _uri );
            }
            else if ( _currentMethod == HTTP_POST )
            {
                md5.add( "POST:" + _uri );
            }
            else if ( _currentMethod == HTTP_PUT )
            {
                md5.add( "PUT:" + _uri );
            }
            else if ( _currentMethod == HTTP_DELETE )
            {
                md5.add( "DELETE:" + _uri );
            }
            else
            {
                md5.add( "GET:" + _uri );
            }
            md5.calculate();
            String _H2 = md5.toString();
            #ifdef DEBUG_ESP_HTTP_SERVER
            DEBUG_OUTPUT.println( "Hash of GET:uri=" + _H2 );
            #endif
            md5.begin();
            if ( authReq.indexOf( "qop=auth" ) != -1 )
            {
                md5.add( _H1 + ":" + _nonce + ":" + _nc + ":" + _cnonce + ":auth:" + _H2 );
            }
            else
            {
                md5.add( _H1 + ":" + _nonce + ":" + _H2 );
            }
            md5.calculate();
            String _responsecheck = md5.toString();
            #ifdef DEBUG_ESP_HTTP_SERVER
            DEBUG_OUTPUT.println( "The Proper response=" + _responsecheck );
            #endif
            if ( _response == _responsecheck )
            {
                authReq = String();
                return true;
            }
        }
        authReq = String();
    }
    return false;
}

// From WString 
// https://github.com/esp8266/Arduino/commit/03f1a540caa5af96a686db81fc3a21b9936dd4a7#diff-3d1eaec7ee8f9cdadc75a401477867a0
unsigned char ExtendedWebServer::equalsConstantTime( const String &lhs, const String &rhs )
{
    // To avoid possible time-based attacks present function
    // compares given strings in a constant time.
    if ( lhs.length() != rhs.length() )
    {
        return 0;
    }

    //at this point lengths are the same
    if ( lhs.length() == 0 )
    {
        return 1;
    }

    //at this point lenghts are the same and non-zero
    const char *p1 = lhs.c_str(); //buffer;
    const char *p2 = rhs.c_str(); // s2.buffer;
    unsigned int equalchars = 0;
    unsigned int diffchars = 0;
    while ( *p1 )
    {
        if ( *p1 == *p2 )
            ++equalchars;
        else
            ++diffchars;
        ++p1;
        ++p2;
    }
    //the following should force a constant time eval of the condition without a compiler "logical shortcut"
    unsigned char equalcond = ( equalchars == lhs.length() );
    unsigned char diffcond = ( diffchars == 0 );
    return ( equalcond & diffcond ); //bitwise AND
}

/*=====================================================================
// IMPLEMENTATION NOTES
//=====================================================================

None

=====================================================================*/
