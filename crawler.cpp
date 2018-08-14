#include "crawler.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

using namespace std;

Crawler :: Crawler(){}
Crawler :: ~Crawler(){}

void Crawler :: crawl(string index, set<string> &unique, ofstream &out)
{
	//open the seed file
	ifstream in(index.c_str());
	string x;
	while(getline(in,x))
	{
		//parse thorugh the webpages
		for(unsigned int i = 0 ; i < x.length(); i++)
		{
			stringstream ss;
			if(x[i]=='(')
			{
				//if links found
				++i;
				while(x[i]!=')')
				{
					//get all the links
					ss << x[i];
					i++;					
				}
			}
			while(ss)
			{
				string next;
				ss >> next;
				//make sure we only operate on the links
				//not on spaces
				if(next.length()>0)
				{
					//check if a specific webpage exist
					ifstream exist(next.c_str());
					//if it does not exist
					//dont print it and do not open it or parse it
					if(unique.find(next)== unique.end()&& !exist.fail())
					{
						//webpage exist
						//By definition of BFS we print the 
						//name of the webpage and traverse 
						//through the graph
						out << next << endl;
						//make sure there are no duplicates
						unique.insert(next);
						crawl(next, unique, out);
					}
					//clsoe file
					exist.close();
				}
			}
		}
	}
	//clsoe file
	in.close();
}

