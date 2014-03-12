#pragma once
#include <string>

namespace by
{
	class Pcs
	{
	public:

		Pcs(const std::string client_id,
			const std::string client_secret);
		std::string GetAccessToken() const;
		void Auth(std::string auth_key);
		
	private:
		std::string access_token;

		const std::string	client_id ;
		const std::string	client_secret ;
	};
	std::string AuthURL(const std::string&	client_id);
}

