#ifndef BST_USING_RECURSE_H
#define BST_USING_RECURSE_H

#include "../Common/Debug.h"		//직접 정의한 매크로 LogPrint, WarningPrint, DBG_NEW
#include <iostream>					//std::cout, std::ostream
#include <utility>					//std::move, std::forward

using namespace std;

template <typename DataType>
class Bst;

template <typename DataType>
class BstNode
{
	friend class Bst<DataType>;

private:

	//데이터가 lvalue인 경우와 rvalue인 경우를 모두 각 참조로 받을 수 있도록 포워딩을 사용함
	//TODO : 단순한 데이터 타입에 대해선 참조가 아니라 값복사 사용하기
	template <typename NewDataType = DataType>
	BstNode(int newKey, NewDataType&& newData) : m_key(newKey), m_data(forward<NewDataType>(newData)), m_pLeftChild(nullptr), m_pRightChild(nullptr)
	{

	}

	~BstNode() noexcept
	{
		delete m_pLeftChild;
		delete m_pRightChild;
	}

	//쓰이지 않는 노드 생성, 할당 방식
	BstNode() = delete;
	BstNode(const BstNode& sourceNode) = delete;
	BstNode(BstNode&& sourceNode) = delete;
	BstNode& operator = (const BstNode& sourceNode) = delete;
	BstNode& operator = (BstNode&& sourceNode) = delete;

private:

	int					m_key;
	DataType			m_data;
	BstNode<DataType>*	m_pLeftChild;
	BstNode<DataType>*	m_pRightChild;
};

template <typename DataType>
class Bst
{
public:

	Bst() : m_pHead(nullptr)
	{
		LogPrint("empty constructor");
	}

	Bst(const Bst<DataType>& sourceTree) : m_pHead(nullptr)
	{
		LogPrint("copy constructor");

		CopyTree(sourceTree);
	}

	Bst(Bst<DataType>&& sourceTree) noexcept : m_pHead(sourceTree.m_pHead)
	{
		LogPrint("move constructor");

		sourceTree.m_pHead = nullptr;
	}

	Bst<DataType>& operator = (const Bst<DataType>& sourceTree)
	{
		LogPrint("copy assignment");

		CopyTree(sourceTree);

		return *this;
	}

	Bst<DataType>& operator = (Bst<DataType>&& sourceTree) noexcept
	{
		LogPrint("move assignment");

		if (this == &sourceTree)
		{
			return *this;
		}

		RemoveTree();

		m_pHead = sourceTree.m_pHead;
		sourceTree.m_pHead = nullptr;

		return *this;
	}

	~Bst() noexcept
	{
		LogPrint("destructor");

		RemoveTree();
	}

	//bool 반환값이 false인 경우 : newKey와 같은 키의 노드가 이미 존재하는 경우
	//데이터가 lvalue인 경우와 rvalue인 경우를 모두 각 참조로 받을 수 있도록 포워딩을 사용함
	//TODO : 단순한 데이터 타입에 대해선 참조가 아니라 값복사 사용하기
	template <typename InsertDataType = DataType>
	bool Insert(int newKey, InsertDataType&& newData)
	{
		LogPrint("insert");

		if (m_pHead == nullptr)
		{
			m_pHead = DBG_NEW BstNode<DataType>(newKey, forward<InsertDataType>(newData));

			return true;
		}
		else
		{
			return InsertRecurse(m_pHead, newKey, newData);
		}
	}

	//bool 반환값이 false인 경우 : targetKey와 같은 키를 가진 노드가 존재하지 않는 경우
	bool Retrieve(int targetKey, DataType& outData) const
	{
		LogPrint("retrieve");

		if (m_pHead == nullptr)
		{
			WarningPrint("cannot retrieve. because tree is empty.");

			return false;
		}

		return RetrieveRecurse(m_pHead, targetKey, outData);
	}

	//bool 반환값이 false인 경우 : targetKey와 같은 키를 가진 노드가 존재하지 않는 경우
	bool Remove(int targetKey)
	{
		LogPrint("remove one item");

		if (m_pHead == nullptr)
		{
			WarningPrint("cannot remove. because tree is empty.");

			return false;
		}
		
		if (m_pHead->m_key == targetKey)
		{
			RemoveTarget(m_pHead);

			return true;
		}
		else
		{
			RemoveRecurse(m_pHead, targetKey);

			return true;
		}
	}

