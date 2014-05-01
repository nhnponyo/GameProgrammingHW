// Lambda_Sort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "string"
#include "list"

class Character
{
public:
	void Name(std::string name) { m_Name = name; }
	std::string& GetName() { return m_Name; }

private:
	std::string m_Name;
};

void CharacterSortByName()
{
	std::list<Character> charList;

	Character agebreak;
	agebreak.Name("agebreak");
	charList.push_back(agebreak);

	Character sm9;
	sm9.Name("sm9");
	charList.push_back(sm9);

	Character miku;
	miku.Name("miku");
	charList.push_back(miku);

	charList.sort([](Character char1, Character char2) -> bool
	{
		bool result = char1.GetName().size() < char2.GetName().size();
		
		return result;
	});

	for (auto iter : charList)
	{
		printf_s("%s\n", iter.GetName().c_str());
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	CharacterSortByName();
	getchar();
	return 0;
}

