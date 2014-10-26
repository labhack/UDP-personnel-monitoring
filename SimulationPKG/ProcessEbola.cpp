/*
*
*		LabHack: Ebola-is-Fun
*		10/25/14 - 6:30PM
*
*		Run Human.H
*
*		GNU GPL License
*/

#include "human.h"

using namespace std;


int main()
{

	DataGather Ebola;
	int i(0);
	double dIn;
	ifstream input;

	input.open("outbreakdata.txt");

	if (input.fail())
		cout << "ERROR: FAIL TO OPEN";


		while (!input.eof())
		{

			//store each element to the array
			for (int j(0); j < 8; j++)
			{
				input >> dIn;
				if (dIn == 0)
					break;
				Ebola.addData(i, dIn);

			}
			i++;


		}
	{


	}
	input.close();
	Ebola.Determine();


	return 0;

}