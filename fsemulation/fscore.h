#pragma once

#include <memory>
#include <vector>
#include <string>

class FSNode;
typedef std::vector<FSNode *> TFSNodeVec;

enum NodeType
{
	NT_File,
	NT_Directory,
	NT_HardLink,
	NT_DynamicLink
};

class FSNode
{
public:
	FSNode(FSNode &) = delete;
	FSNode& operator=(FSNode &) = delete;

	// properties:
	NodeType Type() const;
	FSNode *Parent();
	const FSNode *Parent() const;
	const std::string& Name() const;
	const TFSNodeVec& Childs() const;

	// methods:
	virtual std::string DisplayName() const;

	friend class FSEmulator;

protected:
	FSNode(NodeType type, const std::string &name, FSNode *parent);
	virtual ~FSNode();
private:
	NodeType m_type;
	FSNode *m_parent;
	TFSNodeVec m_childs;
	std::string m_name;
};

class FSLink : public FSNode
{
public:
	FSLink(FSNode &) = delete;
	FSLink& operator=(FSLink &) = delete;

	FSNode *Source();
	const FSNode *Source() const;

	virtual std::string DisplayName() const;

	friend class FSEmulator;

protected:
	FSLink(NodeType type, const std::string &name, FSNode *parent, FSNode *source);
	virtual ~FSLink();
private:
	FSNode *m_source;
};

const FSNode *Root(const FSNode *node);
std::string Path(const FSNode *node);