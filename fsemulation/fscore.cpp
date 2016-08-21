#include "fscore.h"

FSNode::FSNode(const std::string &name, FSNode *parent) :
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

const TFSNodeVec& FSNode::Childs() const
{
	return m_childs;
}