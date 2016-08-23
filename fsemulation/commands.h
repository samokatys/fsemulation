#pragma once

#include <vector>
#include <string>

#include "fsemulator.h"

enum ErrorCodes {
	EC_NoError,

	EC_SyntaxCmd,
	EC_Argument,
	EC_CurrentDirEmpty,
	EC_RemoveCurrentDir,
	EC_RemoveRoot,
	EC_DirNotEmpty,
	EC_PathNotExist,

	EC_FileExist,
	EC_DirectoryExist,
};

typedef std::vector<std::string> TArgsVec;

// 1 - command name, 2 - path
ErrorCodes makeDir(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args);
// 1 - command name, 2 - path
ErrorCodes changeDir(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args);
// 1 - command name, 2 - path
ErrorCodes removeDir(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args);
// 1 - command name, 2 - path
ErrorCodes deleteTree(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args);
// 1 - command name, 2 - path
ErrorCodes makeFile(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args);
// 1 - command name, 2 - src, 3 - dest
ErrorCodes makeHardLink(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args);
// 1 - command name, 2 - src, 3 - dest
ErrorCodes makeDynamicLink(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args);
// 1 - command name, 2 - path
ErrorCodes deleteFile(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args);
// 1 - command name, 2 - src, 3 - dest
ErrorCodes copy(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args);
// 1 - command name, 2 - src, 3 - dest
ErrorCodes move(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args);
// not need args
ErrorCodes printDir(FSEmulator &emul, FSNode **currentDir, const TArgsVec &args);