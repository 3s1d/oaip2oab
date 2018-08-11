/*
 * oab.cpp
 *
 *  Created on: Aug 11, 2018
 *      Author: sid
 */

#include <cmath>
#include <string>

#include "oab.hpp"

void OAB::reset(void)
{
	polygon.clear();
	std::fill(header.name, header.name + sizeof(header.name), 0);
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


