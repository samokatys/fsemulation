// fsemulation.cpp : Defines the entry point for the console application.
//

#include <tchar.h>

#include <algorithm>
#include <iostream>
#include "algstring.h"


const std::string QUIT_CMD("quit");
const std::string MAKE_DIR_CMD("md");
const std::string CHANGE_DIR_CMD("cd");
const std::string REMOVE_DIR_CMD("rd");
const std::string DELETE_TREE_CMD("deltree");
const std::string MAKE_FILE_CMD("mf");
const std::string MAKE_HARD_LINK_CMD("mhl");
const std::string MAKE_DYNAMIC_LINK_CMD("mdl");
const std::string DELETE_CMD("del");
const std::string COPY_CMD("copy");
const std::string MOVE_CMD("move");

std::vector<std::pair<std::string, int> > cmdVec =
{
	{ QUIT_CMD, 0 },
	{ MAKE_DIR_CMD, 1 },
	{ CHANGE_DIR_CMD, 2 },
	{ REMOVE_DIR_CMD, 3 },
	{ DELETE_TREE_CMD, 4 },
	{ MAKE_FILE_CMD, 5 },
	{ MAKE_HARD_LINK_CMD, 6 },
	{ MAKE_DYNAMIC_LINK_CMD, 7 },
	{ DELETE_CMD, 8 },
	{ COPY_CMD, 9 },
	{ MOVE_CMD, 10 }
};

int _tmain(int argc, _TCHAR* argv[])
{
	std::string input;
	std::vector<std::string> tokens;
	while (true) {
		std::cout << "Input command: ";
		std::getline(std::cin, input);

		if (input.empty() == false) {
			split(input, ' ', tokens);

			auto it = std::find_if(cmdVec.begin(), cmdVec.end(),
								   [&tokens] (const std::pair<std::string, int> &p)
									{ return iequals(p.first, tokens[0]); });
			if (it != cmdVec.end()) {
				if( it->second == 0) { break; }


				std::cout << "Command ID: " << it->second << std::endl;
				for (const std::string &s : tokens) {
					std::cout << s << std::endl;
				}
			}
			else {
				std::cout << "Bad command" << std::endl;
			}

			tokens.clear();
		}
	}

	return 0;
}

