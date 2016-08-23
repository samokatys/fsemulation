#pragma once

#include <memory>
#include <set>
#include <vector>
#include <string>

enum NodeType
{
	NT_Directory,
	NT_File,
	NT_HardLink,
	NT_DynamicLink
};

template <typename T>
class FSLess
{
public:
	bool operator()(const T &lhs, const T &rhs) const
	{
		bool res = false;
		if (lhs->Type() == rhs->Type()) {
			if (lhs->Name() == rhs->Name() &&
				(lhs->Type() == NT_DynamicLink || lhs->Type() == NT_HardLink)) {
				res = (*this)((( FSLink * ) lhs)->Source(), (( FSLink * ) rhs)->Source());
			}
			else {
				res = lhs->Name() < rhs->Name();
			}
		}
		else {
			res = lhs->Type() < rhs->Type();
		}

		return res;
	}
};
class FSNode;
typedef std::set<FSNode *, FSLess<FSNode *> > TFSNodes;
// typedef std::set<FSNode *> TFSNodes;

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
	const TFSNodes& Childs() const;

	// methods:
	virtual std::string DisplayName() const;

	friend class FSEmulator;
protected:
	FSNode(NodeType type, const std::string &name, FSNode *parent);
	virtual ~FSNode();
private:
	NodeType m_type;
	FSNode *m_parent;
	TFSNodes m_childs;
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

bool IsParent(const FSNode *parent, const FSNode *child);
FSNode *GetChild(FSNode *parent, const std::string &name);