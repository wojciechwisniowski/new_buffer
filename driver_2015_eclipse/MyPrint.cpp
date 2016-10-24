/*
 * MyPrint.cpp
 *
 *  Created on: Oct 6, 2016
 *      Author: wisnia
 */

#include "MyPrint.h"
//#include<iostream>
#include <stdio.h>

MyPrint::MyPrint() {
	// TODO Auto-generated constructor stub

}

MyPrint::~MyPrint() {
	// TODO Auto-generated destructor stub
}

Print::Print() {
	// TODO Auto-generated constructor stub

}

Print::~Print() {
	// TODO Auto-generated destructor stub
}

void MyPrint::print(const char* s) {
	printf("%s", s);
}
void MyPrint::println(const char* s) {
	printf("%s\n", s);
}

