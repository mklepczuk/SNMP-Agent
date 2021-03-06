#include "stdafx.h"
#include "ParserMIB.h"

ParserMIB::ParserMIB()
{
}


ParserMIB::~ParserMIB()
{
}

void ParserMIB::ParseMib(std::vector<datatypes *> *listoftypes, Tree *tree)
{
	std::string loadedFile = LoadFile("../RFC1213-MIB.txt");
	ParseImports(loadedFile,listoftypes,tree);
	ParseDataTypes(loadedFile, listoftypes,"MIB");
	ParseIdentifiers(loadedFile,"MIB",tree);
	ParseObjects(loadedFile, listoftypes, tree);
}

std::string ParserMIB::LoadFile(std::string path)
{
	std::string temp;
	std::ifstream in(path, std::ios::in | std::ios::binary);
	if (in)
	{
		temp = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	}
	else
	{
		temp = std::string(NULL);
	}
	return temp;
}

void ParserMIB::ParseImports(std::string import,std::vector<datatypes *> *listoftypes, Tree *tree)
{
	std::regex re("IMPORTS\\s*.*\\n.*\\n\\s*FROM\\s(.*)");
	std::sregex_iterator next(import.begin(), import.end(), re);
	std::sregex_iterator end;
	std::smatch match = *next;
	std::string importedfile = LoadFile("../" + match.str(1) + ".txt");
	ParseDataTypes(importedfile, listoftypes,"import");
	ParseIdentifiers(importedfile,"import", tree);
}

void ParserMIB::ParseDataTypes(std::string import,std::vector<datatypes *> *listoftypes,std::string file)
{
	if(file == "import")
	{
		std::regex re("(\\w*)\\s*::=\\s*\\[([^ ]*)\\s*(\\d*)\\]\\s*\\n\\s*(\\w*)\\s*(\\w*\\s*\\w*)\\s*\\((\\d*)..(\\d*)");
		std::sregex_iterator next(import.begin(), import.end(), re);
		std::sregex_iterator end;
		while (next != end)
		{
			std::smatch match = *next;
			listoftypes->push_back(new datatypes(match.str(1),match.str(2),std::stoi(match.str(3)), match.str(4),match.str(5),
				std::stoi(match.str(6)),std::stoul(match.str(7)),std::vector<datatypes *>()));
			next++;
		}

		std::regex re2("(\\w*)\\s*::=\\s*\\[([^ ]*)\\s*(\\d*)\\]\\s*--\\s*\\w*\\s*\\w*-\\w*\\s*\\w*\\n\\s*(\\w*)\\s*(\\w*\\s*\\w*)\\s*\\(SIZE\\s*\\((\\d*)");
		std::sregex_iterator next2(import.begin(), import.end(), re2);
		std::sregex_iterator end2;
		std::smatch match2 = *next2;
		listoftypes->push_back(new datatypes(match2.str(1),match2.str(2),std::stoi(match2.str(3)), match2.str(4),match2.str(5),
			0,std::stoi(match2.str(6)),std::vector<datatypes *>()));

		std::regex re3("(\\w*)\\s*::=\\s*CHOICE\\s*\\{\\s*internet\\s*(\\w*)");
		std::sregex_iterator next3(import.begin(), import.end(), re3);
		std::sregex_iterator end3;
		std::smatch match3 = *next3;
		std::vector<datatypes *>::iterator iter = std::find_if(listoftypes->begin(), listoftypes->end(),[type = match3.str(2)](datatypes * obj)
			{ return obj->name == type;} );
		auto index = std::distance(listoftypes->begin(),iter);
		listoftypes->push_back(new datatypes(match3.str(1),listoftypes->at(index)->visibility,listoftypes->at(index)->id,listoftypes->at(index)->keyWord,
			listoftypes->at(index)->basicType,listoftypes->at(index)->minValue,listoftypes->at(index)->size,std::vector<datatypes *>()));
	}

	if(file == "MIB")
	{
		std::regex re4("(\\w*)\\s*::=\\n\\s*(\\w*\\s*\\w*)\\n--");
		std::sregex_iterator next4(import.begin(), import.end(), re4);
		std::sregex_iterator end4;
		while (next4 != end4)
		{
			std::smatch match4 = *next4;
			listoftypes->push_back(new datatypes(match4.str(1),"CONTEXT-SPECIFIC",4,"UNIVERSAL",match4.str(2),0,255,std::vector<datatypes *>()));
			next4++;
		}
	}
}

