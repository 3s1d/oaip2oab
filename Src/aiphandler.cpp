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

Handler::Handler()
{

}

Handler::~Handler()
{

}

void Handler::start_element(const xml::string& name, const xml::attributes& attr)
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

void Handler::end_element(const xml::string& name)
{
	if(next_data != NEXT_DATA_NONE)
	{
		if(next_data == NEXT_DATA_NAME)
		{
			//removes type from name?
			/* remove type from name */
//todo: 'TMZ-' or 'TMZ ' or 'CTR '
/*			if(!strncmp(handle_str.c_str(), tmp_asp_.type.c_str(), strlen(tmp_asp_.type.c_str())) &&
					handle_str.c_str()[strlen(tmp_asp_.type.c_str())] == ' ')
			{
				handle_str = handle_str.substr(tmp_asp_.type.size()+1);
			}
*/
			tmp_asp.setName(handle_str);
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

void Handler::handle_data(const xml::string& content, const int length)
{
	if(next_data == NEXT_DATA_NONE)
		return;

	std::string element(content.c_str(), 0 , length);
	handle_str += element;
}




/*
 * airspace functions
 */

/*
CLASSA = 0,
CLASSB,
CLASSC,
CLASSD,
CLASSE,
CLASSF,
CLASSG,
DANGER,
PROHIBITED,
RESTRICTED,
CTR,
TMA,
TMZ,
RMZ,
FIR, // from here on not visible by default
UIR,
OTH,
GLIDING,
NOGLIDER,
WAVE,
UNKNOWN, // "UNKNOWN" can be used in OpenAir files
UNDEFINED // also the last one
 */

void Handler::start_asp(const xml::attributes& attr)
{
	/* re-init asp */
	tmp_asp.reset();

	std::string category = std::string(attr["CATEGORY"]);
	tmp_asp.header.type = '?';
	if(category == "A")
		tmp_asp.header.type = 'A';
	else if(category == "B")
		tmp_asp.header.type = 'B';
	else if(category == "C")
		tmp_asp.header.type = 'C';
	else if(category == "D")
		tmp_asp.header.type = 'D';
	else if(category == "E")
		tmp_asp.header.type = 'E';
	else if(category == "F")
		tmp_asp.header.type = 'F';
	else if(category == "G")
		tmp_asp.header.type = 'G';
	else if(category == "PROHIBITED")
		tmp_asp.header.type = 'P';
	else if(category == "DANGER")
		tmp_asp.header.type = 'Q';
	else if(category == "RESTRICTED")
		tmp_asp.header.type = 'R';
	else if(category == "CTR")
		tmp_asp.header.type = 'c';
	else if(category == "TMA")
		tmp_asp.header.type = 'a';
	else if(category == "TMZ")
		tmp_asp.header.type = 'z';
	else if(category == "RMZ")
		tmp_asp.header.type = 'r';
	else if(category == "WAVE")
		tmp_asp.header.type = '-';			//ignore
	else if(category == "GLIDING")
		tmp_asp.header.type = '-';			//ignore
	else if(category == "FIR")
		tmp_asp.header.type = '-';			//ignore
	else
		std::cerr << "unknown airspace type: " << category << std::endl;

	next_data = NEXT_DATA_NONE;
	next_alt = NEXT_ALT_NONE;
}

void Handler::alt_ref(const xml::attributes& attr)
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

void Handler::start_ceiling(const xml::attributes& attr)
{
	next_alt = NEXT_ALT_CEILING;
	alt_ref(attr);
}

void Handler::start_floor(const xml::attributes& attr)
{
	next_alt = NEXT_ALT_FLOOR;
	alt_ref(attr);
}

void Handler::end_altlimit()
{
	next_alt = NEXT_ALT_NONE;
}

void Handler::start_alt(const xml::attributes& attr)
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

void Handler::end_asp()
{
	//std::cout << "end asp" << std::endl;

	if(tmp_asp.header.type == '-')
		return;

//	if(strstr(tmp_asp_.name.c_str(), "paragliding") != NULL)
//		return;

	airspaces.push_back(tmp_asp);
}

void Handler::polygons(std::string line)
{
	std::istringstream ss(line);
	std::string token;

	while(std::getline(ss, token, ','))
	{
		std::istringstream coord_ss(token);
		Coord coord;
		coord_ss >> coord.longitude >> coord.latitude;
		tmp_asp.add(coord);
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


void Handler::handle_comment(const XML_Char *comment)
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



