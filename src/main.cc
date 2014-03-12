

#include <getopt.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <assert.h>

#include "Pcs.hh"
#include "JsonEntry.hh"

namespace by
{
	const std::string& ConfigFilename()
	{
		static const std::string filename =	std::string( ::getenv( "HOME") ) + "/.baiduyun" ;

		return filename ;
	}

	JsonEntry ReadConfig()
	{
		std::ifstream ifile( ConfigFilename().c_str() ) ;
		
		if ( ifile )
		{
			std::string cfg_str(
				(std::istreambuf_iterator<char>( ifile )),
				(std::istreambuf_iterator<char>()) ) ;
			
			return JsonEntry::Parse( cfg_str ) ;
		}
		else
			return JsonEntry();
	}
	
	void SaveConfig( const JsonEntry& config )
	{
		std::ofstream ofile( ConfigFilename().c_str() ) ;
		ofile << config ;
	}
}

int main(int argc, char *argv[])
{
	using namespace by;
	JsonEntry config = ReadConfig() ;
	int c;
	const std::string client_id		= "dOiFFnAqiGhzpsT19ijBqpaM" ;
	const std::string client_secret	= "QVCr57iC3g8AjX5pRlkbSPIrivAtY1BE" ;

	while((c = getopt (argc, argv, "a")) != -1) 
	{
		switch(c)
		{
			case 'a':
			{
				std::cout
					<< "-----------------------\n"
					<< "Please go to this URL and get an authenication code:\n\n"
					<< AuthURL( client_id )
					<< std::endl ;
				
				std::cout
					<< "\n-----------------------\n"
					<< "Please input the authenication code here" << std::endl ;
				std::string code ;
				std::cin >> code ;

				Pcs baidu_oauth(client_id,client_secret);
				baidu_oauth.Auth(code); 

				config.Add( "access_token", JsonEntry( baidu_oauth.GetAccessToken() ) ) ;
				assert( config["access_token"].Value<std::string>() == baidu_oauth.GetAccessToken() ) ;
				SaveConfig( config ) ;
			}
		}
	}
	return 0;
}