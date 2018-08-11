/*
 * coord.hpp
 *
 *  Created on: Aug 11, 2018
 *      Author: sid
 */

#ifndef SRC_COORD_HPP_
#define SRC_COORD_HPP_


class Coord
{
public:
	float latitude;
	float longitude;

	Coord()	: latitude(0), longitude(0) { }
	Coord(float latitude, float longitude) : latitude(latitude), longitude(longitude) { }
	~Coord() { }
};


#endif /* SRC_COORD_HPP_ */
