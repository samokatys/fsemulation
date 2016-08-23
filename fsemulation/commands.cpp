#include "commands.h"

#include "algstring.h"

#include <algorithm>
#include <iostream>

const char NODES_DELIMITER = '\\';

FSNode *getParentNode(FSEmulator &emul, FSNode *currentDir, TArgsVec &path)
{
	FSNode *parent;
	if (emul.IsRoot(&parent, path[0]) == false) {
		parent = currentDir;
	}
	else {
		path.erase(path.begin());
	}

	FSNode *node = parent;
	if (path.empty() == false) {
		for (size_t nodeNum = 0; nodeNum < path.size() - 1; ++nodeNum) {
			node = GetChild(parent, path[nodeNum]);
			if (node == nullptr) { break; }
			parent = node;
		}

		if (path.size() > 1) {
			path.erase(path.begin(), path.begin() + path.size() - 1);
		}
	}

	return node;
}

ErrorCodes makeDir(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args)
{
	if (args.size() != 2) {
		return EC_SyntaxCmd;
	}

	TArgsVec splitDir;
	split(args[1], NODES_DELIMITER, splitDir);

	FSNode *parent = getParentNode(emul, *currentDir, splitDir);
	if (parent == nullptr) {
		return EC_PathNotExist;
	}
	if (splitDir.empty()) {
		return EC_Argument;
	}

	FSNode *node = nullptr;
	ResultStatus status = emul.CreateDir(&node, splitDir[splitDir.size() - 1], parent);
	if (status == RS_Exist) {
		node = GetChild(parent, splitDir[splitDir.size() - 1]);
		if (node->Type() != NT_Directory) {
			return EC_FileExist;
		}
	}
	else if (status != RS_NoError) {
		return EC_Argument;
	}

	return EC_NoError;
}

ErrorCodes changeDir(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args)
{
	if (args.size() != 2) {
		return EC_SyntaxCmd;
	}

	TArgsVec splitDir;
	split(args[1], NODES_DELIMITER, splitDir);

	FSNode *node = getParentNode(emul, *currentDir, splitDir);
	if (node == nullptr) {
		return EC_PathNotExist;
	}

	if (splitDir.empty() == false) {
		node = GetChild(node, splitDir[0]);
		if (node == nullptr) {
			return EC_PathNotExist;
		}
	}

	*currentDir = node;

	return EC_NoError;
}

ErrorCodes removeDir(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args)
{
	if (args.size() != 2) {
		return EC_SyntaxCmd;
	}

	TArgsVec splitDir;
	split(args[1], NODES_DELIMITER, splitDir);

	FSNode *node = getParentNode(emul, *currentDir, splitDir);
	if (node == nullptr) {
		return EC_PathNotExist;
	}
	if (splitDir.empty()) {
		return EC_RemoveRoot;
	}

	node = GetChild(node, splitDir[0]);
	if (node == nullptr) {
		return EC_PathNotExist;
	}
	if (node == *currentDir) {
		return EC_RemoveCurrentDir;
	}

	ResultStatus status = emul.RemoveNode(node);
	if (status == RS_NotEmpty) {
		return EC_DirNotEmpty;
	}
	else if (status != RS_NoError) {
		return EC_Argument;
	}

	return EC_NoError;
}

ErrorCodes removeNode(FSEmulator &emul, FSNode *node)
{
	ErrorCodes code = EC_NoError;
	const TFSNodes &nodes = node->Childs();
	// плохо сделано, но тогда нужно переделывать на именах файлов
	while (nodes.empty() == false) {
		code = removeNode(emul, *nodes.begin());
		if (code != EC_NoError) {
			return code;
		}
	}

	ResultStatus status = emul.RemoveNode(node);
	if (status == RS_NotEmpty) {
		return EC_DirNotEmpty;
	}
	else if (status != RS_NoError) {
		return EC_Argument;
	}

	return EC_NoError;
}

ErrorCodes deleteTree(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args)
{
	if (args.size() != 2) {
		return EC_SyntaxCmd;
	}

	TArgsVec splitDir;
	split(args[1], NODES_DELIMITER, splitDir);

	FSNode *node = getParentNode(emul, *currentDir, splitDir);
	if (node == nullptr) {
		return EC_PathNotExist;
	}
	if (splitDir.empty()) {
		return EC_RemoveRoot;
	}

	node = GetChild(node, splitDir[0]);
	if (node == nullptr) {
		return EC_PathNotExist;
	}
	if (node == *currentDir) {
		return EC_RemoveCurrentDir;
	}

	return removeNode(emul, node);
}

ErrorCodes makeFile(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args)
{
	if (args.size() != 2) {
		return EC_SyntaxCmd;
	}

	TArgsVec splitDir;
	split(args[1], NODES_DELIMITER, splitDir);

	FSNode *parent = getParentNode(emul, *currentDir, splitDir);
	if (parent == nullptr) {
		return EC_PathNotExist;
	}
	if (splitDir.empty()) {
		return EC_Argument;
	}

	FSNode *node = nullptr;
	ResultStatus status = emul.CreateFile(&node, splitDir[0], parent);
	if (status == RS_Exist) {
		node = GetChild(parent, splitDir[0]);
		if (node->Type() != NT_File) {
			return EC_DirectoryExist;
		}
	}
	else if (status != RS_NoError) {
		return EC_Argument;
	}

	return EC_NoError;
}

ErrorCodes makeHardLink(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args)
{
	if (args.size() != 3) {
		return EC_SyntaxCmd;
	}

	return EC_NoError;
}

ErrorCodes makeDynamicLink(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args)
{
	if (args.size() != 3) {
		return EC_SyntaxCmd;
	}

	return EC_NoError;
}

ErrorCodes deleteFile(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args)
{
	if (args.size() != 2) {
		return EC_SyntaxCmd;
	}

	return EC_NoError;
}

ErrorCodes copy(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args)
{
	if (args.size() != 3) {
		return EC_SyntaxCmd;
	}

	return EC_NoError;
}

ErrorCodes move(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args)
{
	if (args.size() != 3) {
		return EC_SyntaxCmd;
	}

	return EC_NoError;
}

void printNode(int level, FSNode *node)
{
	for (int i = 0; i < level; ++i) {
		std::cout << "--";
	}
	std::cout << "> " << node->DisplayName() << std::endl;

	const TFSNodes &childs = node->Childs();
	for (FSNode *c : childs) {
		printNode(level + 1, c);
	}
}

ErrorCodes printDir(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args)
{
	if (*currentDir == nullptr) {
		return EC_CurrentDirEmpty;
	}

	printNode(0, *currentDir);

	return EC_NoError;
}