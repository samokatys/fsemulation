#include "fsemulator.h"

#include <algorithm>

FSEmulator::FSEmulator()
{
}


FSEmulator::~FSEmulator()
{
}

bool FSEmulator::IsRoot(FSNode **root, const std::string &name)
{
	*root = nullptr;
	for (FSNode *node : m_roots) {
		if (node->m_name == name) {
			*root = node;
			break;
		}
	}
	return *root != nullptr;
}

ResultStatus FSEmulator::CreateRoot(FSNode **root, const std::string &name)
{
	if (name.empty()) {
		return RS_Argument;
	}

	for (FSNode *r : m_roots) {
		if (r->Name() == name) {
			return RS_Exist;
		}
	}

	*root = new FSNode(NT_Directory, name, nullptr);
	m_roots.insert(*root);

	return RS_NoError;
}

ResultStatus FSEmulator::RemoveRoot(FSNode *root)
{
	ResultStatus status = RS_NoError;

	size_t num = m_roots.erase(root);
	if (num == 0) {
		status = RS_NotExist;
	}

	delete root;

	return status;
}

template <typename T>
ResultStatus FSEmulator::CreateNode(T **newNode, NodeType type, const std::string &name, FSNode *parent)
{
	if (name.empty()) {
		return RS_Argument;
	}
	if (parent == nullptr) {
		return RS_Argument;
	}
	if (parent->m_type != NT_Directory) {
		return RS_Argument;
	}

	for (FSNode *c : parent->m_childs) {
		if (c->Name() == name) {
			return RS_Exist;
		}
	}

	*newNode = new T(type, name, parent);
	parent->m_childs.insert(*newNode);

	return RS_NoError;
}

ResultStatus FSEmulator::CreateDir(FSNode **dir, const std::string &name, FSNode *parent)
{
	ResultStatus status = CreateNode(dir, NT_Directory, name, parent);
	return status;
}

ResultStatus FSEmulator::CreateFile(FSNode **file, const std::string &name, FSNode *parent)
{
	ResultStatus status = CreateNode(file, NT_File, name, parent);
	return status;
}

ResultStatus FSEmulator::CreateHardLink(FSLink **link, FSNode *src, FSNode *dst)
{
	if (src == nullptr) {
		return RS_Argument;
	}
	if (src->m_type == NT_DynamicLink ||
		src->m_type == NT_HardLink) {
		return RS_Argument;
	}
	if (dst == nullptr) {
		return RS_Argument;
	}
	if (dst->m_type != NT_Directory) {
		return RS_Argument;
	}

	*link = new FSLink(NT_HardLink, "hard", dst, src);
	dst->m_childs.insert(*link);

	return RS_NoError;
}

ResultStatus FSEmulator::CreateDynamicLink(FSLink **link, FSNode *src, FSNode *dst)
{
	if (src == nullptr) {
		return RS_Argument;
	}
	if (src->m_type == NT_DynamicLink ||
		src->m_type == NT_HardLink) {
		return RS_Argument;
	}
	if (dst == nullptr) {
		return RS_Argument;
	}
	if (dst->m_type != NT_Directory) {
		return RS_Argument;
	}

	*link = new FSLink(NT_DynamicLink, "dynamic", dst, src);
	dst->m_childs.insert(*link);

	return RS_NoError;
}

ResultStatus FSEmulator::CopyNode(FSNode *src, FSNode *dst)
{
	return RS_NoError;
}

ResultStatus FSEmulator::MoveNode(FSNode *src, FSNode *dst)
{
	return RS_NoError;
}

ResultStatus FSEmulator::RemoveNode(FSNode *node)
{
	if (node == nullptr) {
		return RS_Argument;
	}
	if (node->m_parent == nullptr) {
		return RS_Argument;
	}
	if (node->m_childs.empty() == false) {
		return RS_NotEmpty;
	}

	// TODO: links check
	size_t num = node->m_parent->m_childs.erase(node);
	if (num == 0) {
		return RS_NotExist;
	}

	delete node;

	return RS_NoError;
}
