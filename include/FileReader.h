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
	FileReader(Common *c);
	void readreq();
	void readcourses();
	void readinputparam();
	void readContrictions(const char *fname);
private:
	Common *conf;
};

}

#endif /* FILEREADER_H_ */
