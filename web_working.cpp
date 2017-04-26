#include <fstream>
#include <queue>
#include <sstream>
#include <iomanip>
	
#include "web_working.hpp"

using namespace std;	

/*
 * ---------------------------------------------------------------------
 * CHECKER QUEUE CLASS
 * 
 * -used in web::assign_relationships() only
 * 
 * -The "checker queue" as I'm calling it for now is pretty much a basic
 * 		queue, with one added featrure.
 * 
 * -It is used when creating the edge weights while constructing 
 * 		the graph. Essentially it is the window of 15 words (we can change
 * 		that number by changing "scope") around a name to see what names are 
 * 		close. It should ideally always be full, except before we've gotten 
 * 		more than 15 words into the document 
 * 
 * -Each time a name/character is dequeued, the rest of the elements in the
 * 		queue are checked, and if they are other characters, the weight of 
 * 		the edge between those characters is increased
 * 
 * -It is assumed an element of the queue is either a pointer to a character 
 * 		included in the graph or a null pointer representing an unimportant word
 * ---------------------------------------------------------------------
 * 
 */


//	CONSTRUCTOR -CHECKER_QUEUE
// 		initializes the queue elements and indicies for an empty queue
	checker_queue::checker_queue()
	{
		for(int i = 0; i < scope; i++)
		{
			list[i] = NULL;
		}
		
		current = 0;
		last = 0;
	}
	
//	IS_FULL()
//		checks to see if queue is full, to know when to pop
	bool checker_queue::is_full()
	{
		if(current+1 == last)
		{return true;}
		else if (current == scope-1 && last == 0)
		{return true;}
		return false;
	}

//	PUSH()
//		inserts pointer (either to character or null) into the queue
	void checker_queue::push(character* insert)
	{
		current++;
		
		if(current == scope)
		{
			current = 0;
		}
		
		if(current == last)
		{
			cout << "HOLDUP";
		}
		else
		{
			list[current] = insert;
		}
	}

//	POP_AND_LOCK()
//		pops and element from the queue in the traditional sense but 
// 		additionally checks other queue elements in order to adjust 
//		weight of edges between characters

	void checker_queue::pop_and_lock()
	{
		
		if(list[last] != NULL)
		{
			list[last]->mentions++;
		
			for( int a = 0; a < scope; a++)
			{
				if(list[a] != list[last] && list[a] != NULL)
				{
					add_edge(list[a],list[last]);
					add_edge(list[last],list[a]);
				}
			}
			
			list[last] = NULL;
		}

		last++;
		
		if(last == scope)
		{
			last = 0;
		}
	}
	
	
//	ADD_EDGE()
//		called above in pop_and_lock(), just adds or strengthens and edge
// 		between two vertices/characters
	void checker_queue::add_edge(character* a, character* b)
	{
		if(a == NULL || b == NULL)
		{
			cout << "ERROR ON ADDING EDGE - NULL CHARACTER" << endl;
		}
		
		else
		{
			bool already_there = false;
			
			for(unsigned int i = 0; i < a->edges.size(); i++)
			{
				if(a->edges[i].next == b)
				{
					a->edges[i].weight++;
					already_there = true;
					break;
				}
			}
			
			if(!already_there)
			{
				relationship insert;
				insert.next = b;
				insert.weight = 1;
				a->edges.push_back(insert);
				
			}
		}
	}

	
/*
 * ---------------------------------------------------------------------
 * WEB CLASS
 * 
 * -Its just the graph, called it the web because I already had a graph.cpp
 * 		from another homework
 * 
 * -Each Vertex is a character
 * 
 * -Each edge has a weight corresponding to the amount of times the
 * 		connected vertices are mentioned within 15 ("scope") words of 
 * 		each other
 * 
 * -I pretty much implemented both methods of edges- the vector method
 * 		similar to the graph homework is the main one
 * 
 * -the adjacency matrix is secondary- it is constructed using the vector 
 * 		method from before
 * 
 * -
 * ---------------------------------------------------------------------
 */
 
 //	DESTRUCTOR
	web::~web()
	{}
	
//	CONSTRUCTOR-WEB(NAMEFILE, TEXTFILE)
//		calls functions to initialize 
//			1) the vertices using the file of names
//			2) the edges using the full book text
	web::web(char* namefile, char* textfile)
	{
		getnames(namefile);
		assign_relationships(textfile);
		update_totalweights();
		update_Smatrix();
		update_adjmatrix();
		//make and fill ranks vector with 1's
		for(unsigned int i=0; i<chars.size(); i++)
		{
			ranks.push_back(1.0);
		}
	}
	
//	IS_CHARACTER(WORD)
//		checks a word to see if it is the name of a character that has 
//		already been put in the web
	int web::is_character(std::string word)
	{
		for(unsigned int i = 0; i < chars.size(); i++)
		{
			for(unsigned int j =0; j < chars[i].name.size(); j++)
			{
				if(word == chars[i].name[j] || word == chars[i].name[j]+"s")
				{return i;}
			}
		}
		
		return -1;
	}
	
