#ifndef BST_USING_WHILE_TEMPLATE_H
#define BST_USING_WHILE_TEMPLATE_H

#include "../Common/Debug.h"		//직접 정의한 LogPrint, WarningPrint
#include "../Common/Stack.h"		//직접 정의한 Stack
#include <iostream>					//std::cout, std::endl
#include <memory>					//std::unique_ptr
#include <utility>					//std::move, std::forward

using namespace std;	

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

	//bool 반환값이 false인 경우 : newKey와 같은 키의 노드가 이미 존재하는 경우 
	template <typename InsertDataType = DataType>
	bool Insert(int newKey, InsertDataType&& newData)
	{
		LogPrint("insert");
		
		unique_ptr<NodeType<DataType>> upNewNode = unique_ptr<NodeType<DataType>>(DBG_NEW NodeType<DataType>(newKey, forward<InsertDataType>(newData)));
		return Search(newKey, &BstTemplate::InsertNode, move(upNewNode));
	}

	//bool 반환값이 false인 경우 : targetKey와 같은 키를 가진 노드가 존재하지 않는 경우
	bool Retrieve(int targetKey, DataType& outData) const
	{
		LogPrint("retrieve");

		return Search(targetKey, &BstTemplate::RetrieveNode, outData);
	}

	//bool 반환값이 false인 경우 : targetKey와 같은 키를 가진 노드가 존재하지 않는 경우
	bool Remove(int targetKey)
	{
		LogPrint("remove one item");

		return Search(targetKey, &BstTemplate::RemoveNode, nullptr);
	}

	//트리의 소멸자와 이동 할당 연산자에 사용되므로 실패를 반환하거나 예외를 던지는 경우가 없도록 하였음
	void RemoveTree() noexcept
	{
		LogPrint("remove tree");

		RemovingBstByRotationRR();
	}

	//트리의 값전달로 인해 복사생성자가 실행되는 것을 막기 위해 레퍼런스 인자를 사용함
	//복사를 통한 인자 전달은 성능에도 안 좋고, 게다가 복사 생성자가 CopyTree(..)를 이용해 구현되어있으므로 CopyTree가 복사 생성자를 이용하면 순환 오류가 남
	void CopyTree(const BstTemplate& sourceBst)
	{
		LogPrint("copy tree");

		BstTemplate<NodeType, DataType> tempTree;
		sourceBst.PreorderTraverse(&BstTemplate::CopyNode, &tempTree);
		*this = move(tempTree);
	}

	void PreorderPrint() const
	{
		LogPrint("preorder print");

		PreorderTraverse(&BstTemplate::PrintTargetNode, nullptr);
	}

	void InorderPrint() const
	{
		LogPrint("inorder print");

		InorderTraverse(&BstTemplate::PrintTargetNode, nullptr);
	}

	void PostorderPrint() const
	{
		LogPrint("postorder print");

		PostorderTraverse(&BstTemplate::PrintTargetNode, nullptr);
	}

protected:	//제너릭 메소드들

	//특정 target_key를 가진 노드의 위치에 대해 수행할 작업을 넘겨주는 제너릭 메소드임
	//상위 메소드와 하위 작업 메소드가 const 메소드인 경우를 지원하기 위한 const 버전의 제너릭 메소드 버전도 같이 있음
	//TODO : InsertNode(..) 하위 작업 메소드의 호출이 인라이닝될 수 있도록 제너릭 프로그래밍 방식을 개선하기
	//TODO : const 여부에 상관없는 하나의 제너릭 메소드로 통합할 수 있도록 제너릭 프로그래밍 방식을 개선하기
	//TODO : 하위 작업 메소드에 전달되는 매개변수 개수를 유동적으로 템플릿할 수 있도록 제너릭 프로그래밍 방식을 개선하기
	template <typename MethodType, typename ArgumentType>
	bool Search(int targetKey, MethodType&& method, ArgumentType&& argument);

	template <typename MethodType, typename ArgumentType>
	bool Search(int targetKey, MethodType&& method, ArgumentType&& argument) const;

	//전위순회로 돌면서 각 노드에 수행할 작업을 수행하는 제너릭 메소드임
	//트리 복사의 소스 트리에서 실행되거나, 순회 출력 메소드에서만 사용되므로 const 메소드로 선언하였음
	//TODO : 하위 작업 메소드에 전달되는 매개변수 개수를 유동적으로 템플릿할 수 있도록 제너릭 프로그래밍 방식을 개선하기
	template <typename MethodType, typename ArgumentType>
	void PreorderTraverse(MethodType&& method, ArgumentType&& argument) const;

	template <typename MethodType, typename ArgumentType>
	void InorderTraverse(MethodType&& method, ArgumentType&& argument) const;

	template <typename MethodType, typename ArgumentType>
	void PostorderTraverse(MethodType&& method, ArgumentType&& argument) const;

