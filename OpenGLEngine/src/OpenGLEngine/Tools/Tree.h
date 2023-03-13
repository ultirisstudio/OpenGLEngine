////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <unordered_set>

namespace OpenGLEngine
{
	template<typename T>
	class Tree
	{
	private:
		T m_value;
		std::vector<Tree<T>*> m_branches;

		Tree<T>* m_parentBranch;
		Tree<T>* m_previousBranch;
		Tree<T>* m_nextBranch;

	public:
		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = Tree;
			using pointer = Tree*;
			using reference = Tree&;

			pointer m_ptr;

			std::unordered_set<pointer> m_visitedBranches;

			iterator(pointer ptr) :
				m_ptr(ptr),
				m_visitedBranches()
			{
				m_visitedBranches.insert(ptr);
			}

			reference operator*() const
			{
				return *m_ptr;
			}

			pointer operator->()
			{
				return m_ptr;
			}

			iterator& operator++()
			{
				for (size_t i = 0; i < m_ptr->getBranchNumber(); i++)
				{
					if (m_visitedBranches.find(&m_ptr->getBranch(i)) == m_visitedBranches.end())
					{
						m_ptr = &m_ptr->getBranch(i);
						m_visitedBranches.insert(m_ptr);
						return *this;
					}
				}

				pointer parent = &m_ptr->getParentBranch();
				while (m_visitedBranches.find(m_ptr) != m_visitedBranches.end())
				{
					for (size_t i = 0; i < parent->getBranchNumber(); i++)
					{
						if (m_visitedBranches.find(&parent->getBranch(i)) == m_visitedBranches.end())
						{
							m_ptr = &parent->getBranch(i);
							m_visitedBranches.insert(m_ptr);
							return *this;
						}

					}

					if (parent->isRoot())
					{
						m_ptr = nullptr;
						return *this;
					}

					parent = &parent->getParentBranch();
				}

				return *this;
			}

			iterator operator++(int)
			{
				iterator tmp = *this;
				++(*this);
				return tmp;
			}

			friend bool operator==(const iterator& a, const iterator& b)
			{
				return a.m_ptr == b.m_ptr;
			};

			friend bool operator!=(const iterator& a, const iterator& b)
			{
				return !(a == b);
			};
		};

		class const_iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = const Tree;
			using pointer = const Tree*;
			using reference = const Tree&;

			pointer m_ptr;

			std::unordered_set<pointer> m_visitedBranches;

			const_iterator(const pointer ptr) :
				m_ptr(ptr),
				m_visitedBranches()
			{
				m_visitedBranches.insert(ptr);
			}

			reference operator*() const
			{
				return *m_ptr;
			}

			pointer operator->() const
			{
				return m_ptr;
			}

			const_iterator& operator++()
			{
				for (size_t i = 0; i < m_ptr->getBranchNumber(); i++)
				{
					if (m_visitedBranches.find(&m_ptr->getBranch(i)) == m_visitedBranches.end())
					{
						m_ptr = &m_ptr->getBranch(i);
						m_visitedBranches.insert(m_ptr);
						return *this;
					}
				}

				pointer parent = &m_ptr->getParentBranch();
				while (m_visitedBranches.find(m_ptr) != m_visitedBranches.end())
				{
					for (size_t i = 0; i < parent->getBranchNumber(); i++)
					{
						if (m_visitedBranches.find(&parent->getBranch(i)) == m_visitedBranches.end())
						{
							m_ptr = &parent->getBranch(i);
							m_visitedBranches.insert(m_ptr);
							return *this;
						}

					}

					if (parent->isRoot())
					{
						m_ptr = nullptr;
						return *this;
					}

					parent = &parent->getParentBranch();
				}

				return *this;
			}

			const_iterator operator++(int)
			{
				const_iterator tmp = *this;
				++(*this);
				return tmp;
			}

			friend bool operator==(const const_iterator& a, const const_iterator& b)
			{
				return a.m_ptr == b.m_ptr;
			};

