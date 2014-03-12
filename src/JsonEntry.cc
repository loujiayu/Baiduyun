#include <json-c/json.h>
#include <stdexcept>
#include <assert.h>
#include <iostream>
#include "JsonEntry.hh"

namespace by
{
	JsonEntry::JsonEntry( ) :
		jobj( ::json_object_new_object() )
	{
		if ( jobj == 0 )
			throw std::runtime_error( "cannot create json object" ) ;
	}

	JsonEntry::JsonEntry( const std::string& str ) :
		jobj( ::json_object_new_string( str.c_str() ) )
	{
		if ( jobj == 0 )
			throw std::runtime_error( "cannot create json string \"" + str + "\"" ) ;

		assert( ::json_object_get_string( jobj ) == str ) ;
	}

	JsonEntry::JsonEntry( const JsonEntry& jsonEntry ) :
		jobj( jsonEntry.jobj )
	{
		assert( jobj != 0 ) ;
		::json_object_get( jobj ) ;
	}

	JsonEntry::JsonEntry( struct json_object *json ) :
		jobj( json )
	{
		assert( json != 0 ) ;
		::json_object_get( jobj ) ;
	}

	void JsonEntry::Add( const std::string& key, const JsonEntry& jsonEntry )
	{
		assert( jobj != 0 ) ;
		assert( jsonEntry.jobj != 0 ) ;
		::json_object_get( jsonEntry.jobj ) ;
		::json_object_object_add( jobj, key.c_str(), jsonEntry.jobj ) ;
	}

	JsonEntry JsonEntry::Parse( const std::string& str )
	{
		struct json_object *jobj = ::json_tokener_parse( str.c_str() ) ;
		if ( jobj == 0 )
			throw std::runtime_error( "json parse error" ) ;
		
		return JsonEntry( jobj ) ;
	}

	JsonEntry JsonEntry::operator[]( const std::string& key ) const
	{
		assert( jobj != 0 ) ;
		
		struct json_object *j = ::json_object_object_get( jobj, key.c_str() ) ;
		if ( j == 0 )
			throw std::runtime_error( "key: " + key + " is not found in object" ) ;
		
		return JsonEntry( j ) ;
	}

	std::ostream& operator<<( std::ostream& os, const JsonEntry& jsonEntry )
	{
		assert( jsonEntry.jobj != 0 ) ;
		return os << ::json_object_to_json_string( jsonEntry.jobj ) ;
	}

	template <>
	std::string JsonEntry::Value<std::string>()  const
	{
		assert( jobj != 0 ) ;
		return ::json_object_get_string( jobj ) ;
	}

	JsonEntry::~JsonEntry( )
	{
		assert( jobj != 0 ) ;
		if ( jobj != 0 )
			::json_object_put( jobj ) ;
	}

}
