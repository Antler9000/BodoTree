#ifndef BST_USING_WHILE_TEMPLATE_H
#define BST_USING_WHILE_TEMPLATE_H

#include "../Common/Debug.h"
#include "../Common/Stack.h"
#include <iostream>
#include <memory>
#include <utility>

template <template <typename> class NodeType, typename DataType>
class BstTemplate
{
public:

	BstTemplate() : m_pHead(nullptr)
	{
		LogPrint("empty constructor");
	}

	BstTemplate(const BstTemplate<NodeType, DataType>& sourceTree) : m_pHead(nullptr)
	{
		LogPrint("copy constructor");

		CopyTree(sourceTree);
	}

	BstTemplate(BstTemplate<NodeType, DataType>&& sourceTree) noexcept : m_pHead(sourceTree.m_pHead)
	{
		LogPrint("move constructor");

		sourceTree.m_pHead = nullptr;
	}

	BstTemplate<NodeType, DataType>& operator = (const BstTemplate<NodeType, DataType>& sourceTree)
	{
		LogPrint("copy assignment");

		if (this == &sourceTree)
		{
			return *this;
		}

		CopyTree(sourceTree);

		return *this;
	}

	BstTemplate<NodeType, DataType>& operator = (BstTemplate<NodeType, DataType>&& sourceTree) noexcept
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

	~BstTemplate() noexcept
	{
		LogPrint("destructor");

		RemoveTree();
	}

	//RETURN	: newKey와 같은 키의 노드가 이미 존재하는 경우 false를 반환함
	//NOTE		: 데이터가 lvalue인 경우와 rvalue인 경우를 모두 각 참조로 받을 수 있도록 포워딩을 사용함
	template <typename InsertDataType = DataType>
	bool Insert(int newKey, InsertDataType&& newData)
	{
		LogPrint("insert");
		
		std::unique_ptr<NodeType<DataType>> upNewNode = std::unique_ptr<NodeType<DataType>>(DBG_NEW NodeType<DataType>(newKey, std::forward<InsertDataType>(newData)));
		return GenericSearch(newKey, &BstTemplate::InsertNode, move(upNewNode));
	}

	//RETURN : targetKey와 같은 키를 가진 노드가 존재하지 않는 경우 false를 반환함
	bool Retrieve(int targetKey, DataType& outData) const
	{
		LogPrint("retrieve");

		return GenericSearch(targetKey, &BstTemplate::RetrieveNode, outData);
	}

	//RETURN : targetKey와 같은 키를 가진 노드가 존재하지 않는 경우 false를 반환함
	bool Remove(int targetKey)
	{
		LogPrint("remove one item");

		return GenericSearch(targetKey, &BstTemplate::RemoveNode, nullptr);
	}

	//NOTE : 트리의 소멸자와 이동 할당 연산자에 사용되므로 예외를 던지는 경우가 없도록 하였음
	void RemoveTree() noexcept
	{
		LogPrint("remove tree");

		RemovingBstByRotationRR();
	}

	//NOTE	:	트리의 값전달로 인해 복사생성자가 실행되는 것을 막기 위해 레퍼런스 인자를 사용함
	//			복사 생성자가 CopyTree(..)를 이용해 구현되어있으므로 CopyTree가 복사 생성자를 이용하면 순환 오류가 남
	void CopyTree(const BstTemplate<NodeType, DataType>& sourceBst)
	{
		LogPrint("copy tree");

		BstTemplate<NodeType, DataType> tempTree;
		sourceBst.GenericPreorderTraverse(&BstTemplate::CopyNode, &tempTree);
		*this = std::move(tempTree);
	}

	//NOTE : 디버깅용
	void PreorderPrint() const
	{
		LogPrint("preorder print");

		GenericPreorderTraverse(&BstTemplate::PrintTargetNode, nullptr);
	}

	void InorderPrint() const
	{
		LogPrint("inorder print");

		GenericInorderTraverse(&BstTemplate::PrintTargetNode, nullptr);
	}

