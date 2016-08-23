#include "fscore.h"

FSNode::FSNode(NodeType type, const std::string &name, FSNode *parent) :
	m_type(type),
	m_parent(parent),
	m_name(name)
{
}

FSNode::~FSNode()
{
	for (FSNode *node : m_childs) {
		delete node;
	}
}

NodeType FSNode::Type() const
{
	return m_type;
}

FSNode *FSNode::Parent()
{
	return m_parent;
}

const FSNode *FSNode::Parent() const
{
	return m_parent;
}

const std::string& FSNode::Name() const
{
	return m_name;
}

const TFSNodes& FSNode::Childs() const
{
	return m_childs;
}

std::string FSNode::DisplayName() const
{
	return m_name;
}

FSLink::FSLink(NodeType type, const std::string &name, FSNode *parent, FSNode *source) :
	FSNode(type, name, parent),
	m_source(source)
{
}

FSLink::~FSLink()
{
}

FSNode *FSLink::Source()
{
	return m_source;
}

const FSNode *FSLink::Source() const
{
	return m_source;
}

std::string FSLink::DisplayName() const
{
	if (Type() == NT_DynamicLink) {
		return "dlink[" + Path(m_source) + "]";
	}
	else if (Type() == NT_HardLink) {
		return "hlink[" + Path(m_source) + "]";
	}
	else {
		return "[" + Path(m_source) + "]";
	}
}

const FSNode *Root(const FSNode *node)
{
	const FSNode *parent = node->Parent();
	while (parent->Parent()) {
		parent = parent->Parent();
	}
	return parent;
}

std::string Path(const FSNode *node)
{
	std::string path(node->DisplayName());
	const FSNode *curNode = node->Parent();
	while (curNode) {
		std::string tmp(curNode->DisplayName() + "\\" + path);
		path.swap(tmp);
		curNode = curNode->Parent();
	}

	return path;
}

bool IsParent(const FSNode *parent, const FSNode *child)
{
	const FSNode *pnode = child->Parent();
	while (pnode && pnode != parent) {
		pnode = pnode->Parent();
	}

	return pnode == parent;
}

FSNode *GetChild(FSNode *parent, const std::string &name)
{
	FSNode *child = nullptr;
	for (FSNode *node : parent->Childs()) {
		if (node->Name() == name) {
			child = node;
			break;
		}
	}

	return child;
}