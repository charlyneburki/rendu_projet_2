#include "network.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include "random.h"

void Network::resize(const size_t& n)
{
	//sets the size of values to n
	values.resize(n);
    RNG.normal(values);
}

bool Network::add_link(const size_t& n, const size_t& p)
{	
	if(n == p)
		return false;
	else if( (n > (size()-1)) or (p > (size()-1)) )
		return false;
	else
	{
		for(auto i : links)
		{
			//if it already exists
			if( (i.first == n and i.second == p) or (i.first == p and i.second == n) )
				return false;
		}
		links.insert( std::pair<const size_t&,const size_t&>(n,p) );
		links.insert( std::pair<const size_t&,const size_t&>(p,n) );
		return true;
	}
}

size_t Network::random_connect(const double& avg_dist)
{
	links.clear();
	size_t count(0);
	
	std::vector<int> poisson_(size());
	std::vector<int> unif_(size());
	std::vector<size_t> valss(size());
	

	RNG.poisson(poisson_, avg_dist);
	RNG.uniform_int(unif_);
	
	std::iota(valss.begin(),valss.end(),0);
	
	for(size_t i(0); i < poisson_.size(); i++)
	{
		RNG.shuffle(valss);
		for(int j(0); j < poisson_[i] ; j++)
		{
			if(add_link(i, unif_[valss[j]]))
				count++;
		}
	}
	
	return count;
}

size_t Network::set_values(const std::vector<double>& new_values)
{
	size_t count(0);
	bool condition( new_values.size() < values.size() );
	size_t taille(0);
	
	if(condition)
	{
		taille = new_values.size();
	}
	else
	{
		taille = values.size();
	}
	
	for(size_t i(0); i < taille; i++)
	{
		values[i] = new_values[i];
		count++;
	}
	
	return count;
}

size_t Network::size() const
{
	return values.size();
}

size_t Network::degree(const size_t &_n) const
{
	size_t count(0);
	for(auto i: links)
	{
		if(i.first == _n )
			count++;
	}
	return count;
}

double Network::value(const size_t &_n) const
{
	return values[_n];
}

std::vector<double> Network::sorted_values() const
{
	std::vector<double> values_copies(values);
	std::sort(values_copies.begin(), values_copies.end(), std::greater<double>());
	return values_copies;
}

std::vector<size_t> Network::neighbors(const size_t& mean_deg) const
{
	std::vector<size_t> _neighbors;
	for(auto i: links)
	{
		if(i.first == mean_deg)
			_neighbors.push_back(i.second);
	}
	
	return _neighbors;
}
