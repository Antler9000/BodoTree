#ifndef BST_USING_RECURSE_H
#define BST_USING_RECURSE_H

#include "../Common/Debug.h"		//정의한 로구 출력 매크로를 사용함
#include <iostream>					//콘솔 출력을 사용함
#include <utility>					//이동 시맨틱을 사용함
using namespace std;				//..

template <typename DataType>
class Bst;

template <typename DataType>
class BstNode
{
	friend class Bst<DataType>;

private:

	template <typename NewDataType = DataType>
	BstNode(int newKey, NewDataType&& newData)
		: m_key(newKey), m_data(forward<NewDataType>(newData)), m_pLeftChild(nullptr), m_pRightChild(nullptr)
	{

	}

	~BstNode() noexcept
	{
		delete m_pLeftChild;
		delete m_pRightChild;
	}

	//쓰이지 않는 노드 생성 방식
	BstNode() = delete;

	BstNode(const BstNode& sourceNode) = delete;
	
	BstNode(BstNode&& sourceNode) = delete;

	BstNode& operator = (const BstNode& sourceNode) = delete;

	BstNode& operator = (BstNode&& sourceNode) = delete;

private:

	int	m_key;
	DataType m_data;
	BstNode<DataType>* m_pLeftChild;
	BstNode<DataType>* m_pRightChild;
};

template <typename DataType>
class Bst
{
public:

	Bst() : m_pHead(nullptr)
	{
		LogPrint("empty constructor");
	}

	Bst(const Bst<DataType>& sourceBst) : m_pHead(nullptr)
	{
		LogPrint("copy constructor");

		CopyTree(sourceBst);
	}

	Bst(Bst<DataType>&& sourceBst) noexcept : m_pHead(sourceBst.m_pHead)
	{
		LogPrint("move constructor");

		sourceBst.m_pHead = nullptr;
	}

	Bst<DataType>& operator = (const Bst<DataType>& sourceBst)
	{
		LogPrint("copy assignment");

		CopyTree(sourceBst);

		return *this;
	}

	Bst<DataType>& operator = (Bst<DataType>&& sourceBst) noexcept
	{
		LogPrint("move assignment");

		if (this == &sourceBst)
		{
			return *this;
		}

		RemoveTree();

		m_pHead = sourceBst.m_pHead;
		sourceBst.m_pHead = nullptr;

		return *this;
	}

	~Bst() noexcept
	{
		LogPrint("destructor");

		RemoveTree();
	}

	//bool 반환값이 false인 경우 : newKey와 같은 키의 노드가 이미 존재하는 경우
	//newData가 lvalue 참조와 rvalue 참조인 경우를 각각 다르게 처리하기 위해서 참조 붕괴를 사용했음
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
	bool Retrieve(int retrieveTargetKey, DataType& outData) const
	{
		LogPrint("retrieve");

		if (m_pHead == nullptr)
		{
			WarningPrint("cannot retrieve. because tree is empty.");

			return false;
		}

		return RetrieveRecurse(m_pHead, retrieveTargetKey, outData);
	}

	//bool 반환값이 false인 경우 : targetKey와 같은 키를 가진 노드가 존재하지 않는 경우
	bool Remove(int removeTargetKey)
	{
		LogPrint("remove one item");

		if (m_pHead == nullptr)
		{
			WarningPrint("cannot remove. because tree is empty.");

			return false;
		}
		
		if (m_pHead->m_key == removeTargetKey)
		{
			RemoveTarget(m_pHead);

			return true;
		}
		else
		{
			RemoveRecurse(m_pHead, removeTargetKey);

			return true;
		}
	}

	//트리의 소멸자와 이동 할당 연산자에 사용되므로 실패를 반환하거나 예외를 던지는 경우가 없도록 하였음
	void RemoveTree() noexcept
	{
		LogPrint("remove tree");

		delete m_pHead;
		m_pHead = nullptr;
	}

	//트리의 값전달로 인해 복사생성자가 실행되는 것을 막기 위해 레퍼런스 인자를 사용함
	//복사를 통한 인자 전달은 성능에도 안 좋고, 게다가 복사 생성자가 CopyTree(..)를 이용해 구현되어있으므로 CopyTree가 복사 생성자를 이용하면 순환 오류가 남
	void CopyTree(const Bst<DataType>& sourceBst)
	{
		LogPrint("copy tree");

		Bst<DataType> tempTree;
		tempTree.CopyTreeRecurse(sourceBst.m_pHead);
		*this = move(tempTree);
	}

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

	template <typename InsertDataType = DataType>
	bool InsertRecurse(BstNode<DataType>* pSearchTargetNode, int newKey, InsertDataType&& newData);

	bool RetrieveRecurse(const BstNode<DataType>* pSearchTargetNode, int retrieiveTargetKey, DataType& outData) const;

	bool RemoveRecurse(BstNode<DataType>* pSearchTargetNode, int removeTargetKey);

	//삭제 위치를 가리키는 자식 포인터를 곤칠 수 있도록 레퍼런스 인자를 사용함
	void RemoveTarget(BstNode<DataType>*& pRemoveTargetNode);

