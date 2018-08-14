#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include "crawler.h"

using namespace std;

void read_config (char* config, string &index, string &output)
{
	//open the config file
	ifstream input(config);
	string x;
	while(getline(input,x))
	{
		int i = 0 ;
		stringstream ss; 
		while(i < (int)x.length())
		{
			//if you find a comment just skip over it
			if(x[i]=='#')
				break;
			ss << x[i];
			i++;
			//if = there is a decleration of a variable
			if(x[i]=='=')
			{
				i++;
				string command;
				ss >> command;
				ss.clear();
				//for the index of the crawler
				if(command == "INDEX_FILE")
				{
					while(i <= (int)x.length())
					{
						//if you find a commnet put the index file first
						//and you go over the next line or if you
						//reach the end of a line
						if(i ==(int) x.length() || x[i] == '#')
						{
							string in;
							ss >> in;
							index = in;
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
				//for the output of the crawler
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
				//not a variable we need 
				//then just skip the line
				else
					break;
			}
		}
	}
	//close file
	input.close();
}


int main(int argc, char* argv[])
{
	string index;
	string output;

	read_config(argv[1], index, output);

	Crawler crawler;
	//for the result of the crawler
	ofstream out(output.c_str());
	//for the input of the crawler
	ifstream in(index.c_str());
	//the set is for checking if there are any duplicates 
	//webpages
	set<string> set;
	string y;
	//assuming every seed file exist
	while(getline(in, y))
	{
		if(set.find(y) == set.end())
		{
			//insert the seed file to the set
			//so there are no duplicates
			set.insert(y);
			//print it on the output file
			out << y << endl;
			//crawl through the webpage
			crawler.crawl(y,set,out);
		}
	}
	//close files
	in.close();
	out.close();
}