/*
*
*		LabHack: Ebola
*		10/25/14 - 3:30PM
*
*		Class DataGather definition
*
*		GNU GPL License
*/


#include "Human.h"

using namespace std;


//Default Constructor
DataGather::DataGather()
{
	data.resize(1);
}

//add data to doulble array
void DataGather::addData(int line, double in)
{	
		if (line > 0)
			data.resize(data.size() + 1);
		data[line].push_back(in);
	

}

//Scan for Alerts
void DataGather::Determine()
{

	for (int i(0); i < data.size() - 1; i++)
	{

		cout << i << endl;

		if (data[i][4] >= ECG_THRESH_H || data[i][4] <= ECG_THRESH_L)
		{
			for (int j(0); j < 60; j++)
				TransmitAll(i + j);
			i += 60;
		}
		else if (data[i][5] <= O2_THRESH_L)
		{
			for (int j(0); j < 60; j++)
				TransmitAll(i + j);
			i += 60;
		}
		else if (data[i][6] >= RESP_THRESH_H || data[i][6] <= RESP_THRESH_L)
		{
			for (int j(0); j < 60; j++)
				TransmitAll(i + j);
			i += 60;
		}
		else if (data[i][7] >= TEMP_THRESH_H || data[i][7] <= TEMP_THRESH_L)
		{
			for (int j(0); j < 60; j++)
				TransmitAll(i + j);
			i += 60;
		}
		else if ((i / 12) == 0)
			TransmitAll(i);

	}
}

//Transmits one line of data
void DataGather::TransmitAll(int line)
{
	
		fstream out;

		out.open("ebola.ebola", fstream::out | fstream::app);

		for (int i(0); i < 8; i++)
		{

			if (i == 0)
				out << (unsigned int) data[line][i] << " ";
			else if (i >0 || i < 4)
				out << data[line][i] << " ";
			else
				out << (unsigned int) data[line][i] << " ";

		}

		out << endl;

		out.close();


}

//outdate: prints all data in the vectors
//void DataGather::print()
//{
//
//	/*ofstream out("ebola.bin",ios::out | ios::binary);
//	unsigned int temp;*/
//
//	ofstream out;
//
//	if (out.fail())
//		cout << "ERROR: FAILD TO WRITE FILE!";
//
//	for (int i(0); i < data.size(); i += 12)
//	{
//
//		for (int j(0); j < data[i].size(); j++)
//		{
//
//			if (j == 0)
//				out << (unsigned int) data[i][j] << " ";
//			/*{
//				temp = (unsigned int) data[i][j];
//				out.write((char*) &temp, sizeof temp);
//				}*/
//			else if (j >0 || j < 4)
//				out << data[i][j] << " ";
//			//out.write((char*) &data[i][j], sizeof data[i][j]);
//			else
//				out << (unsigned int) data[i][j] << " ";
//			/*{
//				temp = (unsigned int) data[i][j];
//				out.write((char*) &temp, sizeof temp);
//				}*/
//
//		}
//
//		out << endl;
//	}
//
//	out.close();
//}




