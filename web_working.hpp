#ifndef WEB_WORKING_HPP
#define WEB_WORKING_HPP

#include<vector>
#include<iostream>


struct character;

//	RELATIONSHIP- EDGE OF GRAPH
struct relationship
{ 
	character *next;
	int weight;
};

//	CHARACTER- VERTEX/NODE OF GRAPH
struct character
{
	int ID;
	std::vector <std::string> name;
	int mentions;
	std::vector <relationship> edges;
	int totalweight;
};

//	CHECKER_QUEUE- USED FOR CREATING GRAPH EDGES FROM BOOK
class checker_queue
{
	private:
		static const int scope = 15;
		character* list[scope];
		int current;
		int last;
		void add_edge(character* a, character* b);
	
	public:
		checker_queue();
		bool is_full();
		void push(character* insert);
		void pop_and_lock();
		
};

// WEB- GRAPH
class web
{
	public:
		web(char* namefile, char* textfile);
		~web();
		
		void display();
		void print_adjmatrix();
		void print_by_mentions();
		void print_by_connections();
		void print_Smatrix();
		void print_ranks();
		void calc_rank(int iterations);
		void LaTexAdj();
		void LaTexS();
		
		
		
	private:
	
		std::vector <character> chars;
		std::vector< std::vector <int> > relationship_matrix;
		std::vector< std::vector <double> > S_matrix;
		std::vector<double> ranks;
		
		int is_character(std::string word);
		void add_char(int ID, std::vector <std::string> name);
		void add_relationship_weight(character* a, character* b);
		void assign_relationships(char* book);
		void getnames(char* filename);	
		void update_totalweights();
		void update_adjmatrix();
		void update_Smatrix();
		
		
		
		
};


#endif // WEB_HPP
