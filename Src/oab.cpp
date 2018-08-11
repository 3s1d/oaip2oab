/*
 * oab.cpp
 *
 *  Created on: Aug 11, 2018
 *      Author: sid
 */

#include <cmath>

#include "oab.hpp"

void OAB::reset(void)
{
	polygon.clear();
}

void OAB::add(Coord &coord)
{
	oab_edge_t point;
	point.lat_rad = deg2rad(coord.latitude);
	point.lon_rad = deg2rad(coord.longitude);

	polygon.push_back(point);
}