			friend bool operator!=(const const_iterator& a, const const_iterator& b)
			{
				return !(a == b);
			};
		};

		iterator begin()
		{
			return iterator(this);
		}

		iterator end()
		{
			return iterator(nullptr);
		}

		const_iterator cbegin() const
		{
			return const_iterator(this);
		}

		const_iterator cend() const
		{
			return const_iterator(nullptr);
		}

		Tree() :
			m_value(),
			m_parentBranch(nullptr),
			m_previousBranch(nullptr),
			m_nextBranch(nullptr),
			m_branches()
		{

		}

		Tree(const T& value) :
			m_value(value),
			m_parentBranch(nullptr),
			m_previousBranch(nullptr),
			m_nextBranch(nullptr),
			m_branches()
		{

		}

		Tree(const Tree<T>& other) :
			m_value(other.m_value),
			m_parentBranch(other.m_parentBranch),
			m_previousBranch(other.m_previousBranch),
			m_nextBranch(other.m_nextBranch),
			m_branches(other.m_branches)
		{

		}

		Tree(Tree<T>&& other) :
			m_value(std::move(other.m_value)),
			m_parentBranch(std::move(other.m_parentBranch)),
			m_previousBranch(std::move(other.m_previousBranch)),
			m_nextBranch(std::move(other.m_nextBranch)),
			m_branches(std::move(other.m_branches))
		{

		}

		~Tree()
		{
			clearBranches();
		}

		Tree<T>& operator[](size_t index)
		{
			return *m_branches[index];
		}

		void operator=(const Tree<T>& other)
		{
			m_value = other.m_value;
			m_parentBranch = other.m_parentBranch;
			m_previousBranch = other.m_previousBranch;
			m_nextBranch = other.m_nextBranch;
			m_branches = other.m_branches;
		}

		bool isLeaf() const
		{
			return m_branches.empty();
		}

		bool isRoot() const
		{
			return !m_parentBranch;
		}

		bool isOnlyBranch() const
		{
			return !m_previousBranch && !m_nextBranch;
		}

		bool hasPreviousBranch() const
		{
			return m_previousBranch != nullptr;
		}

		bool hasNextBranch() const
		{
			return m_nextBranch != nullptr;
		}

		void clearBranches()
		{
			while (!isLeaf())
				popBranch();
		}

		const T& getValue() const
		{
			return m_value;
		}

		Tree<T>& getParentBranch() const
		{
			return *m_parentBranch;
		}

		Tree<T>& getPreviousBranch() const
		{
			return *m_previousBranch;
		}

		Tree<T>& getNextBranch() const
		{
			return *m_nextBranch;
		}

		Tree<T>& getBranch(size_t index) const
		{
			return *m_branches[index];
		}

		size_t getBranchNumber() const
		{
			return m_branches.size();
		}

		void setValue(const T& value)
		{
			m_value = value;
		}

		void setParentBranch(Tree<T>* parentBranch)
		{
			m_parentBranch = parentBranch;
		}

		void setPreviousBranch(Tree<T>* previousBranch)
		{
			m_previousBranch = previousBranch;
		}

		void setNextBranch(Tree<T>* nextBranch)
		{
			m_nextBranch = nextBranch;
		}

		void addBranch(size_t index, T value)
		{
			if (index == getBranchNumber())
			{
				pushBranch(value);
			}
			else
			{
				Tree<T>* child = new Tree<T>(value);
				child->setParentBranch(this);

				if (index == 0)
				{
					child->setPreviousBranch(nullptr);
					child->setNextBranch(m_branches.front());

					m_branches.front()->setPreviousBranch(child);
				}
				else
				{
					child->setPreviousBranch(m_branches[index]);
					child->setNextBranch(m_branches[index + 1]);

					m_branches[index]->setNextBranch(child);
					m_branches[index + 1]->setPreviousBranch(child);
				}

				m_branches.insert(m_branches.begin() + index, child);
			}
		}

		void removeBranch(size_t index)
		{
			if (index == getBranchNumber() - 1)
			{
				popBranch();
			}
			else
			{
				m_branches[index]->setParentBranch(nullptr);
				m_branches[index]->setPreviousBranch(nullptr);
				m_branches[index]->setNextBranch(nullptr);

				delete m_branches[index];
				m_branches.erase(m_branches.begin() + index);

				if (!isLeaf())
				{
					if (index == 0)
						m_branches.back()->setPreviousBranch(nullptr);
					else
					{
						m_branches[index - 1]->setNextBranch(m_branches[index]);
						m_branches[index]->setPreviousBranch(nullptr);
					}
				}
			}
		}

		void pushBranch(T value)
		{
			Tree<T>* child = new Tree<T>(value);

			child->setParentBranch(this);
			child->setNextBranch(nullptr);

			if (!isLeaf())
			{
				m_branches.back()->setNextBranch(child);
				child->setPreviousBranch(m_branches.back());
			}
			else
				child->setPreviousBranch(nullptr);

			m_branches.push_back(child);
		}

		void popBranch()
		{
			m_branches.back()->setParentBranch(nullptr);
			m_branches.back()->setPreviousBranch(nullptr);
			m_branches.back()->setNextBranch(nullptr);

			delete m_branches.back();
			m_branches.pop_back();

			if (!isLeaf())
				m_branches.back()->setNextBranch(nullptr);
		}
	};
}