	//트리의 소멸자와 이동 할당 연산자에 사용되므로 예외를 던지는 경우가 없도록 하였음
	void RemoveTree() noexcept
	{
		LogPrint("remove tree");

		delete m_pHead;
		m_pHead = nullptr;
	}

	//트리의 값전달로 인해 복사생성자가 실행되는 것을 막기 위해 레퍼런스 인자를 사용함
	//복사를 통한 인자 전달은 성능에도 안 좋고, 게다가 복사 생성자가 CopyTree(..)를 이용해 구현되어있으므로 CopyTree가 복사 생성자를 이용하면 순환 오류가 남
	void CopyTree(const Bst<DataType>& sourceTree)
	{
		LogPrint("copy tree");

		Bst<DataType> tempTree;
		tempTree.CopyTreeRecurse(sourceTree.m_pHead);
		*this = move(tempTree);
	}

	//디버깅용 퍼블릭 메소드들임
	void PreorderPrint() const
	{
		LogPrint("preorder print");

		if (m_pHead == nullptr)
		{
			WarningPrint("cannot traverse print. becuase tree is empty.");

			return;
		}

		PreorderPrintRecurse(m_pHead);
	}

	void InorderPrint() const
	{
		LogPrint("inorder print");

		if (m_pHead == nullptr)
		{
			WarningPrint("cannot traverse print. becuase tree is empty.");

			return;
		}

		InorderPrintRecurse(m_pHead);
	}

	void PostorderPrint() const
	{
		LogPrint("postorder print");

		if (m_pHead == nullptr)
		{
			WarningPrint("cannot traverse print. becuase tree is empty.");

			return;
		}

		PostorderPrintRecurse(m_pHead);
	}

private:

	//데이터가 lvalue인 경우와 rvalue인 경우를 모두 각 참조로 받을 수 있도록 포워딩을 사용함
	//TODO : 단순한 데이터 타입에 대해선 참조가 아니라 값복사 사용하기
	template <typename InsertDataType = DataType>
	bool InsertRecurse(BstNode<DataType>* pSearchNode, int newKey, InsertDataType&& newData);

	bool RetrieveRecurse(const BstNode<DataType>* pSearchNode, int targetKey, DataType& outData) const;

	bool RemoveRecurse(BstNode<DataType>* pSearchNode, int targetKey);

	//삭제 위치를 가리키는 자식 포인터를 곤칠 수 있도록 레퍼런스 인자를 사용함
	void RemoveTarget(BstNode<DataType>*& pTargetNode);

	void ReplaceWithInorderPredecessor(BstNode<DataType>*& pTargetNode);

	void ReplaceWithInorderSuccessor(BstNode<DataType>*& pTargetNode);

	void CopyTreeRecurse(const BstNode<DataType>* pSourceNode);

	void PreorderPrintRecurse(const BstNode<DataType>* pTraverseNode) const;

	void InorderPrintRecurse(const BstNode<DataType>* pTraverseNode) const;

	void PostorderPrintRecurse(const BstNode<DataType>* pTraverseNode) const;

private:

	BstNode<DataType>* m_pHead;
};

template <typename DataType>
template <typename InsertDataType>
bool Bst<DataType>::InsertRecurse(BstNode<DataType>* pSearchNode, int newKey, InsertDataType&& newData)
{
	LogPrint("insert recurse");

	if (newKey < pSearchNode->m_key)
	{
		if (pSearchNode->m_pLeftChild == nullptr)
		{
			pSearchNode->m_pLeftChild = DBG_NEW BstNode<DataType>(newKey, forward<InsertDataType>(newData));

			return true;
		}
		else
		{
			return InsertRecurse(pSearchNode->m_pLeftChild, newKey, forward<InsertDataType>(newData));
		}
	}
	else if (pSearchNode->m_key < newKey)
	{
		if (pSearchNode->m_pRightChild == nullptr)
		{
			pSearchNode->m_pRightChild = DBG_NEW BstNode<DataType>(newKey, forward<InsertDataType>(newData));

			return true;
		}
		else
		{
			return InsertRecurse(pSearchNode->m_pRightChild, newKey, forward<InsertDataType>(newData));
		}
	}
	else
	{
		WarningPrint("cannot insert because there is same key in tree already!");

		return false;
	}
}

