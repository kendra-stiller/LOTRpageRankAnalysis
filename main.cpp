#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "web.hpp"

using namespace std;

//----------------------------------------------------------------------
//COMPILE MAIN.CPP AND WEB.CPP
//
//COMMAND LINE: a./ [NAMEFILE].txt [BOOKTEXTFILE].txt
//		[NAMEFILE].txt
//			the namefile should be a file with the name of each character 
//			on a new line
//		[BOOKTEXTFILE].txt
//			the full text of the book, in the hungergames example we had
//			from HW1 everything was lowercase, makes things easier
//----------------------------------------------------------------------

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		cout << "incorrrect arguments" << endl;
		return 0;
	}
	else
	{
		web hgg(argv[1], argv[2]);//create the web
		hgg.display();//display the characters
		hgg.update_matrix();//create adjacency matrix
		hgg.print_matrix();//print the adjacency matrix
		return 0;
	}
}


