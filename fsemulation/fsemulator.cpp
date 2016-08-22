#include "fsemulator.h"

#include <algorithm>

FSEmulator::FSEmulator()
{
}


FSEmulator::~FSEmulator()
{
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
	m_roots.push_back(*root);

	return RS_NoError;
}

ResultStatus FSEmulator::RemoveRoot(FSNode *root)
{
	ResultStatus status = RS_NoError;

	auto end = std::remove_if(m_roots.begin(), m_roots.end(), root);
	if (end != m_roots.end()) {
		delete root;
		m_roots.erase(end, m_roots.end());
	}
	else {
		status = RS_NotExist;
	}

	return status;
}

ResultStatus FSEmulator::CreateNode(FSNode **newNode, NodeType type, const std::string &name, FSNode *node)
{
	if (name.empty()) {
		return RS_Argument;
	}
	if (node == nullptr) {
		return RS_Argument;
	}
	if (node->m_type != NT_Directory) {
		return RS_Argument;
	}

	for (FSNode *c : node->m_childs) {
		if (c->Name() == name) {
			return RS_Exist;
		}
	}

	*newNode = new FSNode(type, name, nullptr);
	node->m_childs.push_back(*newNode);

	return RS_NoError;
}

ResultStatus FSEmulator::CreateDir(FSNode **dir, const std::string &name, FSNode *node)
{
	if (name.empty()) {
		return RS_Argument;
	}
	if (node == nullptr) {
		return RS_Argument;
	}
	if (node->m_type != NT_Directory) {
		return RS_Argument;
	}

	for (FSNode *c : node->m_childs) {
		if (c->Name() == name) {
			return RS_Exist;
		}
	}

	*dir = new FSNode(NT_Directory, name, nullptr);
	node->m_childs.push_back(*dir);

	return RS_NoError;
}

ResultStatus FSEmulator::CreateFile(FSNode **file, const std::string &name, FSNode *node)
{
	if (name.empty()) {
		return RS_Argument;
	}
	if (node == nullptr) {
		return RS_Argument;
	}
	if (node->m_type != NT_Directory) {
		return RS_Argument;
	}

	for (FSNode *c : node->m_childs) {
		if (c->Name() == name) {
			return RS_Exist;
		}
	}
}

ResultStatus FSEmulator::CreateHardLink(FSLink **link, FSNode *src, FSNode *dst)
{

}

ResultStatus FSEmulator::CreateDynamicLink(FSLink **link, FSNode *src, FSNode *dst)
{

}

ResultStatus FSEmulator::CopyNode(FSNode *src, FSNode *dst)
{

}

ResultStatus FSEmulator::MoveNode(FSNode *src, FSNode *dst)
{

}

ResultStatus FSEmulator::RemoveNode(FSNode *node)
{

}