template <typename DataType>
bool Bst<DataType>::RetrieveRecurse(const BstNode<DataType>* pSearchNode, int targetKey, DataType& outData) const
{
	LogPrint("retrieve recurse");

	if (targetKey < pSearchNode->m_key)
	{
		if (pSearchNode->m_pLeftChild != nullptr)
		{
			return RetrieveRecurse(pSearchNode->m_pLeftChild, targetKey, outData);
		}
		else
		{
			WarningPrint("cannot retrieve because there is no same key in tree!");

			return false;
		}
	}
	else if (targetKey > pSearchNode->m_key)
	{
		if (pSearchNode->m_pRightChild != nullptr)
		{
			return RetrieveRecurse(pSearchNode->m_pRightChild, targetKey, outData);
		}
		else
		{
			WarningPrint("cannot retrieve because there is no same key in tree!");

			return false;
		}
	}
	else
	{
		outData = pSearchNode->m_data;

		return true;
	}
}

template <typename DataType>
bool Bst<DataType>::RemoveRecurse(BstNode<DataType>* pSearchNode, int targetKey)
{
	LogPrint("remove recurse");

	if (targetKey < pSearchNode->m_key)
	{
		if (pSearchNode->m_pLeftChild->m_key == targetKey)
		{
			RemoveTarget(pSearchNode->m_pLeftChild);

			return true;
		}
		else
		{
			return RemoveRecurse(pSearchNode->m_pLeftChild, targetKey);
		}
	}
	else if (targetKey > pSearchNode->m_key)
	{
		if (pSearchNode->m_pRightChild->m_key == targetKey)
		{
			RemoveTarget(pSearchNode->m_pRightChild);

			return true;
		}
		else
		{
			return RemoveRecurse(pSearchNode->m_pRightChild, targetKey);
		}
	}
	else
	{
		ErrorPrint("should not reach here while removing! it should be removed on previous call on parent.");

		return false;
	}

	WarningPrint("cannot remove because there is no same key in tree!");

	return false;
}

template <typename DataType>
inline void Bst<DataType>::RemoveTarget(BstNode<DataType>*& pTargetNode)
{
	LogPrint("remove target");

	//중위선행자와 중위후속자가 둘 다 있는 경우에는 균형 유지에 조금이나마 도움이 되기 위해서 대체할 대상을 다소 무작위적으로 선택함
	if (pTargetNode->m_pLeftChild != nullptr && pTargetNode->m_pRightChild != nullptr)
	{
		if (pTargetNode->m_key % 2 == 0)
		{
			ReplaceWithInorderPredecessor(pTargetNode);
		}
		else
		{
			ReplaceWithInorderSuccessor(pTargetNode);
		}
	}
	else if (pTargetNode->m_pLeftChild == nullptr && pTargetNode->m_pRightChild != nullptr)
	{
		ReplaceWithInorderSuccessor(pTargetNode);
	}
	else if (pTargetNode->m_pLeftChild != nullptr && pTargetNode->m_pRightChild == nullptr)
	{
		ReplaceWithInorderPredecessor(pTargetNode);
	}
	else
	{
		delete pTargetNode;
		pTargetNode = nullptr;
	}
}

template <typename DataType>
inline void Bst<DataType>::ReplaceWithInorderPredecessor(BstNode<DataType>*& pTargetNode)
{
	LogPrint("replace with inorder predecessor");

	if (pTargetNode->m_pLeftChild->m_pRightChild == nullptr)
	{
		BstNode<DataType>* pInorderPredecessor = pTargetNode->m_pLeftChild;
		pInorderPredecessor->m_pRightChild = pTargetNode->m_pRightChild;

		pTargetNode->m_pLeftChild = nullptr;
		pTargetNode->m_pRightChild = nullptr;
		delete pTargetNode;

		pTargetNode = pInorderPredecessor;
	}
	else
	{
		BstNode<DataType>* pPrevious = nullptr;
		BstNode<DataType>* pTraverse = pTargetNode->m_pLeftChild;
		while (pTraverse->m_pRightChild != nullptr)
		{
			pPrevious = pTraverse;
			pTraverse = pTraverse->m_pRightChild;
		}

		pPrevious->m_pRightChild = pTraverse->m_pLeftChild;
		pTraverse->m_pLeftChild = pTargetNode->m_pLeftChild;
		pTraverse->m_pRightChild = pTargetNode->m_pRightChild;

		pTargetNode->m_pLeftChild = nullptr;
		pTargetNode->m_pRightChild = nullptr;
		delete pTargetNode;

		pTargetNode = pTraverse;
	}
}

