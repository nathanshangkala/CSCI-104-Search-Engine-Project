#ifndef WEB_PAGE_H
#define WEB_PAGE_H
#include<set>
#include<string>
#include<vector>
#include<map>

class WebPage{

	public:
		WebPage();
		~WebPage();
		std :: map<std :: string, std :: set <std :: string> > keywords;
		//use vector to account for duplicate outcoming links
		std :: map<std :: string, std :: vector< std:: string> > links;
		std :: map <std :: string , std :: vector<std:: string> > content;
		std :: map<std::string, std::set<std::string> > incoming_links;
};
#endif
