#pragma once
#include <stdexcept>
#include <algorithm>
using namespace std;
template<typename T>
class TernaryTreePrefixIterator;
template<typename T>
class TernaryTree
{
public:

	using TTree = TernaryTree<T>;
	using TSubTree = TTree*;
private:

	T fKey;
	TSubTree fSubTrees[3];
	// private default constructor used for declaration of NIL
	TernaryTree() :
		fKey(T())
	{
		for (size_t i = 0; i < 3; i++)
		{
			fSubTrees[i] = &NIL;
		}
	}
public:
	using Iterator = TernaryTreePrefixIterator<T>;

	static TTree NIL; // sentinel
	// getters for subtrees
	const TTree& getLeft() const { return *fSubTrees[0]; }
	const TTree& getMiddle() const { return *fSubTrees[1]; }
	const TTree& getRight() const { return *fSubTrees[2]; }
	// add a subtree
	void addLeft(const TTree& aTTree) { addSubTree(0, aTTree); }
	void addMiddle(const TTree& aTTree) { addSubTree(1, aTTree); }
	void addRight(const TTree& aTTree) { addSubTree(2, aTTree); }

	// remove a subtree, may through a domain error
	const TTree& removeLeft() { return removeSubTree(0); }
	const TTree& removeMiddle() { return removeSubTree(1); }
	const TTree& removeRight() { return removeSubTree(2); }

		/////////////////////////////////////////////////////////////////////////
		// Problem 1: TernaryTree Basic Infrastructure
private:
	// remove a subtree, may throw a domain error [22]
	const TTree& removeSubTree(size_t aSubtreeIndex)
	{
		if (fSubTrees[aSubtreeIndex]->empty())
		{
			throw domain_error("Subtree is NIL");
		}

		const TTree& removedSubtree = *fSubTrees[aSubtreeIndex];
		fSubTrees[aSubtreeIndex] = &NIL;

		return removedSubtree;
	}

	// add a subtree; must avoid memory leaks; may throw domain error [18]
	void addSubTree(size_t aSubtreeIndex, const TTree& aTTree)
	{
		if (aSubtreeIndex >= 3)
		{
			throw out_of_range("Illegal subtree index");
		}

		if (empty())
		{
			throw domain_error("Operation not supported");
		}

		if (!fSubTrees[aSubtreeIndex]->empty())
		{
			throw domain_error("Subtree is not NIL");
		}

		fSubTrees[aSubtreeIndex] = const_cast<TTree*>(&aTTree);
	}

public:

	// TernaryTree l-value constructor [10]
	TernaryTree(const T& aKey) : fKey(aKey)
	{
		for (size_t i = 0; i < 3; i++)
		{
			fSubTrees[i] = &NIL;
		}
	}

	// destructor (free sub-trees, must not free empty trees) [14]
	~TernaryTree()
	{
		if (!empty())
		{
			for (int i = 0; i < 3; i++)
			{
				if (!fSubTrees[i]->empty())
				{
					delete fSubTrees[i];
				}
			}
		}
	}

	// return key value, may throw domain_error if empty [6] 
	const T& operator*() const
	{
		if (empty())
		{
			throw domain_error("Attempting to dereference an empty tree");
		}
		return fKey;
	}

	// returns true if this ternary tree is empty [4]
	bool empty() const
	{
		return this == &NIL;
	}

	// returns true if this ternary tree is a leaf [10]
	bool leaf() const
	{
		for (int i = 0; i < 3; i++)
		{
			if (!fSubTrees[i]->empty()) return false;
		}
		return true;
	}

	// return height of ternary tree, may throw domain_error if empty [48]
	size_t height() const
	{
		if (empty())
		{
			throw domain_error("Operation not supported");
		}

		if (leaf())
		{
			return 0;
		}

		size_t maxSubtreeHeight = 0;

		for (const auto& subtree : fSubTrees)
		{
			maxSubtreeHeight = max(maxSubtreeHeight, subtree->empty() ? 0 : subtree->height());
		}

		return maxSubtreeHeight + 1;
	}

	/////////////////////////////////////////////////////////////////////////
	// Problem 2: TernaryTree Copy Semantics

	 // copy constructor, must not copy empty ternary tree [10]
	TernaryTree(const TTree& aOtherTTree) : fKey(aOtherTTree.fKey)
	{
		for (size_t i = 0; i < 3; i++)
		{
			if (aOtherTTree.fSubTrees[i] == &NIL)
			{
				fSubTrees[i] = &NIL;
			}
			else
			{
				fSubTrees[i] = new TTree(*aOtherTTree.fSubTrees[i]);
			}
		}
	}
	// copy assignment operator, must not copy empty ternary tree
	// may throw a domain error on attempts to copy NIL [36]
	TTree& operator=(const TTree& aOtherTTree)
	{
		if (!aOtherTTree.empty())
		{
			this->~TernaryTree();

			fKey = aOtherTTree.fKey;

			for (size_t i = 0; i < 3; i++)
			{
				if (aOtherTTree.fSubTrees[i]->empty())
				{
					fSubTrees[i] = &NIL;
				}
				else
				{
					fSubTrees[i] = new TTree(*aOtherTTree.fSubTrees[i]);
				}
			}
		}
		else
		{
			throw domain_error("NIL as source not permitted.");
		}
		return *this;
	}

	// clone ternary tree, must not copy empty trees [10]
	TSubTree clone() const
	{
		if (empty())
		{
			return const_cast<TSubTree>(this);
		}

		TSubTree clonedTree = new TTree(fKey);
		for (size_t i = 0; i < 3; i++)
		{
			clonedTree->fSubTrees[i] = fSubTrees[i]->clone();
		}

		return clonedTree;
	}
	/////////////////////////////////////////////////////////////////////////
	// Problem 3: TernaryTree Move Semantics
	 // TTree r-value constructor [12]
	TernaryTree(T&& aKey) : fKey(move(aKey))
	{
		for (size_t i = 0; i < 3; i++)
		{
			fSubTrees[i] = &NIL;
		}
	}
	// move constructor, must not copy empty ternary tree [12]
	TernaryTree(TTree&& aOtherTTree) : fKey(move(aOtherTTree.fKey))
	{
		for (size_t i = 0; i < 3; i++)
		{
			fSubTrees[i] = aOtherTTree.fSubTrees[i];
			aOtherTTree.fSubTrees[i] = &NIL;
		}
	}
	// move assignment operator, must not copy empty ternary tree [36]
	TTree& operator=(TTree&& aOtherTTree)
	{
		if (this != &aOtherTTree)
		{
			if (!aOtherTTree.empty())
			{
				this->~TernaryTree();

				fKey = move(aOtherTTree.fKey);

				for (int i = 0; i < 3; i++)
				{
					fSubTrees[i] = aOtherTTree.fSubTrees[i];
					aOtherTTree.fSubTrees[i] = &NIL;
				}
			}
			else
			{
				throw domain_error("NIL as source not permitted.");
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////
	// Problem 4: TernaryTree Prefix Iterator
	 // return ternary tree prefix iterator positioned at start [4]
	Iterator begin() const
	{
		return Iterator(this).begin();
	}
	// return ternary prefix iterator positioned at end [6]
	Iterator end() const
	{
		return Iterator(this).end();
	}
};
template<typename T>
TernaryTree<T> TernaryTree<T>::NIL;
