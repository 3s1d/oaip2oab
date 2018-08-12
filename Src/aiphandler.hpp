/*
 * aiphandler.hpp
 *
 *  Created on: Aug 11, 2018
 *      Author: sid
 */

#ifndef SRC_AIPHANDLER_HPP_
#define SRC_AIPHANDLER_HPP_

#include <vector>

#include "oab.hpp"
#include "xml.hpp"

#define NEXT_DATA_NONE		0
#define NEXT_DATA_NAME		1
#define NEXT_DATA_ALTITUDE	2
#define NEXT_DATA_POLYGON	3
#define NEXT_DATA_COUNTRY	4

#define NEXT_ALT_CEILING	1
#define NEXT_ALT_FLOOR		-1
#define NEXT_ALT_NONE		0

#define NEXT_ALTREF_NONE	0
#define NEXT_ALTREF_STD		1
#define NEXT_ALTREF_GND		2
#define NEXT_ALTREF_MSL		3

namespace aip
{

class Handler
{
private:
	void start_asp(const xml::attributes& attr);
	void start_ceiling(const xml::attributes& attr);
	void start_floor(const xml::attributes& attr);
	void start_alt(const xml::attributes& attr);
	void end_altlimit();
	void end_asp();

	void alt_ref(const xml::attributes& attr);
	void polygons(std::string line);

	std::string handle_str;

	int next_data = NEXT_DATA_NONE;
	int next_alt = NEXT_ALT_NONE;
	int next_altref = NEXT_ALTREF_NONE;

	OAB tmp_asp;
	std::vector<OAB> airspaces;
public:
	Handler();
	~Handler();

	void start_element(const xml::string& name, const xml::attributes& attr);
	void end_element(const xml::string& name);
	void handle_data(const xml::string& content, const int length);
	void handle_comment(const XML_Char *comment);

	void test()
	{
		std::cout << "num: " << airspaces.size() << std::endl;
		for(auto asp : airspaces)
		{
			std::cout << asp.polygon.size() << std::endl;
		}
	}
	bool writeOba(const char *fname);
};

}



#endif /* SRC_AIPHANDLER_HPP_ */
