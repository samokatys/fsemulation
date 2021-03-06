#include "fsemulator.h"

#include <vector>
#include <algorithm>

#include "algstring.h"

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
	if (type == NT_Directory) {
		if (CheckDirName(name) == false) {
			return RS_Argument;
		}
	}
	else if (type == NT_File) {
		if (CheckFileName(name) == false) {
			return RS_Argument;
		}
	}
	else {
		if (CheckDirName(name) == false) {
			return RS_Argument;
		}
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

	auto it = m_linkTable.find(src);
	if (it == m_linkTable.end()) {
		m_linkTable.insert(std::make_pair(src, std::vector<FSLink *>()));
	}
	m_linkTable[src].push_back(*link);

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

	auto it = m_linkTable.find(src);
	if (it == m_linkTable.end()) {
		m_linkTable.insert(std::make_pair(src, std::vector<FSLink *>()));
	}
	m_linkTable[src].push_back(*link);

	return RS_NoError;
}

ResultStatus FSEmulator::CopyNode(FSNode *src, FSNode *dst)
{
	if (src == nullptr) {
		return RS_Argument;
	}
	if (dst == nullptr) {
		return RS_Argument;
	}
	if (dst->Type() != NT_Directory) {
		return RS_Argument;
	}

	if (src->Type() == NT_Directory) {
		FSNode *node;
		ResultStatus status = CreateNode(&node, src->Type(), src->Name(), dst);
		if (status == RS_NoError) {
			for (FSNode *child : src->m_childs) {
				status = CopyNode(child, node);
				if (status != RS_NoError) { break; }
			}
		}
		return status;
	}
	else if (src->Type() == NT_File) {
		FSNode *node;
		return CreateNode(&node, src->Type(), src->Name(), dst);
	}
	else if (src->Type() == NT_HardLink) {
		FSLink *link;
		return CreateHardLink(&link, (( FSLink * ) src)->Source(), dst);
	}
	else if (src->Type() == NT_DynamicLink) {
		FSLink *link;
		return CreateHardLink(&link, (( FSLink * ) src)->Source(), dst);
	}

	return RS_NoError;
}

ResultStatus FSEmulator::MoveNode(FSNode *src, FSNode *dst)
{
	if (src == nullptr) {
		return RS_Argument;
	}
	if (dst == nullptr) {
		return RS_Argument;
	}
	if (dst->Type() != NT_Directory) {
		return RS_Argument;
	}

	if (src->m_type == NT_Directory) {
		for (auto it = m_linkTable.begin(); it != m_linkTable.end(); ++it) {
			// ���� ���� �� �������� ��� ��� ������ ����� hardlink
			if (IsParent(src, it->first) || src == it->first) {
				auto &links = it->second;
				if (std::any_of(links.begin(), links.end(),
					[] (const FSLink *l) { return l->Type() == NT_HardLink; })) {
					return RS_HasHLink;
				}
			}
		}
	}
	else if (src->m_type == NT_File) {
		auto it = m_linkTable.find(src);
		if (it != m_linkTable.end()) {
			auto &links = m_linkTable[src];
			if (std::any_of(links.begin(), links.end(),
				[] (const FSLink *l) { return l->Type() == NT_HardLink; })) {
				return RS_HasHLink;
			}
		}
	}

	src->m_parent->m_childs.erase(src);
	src->m_parent = dst;
	dst->m_childs.insert(src);

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

	auto it = m_linkTable.find(node);
	if (it != m_linkTable.end()) {
		auto &links = m_linkTable[node];
		if (std::any_of(links.begin(), links.end(),
			[] (const FSLink *l) { return l->Type() == NT_HardLink; })) {
			return RS_HasHLink;
		}
		// only dynamic links
		else {
			ResultStatus status = RS_NoError;
			while (links.empty() == false) {
				RemoveNode(*links.begin());
				links.erase(links.begin());
			}
		}
	}

	size_t num = node->m_parent->m_childs.erase(node);
	if (num == 0) {
		return RS_NotExist;
	}

	delete node;

	return RS_NoError;
}

bool FSEmulator::CheckNameSymbols(const std::string &name)
{
	for (size_t i = 0; i < name.size(); ++i) {
		if (isalnum(name[i]) == 0) {
			return false;
		}
	}

	return true;
}

bool FSEmulator::CheckFileName(const std::string &name)
{
	if (name.empty()) {
		return false;
	}

	std::vector<std::string> tokens;
	split(name, '.', tokens);

	if (tokens.size() > 2) {
		return false;
	}
	if (tokens[0].size() > 8) {
		return false;
	}
	if (tokens.size() == 1 && tokens[1].size() > 3) {
		return false;
	}

	return CheckNameSymbols(tokens[0]) &&
		CheckNameSymbols(tokens[1]);
}

bool FSEmulator::CheckDirName(const std::string &name)
{
	if (name.empty()) {
		return false;
	}

	std::vector<std::string> tokens;
	split(name, '.', tokens);

	if (tokens.size() > 1) {
		return false;
	}
	if (tokens[0].size() > 8) {
		return false;
	}

	return CheckNameSymbols(tokens[0]);
}