	void ReplaceWithInorderPredecessor(BstNode<DataType>*& pRemoveTargetNode);

	void ReplaceWithInorderSuccessor(BstNode<DataType>*& pRemoveTargetNode);

	void CopyTreeRecurse(const BstNode<DataType>* pSourceNode);

	void PreorderPrintRecurse(const BstNode<DataType>* pTargetNode) const;

	void InorderPrintRecurse(const BstNode<DataType>* pTargetNode) const;

	void PostorderPrintRecurse(const BstNode<DataType>* pTargetNode) const;

private:

	BstNode<DataType>* m_pHead;
};

template <typename DataType>
template <typename InsertDataType>
bool Bst<DataType>::InsertRecurse(BstNode<DataType>* pSearchTargetNode, int newKey, InsertDataType&& newData)
{
	if (newKey < pSearchTargetNode->m_key)
	{
		if (pSearchTargetNode->m_pLeftChild == nullptr)
		{
			pSearchTargetNode->m_pLeftChild = DBG_NEW BstNode<DataType>(newKey, forward<InsertDataType>(newData));

			return true;
		}
		else
		{
			return InsertRecurse(pSearchTargetNode->m_pLeftChild, newKey, forward<InsertDataType>(newData));
		}
	}
	else if (pSearchTargetNode->m_key < newKey)
	{
		if (pSearchTargetNode->m_pRightChild == nullptr)
		{
			pSearchTargetNode->m_pRightChild = DBG_NEW BstNode<DataType>(newKey, forward<InsertDataType>(newData));

			return true;
		}
		else
		{
			return InsertRecurse(pSearchTargetNode->m_pRightChild, newKey, forward<InsertDataType>(newData));
		}
	}
	else
	{
		WarningPrint("cannot insert because there is same key in tree already!");

		return false;
	}
}

template <typename DataType>
bool Bst<DataType>::RetrieveRecurse(const BstNode<DataType>* pSearchTargetNode, int retrieveTargetKey, DataType& outData) const
{
	if (retrieveTargetKey < pSearchTargetNode->m_key)
	{
		if (pSearchTargetNode->m_pLeftChild != nullptr)
		{
			return RetrieveRecurse(pSearchTargetNode->m_pLeftChild, retrieveTargetKey, outData);
		}
		else
		{
			WarningPrint("cannot retrieve because there is no same key in tree!");

			return false;
		}
	}
	else if (retrieveTargetKey > pSearchTargetNode->m_key)
	{
		if (pSearchTargetNode->m_pRightChild != nullptr)
		{
			return RetrieveRecurse(pSearchTargetNode->m_pRightChild, retrieveTargetKey, outData);
		}
		else
		{
			WarningPrint("cannot retrieve because there is no same key in tree!");

			return false;
		}
	}
	else
	{
		outData = pSearchTargetNode->m_data;

		return true;
	}
}