	void PostorderPrint() const
	{
		LogPrint("postorder print");

		GenericPostorderTraverse(&BstTemplate::PrintTargetNode, nullptr);
	}

protected:	//NOTE : 제너릭 메소드들

	//NOTE : 메소드나 인자가 lvalue인 경우와 rvalue인 경우를 모두 각 참조로 받을 수 있도록 포워딩을 사용함
	//NOTE : 상위 메소드와 하위 작업 메소드가 const 메소드인 경우를 지원하기 위한 const 버전의 제너릭 메소드 버전도 같이 있음
	template <typename MethodType, typename ArgumentType>
	bool GenericSearch(int targetKey, MethodType&& method, ArgumentType&& argument);

	template <typename MethodType, typename ArgumentType>
	bool GenericSearch(int targetKey, MethodType&& method, ArgumentType&& argument) const;

	//NOTE : 메소드나 인자가 lvalue인 경우와 rvalue인 경우를 모두 각 참조로 받을 수 있도록 포워딩을 사용함
	//NOTE : 트리 복사의 소스 트리에서 실행되거나, 순회 출력 메소드에서만 사용되므로 const 메소드로 선언하였음
	template <typename MethodType, typename ArgumentType>
	void GenericPreorderTraverse(MethodType&& method, ArgumentType&& argument) const;

	template <typename MethodType, typename ArgumentType>
	void GenericInorderTraverse(MethodType&& method, ArgumentType&& argument) const;

	template <typename MethodType, typename ArgumentType>
	void GenericPostorderTraverse(MethodType&& method, ArgumentType&& argument) const;

protected:	//NOTE : 제너릭 메소드에 전달되는 하위 작업 메소드들

	//NOTE : 삽입 위치를 가리키는 자식 포인터를 곤칠 수 있도록 레퍼런스 인자를 사용함
	bool InsertNode(NodeType<DataType>*& pInsertPosition, std::unique_ptr<NodeType<DataType>> upNewNode);

	bool RetrieveNode(const NodeType<DataType>* pTargetNode, DataType& outData) const;

	//NOTE : 삭제 위치를 가리키는 자식 포인터를 곤칠 수 있도록 레퍼런스 인자를 사용함
	bool RemoveNode(NodeType<DataType>*& pTargetNode, void* pDummyParameter);
	void ReplaceWithInorderPredecessor(NodeType<DataType>*& pTargetNode);
	void ReplaceWithInorderSuccessor(NodeType<DataType>*& pTargetNode);

	void CopyNode(const NodeType<DataType>* pSourceNode, BstTemplate<NodeType, DataType>* pDestBst) const;

	void PrintTargetNode(const NodeType<DataType>* pTargetNode, void* pDummyParameter) const;

protected:	//NOTE : 논 제너릭 하위 메소드

	//NOTE : 트리의 소멸자와 이동 할당 연산자의 하위 메소드로 사용되므로 예외를 던지는 경우가 없도록 하였음
	void RemovingBstByRotationRR() noexcept;

protected:

	NodeType<DataType>* m_pHead;
};

template <template <typename> class NodeType, typename DataType>
template <typename MethodType, typename ArgumentType>
inline bool BstTemplate<NodeType, DataType>::GenericSearch(int targetKey, MethodType&& method, ArgumentType&& argument)
{
	LogPrint("generic search method (not const method)");

	if (m_pHead == nullptr)
	{
		return (this->*std::forward<MethodType>(method))(m_pHead, std::forward<ArgumentType>(argument));
	}
	else if (targetKey == m_pHead->m_key)
	{
		return (this->*std::forward<MethodType>(method))(m_pHead, std::forward<ArgumentType>(argument));
	}
	else
	{
		NodeType<DataType>* pSearchNode = m_pHead;
		while (true)
		{
			if (targetKey < pSearchNode->m_key)
			{
				if (pSearchNode->m_pLeftChild == nullptr || pSearchNode->m_pLeftChild->m_key == targetKey)
				{
					return (this->*std::forward<MethodType>(method))(pSearchNode->m_pLeftChild, std::forward<ArgumentType>(argument));
				}
				else
				{
					pSearchNode = pSearchNode->m_pLeftChild;
				}
			}
			else
			{
				if (pSearchNode->m_pRightChild == nullptr || pSearchNode->m_pRightChild->m_key == targetKey)
				{
					return (this->*std::forward<MethodType>(method))(pSearchNode->m_pRightChild, std::forward<ArgumentType>(argument));
				}
				else
				{
					pSearchNode = pSearchNode->m_pRightChild;
				}
			}
		}
	}
}