template <typename DataType>
inline void Bst<DataType>::ReplaceWithInorderSuccessor(BstNode<DataType>*& pTargetNode)
{
	LogPrint("replace with inorder successor");

	if (pTargetNode->m_pRightChild->m_pLeftChild == nullptr)
	{
		BstNode<DataType>* pInorderSuccessor = pTargetNode->m_pRightChild;
		pInorderSuccessor->m_pLeftChild = pTargetNode->m_pLeftChild;

		pTargetNode->m_pLeftChild = nullptr;
		pTargetNode->m_pRightChild = nullptr;
		delete pTargetNode;

		pTargetNode = pInorderSuccessor;
	}
	else
	{
		BstNode<DataType>* pPrevious = nullptr;
		BstNode<DataType>* pTraverse = pTargetNode->m_pRightChild;
		while (pTraverse->m_pLeftChild != nullptr)
		{
			pPrevious = pTraverse;
			pTraverse = pTraverse->m_pLeftChild;
		}

		pPrevious->m_pLeftChild = pTraverse->m_pRightChild;
		pTraverse->m_pRightChild = pTargetNode->m_pRightChild;
		pTraverse->m_pLeftChild = pTargetNode->m_pLeftChild;

		pTargetNode->m_pLeftChild = nullptr;
		pTargetNode->m_pRightChild = nullptr;
		delete pTargetNode;

		pTargetNode = pTraverse;
	}
}

template <typename DataType>
void Bst<DataType>::CopyTreeRecurse(const BstNode<DataType>* pSourceNode)
{
	LogPrint("copy tree recurse");

	Insert(pSourceNode->m_key, pSourceNode->m_data);
	if (pSourceNode->m_pLeftChild != nullptr) CopyTreeRecurse(pSourceNode->m_pLeftChild);
	if (pSourceNode->m_pRightChild != nullptr) CopyTreeRecurse(pSourceNode->m_pRightChild);
}

template <typename DataType>
void Bst<DataType>::PreorderPrintRecurse(const BstNode<DataType>* pTargetNode) const
{
	LogPrint("preorder print recurse");

	cout << "키 : " << pTargetNode->m_key << " / 데이터 : " << pTargetNode->m_data << endl;
	if (pTargetNode->m_pLeftChild != nullptr) PreorderPrintRecurse(pTargetNode->m_pLeftChild);
	if (pTargetNode->m_pRightChild != nullptr) PreorderPrintRecurse(pTargetNode->m_pRightChild);
}

template <typename DataType>
void Bst<DataType>::InorderPrintRecurse(const BstNode<DataType>* pTargetNode) const
{
	LogPrint("inorder print recurse");

	if (pTargetNode->m_pLeftChild != nullptr) InorderPrintRecurse(pTargetNode->m_pLeftChild);
	cout << "키 : " << pTargetNode->m_key << " / 데이터 : " << pTargetNode->m_data << endl;
	if (pTargetNode->m_pRightChild != nullptr) InorderPrintRecurse(pTargetNode->m_pRightChild);
}

template <typename DataType>
void Bst<DataType>::PostorderPrintRecurse(const BstNode<DataType>* pTargetNode) const
{
	LogPrint("postorder print recurse");

	if (pTargetNode->m_pLeftChild != nullptr) PostorderPrintRecurse(pTargetNode->m_pLeftChild);
	if (pTargetNode->m_pRightChild != nullptr) PostorderPrintRecurse(pTargetNode->m_pRightChild);
	cout << "키 : " << pTargetNode->m_key << " / 데이터 : " << pTargetNode->m_data << endl;
}

#endif //BST_USING_RECURSE_H