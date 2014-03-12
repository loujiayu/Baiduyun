#pragma once
#include <string>


namespace by {
	class JsonEntry
	{
	public:
		JsonEntry();
		JsonEntry(const std::string& str);
		JsonEntry(const JsonEntry& obj);
		JsonEntry(struct json_object *JsonEntry);
		~JsonEntry();

		static JsonEntry Parse(const std::string& str);

		void Add( const std::string& key, const JsonEntry& jsonEntry ) ;

		JsonEntry operator[]( const std::string& key ) const ;
		friend std::ostream& operator<<( std::ostream& os, const JsonEntry& JsonEntry ) ;

		template <typename T>
		T Value() const ;
	private:
		struct json_object	*jobj ;
	};

}