template <template <typename> class NodeType, typename DataType>
template <typename MethodType, typename ArgumentType>
inline bool BstTemplate<NodeType, DataType>::GenericSearch(int targetKey, MethodType&& method, ArgumentType&& argument) const
{
	LogPrint("generic search method (const method)");

	if (m_pHead == nullptr)
	{
		return (this->*std::forward<MethodType>(method))(m_pHead, std::forward<ArgumentType>(argument));
	}
	else if (targetKey == m_pHead->m_key)
	{
		return (this->*std::forward<MethodType>(method))(m_pHead, std::forward<ArgumentType>(argument));
	}
	else
	{
		NodeType<DataType>* pSearchNode = m_pHead;
		while (true)
		{
			if (targetKey < pSearchNode->m_key)
			{
				if (pSearchNode->m_pLeftChild == nullptr || pSearchNode->m_pLeftChild->m_key == targetKey)
				{
					return (this->*std::forward<MethodType>(method))(pSearchNode->m_pLeftChild, std::forward<ArgumentType>(argument));
				}
				else
				{
					pSearchNode = pSearchNode->m_pLeftChild;
				}
			}
			else
			{
				if (pSearchNode->m_pRightChild == nullptr || pSearchNode->m_pRightChild->m_key == targetKey)
				{
					return (this->*std::forward<MethodType>(method))(pSearchNode->m_pRightChild, std::forward<ArgumentType>(argument));
				}
				else
				{
					pSearchNode = pSearchNode->m_pRightChild;
				}
			}
		}
	}
}

template <template <typename> class NodeType, typename DataType>
template <typename MethodType, typename ArgumentType>
inline void BstTemplate<NodeType, DataType>::GenericPreorderTraverse(MethodType&& method, ArgumentType&& argument) const
{
	LogPrint("generic preorder traverse method");

	NodeType<DataType>* pTraverseNode = nullptr;
	Stack<NodeType<DataType>*> rightChildStack;
	rightChildStack.Push(this->m_pHead);
	while (rightChildStack.Pop(pTraverseNode) == true)
	{
		while (pTraverseNode != nullptr)
		{
			(this->*std::forward<MethodType>(method))(pTraverseNode, std::forward<ArgumentType>(argument));

			if (pTraverseNode->m_pRightChild != nullptr)
			{
				rightChildStack.Push(pTraverseNode->m_pRightChild);
			}

			pTraverseNode = pTraverseNode->m_pLeftChild;
		}
	}
}

template <template <typename> class NodeType, typename DataType>
template <typename MethodType, typename ArgumentType>
inline void BstTemplate<NodeType, DataType>::GenericInorderTraverse(MethodType&& method, ArgumentType&& argument) const
{
	LogPrint("generic inorder traverse method");

	NodeType<DataType>* pTraverseNode = m_pHead;
	Stack<NodeType<DataType>*> rightSideAncestorStack;
	while (pTraverseNode != nullptr)
	{
		rightSideAncestorStack.Push(pTraverseNode);
		pTraverseNode = pTraverseNode->m_pLeftChild;
	}
	while (rightSideAncestorStack.Pop(pTraverseNode) == true)
	{
		(this->*std::forward<MethodType>(method))(pTraverseNode, std::forward<ArgumentType>(argument));

		if (pTraverseNode->m_pRightChild != nullptr)
		{
			pTraverseNode = pTraverseNode->m_pRightChild;

			while (pTraverseNode != nullptr)
			{
				rightSideAncestorStack.Push(pTraverseNode);
				pTraverseNode = pTraverseNode->m_pLeftChild;
			}
		}
	}
}

