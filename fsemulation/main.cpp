// fsemulation.cpp : Defines the entry point for the console application.
//

#include <tchar.h>

#include <algorithm>
#include <iostream>
#include "algstring.h"
#include "commands.h"


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
const std::string PRINT_CMD("print");

typedef ErrorCodes(*CommandFunc)(FSEmulator &emul, FSNode **currentDir, const TArgsVec &);

std::vector<std::pair<std::string, CommandFunc> > cmdVec =
{
	{ QUIT_CMD, nullptr },
	{ MAKE_DIR_CMD, makeDir },
	{ CHANGE_DIR_CMD, changeDir },
	{ REMOVE_DIR_CMD, removeDir },
	{ DELETE_TREE_CMD, deleteTree },
	{ MAKE_FILE_CMD, makeFile },
	{ MAKE_HARD_LINK_CMD, makeHardLink },
	{ MAKE_DYNAMIC_LINK_CMD, makeDynamicLink },
	{ DELETE_CMD, deleteFile },
	{ COPY_CMD, copy },
	{ MOVE_CMD, move },
	{ PRINT_CMD, printDir }
};

void printError(ErrorCodes err);

int _tmain(int argc, _TCHAR* argv[])
{
	FSEmulator emulator;
	FSNode *currentDir;
	std::string input;
	std::vector<std::string> tokens;

	ResultStatus status = emulator.CreateRoot(&currentDir, "C:");
	if (status != RS_NoError) {
		std::cout << "ERROR: cannot create root 0x" << std::hex << status << std::endl;
		return status;
	}

	while (true) {
		std::cout << "Input command: ";
		std::getline(std::cin, input);

		if (input.empty() == false) {
			split(input, ' ', tokens);

			auto it = std::find_if(cmdVec.begin(), cmdVec.end(),
									[&tokens](const std::pair<std::string, CommandFunc> &p)
									{ return iequals(p.first, tokens[0]); });
			if (it != cmdVec.end()) {
				if( it->second == nullptr) { break; }

				ErrorCodes err = it->second(emulator, &currentDir, tokens);
				if (err != EC_NoError) {
					printError(err);
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

void printError(ErrorCodes err)
{
	std::cout << "ERROR: ";

	switch (err) {
		case EC_SyntaxCmd: std::cout << "Command syntax"; break;
		case EC_Argument:std::cout << "Incorrect argument"; break;
		case EC_CurrentDirEmpty:std::cout << "Current dir is empty"; break;
		case EC_RemoveCurrentDir:std::cout << "Cannot remove current dir"; break;
		case EC_RemoveRoot:std::cout << "Cannot remove root dir"; break;
		case EC_DirNotEmpty:std::cout << "Dir not empty"; break;
		case EC_PathNotExist:std::cout << "Path not exist"; break;
		case EC_NotFile:std::cout << "Not file"; break;
		case EC_NotDirectory:std::cout << "Not directory"; break;
		case EC_HasHardLink:std::cout << "Has hard link"; break;
		case EC_FileExist:std::cout << "File already exist"; break;
		case EC_DirectoryExist:std::cout << "Directory already exist"; break;
		default: std::cout << "0x" << std::hex << err; break;
	}

	std::cout << std::endl;
}
