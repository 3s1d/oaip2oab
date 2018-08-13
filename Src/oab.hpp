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

//todo type mapping! enum!

/*
 * Format:
 * 3byte char      'OAB'
 * 1byte uint8_t   version number, currently 1
 * 8byte time_t	   built time
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

#define OBA_ALTREF_GND			0x0001
#define OBA_ALTREF_MSL			0x0002
#define OBA_ALTREF_FL			0x0003

#define OBA_ALTREF_BOTTON_OFFSET	0
#define OBA_ALTREF_TOP_OFFSET		3		//1bit reserved

class OAB
{
public:
	typedef enum : char
	{
		CLASSA = 'A',
		CLASSB = 'B',
		CLASSC = 'C',
		CLASSD = 'D',
		CLASSE = 'E',
		CLASSF = 'F',
		CLASSG = 'G',
		DANGER = 'Q',
		PROHIBITED = 'P',
		RESTRICTED = 'R',
		CTR = 'c',
		TMA = 'a',
		TMZ = 'z',
		RMZ = 'r',
		FIR = 'f', 			// from here on we drop everything
		UIR = 'u',
		OTH = 'o',
		GLIDING = 'g',
		NOGLIDER = 'n',
		WAVE = 'W',
		UNKNOWN = '?', 			// "UNKNOWN" can be used in OpenAir files
		UNDEFINED = '?',		// also the last one
		IGNORE = '-'
	} airspace_type_t;

	const char *id = "OAB\x1";
 	typedef struct
	{
		/* name */
		char name[31];
		airspace_type_t type;

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
	void finalize(void);
	void add2RadVec(Coord &coord);

	/* last position */
	Coord last_p;

	/* last inored position */
	Coord last_ign_p;
	bool last_ign;


public:
	oab_header_t header = {{0}};
	std::vector<oab_edge_t> polygon;	//rad
	float polygonSample_dist = 0.0f;

	void reset(void);
	void setName(std::string &name);
	void add(Coord &coord);

	void writeFileHeader(std::ofstream &file);
	void write(std::ofstream &file);
};



#endif /* SRC_OAB_HPP_ */
