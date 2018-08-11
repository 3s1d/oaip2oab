/*
 * oaip2oab.cpp
 *
 *  Created on: Aug 11, 2018
 *      Author: sid
 */


#include <iostream>

#include "aiphandler.hpp"
#include "xml.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	cerr << "openAIP to open air binary (OAB) converter" << endl; // prints API to AIR converter

	if (argc != 2)
	{
		cerr << "usage: " << argv[0] << " input.aip " << endl;
		return 1;
	}

	aip::handler aiphandler;
	xml::parser < aip::handler > parser(aiphandler);
	parser.parse(argv[1]);

	return 0;
}


