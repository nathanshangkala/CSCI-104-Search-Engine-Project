#include<set>

template<class T>
std :: set<T> intersectSet(const std :: set<T> first, const std ::  set<T> second)
{

	std :: set<T> result;
	//use a iterator
	typename std :: set<T> :: iterator one =first.begin();
	typename std :: set<T> :: iterator two = second.begin();
	while (one != first.end() && two != second.end())
	{	
		//if same value insert it in the result set
		if(*one == *two)
		{
			result.insert(*one);
			one++; two++;
		}
		//mve thhe iterators
		else if(*one > *two)
			two++;
		else if(*one < *two)
			one++;
	}
	// intersection of two set
	return result;
}

template<class T>
std :: set<T> unionSet(/*const*/ std :: set<T> first,const std ::  set<T> second)
{

	for(typename std::set<T> :: const_iterator it =second.begin(); it != second.end(); it++)
	{
		//find union of set
			if(first.find(*it) == first.end())
			first.insert(*it);
	}
	return first;
}