// AgentSNMP.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"
#include "Tree.h"
#include "ParserMIB.h"
#include "Types.h"

int main()
{
	Tree *nTree = new Tree();
	ParserMIB *nParser = new ParserMIB();
	Types *typesList = new Types();
	nParser -> ParseMib(typesList->GetList(),nTree);
	nTree -> Show();
	nTree -> SearchName("ip");
	node *t = nTree->GetFound();
	std::cout<<"\n"<<t->mib<<" "<<t->name;
    return 0;
}

