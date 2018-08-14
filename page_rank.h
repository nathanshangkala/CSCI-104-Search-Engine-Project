#ifndef PAGE_RANK
#define PAGE_RANK
#include<set>
#include<vector>
#include "webpage.h"

class PageRank
{
public:
	PageRank();
	~PageRank();
	//page rank function
	void page_rank(WebPage &page, std::set<std::string> &result, int step_number, double probability, std::ofstream &out);
};

#endif