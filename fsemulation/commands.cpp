#include "commands.h"

ErrorCodes makeDir(const TArgsVec &args)
{
	if (args.size() != 2) {
		return EC_SyntaxCmd;
	}

	return EC_NoError;
}

ErrorCodes changeDir(const TArgsVec &args)
{
	if (args.size() != 2) {
		return EC_SyntaxCmd;
	}

	return EC_NoError;
}

ErrorCodes removeDir(const TArgsVec &args)
{
	if (args.size() != 2) {
		return EC_SyntaxCmd;
	}

	return EC_NoError;
}

ErrorCodes deleteTree(const TArgsVec &args)
{
	if (args.size() != 2) {
		return EC_SyntaxCmd;
	}

	return EC_NoError;
}

ErrorCodes makeFile(const TArgsVec &args)
{
	if (args.size() != 2) {
		return EC_SyntaxCmd;
	}

	return EC_NoError;
}

ErrorCodes makeHardLink(const TArgsVec &args)
{
	if (args.size() != 3) {
		return EC_SyntaxCmd;
	}

	return EC_NoError;
}

ErrorCodes makeDynamicLink(const TArgsVec &args)
{
	if (args.size() != 3) {
		return EC_SyntaxCmd;
	}

	return EC_NoError;
}

ErrorCodes deleteFile(const TArgsVec &args)
{
	if (args.size() != 2) {
		return EC_SyntaxCmd;
	}

	return EC_NoError;
}

ErrorCodes copy(const TArgsVec &args)
{
	if (args.size() != 3) {
		return EC_SyntaxCmd;
	}

	return EC_NoError;
}

ErrorCodes move(const TArgsVec &args)
{
	if (args.size() != 3) {
		return EC_SyntaxCmd;
	}

	return EC_NoError;
}
