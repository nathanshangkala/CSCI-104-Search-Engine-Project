#include "webpage.h"
#include "setutility.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<stack>
#include<iterator>
#include <vector>
#include <stdio.h>
#include <ctype.h>
#include<string>

void parse(const std :: string &x, const std ::  string &filename, WebPage &page){
	if(page.content.size()==0)
	{
		//save  the content of a webpage
		std :: vector<std :: string> y;
		y.push_back(x);
		page.content.insert(std :: pair <std :: string,std :: vector<std :: string> >(filename, y));
	}
	else
		page.content[filename].push_back(x);
	for(unsigned int i = 0 ; i < x.length() ; i++)
	{
		std :: string in;
		std :: stringstream ss;
		std :: string temp;
		//anchor text
		if(x[i]=='[')
		{
			++i;
			while(x[i] !=']')
			{
				//ignore special characters
				if(islower(x[i])||isdigit(x[i]) || x[i] == ' ' || isupper(x[i]))
					ss << x[i];
				else
					ss << ' ';
				i++;
			}
			while(ss)
			{
				std :: string anchor;
				std :: string anchortemp;
				ss>> anchor;
				//make it case in-sensitive
				for(unsigned int k = 0 ;k < anchor.length(); k++)
					anchortemp += tolower(anchor[k]);
				if(page.keywords.find(anchortemp) == page.keywords.end())
				{
					//if a word does not exist in the search engine
					std :: set<std :: string> files;
					files.insert(filename);
					page.keywords.insert(std :: pair <std :: string,std ::  set<std :: string> >(anchortemp, files));
				}
				else
					//if a word wxist just add the set it corraltes to with the new filenamse
					page.keywords[anchortemp].insert(filename);

			}
		}
		else if(x[i]=='(')
		{
			++i;
			while(x[i] !=')')
			{
				ss << x[i];
				i++;
			}
			while(ss)
			{
				std :: string link;
				ss >> link;
				if(link.length()>0)
				{
					//save the links
					if(page.links.find(filename) == page.links.end())
					{
						//when a link is first added
						std :: vector<std :: string> links;
						links.push_back(link);
						page.links.insert(std :: pair<std ::  string,std ::  vector<std :: string> >(filename,links));
					}
					else
					{
						//increment number of link of a webpage
						page.links[filename].push_back(link);
					}
				}
			}
		}
		else
		{
			//parse through special characters
			while(islower(x[i])|| isupper(x[i]) || isdigit(x[i]))
			{
				ss << x[i];
				i++;
			}
			ss >> in;
			if(in.length()>0)
			{
				//case insensitive
				for(unsigned int k = 0 ;k < in.length(); k++)
					temp += tolower(in[k]);
				if(page.keywords.find(temp) == page.keywords.end())
				{
					//if the word is unique
					std :: set<std :: string> files;
					files.insert(filename);
					page.keywords.insert(std :: pair <std :: string,std ::  set<std :: string> >(temp, files));
				}
				else
					page.keywords[temp].insert(filename);
			}
		}
	}
	return;
}

void get_incoming(WebPage &page)
{
	//this functions get all the incoming of a specific webpage
	//since all of the webpages are recorded in the content map
	//iterate through the keys of content which is all the webpages
	//have been previously parsed
	for(std::map< std::string,std:: vector<std::string> > :: iterator it = page.content.begin(); it != page.content.end(); it++)
	{
		//the key of the content will also be the key of income_links key
		// incoming_links(webpage_name, set<webpages_icoming_to_a_webpage)
		std::string web_name = it->first;
		std::set <std::string> result;
		for(std::map< std::string,std:: vector<std::string> > :: iterator it2 = page.links.begin(); it2 != page.links.end(); it2++)
		{
			for(unsigned int i = 0 ; i < it2->second.size(); i++)
			{
				//insert every incoming link of a webpage
				//we dont have to worry about duplicates since set have
				//unique keys
				if(web_name== it2->second[i])
					result.insert(it2->first);
			}
		}
		//insert the new pair into the incoming map
		//that holds all a specific webpage incoming links 
		page.incoming_links.insert(std::make_pair(web_name, result));
	}
}

std::set<std::string> single (const std :: string &query, WebPage &page)
{
	//get the set of all the webpages that has that specific word
	std:: set<std::string> result = page.keywords[query];
	return result;
}

std::set<std::string> intersectString (const std :: vector <std::string> &query, WebPage &page)
{
	//find the sets that has all those word
	std ::vector<std ::set<std::string> > list;
	for(unsigned int i =0; i < query.size(); i++)
		list.push_back(page.keywords[query[i]]);

	//find intersection of all set using intersectSet in 
	//setutility.h
	for(unsigned int i = 0;i < list.size()-1; i++)
		list[i+1]= intersectSet(list[i], list[i+1]);

	//return the result of AND operation
	return list[list.size()-1]; 
}

std::set<std::string> unionString (const std :: vector<std::string> &query, WebPage &page/*, std::ofstream &out*/)
{
	//find the sets that has all those word
	std ::vector<std ::set<std::string> > list;
	for(unsigned int i =0; i < query.size(); i++)
		list.push_back(page.keywords[query[i]]);

	//find intersection of all set using unionSet in 
	//setutility.h
	for(unsigned int i = 0 ; i < list.size()-1; i++)
		list[i+1] = unionSet(list[i], list[i+1]);

	//return the result of OR operation
	return list[list.size()-1];
}

void  incoming (const std::string &query , std::ofstream &out, WebPage &page)
{
	std::set<std::string> result= page.incoming_links[query];
	if(result.size()>0)
	{
		//there are webpages linked to the query
		out << result.size() << std::endl;
		for(std::set<std::string> :: iterator it = result.begin(); it != result.end(); it++)
			out << *it << std::endl;
	}
	else if(result.size() == 0 )
		//no webpages links to query
		out << 0 << std::endl;
}

void outcoming (const std ::string &query, std::ofstream &out, WebPage &page)
{
	std::vector<std::string> temp = page.links[query];
	if(temp.size()>0)
	{
		out << temp.size() << std::endl;
		//print out the links of a specified webpage
		for(unsigned int i = 0 ; i < temp.size(); i++)
			out << temp[i] << std::endl;
	}
	else if (temp.size()==0)
		out << 0 << std:: endl;
}

void print(const std :: string &query, WebPage &page, std :: ofstream &out)
{
	//print name of the file
	//file does not exist
	if(page.content[query].size()==0)
	{
		out << "0" << std::endl;
		return;
	}
	out << query << std::endl;
	std :: vector <std ::string> result;
	result = page.content[query];
	//print all the contents whilst ignoring the (links)
	for(unsigned int i = 0 ; i < result.size(); i++)
	{
		std :: string x = result[i];
		for(unsigned int k =0 ; k < x.length(); k++)
		{
			if(x[k] == '(')
			{
				while(x[k] != ')')
					k++;
				if(k != x.length())
					k++;
			}
			if(k != x.length())
				out <<  x[k];
		}
		out << std :: endl;
	}
	return;
}