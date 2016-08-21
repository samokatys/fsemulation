#pragma once

#include <memory>
#include <vector>
#include <string>

typedef std::vector<FSNode *> TFSNodeVec;

class FSNode 
{
public:
	FSNode *Parent();
	const FSNode *Parent() const;
	
	const std::string& Name() const;

	const TFSNodeVec& Childs() const;

protected:
	FSNode(const std::string &name, FSNode *parent);
	virtual ~FSNode();
private:
	FSNode *m_parent;
	TFSNodeVec m_childs;
	std::string m_name;
};
