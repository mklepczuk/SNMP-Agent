// AgentSNMP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Tree.h"
#include <iostream>
#include <cstdlib>


int main()
{
	Tree *nTree = new Tree();
	std::vector<std::vector<int>> keys;

	for (int i = 0; i < 9; i++)
	{
		std::vector<int> mibs;
		keys.push_back(mibs);
		for (int i = 0; i < 9; i++)
		{
			int wylosowana_liczba =( std::rand() % 50 ) + i;
			mibs.at(i) = wylosowana_liczba;
		}
	}

	for (int i = 0; i < 9; i++)
	{
		nTree->insert(keys.at(i),"Misiek");
	}

	for ( const node * e : nTree.)
	{
		std::cout << e->mib << std::endl;
	}

    return 0;
}

