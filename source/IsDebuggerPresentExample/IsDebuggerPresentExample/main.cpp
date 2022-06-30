#include <iostream>
#include <windows.h>



int main()
{
	// Just so the output won't be spammed.
	bool debugged = true;
	bool notbugged = true;

	while (true)
	{
		if (IsDebuggerPresent())
		{
			if (notbugged)
			{
				notbugged = false;
				debugged = true;
				std::cout << "Process being debugged!" << std::endl;
			}
		}
		else
		{
			if (debugged)
			{
				notbugged = true;
				debugged = false;
				std::cout << "We cool!" << std::endl;
			}
		}
	}
}