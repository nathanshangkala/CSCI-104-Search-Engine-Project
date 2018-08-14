#ifndef CRAWLER_H
#define CRAWLER_H
#include<set>
#include<fstream>

class Crawler{
public:
	Crawler();
	~Crawler();
	//crawl function
	void crawl(std ::string index, std::set<std::string> &unique, std::ofstream &out);
};
#endif
