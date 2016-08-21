#pragma once

#include <vector>
#include <string>

enum ErrorCodes {
	EC_NoError,

	EC_SyntaxCmd
};

typedef std::vector<std::string> TArgsVec;

// 1 - command name, 2 - path
ErrorCodes makeDir(const TArgsVec &args);
// 1 - command name, 2 - path
ErrorCodes changeDir(const TArgsVec &args);
// 1 - command name, 2 - path
ErrorCodes removeDir(const TArgsVec &args);
// 1 - command name, 2 - path
ErrorCodes deleteTree(const TArgsVec &args);
// 1 - command name, 2 - path
ErrorCodes makeFile(const TArgsVec &args);
// 1 - command name, 2 - src, 3 - dest
ErrorCodes makeHardLink(const TArgsVec &args);
// 1 - command name, 2 - src, 3 - dest
ErrorCodes makeDynamicLink(const TArgsVec &args);
// 1 - command name, 2 - path
ErrorCodes deleteFile(const TArgsVec &args);
// 1 - command name, 2 - src, 3 - dest
ErrorCodes copy(const TArgsVec &args);
// 1 - command name, 2 - src, 3 - dest
ErrorCodes move(const TArgsVec &args);