void ParserMIB::ParseIdentifiers(std::string import, std::string file, Tree *nTree)
{
	if(file == "import")
	{
		std::regex re("(\\w*)\\s*OBJECT IDENTIFIER ::= \\{ (\\w*) ((\\w*)\\((\\d*)\\) (\\w*)\\((\\d*)\\) )?(\\d*)");
		std::sregex_iterator next(import.begin(), import.end(), re);
		std::sregex_iterator end;
		while (next != end)
		{
			std::smatch match = *next;
			if(match[3].matched != false)
			{
				nTree->Insert(std::stoi(match.str(5)),"","","",match.str(2),match.str(4),nullptr);
				nTree->Insert(std::stoi(match.str(7)),"","","",match.str(4),match.str(6),nullptr);
				nTree->Insert(std::stoi(match.str(8)),"","","",match.str(6),match.str(1),nullptr);
			}
			else
			{
				nTree->Insert(std::stoi(match.str(8)),"","","",match.str(2),match.str(1),nullptr);
			}
			next++;
		}
	}
	if(file == "MIB")
	{
		std::regex re2("(\\w*)(\\-\\w*)?\\s*OBJECT IDENTIFIER ::= \\{ (\\w*)(\\-\\w*)?\\s*(\\d*)");
		std::sregex_iterator next2(import.begin(), import.end(), re2);
		std::sregex_iterator end2;
		while (next2 != end2)
		{
			std::smatch match2 = *next2;
			if(match2[4].matched == false)
			{
				nTree->Insert(std::stoi(match2.str(5)),"","","",match2.str(3),(match2.str(1)+match2.str(2)),nullptr);
			}
			else
			{
				nTree->Insert(std::stoi(match2.str(5)),"","","",(match2.str(3)+match2.str(4)),match2.str(1),nullptr);
			}
			next2++;
		}
	}
}

