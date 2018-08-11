/*
 * oab.hpp
 *
 *  Created on: Aug 11, 2018
 *      Author: sid
 */

#ifndef SRC_OAB_HPP_
#define SRC_OAB_HPP_

#define deg2rad(angleDegrees) (angleDegrees * M_PI / 180.0)
#define rad2deg(angleRadians) (angleRadians * 180.0 / M_PI)

/*
 * Format:
 * 3byte char     'OAB'
 * 1byte uint8_t  version number, currently 1
 *
 * Airspace:
 * Header:
 * 31byte char     name
 * 1byte  char     type
 * 2byte  int16_t  top altitude ft
 * 2byte  int16_t  bottom altitude ft
 * 4byte  float    top (latitude rad)
 * 4byte  float    bottom (latitude rad)
 * 4byte  float    left (longitude rad)
 * 4byte  float    right (longitude rad)
 * 2byte  uint16_t  flags
 * 2byte  uint16_t number of polygons
 * --------------------------------------
 * 56bytes
 *
 * Polygon(only, last closing point is not present):
 * 4byte float latitude rad
 * 4byte float longitude rad
 * --------------------------
 * 8bytes
 */

#include <vector>

#include "coord.hpp"

class OAB
{
public:
	const char *id = "OAB\x1";
 	typedef struct
	{
		/* name */
		char name[31];
		char type;

		/* altitude */
		int16_t altitudeTop_ft;
		int16_t altitudeBottom_ft;

		/* bounding box */
		float topLat_rad;
		float bottomLat_rad;
		float leftLon_rad;
		float rightLon_rad;

		uint16_t flags;
		uint16_t numPoly;
	} __attribute__((packed)) oab_header_t;

	typedef struct
	{
		float lat_rad;
		float lon_rad;
	} __attribute__((packed)) oab_edge_t;

private:

public:
	oab_header_t header = {{0}};
	std::vector<oab_edge_t> polygon;

	void reset(void);
	void add(Coord &coord);
};



#endif /* SRC_OAB_HPP_ */