template <template <typename> class NodeType, typename DataType>
template <typename MethodType, typename ArgumentType>
inline void BstTemplate<NodeType, DataType>::GenericPostorderTraverse(MethodType&& method, ArgumentType&& argument) const
{
	LogPrint("generic postorder traverse method");

	struct Record
	{
		enum NodeJob
		{
			KEEP_SEARCH,
			DO_TODO
		};

		NodeJob nodeJob;
		NodeType<DataType>* pNode;
	};

	Record traverseRecord;
	Stack<Record> depthFirstSearchStack;
	depthFirstSearchStack.Push({ Record::KEEP_SEARCH , m_pHead });
	while (depthFirstSearchStack.Pop(traverseRecord) == true)
	{
		if (traverseRecord.nodeJob == Record::KEEP_SEARCH)
		{
			depthFirstSearchStack.Push({ Record::DO_TODO , traverseRecord.pNode });

			if (traverseRecord.pNode->m_pRightChild != nullptr)
			{
				depthFirstSearchStack.Push({ Record::KEEP_SEARCH, traverseRecord.pNode->m_pRightChild });
			}

			if (traverseRecord.pNode->m_pLeftChild != nullptr)
			{
				depthFirstSearchStack.Push({ Record::KEEP_SEARCH, traverseRecord.pNode->m_pLeftChild });
			}
		}
		else if (traverseRecord.nodeJob == Record::DO_TODO)
		{
			(this->*std::forward<MethodType>(method))(traverseRecord.pNode, std::forward<ArgumentType>(argument));
		}
	}

	return;
}

template <template <typename> class NodeType, typename DataType>
inline bool BstTemplate<NodeType, DataType>::InsertNode(NodeType<DataType>*& pInsertPosition, std::unique_ptr<NodeType<DataType>> upNewNode)
{
	LogPrint("insert node task method");

	if (pInsertPosition != nullptr)
	{
		WarningPrint("cannot insert because there is same key in tree already!");

		return false;
	}

	pInsertPosition = upNewNode.release();

	return true;
}

template <template <typename> class NodeType, typename DataType>
inline bool BstTemplate<NodeType, DataType>::RetrieveNode(const NodeType<DataType>* pTargetNode, DataType& outData) const
{
	LogPrint("retrieve node task method");

	if (pTargetNode == nullptr)
	{
		WarningPrint("cannot retrieve because there is no same key in tree!");

		return false;
	}

	outData = pTargetNode->m_data;

	return true;
}

template <template <typename> class NodeType, typename DataType>
inline bool BstTemplate<NodeType, DataType>::RemoveNode(NodeType<DataType>*& pTargetNode, void* pDummyParameter)
{
	LogPrint("remove node task method");

	if (pTargetNode == nullptr)
	{
		WarningPrint("cannot remove because there is no same key in tree!");

		return false;
	}

	//NOTE : 중위선행자와 중위후속자가 둘 다 있는 경우에는 균형 유지에 조금이나마 도움이 되기 위해서 대체할 대상을 다소 무작위적인 홀짱 방식으로 선택함
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
	else if (pTargetNode->m_pLeftChild != nullptr)
	{
		ReplaceWithInorderPredecessor(pTargetNode);
	}
	else if (pTargetNode->m_pRightChild != nullptr)
	{
		ReplaceWithInorderSuccessor(pTargetNode);
	}
	else
	{
		delete pTargetNode;
		pTargetNode = nullptr;
	}

	return true;
}