void ParserMIB::ParseObjects(std::string import, std::vector<datatypes*> *listoftypes, Tree *nTree)
{
	std::string s ="(\\w*) OBJECT-TYPE\\n.*SYNTAX  (SEQUENCE OF )?(\\w*\\s*\\w*)(\\{[^\\}]*\\})?( \\(SIZE \\(0\\.\\.255\\)\\))?( \\((\\d*)\\.\\.(\\d*)\\))?";
	std::string s2 = "\\n.*ACCESS  (\\w*\\-\\w*)\\n.*STATUS  (\\w*)\\n.*DESCRIPTION\\n.*\"([^\"]*)\"\\n.*([^\"]*)?::= \\{ (\\w*) (\\d*)";
	std::regex re(s+s2);
	std::sregex_iterator next(import.begin(), import.end(), re);
	std::sregex_iterator end;
	while (next != end)
	{
		std::smatch match = *next;
		if(match[4].matched != false) //INT O OKREŚLONYCH WARTOŚCIACH
		{
			std::regex re2("(\\d+)\\)[^\\,]");
			std::string s = match.str(4);
			std::sregex_iterator next2(s.begin(),s.end(), re2);
			std::smatch match2 = *next2;
			std::vector<datatypes *>::iterator iter = std::find_if(listoftypes->begin(), listoftypes->end(),[type = (match.str(3)+match2.str(1))](datatypes * obj)
			{ return obj->name == type;} );
			if ( iter != listoftypes->end() )
			{
				auto index = std::distance(listoftypes->begin(),iter);
				nTree->Insert(std::stoi(match.str(14)),match.str(9),match.str(10),match.str(11),match.str(13),match.str(1),listoftypes->at(index));
			}
			else
			{
				listoftypes->push_back(new datatypes((match.str(3)+match2.str(1)),"CONTEXT-SPECIFIC",2,"UNIVERSAL",match.str(3),
					1,std::stoul(match2.str(1)),std::vector<datatypes *>()));
				nTree->Insert(std::stoi(match.str(14)),match.str(9),match.str(10),match.str(11),match.str(13),match.str(1),listoftypes->back());
			}
		}
		else if(match[6].matched != false) //MNIEJSZE INT
		{
			std::vector<datatypes *>::iterator iter = std::find_if(listoftypes->begin(), listoftypes->end(),[type = (match.str(3)+match.str(8))](datatypes * obj)
			{ return obj->name == type;} );
			if ( iter != listoftypes->end() )
			{
				auto index = std::distance(listoftypes->begin(),iter);
				nTree->Insert(std::stoi(match.str(14)),match.str(9),match.str(10),match.str(11),match.str(13),match.str(1),listoftypes->at(index));
			}
			else
			{
				listoftypes->push_back(new datatypes((match.str(3)+match.str(8)),"CONTEXT-SPECIFIC",2,"UNIVERSAL",match.str(3),
					std::stoi(match.str(7)),std::stoul(match.str(8)),std::vector<datatypes *>()));
				nTree->Insert(std::stoi(match.str(14)),match.str(9),match.str(10),match.str(11),match.str(13),match.str(1),listoftypes->back());
			}
		}
		else if(match[2].matched != false)  //SEKWENCJE
		{
			std::regex re3(match.str(3)+" ::=\\n.*([^\\}]*)");
			std::sregex_iterator next3(import.begin(),import.end(), re3);
			std::smatch match3 = *next3;
			s = match3.str(1);
			std::vector<datatypes *> sequence = std::vector<datatypes *>();

			std::regex re4("(            (\\w*\\s*\\w*)(\\((\\d*)\\.\\.(\\d*)\\))?)");
			std::sregex_iterator next4(s.begin(),s.end(),re4);
			std::sregex_iterator end4;
			while (next4 != end4)
			{
				std::smatch match4 = *next4;
				if(match4[3].matched != false)
				{
					std::vector<datatypes *>::iterator iter = std::find_if(listoftypes->begin(), listoftypes->end(),[type = (match4.str(2)+match4.str(5))](datatypes * obj)
					{ return obj->name == type;} );
					if ( iter != listoftypes->end() )
					{
						auto index = std::distance(listoftypes->begin(),iter);
						sequence.push_back(listoftypes->at(index));
					}
					else
					{
						listoftypes->push_back(new datatypes((match4.str(2)+match4.str(5)),"CONTEXT-SPECIFIC",2,"UNIVERSAL",match4.str(2),
						std::stoi(match4.str(4)),std::stoul(match4.str(5)),std::vector<datatypes *>()));
						sequence.push_back(listoftypes->back());
					}
				}
				else
				{
					std::vector<datatypes *>::iterator iter = std::find_if(listoftypes->begin(), listoftypes->end(),[type = match4.str(2)](datatypes * obj)
					{ return obj->name == type;} );
					if ( iter != listoftypes->end() )
					{
						auto index = std::distance(listoftypes->begin(),iter);
						sequence.push_back(listoftypes->at(index));
					}
					else
					{
						std::regex re5("\\w*");
						std::string temp = match4.str(2);
						std::sregex_iterator next5(temp.begin(),temp.end(),re5);
						std::smatch match5 = *next5;
						std::vector<datatypes *>::iterator iter = std::find_if(listoftypes->begin(), listoftypes->end(),[type = match5.str(0)](datatypes * obj)
						{ return obj->name == type;} );
						if ( iter != listoftypes->end() )
						{
							auto index = std::distance(listoftypes->begin(),iter);
							sequence.push_back(listoftypes->at(index));
						}
					}
				}
				next4++;
			}
			listoftypes->push_back(new datatypes(match.str(3),"CONTEXT-SPECIFIC",16,"UNIVERSAL",match.str(3),0,0,sequence));
			nTree->Insert(std::stoi(match.str(14)),match.str(9),match.str(10),match.str(11),match.str(13),match.str(1),listoftypes->back());
		}
		else
		{
			std::vector<datatypes *>::iterator iter = std::find_if(listoftypes->begin(), listoftypes->end(),[type = match.str(3)](datatypes * obj)
			{ return obj->name == type;} );
			if ( iter != listoftypes->end() )
			{
				auto index = std::distance(listoftypes->begin(),iter);
				nTree->Insert(std::stoi(match.str(14)),match.str(9),match.str(10),match.str(11),match.str(13),match.str(1),listoftypes->at(index));
			}
			else
			{
				if(match.str(3) == "OBJECT IDENTIFIER")
					listoftypes->push_back(new datatypes((match.str(3)),"CONTEXT-SPECIFIC",6,"UNIVERSAL",match.str(3),0,255,std::vector<datatypes *>()));
				else
					listoftypes->push_back(new datatypes((match.str(3)),"CONTEXT-SPECIFIC",2,"UNIVERSAL",match.str(3),(-2147483647-1),2147483647,std::vector<datatypes *>()));
				nTree->Insert(std::stoi(match.str(14)),match.str(9),match.str(10),match.str(11),match.str(13),match.str(1),listoftypes->back());
			}
		}
		next++;
	}
}