protected:	//제너릭 메소드에 전달되는 하위 작업 메소드들

	//삽입 위치를 가리키는 자식 포인터를 곤칠 수 있도록 레퍼런스 인자를 사용함
	bool InsertNode(NodeType<DataType>*& pInsertPosition, unique_ptr<NodeType<DataType>> upNewNode);

	bool RetrieveNode(const NodeType<DataType>* pTargetNode, DataType& outData) const;

	//삭제 위치를 가리키는 자식 포인터를 곤칠 수 있도록 레퍼런스 인자를 사용함
	//TODO : 제너릭 메소드들에 전달되는 매개변수의 개수가 유동적으로 조정될 수 있게 되면 더미 매개변수를 지우기
	//TODO : 하위 메소드 호출이 인라이닝화 될 수 있도록 로직 개선하기
	bool RemoveNode(NodeType<DataType>*& pTargetNode, void* pDummyParameter);

	void ReplaceWithInorderPredecessor(NodeType<DataType>*& pTargetNode);

	void ReplaceWithInorderSuccessor(NodeType<DataType>*& pTargetNode);

	void CopyNode(const NodeType<DataType>* pSourceNode, BstTemplate<NodeType, DataType>* pDestBst) const;

	//TODO : 제너릭 메소드들에 전달되는 매개변수의 개수가 유동적으로 조정될 수 있게 되면 더미 매개변수를 지우기
	void PrintTargetNode(const NodeType<DataType>* pTargetNode, void* pDummyParameter) const;

protected:	//논 제너릭 하위 메소드

	//트리의 소멸자와 이동 할당 연산자의 하위 메소드로 사용되므로 실패를 반환하거나 예외를 던지는 경우가 없도록 하였음
	void RemovingBstByRotationRR() noexcept;

protected:

	NodeType<DataType>* m_pHead;
};

template <template <typename> class NodeType, typename DataType>
template <typename MethodType, typename ArgumentType>
inline bool BstTemplate<NodeType, DataType>::Search(int targetKey, MethodType&& method, ArgumentType&& argument)
{
	LogPrint("generic search method (not const method)");

	if (m_pHead == nullptr)
	{
		return (this->*forward<MethodType>(method))(m_pHead, forward<ArgumentType>(argument));
	}
	else if (targetKey == m_pHead->m_key)
	{
		return (this->*forward<MethodType>(method))(m_pHead, forward<ArgumentType>(argument));
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
					return (this->*forward<MethodType>(method))(pSearchNode->m_pLeftChild, forward<ArgumentType>(argument));
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
					return (this->*forward<MethodType>(method))(pSearchNode->m_pRightChild, forward<ArgumentType>(argument));
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
inline bool BstTemplate<NodeType, DataType>::Search(int targetKey, MethodType&& method, ArgumentType&& argument) const
{
	LogPrint("generic search method (const method)");

	if (m_pHead == nullptr)
	{
		return (this->*forward<MethodType>(method))(m_pHead, forward<ArgumentType>(argument));
	}
	else if (targetKey == m_pHead->m_key)
	{
		return (this->*forward<MethodType>(method))(m_pHead, forward<ArgumentType>(argument));
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
					return (this->*forward<MethodType>(method))(pSearchNode->m_pLeftChild, forward<ArgumentType>(argument));
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
					return (this->*forward<MethodType>(method))(pSearchNode->m_pRightChild, forward<ArgumentType>(argument));
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
inline void BstTemplate<NodeType, DataType>::PreorderTraverse(MethodType&& method, ArgumentType&& argument) const
{
	LogPrint("generic preorder traverse method");

	NodeType<DataType>* pTraverseNode = nullptr;
	Stack<NodeType<DataType>*> rightChildStack;
	rightChildStack.Push(this->m_pHead);
	while (rightChildStack.Pop(pTraverseNode) == true)
	{
		while (pTraverseNode != nullptr)
		{
			(this->*forward<MethodType>(method))(pTraverseNode, forward<ArgumentType>(argument));

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
inline void BstTemplate<NodeType, DataType>::InorderTraverse(MethodType&& method, ArgumentType&& argument) const
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
		(this->*forward<MethodType>(method))(pTraverseNode, forward<ArgumentType>(argument));

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
inline void BstTemplate<NodeType, DataType>::PostorderTraverse(MethodType&& method, ArgumentType&& argument) const
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
			(this->*forward<MethodType>(method))(traverseRecord.pNode, forward<ArgumentType>(argument));
		}
	}

	return;
}

template <template <typename> class NodeType, typename DataType>
inline bool BstTemplate<NodeType, DataType>::InsertNode(NodeType<DataType>*& pInsertPosition, unique_ptr<NodeType<DataType>> upNewNode)
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

	unique_ptr<NodeType<DataType>> upCopiedNode = unique_ptr<NodeType<DataType>>(DBG_NEW NodeType<DataType>(*pSourceNode));
	pDestBst->Search(pSourceNode->m_key, &BstTemplate::InsertNode, move(upCopiedNode));
}

template <template <typename> class NodeType, typename DataType>
inline void BstTemplate<NodeType, DataType>::PrintTargetNode(const NodeType<DataType>* pTargetNode, void* pDummyParameter) const
{
	LogPrint("print node task method");

	cout << *pTargetNode << endl;
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