#include "Pcs.hh"
#include "Http.hh"
#include "JsonEntry.hh"
#include <iostream>
namespace by

{
	const std::string token_url		= "https://openapi.baidu.com/oauth/2.0/token" ;

	Pcs::Pcs(const std::string _client_id,
			const std::string _client_secret) : 
		client_id(_client_id),
		client_secret(_client_secret) {}

	void Pcs::Auth(std::string auth_key)
	{
		std::string post = "grant_type=authorization_code"
						"&code=" + auth_key +
						"&client_id=dOiFFnAqiGhzpsT19ijBqpaM"
						"&client_secret=QVCr57iC3g8AjX5pRlkbSPIrivAtY1BE"
						"&redirect_uri=oob";
						
		JsonEntry resp = JsonEntry::Parse( HttpPostData( token_url, post ) ) ;
		// JsonEntry resp (HttpPostData( token_url, post ) );
		access_token	= resp["access_token"].Value<std::string>() ;
		
		std::cout << access_token << std::endl;
	}

	std::string AuthURL(const std::string&	client_id)
	{
			return "https://openapi.baidu.com/oauth/2.0/authorize?"
					"scope=netdisk"												
					"&redirect_uri=oob"				
					"&response_type=code"				
					"&client_id="+client_id ;		
	}

	std::string Pcs::GetAccessToken() const
	{
		return access_token;
	}
}