template <typename DataType>
bool Bst<DataType>::RemoveRecurse(BstNode<DataType>* pSearchTargetNode, int removeTargetKey)
{
	if (removeTargetKey < pSearchTargetNode->m_key)
	{
		if (pSearchTargetNode->m_pLeftChild->m_key == removeTargetKey)
		{
			RemoveTarget(pSearchTargetNode->m_pLeftChild);

			return true;
		}
		else
		{
			return RemoveRecurse(pSearchTargetNode->m_pLeftChild, removeTargetKey);
		}
	}
	else if (removeTargetKey > pSearchTargetNode->m_key)
	{
		if (pSearchTargetNode->m_pRightChild->m_key == removeTargetKey)
		{
			RemoveTarget(pSearchTargetNode->m_pRightChild);

			return true;
		}
		else
		{
			return RemoveRecurse(pSearchTargetNode->m_pRightChild, removeTargetKey);
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
inline void Bst<DataType>::RemoveTarget(BstNode<DataType>*& pRemoveTargetNode)
{
	//중위선행자와 중위후속자가 둘 다 있는 경우에는 균형 유지에 조금이나마 도움이 되기 위해서 대체할 대상을 다소 무작위적으로 선택함
	if (pRemoveTargetNode->m_pLeftChild != nullptr && pRemoveTargetNode->m_pRightChild != nullptr)
	{
		if (pRemoveTargetNode->m_key % 2 == 0)
		{
			ReplaceWithInorderPredecessor(pRemoveTargetNode);
		}
		else
		{
			ReplaceWithInorderSuccessor(pRemoveTargetNode);
		}
	}
	else if (pRemoveTargetNode->m_pLeftChild == nullptr && pRemoveTargetNode->m_pRightChild != nullptr)
	{
		ReplaceWithInorderSuccessor(pRemoveTargetNode);
	}
	else if (pRemoveTargetNode->m_pLeftChild != nullptr && pRemoveTargetNode->m_pRightChild == nullptr)
	{
		ReplaceWithInorderPredecessor(pRemoveTargetNode);
	}
	else
	{
		delete pRemoveTargetNode;
		pRemoveTargetNode = nullptr;
	}
}

template <typename DataType>
inline void Bst<DataType>::ReplaceWithInorderPredecessor(BstNode<DataType>*& pRemoveTargetNode)
{
	if (pRemoveTargetNode->m_pLeftChild->m_pRightChild == nullptr)
	{
		BstNode<DataType>* pInorderPredecessor = pRemoveTargetNode->m_pLeftChild;
		pInorderPredecessor->m_pRightChild = pRemoveTargetNode->m_pRightChild;

		pRemoveTargetNode->m_pLeftChild = nullptr;
		pRemoveTargetNode->m_pRightChild = nullptr;
		delete pRemoveTargetNode;

		pRemoveTargetNode = pInorderPredecessor;
	}
	else
	{
		BstNode<DataType>* pPrevious = nullptr;
		BstNode<DataType>* pTraverse = pRemoveTargetNode->m_pLeftChild;
		while (pTraverse->m_pRightChild != nullptr)
		{
			pPrevious = pTraverse;
			pTraverse = pTraverse->m_pRightChild;
		}

		pPrevious->m_pRightChild = pTraverse->m_pLeftChild;
		pTraverse->m_pLeftChild = pRemoveTargetNode->m_pLeftChild;
		pTraverse->m_pRightChild = pRemoveTargetNode->m_pRightChild;

		pRemoveTargetNode->m_pLeftChild = nullptr;
		pRemoveTargetNode->m_pRightChild = nullptr;
		delete pRemoveTargetNode;

		pRemoveTargetNode = pTraverse;
	}
}

template <typename DataType>
inline void Bst<DataType>::ReplaceWithInorderSuccessor(BstNode<DataType>*& pRemoveTargetNode)
{
	if (pRemoveTargetNode->m_pRightChild->m_pLeftChild == nullptr)
	{
		BstNode<DataType>* pInorderSuccessor = pRemoveTargetNode->m_pRightChild;
		pInorderSuccessor->m_pLeftChild = pRemoveTargetNode->m_pLeftChild;

		pRemoveTargetNode->m_pLeftChild = nullptr;
		pRemoveTargetNode->m_pRightChild = nullptr;
		delete pRemoveTargetNode;

		pRemoveTargetNode = pInorderSuccessor;
	}
	else
	{
		BstNode<DataType>* pPrevious = nullptr;
		BstNode<DataType>* pTraverse = pRemoveTargetNode->m_pRightChild;
		while (pTraverse->m_pLeftChild != nullptr)
		{
			pPrevious = pTraverse;
			pTraverse = pTraverse->m_pLeftChild;
		}

		pPrevious->m_pLeftChild = pTraverse->m_pRightChild;
		pTraverse->m_pRightChild = pRemoveTargetNode->m_pRightChild;
		pTraverse->m_pLeftChild = pRemoveTargetNode->m_pLeftChild;

		pRemoveTargetNode->m_pLeftChild = nullptr;
		pRemoveTargetNode->m_pRightChild = nullptr;
		delete pRemoveTargetNode;

		pRemoveTargetNode = pTraverse;
	}
}

template <typename DataType>
void Bst<DataType>::CopyTreeRecurse(const BstNode<DataType>* pSourceNode)
{
	Insert(pSourceNode->m_key, pSourceNode->m_data);
	if (pSourceNode->m_pLeftChild != nullptr) CopyTreeRecurse(pSourceNode->m_pLeftChild);
	if (pSourceNode->m_pRightChild != nullptr) CopyTreeRecurse(pSourceNode->m_pRightChild);
}

template <typename DataType>
void Bst<DataType>::PreorderPrintRecurse(const BstNode<DataType>* pTargetNode) const
{
	cout << "키 : " << pTargetNode->m_key << " / 데이터 : " << pTargetNode->m_data << endl;
	if (pTargetNode->m_pLeftChild != nullptr) PreorderPrintRecurse(pTargetNode->m_pLeftChild);
	if (pTargetNode->m_pRightChild != nullptr) PreorderPrintRecurse(pTargetNode->m_pRightChild);
}

template <typename DataType>
void Bst<DataType>::InorderPrintRecurse(const BstNode<DataType>* pTargetNode) const
{
	if (pTargetNode->m_pLeftChild != nullptr) InorderPrintRecurse(pTargetNode->m_pLeftChild);
	cout << "키 : " << pTargetNode->m_key << " / 데이터 : " << pTargetNode->m_data << endl;
	if (pTargetNode->m_pRightChild != nullptr) InorderPrintRecurse(pTargetNode->m_pRightChild);
}

template <typename DataType>
void Bst<DataType>::PostorderPrintRecurse(const BstNode<DataType>* pTargetNode) const
{
	if (pTargetNode->m_pLeftChild != nullptr) PostorderPrintRecurse(pTargetNode->m_pLeftChild);
	if (pTargetNode->m_pRightChild != nullptr) PostorderPrintRecurse(pTargetNode->m_pRightChild);
	cout << "키 : " << pTargetNode->m_key << " / 데이터 : " << pTargetNode->m_data << endl;
}

#endif //BST_USING_RECURSE_H