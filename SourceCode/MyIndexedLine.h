
/**
 *
 * Line representation with two indices
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2015
 *
 *
 */

#ifndef __My_Indexed_Line__
#define __My_Indexed_Line__

 //namespace CVSystem
 //{
struct MyIndexedLine
{
public:
	// start index
	int index0;

	// end index
	int index1;

	// Constructor
	MyIndexedLine(int index0, int index1)
	{
		this->index0 = index0;
		this->index1 = index1;
	}

	// Construction without parameters
	MyIndexedLine()
	{
		this->index0 = -1;
		this->index1 = -1;
	}
};
//}

#endif
