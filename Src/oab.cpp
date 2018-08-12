/*
 * oab.cpp
 *
 *  Created on: Aug 11, 2018
 *      Author: sid
 */

#include <cmath>
#include <string>
#include <iostream>
#include <fstream>

#include "oab.hpp"

void OAB::reset(void)
{
	polygon.clear();
	std::fill(header.name, header.name + sizeof(header.name), 0);
	header.flags = 0;
}

void OAB::setName(std::string &name)
{
	snprintf(header.name, sizeof(header.name), "%s", name.c_str());
}

void OAB::add(Coord &coord)
{
	oab_edge_t point;
	point.lat_rad = deg2rad(coord.latitude);
	point.lon_rad = deg2rad(coord.longitude);

	polygon.push_back(point);
}


void OAB::writeFileHeader(std::ofstream &file)
{
	file.write(id, strlen(id));

	time_t t = time(nullptr);
	file.write((char *) &t, sizeof(time_t));
}

void OAB::write(std::ofstream &file)
{
	/* finalize header */
//todo separate function...
//todo min/max
	header.numPoly = polygon.size();

	/* write header */
	file.write((char *) &header, sizeof(oab_header_t));

	/* write polygons */
	for(auto poly : polygon)
		file.write((char *) &poly, sizeof(oab_edge_t));
}
