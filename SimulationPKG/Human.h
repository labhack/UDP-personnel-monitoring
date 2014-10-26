/*
*
*		LabHack: Ebola
*		10/25/14 - 1:30PM
*		Developer: Ben Natarian
*		Class DataGather delcaration
*		
*		GNU GPL License 
*/

#ifndef HUMAN_H
#define HUMAN_H


#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class DataGather
{

private:
	
	vector<vector<double>> data;
	ifstream input;
	const unsigned int ECG_THRESH_H = 150;
	const unsigned int ECG_THRESH_L = 40;
	const unsigned int O2_THRESH_L = 95;
	const unsigned int RESP_THRESH_H = 30;
	const unsigned int RESP_THRESH_L = 8;
	const unsigned int TEMP_THRESH_H = 100;
	const unsigned int TEMP_THRESH_L = 96;


	void TransmitAll(int line);

public:

	DataGather();
	void addData(int line, double in);
	void Determine();
	void print();

};

#endif
