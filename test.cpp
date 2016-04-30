#include "OptionalScopedObject.h"
#include <iostream>

class ScopedMessager
{
public:
	ScopedMessager(const char* name) : m_name(name)
	{
		std::cout << "Ctor of \"" << m_name << "\"" << std::endl;
	}

	~ScopedMessager()
	{
		std::cout << "Dtor of \"" << m_name << "\"" << std::endl;
	}

private:
	const char* m_name;
};


int main()
{
	ScopedMessager messager0("zero");

	OptionalScopedObject<ScopedMessager> hidden;
	OptionalScopedObject<ScopedMessager> open;
	open.construct("always there");

	OptionalScopedObject<ScopedMessager> optional;
	std::cout << "Want optional one?" << std::endl;
	bool next = false;
	std::cin >> next;
	if(next)
	{
		optional.construct("optional");
	}

	return 0;
}