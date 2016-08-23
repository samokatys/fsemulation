#pragma once

#include "fscore.h"

enum ResultStatus
{
	RS_NoError,

	RS_Argument,
	RS_Exist,
	RS_NotExist,
	RS_NotEmpty
};

class FSEmulator
{
public:
	FSEmulator();
	virtual ~FSEmulator();

	ResultStatus CreateRoot(FSNode **root, const std::string &name);
	ResultStatus RemoveRoot(FSNode *root);

	ResultStatus CreateDir(FSNode **dir, const std::string &name, FSNode *parent);
	ResultStatus CreateFile(FSNode **file, const std::string &name, FSNode *parent);
	ResultStatus CreateHardLink(FSLink **link, FSNode *src, FSNode *dst);
	ResultStatus CreateDynamicLink(FSLink **link, FSNode *src, FSNode *dst);

	ResultStatus CopyNode(FSNode *src, FSNode *dst);
	ResultStatus MoveNode(FSNode *src, FSNode *dst);
	ResultStatus RemoveNode(FSNode *node);

private:
	template <typename T>
	ResultStatus CreateNode(T **newNode, NodeType type, const std::string &name, FSNode *parent);

private:
	TFSNodes m_roots;
};

