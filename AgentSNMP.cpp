// AgentSNMP.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Tree.h"

int main()
{
	Tree *nTree = new Tree();
	std::vector<int> keys;
	std::vector<int> keys2;
	std::vector<int> keys3;

	for (int i = 0; i < 9; i++)
	{
		int wylosowana_liczba =i;
		keys.push_back(wylosowana_liczba);
	}

	keys2.push_back(0);
	keys2.push_back(2);

	keys2.push_back(0);
	keys2.push_back(2);

	nTree->insert(keys,"root");
	nTree->insert(keys,"Misiek");
	nTree->insert(keys,"lambda");
	nTree->insert(keys2,"tescik");

    return 0;
}

