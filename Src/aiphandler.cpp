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
#include <fstream>

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
	{
		std::cerr << "Unknown element: " << name << std::endl;
		exit(EXIT_FAILURE);
	}

}

void Handler::end_element(const xml::string& name)
{
	if(next_data != NEXT_DATA_NONE)
	{
		if(next_data == NEXT_DATA_NAME)
		{
			/* remove type from name */
			if((tmp_asp.header.type==OAB::TMZ && handle_str.compare(0, 3, "TMZ")==0) ||
					(tmp_asp.header.type==OAB::TMA && handle_str.compare(0, 3, "TMA")==0) ||
					(tmp_asp.header.type==OAB::CTR && handle_str.compare(0, 3, "CTR")==0))
				handle_str = handle_str.substr(4);

			tmp_asp.setName(handle_str);
		}
		else if(next_data == NEXT_DATA_ALTITUDE)
		{
			/* convert altitude value */
			int value = atoi(handle_str.c_str());

			if(next_alt == NEXT_ALT_CEILING)
				tmp_asp.header.altitudeTop_ft = value;
			else if(next_alt == NEXT_ALT_FLOOR)
				tmp_asp.header.altitudeBottom_ft = value;
			else
			{
				std::cerr << "limit unknown" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		else if(next_data == NEXT_DATA_POLYGON)
		{
			polygons(handle_str);
		}
		else if(next_data == NEXT_DATA_COUNTRY)
		{

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

void Handler::start_asp(const xml::attributes& attr)
{
	/* re-init asp */
	tmp_asp.reset();

	std::string category = std::string(attr["CATEGORY"]);
	tmp_asp.header.type = OAB::UNDEFINED;
	if(category == "A")
		tmp_asp.header.type = OAB::CLASSA;
	else if(category == "B")
		tmp_asp.header.type = OAB::CLASSB;
	else if(category == "C")
		tmp_asp.header.type = OAB::CLASSC;
	else if(category == "D")
		tmp_asp.header.type = OAB::CLASSD;
	else if(category == "E")
		tmp_asp.header.type = OAB::IGNORE;				//ignore
	else if(category == "F")
		tmp_asp.header.type = OAB::IGNORE;				//ignore
	else if(category == "G")
		tmp_asp.header.type = OAB::IGNORE;				//ignore
	else if(category == "PROHIBITED")
		tmp_asp.header.type = OAB::PROHIBITED;
	else if(category == "DANGER")
		tmp_asp.header.type = OAB::DANGER;
	else if(category == "RESTRICTED")
		tmp_asp.header.type = OAB::RESTRICTED;
	else if(category == "CTR")
		tmp_asp.header.type = OAB::CTR;
	else if(category == "TMA")
		tmp_asp.header.type = OAB::TMA;
	else if(category == "TMZ")
		tmp_asp.header.type = OAB::TMZ;
	else if(category == "RMZ")
		tmp_asp.header.type = OAB::RMZ;
	else if(category == "WAVE")
		tmp_asp.header.type = OAB::IGNORE;				//ignore
	else if(category == "GLIDING")
		tmp_asp.header.type = OAB::IGNORE;				//ignore
	else if(category == "FIR")
		tmp_asp.header.type = OAB::IGNORE;				//ignore
	else
	{
		std::cerr << "unknown airspace type: " << category << std::endl;
		exit(EXIT_FAILURE);
	}

	next_data = NEXT_DATA_NONE;
	next_alt = NEXT_ALT_NONE;
	next_altref = NEXT_ALTREF_NONE;
}

void Handler::alt_ref(const xml::attributes& attr)
{
	std::string reference = std::string(attr["REFERENCE"]);
	if(reference == "STD")
	{
		next_altref = NEXT_ALTREF_STD;
	}
	else if(reference =="MSL")
	{
		next_altref = NEXT_ALTREF_MSL;
	}
	else if(reference =="GND")
	{
		next_altref = NEXT_ALTREF_GND;
	}
	else
	{
		next_altref = NEXT_ALTREF_NONE;
		std::cerr << "unknown altitude reference " << reference << " (" << tmp_asp.header.name << ")" << std::endl;
		exit(EXIT_FAILURE);
	}
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
	next_altref = NEXT_ALTREF_NONE;
}

void Handler::start_alt(const xml::attributes& attr)
{
	std::string unit = std::string(attr["UNIT"]);
	uint16_t altref = 0;

	if(unit == "FL")
	{
		altref = OAB_ALTREF_FL;
	}
	else if(next_altref == NEXT_ALTREF_GND && unit ==  "F")
	{
		altref = OAB_ALTREF_GND;
	}
	else if(next_altref == NEXT_ALTREF_MSL && unit == "F")
	{
		altref = OAB_ALTREF_MSL;
	}
	else
	{
		std::cerr << "unknown unit " << unit << " for reference " << next_altref << std::endl;
		exit(EXIT_FAILURE);
	}

	if(next_alt == NEXT_ALT_CEILING)
	{
		tmp_asp.header.flags |= altref << OAB_ALTREF_TOP_OFFSET;
	}
	else if(next_alt == NEXT_ALT_FLOOR)
	{
		tmp_asp.header.flags |= altref << OAB_ALTREF_BOTTON_OFFSET;
	}
	else
	{
		std::cerr << "unknown altitude" << std::endl;
		exit(EXIT_FAILURE);
	}

	/* eval value */
	next_data = NEXT_DATA_ALTITUDE;
}

void Handler::end_asp()
{
	/* ignore airspace */
	if(tmp_asp.header.type == OAB::IGNORE)
		return;

	airspaces.push_back(tmp_asp);
}

void Handler::polygons(std::string line)
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

	/* reduce polygon count */
	tmp_asp.polygonSample_dist = -100.0f;
	while(tmp_asp.polygon.size() > 250 || tmp_asp.polygonSample_dist < 0.0)					//TODO magic number here
	{
		tmp_asp.polygonSample_dist += 100.0;
		tmp_asp.polygon.clear();

		for(auto coord: poly)
			tmp_asp.add(coord);
	}

	if(poly.size() != tmp_asp.polygon.size())
		std::cerr << "Reducing polygon size of " << tmp_asp.header.name << ": " << poly.size() << " -> " << tmp_asp.polygon.size() <<
				" (" << tmp_asp.polygonSample_dist << "m)" << std::endl;
}


void Handler::handle_comment(const XML_Char *comment)
{

}

bool Handler::writeOba(const char *fname)
{
	std::ofstream myFile (fname, std::ios::out | std::ios::binary);

	/* header */
	tmp_asp.writeFileHeader(myFile);

	/* airspaces */
	for(auto airspace : airspaces)
		airspace.write(myFile);

	myFile.close();

	std::cout << "Written " << airspaces.size() << " airspaces" << std::endl;

	return true;
}



