#pragma once

#include "fscore.h"

enum ResultStatus
{
	RS_NoError,

	RS_Argument,
	RS_Exist,
	RS_NotExist,
};

class FSEmulator
{
public:
	FSEmulator();
	virtual ~FSEmulator();

	ResultStatus CreateRoot(FSNode **root, const std::string &name);
	ResultStatus RemoveRoot(FSNode *root);

	ResultStatus CreateDir(FSNode **dir, const std::string &name, FSNode *node);
	ResultStatus CreateFile(FSNode **file, const std::string &name, FSNode *node);
	ResultStatus CreateHardLink(FSLink **link, FSNode *src, FSNode *dst);
	ResultStatus CreateDynamicLink(FSLink **link, FSNode *src, FSNode *dst);

	ResultStatus CopyNode(FSNode *src, FSNode *dst);
	ResultStatus MoveNode(FSNode *src, FSNode *dst);
	ResultStatus RemoveNode(FSNode *node);

private:
	ResultStatus CreateNode(FSNode **newNode, NodeType type, const std::string &name, FSNode *node);

private:
	TFSNodeVec m_roots;
};

