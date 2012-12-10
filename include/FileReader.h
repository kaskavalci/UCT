/*
 * FileReader.h
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#ifndef FILEREADER_H_
#define FILEREADER_H_

#include "Common.h"

namespace std {

class FileReader {
public:
	FileReader();
	void read();
private:
	Common *conf;
	void readreq();
	void readcourses();
	void readinputparam();
};

}

#endif /* FILEREADER_H_ */
