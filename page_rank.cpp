#include "page_rank.h"
#include<iostream>
#include<functional>
#include<algorithm>
#include<string>
#include<sstream>
#include<fstream>

using namespace std;

PageRank :: PageRank(){}
PageRank :: ~PageRank(){}

void PageRank :: page_rank(WebPage &page, set<string> &result, int step_number, double probability, ofstream &out)
{
	//expanding the candidate set
	set<string> expand;
	//for every webpage of the result set
	for(set<string> :: iterator it = result.begin(); it != result.end(); it++)
	{
		//get incoming
		set<string> income = page.incoming_links[*it];
		for(set<string> :: iterator it2 = income.begin(); it2 != income.end(); it2++)
			expand.insert(*it2);
		//get outcoming
		vector<string> outcome = page.links[*it];
		for(unsigned int i = 0 ; i < outcome.size(); i++)
			expand.insert(outcome[i]);
	}
	
	//insert every expansion into the set result in a larger candidate set 
	for(set<string> :: iterator it = expand.begin(); it != expand.end(); it++)
		result.insert(*it);

	//we dont have to check any duplicates for the candidate set
	//since the keys in set are unique


	//number of vertices
	double n = result.size();
	//initial pagerank of the nodes
	double initial_rank = 1/n;


	double matrix [result.size()][result.size()];
	vector<double> pagerank;

	//intial rank which is 1/n
	for(unsigned int i = 0 ; i < result.size(); i++)
		pagerank.push_back(initial_rank);

	//initialize matrix
	for(unsigned int i = 0 ; i < result.size(); i++)
	{
		for(unsigned int  j = 0; j < result.size(); j++)
			matrix[i][j]=0.0;
	}

	int x =0 ; int y = 0;
	//when you move down the matrix you have to
	//increment it2++
	set<string> :: iterator it2=  result.begin();
	while(x < (int)result.size())
	{
		y=0;
		set<string>income = page.incoming_links[*it2];
		while(y < (int)result.size())
		{
			for(set<string> :: iterator it = result.begin(); it != result.end(); it++)
			{
				//this is a self loop 
				if(x==y)
				{
					// double outs = page.links[*it].size()+1;
					vector<string> outcome = page.links[*it];
					double outs =1;
					for(unsigned int p = 0 ; p < outcome.size(); p++)
					{
						if(result.find(outcome[p]) != result.end())
							outs++;
					}
					matrix[x][y] = 1/outs;
					y++;
				}

				//if a node has an incoming
				else if(income.find(*it) != income.end())
				{
					// double outs = page.links[*it].size()+1;
					vector<string> outcome = page.links[*it];
					double outs =1;
					for(unsigned int p = 0 ; p < outcome.size(); p++)
					{
						if(result.find(outcome[p]) != result.end())
							outs++;
					}
					matrix[x][y] = 1/outs;
					y++;
				}
				//no incoming leave the value at matrix[x][y] as 0
				else
					y++;
			}
		}
		it2++;
		x++;
	}
	
	//constants
	double constant = 1- probability; 
	double constant2 = probability *(initial_rank);
	//do the page rank formula
	//step_number number of times
	double temp[result.size()];


	for(int k = 0 ; k < step_number; k++)
	{
		//do the adjeceny matrix multiplication to find
		//pagerank
		for(unsigned int i = 0 ; i <result.size(); i++)
		{
			double sum=0;
			for(unsigned int j = 0 ;  j < result.size(); j++)
			{
				double res = matrix[i][j] * pagerank[j];
				sum+= res;
			}
			temp[i] = sum;
		}
		//update pagerank
		for(unsigned int a = 0; a < result.size(); a++)
		{
			temp[a] = (constant* temp[a]) + constant2;
			pagerank[a] = temp[a];
		}
	}

	//in the case of a webpage having the exact same page rank
	//that is why a multimap is used
	multimap <double, string, greater<double> > output;
	set<string> :: iterator it = result.begin();
	//have a page rank connect to a webpage and sort it from largest to smallest
	//since the comparator used is greater<double>
	for(unsigned int i =0  ; i < pagerank.size(); i++)
	{
		output.insert(make_pair(pagerank[i], *it));
		it++;
	}
	//output the number of webpages
	out << output.size() << endl;
	//output the name of the webpages by its page rank
	//from largest to smallest
	for(multimap <double, string, greater<double> > :: iterator it = output.begin(); it != output.end(); it++)
		out << it->second << endl;
}
