/*
 * MyPrint.h
 *
 *  Created on: Oct 6, 2016
 *      Author: wisnia
 */

#ifndef MYPRINT_H_
#define MYPRINT_H_


class Print{
public:
	Print();
	virtual void print(const char*){};
	virtual void println(const char*){};
	virtual ~Print();
};

class MyPrint: public Print {
public:
	MyPrint();
	virtual ~MyPrint();
	void print(const char*);
	void println(const char*);
};


#endif /* MYPRINT_H_ */
