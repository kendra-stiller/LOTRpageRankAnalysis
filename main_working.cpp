#include <iostream>
#include <fstream>
//#include <vector>
#include <string>

#include "web_working.hpp"

using namespace std;

//----------------------------------------------------------------------
//COMPILE MAIN_WORKING.CPP AND WEB_WORKING.CPP
//
//COMMAND LINE: a./ [NAMEFILE].txt [BOOKTEXTFILE].txt
//		[NAMEFILE].txt
//			the namefile should be a file with the name of each character 
//			on a new line
//		[BOOKTEXTFILE].txt
//			the full text of the book, in the hungergames example we had
//			from HW1 everything was lowercase, makes things easier
//----------------------------------------------------------------------


void displaymenu();


int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		cout << "incorrrect arguments" << endl;
		return 0;
	}
	
	else
	{
		web graph(argv[1], argv[2]);//create the web
	
		bool quit = false;
		string menu_option;
		int input;
		
		while(!quit)
		{
			displaymenu();
			getline(cin, menu_option);
			input = stoi(menu_option);
			switch(input)
			{
				case 1:
				{
					graph.display();
				}
				break;
				
				case 2:
				{
					//graph.update_adjmatrix();
					graph.print_adjmatrix();
				}
				break;
				
				case 3:
				{
					graph.print_by_mentions();
				}
				break;
				
				case 4:
				{
					graph.print_by_connections();
				}
				break;
				
				case 5:
				{
					//graph.update_Smatrix();
					graph.print_Smatrix();
				}
				break;
				
				case 6:
				{
					cout << "iterations?"<< endl;
					std::string iter;
					getline(cin, iter);
					graph.calc_rank(stoi(iter));
					graph.print_ranks();
				}
				break;
				
				case 7:
				{
					graph.LaTexAdj();
				}
				break;
				
				case 8:
				{
					graph.LaTexS();
				}
				break;
				
				case 9:
				{
					quit = true;
					cout << "Goo-bye" << endl;
				}
				break;
			}
		}
		
		return 0;
	}
}


void displaymenu()
{
	cout << "Select an option" << endl;
	cout << "1) Display Characters and Relationships" << endl;
	cout << "2) Display Adjacency Matrix" << endl;
	cout << "3) Rank by Mentions" << endl;
	cout << "4) Rank by Connectedness" << endl;
	cout << "5) Display Stochastic Matrix" << endl;
	cout << "6) Display ranks" << endl;
	cout << "7) Print_for_LaTex - Adjacency" << endl;
	cout << "8) Print_for_LaTex - Stochaistic" << endl;
	cout << "9) Quit" << endl;
}






