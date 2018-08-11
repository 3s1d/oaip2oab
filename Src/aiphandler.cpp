/*
 * aiphandler.cpp
 *
 *  Created on: Aug 11, 2018
 *      Author: sid
 */


#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

#include "aiphandler.hpp"
#include "coord.hpp"
#include "xml.hpp"

using namespace aip;

handler::handler()
{

}

handler::~handler()
{

}

void handler::start_element(const xml::string& name, const xml::attributes& attr)
{
	if(name == "ASP")
		start_asp(attr);
	else if(name == "NAME")
		next_data = NEXT_DATA_NAME;
	else if(name == "ALTLIMIT_TOP")
		start_ceiling(attr);
	else if(name == "ALTLIMIT_BOTTOM")
		start_floor(attr);
	else if(name == "ALT")
		start_alt(attr);
	else if(name == "POLYGON")
		next_data = NEXT_DATA_POLYGON;
	else if(name == "COUNTRY")
		next_data = NEXT_DATA_COUNTRY;
	else if(name == "OPENAIP" || name  == "AIRSPACES"  || name  == "VERSION"  || name  == "ID" || name  == "GEOMETRY")
	{ /* do nothing */ }
	else
		std::cerr << "Unknown element: " << name << std::endl;

}

void handler::end_element(const xml::string& name)
{
	//std::cout << "name " << name.c_str() << ">>>>>" << std::endl;

	if(next_data != NEXT_DATA_NONE)
	{
		if(next_data == NEXT_DATA_NAME)
		{
			//removes type from name?
/*			if(!strncmp(handle_str.c_str(), tmp_asp_.type.c_str(), strlen(tmp_asp_.type.c_str())) &&
					handle_str.c_str()[strlen(tmp_asp_.type.c_str())] == ' ')
			{
				handle_str = handle_str.substr(tmp_asp_.type.size()+1);
			}

			tmp_asp_.name = handle_str;
*/
//			std::cout << "Name: " << handle_str << std::endl;
		}
		else if(next_data == NEXT_DATA_ALTITUDE)
		{
/*			int value = atoi(handle_str.c_str());

			if(next_alt == NEXT_ALT_CEILING)
				tmp_asp_.ceiling.value = value;
			else if(next_alt == NEXT_ALT_FLOOR)
				tmp_asp_.floor.value = value;
			else
				std::cerr << "limit unknown" << std::endl;
*/		}
		else if(next_data == NEXT_DATA_POLYGON)
		{
			polygons(handle_str);
		}
		else if(next_data == NEXT_DATA_COUNTRY)
		{
//			tmp_asp_.country = handle_str;
		}


		handle_str.clear();
		next_data = NEXT_DATA_NONE;
	}

	if(name == "ALTLIMIT_TOP" || name == "ALTLIMIT_BOTTOM")
		end_altlimit();
	else if(name == "ASP")
		end_asp();

}

void handler::handle_data(const xml::string& content, const int length)
{
	if(next_data == NEXT_DATA_NONE)
		return;

	std::string element(content.c_str(), 0 , length);
	handle_str += element;
}




/*
 *
 * airspace functions
 *
 */
void handler::start_asp(const xml::attributes& attr)
{
	/* re-init asp */
/*	tmp_asp_.clean();

	tmp_asp_.type = attr["CATEGORY"];
	if(tmp_asp_.type == "RESTRICTED")
		tmp_asp_.type = "R";
	else if(tmp_asp_.type == "WAVE")
		tmp_asp_.type = "W";
	else if(tmp_asp_.type == "PROHIBITED")
		tmp_asp_.type = "P";
	else if(tmp_asp_.type == "DANGER")
		tmp_asp_.type = "Q";
	else if(tmp_asp_.type == "GLIDING")
		tmp_asp_.type = "GSEC";			//todo W?

	if(tmp_asp_.type.length() > 4)
		std::cerr << "unknown airspace type: " << tmp_asp_.type << std::endl;
*/
	next_data = NEXT_DATA_NONE;
	next_alt = NEXT_ALT_NONE;
}