//	ASSIGN_RELATIONSHIPS(FULLTEXT)
//		uses the text file to define relationships/edges between characters
//		uses the checker_queue class
//
//		POSSIBLE ISSUES: 
//			-nicknames/titles
//			-possesives i.e. Gandalf's != Gandalf
	void web::assign_relationships(char *FileText)
	{
		ifstream book;
		string word;
		book.open(FileText);
		if(book.is_open())
		{
			checker_queue checker;
			
			
			//look at every word in the book
			while(book >> word)
			{
				
				int index = is_character(word);//check each word
				
				if( index == -1 )//if it's not a character
				{
					checker.push(NULL);//queue a NULL pointer 
				}
				
				else // if it is a character
				{
					checker.push(&chars[index]); //queue pointer to character
				}
				
				if(checker.is_full())// when the queue is full, dequeue
				{
					checker.pop_and_lock();//creates/strengthens edges
				}
			}
		}
		
		else
		{
			cout << "ERROR IN OPENING BOOK" << endl;
		}
	}

//	GETNAMES(NAMEFILE)
//		uses the name file to insert all the characters in the web
	void web::getnames(char* FileName)
	{

		 ifstream File;
		// std::vector<std::string> name;
		 //stringstream ss;
		 string line;
		 string temp;
		 File.open(FileName);
		 
		 if (File.is_open())
		 {
			 int id = 0;
			 
			 while(getline(File, line))
			 {
				stringstream ss(line);
				std::vector<std::string> name;
				while(getline(ss, temp, ','))
				{
					name.push_back(temp);
				}
				add_char(id, name);
				id++;
			}
			File.close();
		 }
		 
		 //Error message
		 else
		 {
			 cout<< "Something went wrong with reading the name file"<<endl;
		 }
		
	}
	
//	ADD_CHAR(ID, NAME)
//		adds a new character to the graph
	void web::add_char(int ID, std::vector <std::string> name)
	{
		character insert;
		insert.name = name;
		insert.ID = ID;
		insert.mentions = 0;
		insert.totalweight=0;
		chars.push_back(insert);
	}
	
//	ADD_RELATIONSHIP_WEIGHT(TWO CHARACTERS)
//		not actually used anywhere because the checkerqueue takes care 
//		of all the same functionallity
	void web::add_relationship_weight(character* a, character* b)
	{
		if(a == NULL || b == NULL)
		{
			cout << "ERROR ON ADDING EDGE - NULL CHARACTER" << endl;
		}
		
		else
		{
			bool already_there = false;
			
			for(unsigned int i = 0; i < a->edges.size(); i++)
			{
				if(a->edges[i].next == b)
				{
					a->edges[i].weight++;
					already_there = true;
					break;
				}
			}
			
			if(!already_there)
			{
				relationship insert;
				insert.next = b;
				insert.weight = 1;
				a->edges.push_back(insert);
				
			}
		}
	}
	
// 	DISPLAY()
//		Displays each character, how many time's he/she is mentioned, and 
//		his/her relation to other characters
	void web::display()
	{
		for(unsigned int x = 0; x < chars.size(); x++)
		{
			cout << endl << "(" << chars[x].ID << ") ";
			for(unsigned int y = 0; y < chars[x].name.size(); y++)
			{
				cout << chars[x].name[y] << ", ";
			}
			cout << " mentioned " << chars[x].mentions << " times" << endl;
			cout << "\t";
			
			for (unsigned int y = 0; y < chars[x].edges.size(); y++)
			{
				cout << chars[x].edges[y].next->name[0] << ":" << chars[x].edges[y].weight << "," ;
			}
		}
		
		cout << endl << endl;
	}
		
//	PRINT_MATRIX()
//		prints out the adjacency matrix. formating can get weird if it's big		
	void web::print_adjmatrix()
	{
		
		cout << endl;
		
		//uncomment below for badly formatted column labels
		
		/*
		for(unsigned int i = 0; i < chars.size(); i++)
		{
				cout << chars[i].name << "\t";
		}
		*/
		
		for(unsigned int i = 0; i < relationship_matrix.size(); i++)
		{
			cout << endl   << "[ \t" ;
			for(unsigned int j = 0; j < relationship_matrix.size(); j++)
			{
				cout << relationship_matrix[i][j] << "\t";
			}
			cout << " ]\t" << chars[i].name[0] << endl;
		}
		cout << endl;
	}

//	UPDATE_MATRIX()
//		-must be called to create the adjacency matrix from the edge vectors. 
//		-should be symetric.
	void web::update_adjmatrix()
	{
		relationship_matrix.clear();
		
		
		//create clear matrix
		for(unsigned int i = 0; i < chars.size(); i++)
			{
				relationship_matrix.push_back({});
				for(unsigned int j = 0; j < chars.size(); j++)
				{
					relationship_matrix[i].push_back(0);
				}
			}	
			
			
		//update entries
		for(unsigned int x = 0; x < chars.size(); x++)
		{
			
			for (unsigned int y = 0; y < chars[x].edges.size(); y++)
			{
				relationship_matrix[chars[x].ID][chars[x].edges[y].next->ID] = chars[x].edges[y].weight;
				
			}
		}
	}

