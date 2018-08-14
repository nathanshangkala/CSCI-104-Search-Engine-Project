#include<iostream>
#include<vector>
#include<set>
#include<stack>
#include<string>
#include<sstream>
#include<fstream>
#include"search_engine.h"
#include"webpage.h"
#include"page_rank.h"

using namespace std;

//reads the config file for the search engine & page rank
void read_config (char* config, int &step_number, double &probability, string &index,string &query,string &output)
{
	ifstream input(config);
	string x;
	while(getline(input,x))
	{
		int i = 0 ;
		stringstream ss; 
		while(i < (int)x.length())
		{
			//if you see a comment go to next line
			if(x[i]=='#')
				break;
			ss << x[i];
			i++;
			//a varable was declared
			if(x[i]=='=')
			{
				i++;
				string command;
				ss >> command;
				ss.clear();
				//get the restart constant
				if(command == "RESTART_PROBABILITY")
				{
					while(i <= (int)x.length())
					{
						if(i ==(int) x.length() || x[i] == '#')
						{
							float in;
							ss >> in;
							probability = in;
							i++;
							break;
						}
						else
						{
							ss << x[i];
							++i;
						}
					}
				}
				//get the step number constant
				else if(command == "STEP_NUMBER")
				{
					while(i <= (int)x.length())
					{
						if(i == (int)x.length() || x[i] == '#')
						{
							int out;
							ss >> out;
							step_number = out;
							i++;
							break;
						}
						else
						{
							ss << x[i];
							++i;
						}
					}

				}
				//get the index file
				else if(command == "INDEX_FILE")
				{
					while(i <= (int)x.length())
					{
						if(i == (int)x.length() || x[i] == '#')
						{
							string out;
							ss >> out;
							index = out;
							i++;
							break;
						}
						else
						{
							ss << x[i];
							++i;
						}
					}

				}
				//get the query file
				else if(command == "QUERY_FILE")
				{
					while(i <= (int)x.length())
					{
						if(i == (int)x.length() || x[i] == '#')
						{
							string out;
							ss >> out;
							query = out;
							i++;
							break;
						}
						else
						{
							ss << x[i];
							++i;
						}
					}

				}
				//get the output file
				else if(command == "OUTPUT_FILE")
				{
					while(i <= (int)x.length())
					{
						if(i == (int)x.length() || x[i] == '#')
						{
							string out;
							ss >> out;
							output = out;
							i++;
							break;
						}
						else
						{
							ss << x[i];
							++i;
						}
					}

				}
				//no desired varriable 
				else
					break;
			}
		}
	}
	input.close();
}



void search(const char *index,const  char *query,const  char *output, int step_number, float probability)
{
	//initialize a webpage class
	//that holds all data for the search
	WebPage page;
	//initialize pagerank
	PageRank rank;
	ifstream input(index);
	string x;
	while (getline(input,x))
	{
		//open every file
		ifstream filepath(x.c_str());
		string y;
		//if the file exist
		if(!filepath.fail())
		{
			//parse it
			while(getline(filepath,y))
				parse(y,x,page);
		}
		filepath.close();
 	}
 	//get all incoming of all webpages parsed
 	get_incoming(page);
 	//read query
 	ifstream operations(query);
 	//write output
 	ofstream out(output);
 	string line;
 	while(getline(operations, line))
 	{
 		//read thorugh every line of query
 		if(line.length()>0)
 		{
 			//number of words operated
 			vector <string> operate;
			stringstream ss;
			string command = "";
	 		for(unsigned int i =0 ; i < line.length(); i++)
	 			ss << line[i];
	 		//get the command
	 		ss >> command;
 			while(ss)
 			{
 				if(command == "AND" || command == "OR")
 				{
 					string temp;
 					string word;
 					ss>> word;
 					for(unsigned int k = 0;  k < word.length() ; k++)
 					{
 						char c = tolower(word[k]);
 						temp+= c;
 					}
 					if(word.length()>0)
	 					operate.push_back(temp);
 				}
 				else
 				{
 					string word;
 					ss >> word;
 					if(word.length()>0)
 					{
 						operate.push_back(word);
 					}
 				}
 			}
	 		if(operate.size() == 0)
	 		{
	 			//if there is only one word
	 			string temp;
	 			for(unsigned int i = 0 ; i < command.length(); i++)
	 				temp+=tolower(command[i]);
	 			//find all webapges containing that word
	 			set<string> result = single(temp, page);
	 			//rank the result
	 			rank.page_rank(page,result, step_number, probability, out);
	 		}
	 		else if(command == "AND")
	 		{
	 			//find all webpages that have all the words
	 			set<string> result =intersectString(operate,page);
	 			rank.page_rank(page,result, step_number, probability, out);

	 		}
 			else if(command == "OR")
 			{
	 			//find all webpages that have a  word in query
 				set<string> result = unionString(operate, page); 
	 			rank.page_rank(page,result, step_number, probability, out);
 			}
	 		else if(command =="INCOMING")
	 		{
	 			//if more than one webpages was given
	 			if(operate.size()>1)
	 				out << "Invalid query" << endl;
	 			else
	 			{
	 				//find incoming of a webpage
	 				string target = *operate.begin();
	 				incoming(target,out,page);
	 			}
	 			
	 		}
	 		else if(command == "OUTGOING")
	 		{
	 			//if more than one webpages was given
	 			if(operate.size()>1)
	 				out << "Invalid query" << endl;
	 			else
	 			{
	 				//find outcoming of a webapge
		 			string target = *operate.begin();
		 			outcoming(target, out,page);
		 		}
	 		}
	 		else if(command == "PRINT")
	 		{
	 			//if more than one webpage is given
	 			if(operate.size()>1)
	 				out << "Invalid query" << endl;
	 			else
	 			{
	 				//print content of a webpage
		 			string target = *operate.begin();
		 			print(target, page, out);
		 		}
	 		}
	 		else
	 			//invalid query
	 			out << "Invalid query" << endl;
 		}
 	}
 	input.close();
 	operations.close();
 	out.close();
}

int main (int argc, char *argv[])
{
	//all variables needed for search
	int step_number=0;
	double probability=0;
	string index;
	string query;
	string output;
	//get all variables needed
	read_config(argv[1], step_number, probability, index, query, output);
	//do the serach
	search(index.c_str(),query.c_str(),output.c_str(), step_number, probability);
}