template <template <typename> class NodeType, typename DataType>
inline void BstTemplate<NodeType, DataType>::ReplaceWithInorderPredecessor(NodeType<DataType>*& pTargetNode)
{
	LogPrint("replace with inorder predecessor");

	if (pTargetNode->m_pLeftChild->m_pRightChild == nullptr)
	{
		NodeType<DataType>* pInorderPredecessor = pTargetNode->m_pLeftChild;
		pInorderPredecessor->m_pRightChild = pTargetNode->m_pRightChild;
		delete pTargetNode;
		pTargetNode = pInorderPredecessor;
	}
	else
	{
		NodeType<DataType>* pPrevious = nullptr;
		NodeType<DataType>* pTraverse = pTargetNode->m_pLeftChild;
		while (pTraverse->m_pRightChild != nullptr)
		{
			pPrevious = pTraverse;
			pTraverse = pTraverse->m_pRightChild;
		}

		pPrevious->m_pRightChild = pTraverse->m_pLeftChild;
		pTraverse->m_pLeftChild = pTargetNode->m_pLeftChild;
		pTraverse->m_pRightChild = pTargetNode->m_pRightChild;

		delete pTargetNode;
		pTargetNode = pTraverse;
	}
}

template <template <typename> class NodeType, typename DataType>
inline void BstTemplate<NodeType, DataType>::ReplaceWithInorderSuccessor(NodeType<DataType>*& pTargetNode)
{
	LogPrint("replace with inorder successor");

	if (pTargetNode->m_pRightChild->m_pLeftChild == nullptr)
	{
		NodeType<DataType>* pInorderSuccessor = pTargetNode->m_pRightChild;
		pInorderSuccessor->m_pLeftChild = pTargetNode->m_pLeftChild;
		delete pTargetNode;
		pTargetNode = pInorderSuccessor;
	}
	else
	{
		NodeType<DataType>* pPrevious = nullptr;
		NodeType<DataType>* pTraverse = pTargetNode->m_pRightChild;
		while (pTraverse->m_pLeftChild != nullptr)
		{
			pPrevious = pTraverse;
			pTraverse = pTraverse->m_pLeftChild;
		}

		pPrevious->m_pLeftChild = pTraverse->m_pRightChild;
		pTraverse->m_pRightChild = pTargetNode->m_pRightChild;
		pTraverse->m_pLeftChild = pTargetNode->m_pLeftChild;

		delete pTargetNode;
		pTargetNode = pTraverse;
	}
}

template <template <typename> class NodeType, typename DataType>
inline void BstTemplate<NodeType, DataType>::CopyNode(const NodeType<DataType>* pSourceNode, BstTemplate<NodeType, DataType>* pDestBst) const
{
	LogPrint("copy node task method");

	std::unique_ptr<NodeType<DataType>> upCopiedNode = std::unique_ptr<NodeType<DataType>>(DBG_NEW NodeType<DataType>(*pSourceNode));
	pDestBst->GenericSearch(pSourceNode->m_key, &BstTemplate::InsertNode, move(upCopiedNode));
}

template <template <typename> class NodeType, typename DataType>
inline void BstTemplate<NodeType, DataType>::PrintTargetNode(const NodeType<DataType>* pTargetNode, void* pDummyParameter) const
{
	LogPrint("print node task method");

	std::cout << *pTargetNode << std::endl;
}

template <template <typename> class NodeType, typename DataType>
inline void BstTemplate<NodeType, DataType>::RemovingBstByRotationRR() noexcept
{
	LogPrint("removing bst by using Right Right rotation");

	while (m_pHead != nullptr)
	{
		if (m_pHead->m_pRightChild != nullptr)
		{
			NodeType<DataType>* pNewHeadNode = m_pHead->m_pRightChild;
			m_pHead->m_pRightChild = m_pHead->m_pRightChild->m_pLeftChild;
			pNewHeadNode->m_pLeftChild = m_pHead;
			m_pHead = pNewHeadNode;

			continue;
		}

		NodeType<DataType>* pTrashNode = m_pHead;
		m_pHead = m_pHead->m_pLeftChild;
		delete pTrashNode;
	}
}

#endif //BST_USING_WHILE_TEMPLATE_H