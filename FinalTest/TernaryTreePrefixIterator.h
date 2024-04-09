
// COS30008, Final Exam, 2022

#pragma once

#include "TernaryTree.h"

#include <stack>

template<typename T>
class TernaryTreePrefixIterator
{
private:
    using TTree = TernaryTree<T>;
    using TTreeNode = TTree*;
    using TTreeStack = std::stack<const TTree*>;

    const TTree* fTTree;                // ternary tree
    TTreeStack fStack;                  // traversal stack

public:

    using Iterator = TernaryTreePrefixIterator<T>;

    Iterator operator++(int)
    {
        Iterator old = *this;

        ++(*this);

        return old;
    }

    bool operator!=(const Iterator& aOtherIter) const
    {
        return !(*this == aOtherIter);
    }

/////////////////////////////////////////////////////////////////////////
// Problem 4: TernaryTree Prefix Iterator

private:

    // push subtree of aNode [30]
    void push_subtrees(const TTree* aNode)
    {
        if (!(*aNode).getRight().empty())
        {
            fStack.push(const_cast<TTreeNode>(&(*aNode).getRight()));
        }
        if (!(*aNode).getMiddle().empty())
        {
            fStack.push(const_cast<TTreeNode>(&(*aNode).getMiddle()));
        }
        if (!(*aNode).getLeft().empty())
        {
            fStack.push(const_cast<TTreeNode>(&(*aNode).getLeft()));
        }
    }

public:

    // iterator constructor [12]
    TernaryTreePrefixIterator(const TTree* aTTree) : fTTree(aTTree)
    {
        if (!(*fTTree).empty())
        {
            fStack.push(const_cast<TTreeNode>(fTTree));
        }
    }

    // iterator dereference [8]
    const T& operator*() const
    {
        return **fStack.top();
    }

    // prefix increment [12]
    Iterator& operator++()
    {

        if (!fStack.empty())
        {
            TTreeNode topNode = const_cast<TTreeNode>(fStack.top());
            fStack.pop();
            push_subtrees(topNode);
        }

        return *this;
    }

    // iterator equivalence [12]
    bool operator==(const Iterator& aOtherIter) const
    {
        if (fTTree == nullptr && aOtherIter.fTTree == nullptr)
        {
            return true;
        }
        else if (fTTree != nullptr && aOtherIter.fTTree != nullptr)
        {
            return (fTTree == aOtherIter.fTTree) && (fStack.size() == aOtherIter.fStack.size());
        }

        return false;
    }

    // auxiliaries [4,10]
    Iterator begin() const
    {
        Iterator tmp = *this;
        tmp.fStack = TTreeStack();
        tmp.fStack.push(const_cast<TTreeNode>(tmp.fTTree));
        return tmp;
    }
    Iterator end() const
    {
        Iterator tmp = *this;
        tmp.fStack = TTreeStack();
        return tmp;
    }
};