void handler::alt_ref(const xml::attributes& attr)
{
/*	int ref;
	if(!strcmp(attr["REFERENCE"],"STD"))
		ref = REF_STD;
	else if(!strcmp(attr["REFERENCE"],"MSL"))
		ref = REF_MSL;
	else if(!strcmp(attr["REFERENCE"],"GND"))
		ref = REF_GND;
	else
	{
		ref = REF_MSL;
		std::cerr << "unknown reference, assuming MSL" << std::endl;
	}

	if(next_alt == NEXT_ALT_CEILING)
		tmp_asp_.ceiling.refernce = ref;
	else if(next_alt == NEXT_ALT_FLOOR)
		tmp_asp_.floor.refernce = ref;
	else
		std::cerr << "unknown limit for reference" << std::endl;
*/
}

void handler::start_ceiling(const xml::attributes& attr)
{
	next_alt = NEXT_ALT_CEILING;
	alt_ref(attr);
}

void handler::start_floor(const xml::attributes& attr)
{
	next_alt = NEXT_ALT_FLOOR;
	alt_ref(attr);
}

void handler::end_altlimit()
{
	next_alt = NEXT_ALT_NONE;
}

void handler::start_alt(const xml::attributes& attr)
{
/*	int unit;
	if(!strcmp(attr["UNIT"], "FL"))
		unit = UNIT_FL;
	else if(!strcmp(attr["UNIT"], "F"))
		unit = UNIT_FT;
	else
	{
		unit = UNIT_FT;
		std::cerr << "unknown unit" << std::endl;
	}

	if(next_alt == NEXT_ALT_CEILING)
		tmp_asp_.ceiling.unit = unit;
	else if(next_alt == NEXT_ALT_FLOOR)
		tmp_asp_.floor.unit = unit;
	else
		std::cerr << "unknown limit for unit" << std::endl;
*/
	next_data = NEXT_DATA_ALTITUDE;
}

void handler::end_asp()
{
/*	if(tmp_asp_.type == "FIR")
		return;

	if(strstr(tmp_asp_.name.c_str(), "paragliding") != NULL)
		return;

	if(tmp_asp_.country != country)
	{
		//std::cerr << "wrong country" << std::endl;
		return;
	}

	std::cout << tmp_asp_;
*/
}

void handler::polygons(std::string line)
{
	std::istringstream ss(line);
	std::string token;

	std::vector<Coord> poly;
	while(std::getline(ss, token, ','))
	{
		std::istringstream coord_ss(token);
		Coord coord;
		coord_ss >> coord.longitude >> coord.latitude;
		poly.push_back(coord);
	}
//todo  last point unneeded!!!!
/*
	tmp_asp_.poly_sample_dist = 0.0;

	while(tmp_asp_.polygon.size() > 250 || tmp_asp_.poly_sample_dist == 0.0)
	{
		tmp_asp_.poly_sample_dist += 100.0;
		tmp_asp_.polygon.clear();

		for(p2df coord: poly)
			tmp_asp_.add_sample(coord);
		tmp_asp_.finalize_sample();
	}

	std::cerr << tmp_asp_.name << ": " << poly.size() << " -> " << tmp_asp_.polygon.size() << " (" << tmp_asp_.poly_sample_dist << "m)" << std::endl;
*/
//	std::cout << "size:" << poly.size() << std::endl;
}


void handler::handle_comment(const XML_Char *comment)
{
/*	static bool greater = false;

	std::cout<<comment;

	if(greater == false)
	{
		std::cout << "*" << std::endl;
		std::cout << "*  Modified by Skytraxx GmbH to reduce the border complexity" << std::endl;
		std::cout << "*  (In order to be less resource demanding)" << std::endl;
		std::cout << "*  Modified on: ";
		std::time_t result = std::time(NULL);
		std::cout << std::ctime(&result) << std::endl;
	}*/
}