void web::print_by_mentions()
{
	//create vector of pointers to characters
	std::vector <character*> pointers;
	for(unsigned int i = 0; i < chars.size(); i++)
	{
		pointers.push_back(&chars[i]);
	}
	
	//Insertion sort the pointers vector
	unsigned int j;
	character* temp;

	for (unsigned int i = 0; i < pointers.size(); i++) 
	{ 
		j = i;
		while (j > 0 && pointers[j]->mentions > pointers[j-1]->mentions) 
		{ 
			temp = pointers[j];
			pointers[j] = pointers[j-1];
			pointers[j-1] = temp;
			j--;
		} 
	}
	
	//Print them in order
	for(unsigned int i = 0; i < chars.size(); i++)
	{
		cout << i << ". " << pointers[i]->name[0] << " mentioned " << pointers[i]->mentions << " times" <<endl;
	}
	
	
	cout<<endl;
}

void web::update_totalweights()
{
	for (unsigned int i=0; i < chars.size(); i++)
	{
		chars[i].totalweight = 0;
		
		for (unsigned int j=0; j < chars[i].edges.size(); j++)
		{
			chars[i].totalweight = chars[i].totalweight + chars[i].edges[j].weight;
		}
	}
}


void web::print_by_connections()
{
	//create vector of pointers to characters
	std::vector <character*> pointers;
	for(unsigned int i = 0; i < chars.size(); i++)
	{
		pointers.push_back(&chars[i]);
	}
	
	//Insertion sort the pointers vector
	unsigned int j;
	character* temp;

	for (unsigned int i = 0; i < pointers.size(); i++) 
	{ 
		j = i;
		while (j > 0 && pointers[j]->totalweight > pointers[j-1]->totalweight) 
		{ 
			temp = pointers[j];
			pointers[j] = pointers[j-1];
			pointers[j-1] = temp;
			j--;
		} 
	}
	
	//Print them in order
	for(unsigned int i = 0; i < chars.size(); i++)
	{
		cout << i << ". " << pointers[i]->name[0] << " connected " << pointers[i]->totalweight << " times" <<endl;
	}
	
	
	cout<<endl;
}



void web::update_Smatrix()
{
	S_matrix.clear();
		
		
		//create clear nxn matrix
		for(unsigned int i = 0; i < chars.size(); i++)
			{
				S_matrix.push_back({});
				for(unsigned int j = 0; j < chars.size(); j++)
				{
					S_matrix[i].push_back(0);
				}
			}	
			
			
		//update entries
		for(unsigned int u = 0; u < chars.size(); u++)
		{
			
			for (unsigned int v = 0; v < chars[u].edges.size(); v++)
			{
				S_matrix[chars[u].edges[v].next->ID][u] = double (chars[u].edges[v].weight)/double(chars[u].totalweight);
				
			}
		}
}

void web::print_Smatrix()
{
		cout << endl;
		
		for(unsigned int i = 0; i < S_matrix.size(); i++)
		{
			cout << endl   << "[ \t" ;
			for(unsigned int j = 0; j < S_matrix.size(); j++)
			{
				cout << S_matrix[i][j] << "\t";
			}
			cout << " ]\t" << chars[i].name[0] << endl;
		}
		cout << endl;
}

void web::calc_rank(int iterations)
{
	//clear ranks vector, reassigns to 1
	ranks.clear();
	for(unsigned int i=0; i<chars.size(); i++)
		{
			ranks.push_back(1);
		}
		
	// create lhs vector->updated rank	
	std::vector <double> temp = ranks;
	int size = chars.size();
	double entry;
	
	//Matrix multiply S_matrix*rank = updated rank [iterations times]
	for(int a=0; a < iterations; a++)//iterate to improve convergence
	{
		for(int i = 0; i < size; i++)
		{
			entry = 0;
			
			for(int j = 0; j < size; j++)
			{
				entry = entry + S_matrix[i][j]*ranks[j];
			}
			temp[i] = 0.85*entry+0.15;
		}
		ranks = temp;
	}
	
	
	
}

void web::print_ranks()
{
	for(unsigned int i = 0; i < ranks.size(); i++)
	{
		cout << chars[i].name[0] << ":" << ranks[i] <<endl;
	}
}

void web::LaTexAdj()
{
	for(unsigned int i=0; i < relationship_matrix.size(); i++)
	{
		for(unsigned int j = 0; j < relationship_matrix[0].size(); j++)
		{
			cout<< relationship_matrix[i][j];
			if(j!=relationship_matrix.size()-1)
			{
				cout << " & ";
			}
		}
		cout<< " \\\\" <<endl;
	}
}

void web::LaTexS()
{
	for(unsigned int i=0; i < S_matrix.size(); i++)
	{
		for(unsigned int j = 0; j < S_matrix[0].size(); j++)
		{
			cout<< fixed << std::setprecision(3) <<S_matrix[i][j];
			
			if(j!=S_matrix.size()-1)
			{
				cout << " & ";
			}
		}
		cout<<"\\\\"<<endl;
	}
}
		
