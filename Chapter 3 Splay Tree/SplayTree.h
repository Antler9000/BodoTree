#ifndef SplayTree_H
#define SplayTree_H

#include "../Common/BstUsingWhileTemplate.h"
#include "../Common/Debug.h"
#include "../Common/Stack.h"
#include <iostream>
#include <utility>

template <typename DataType>
class SplayTree;

//NOTE : BstNode와 구성이 동일하지만, 단원의 구분을 위해서 SplayNode를 별도로 정의했음
template <typename DataType>
class SplayNode
{
	friend class BstTemplate<SplayNode, DataType>;
	friend class SplayTree<DataType>;

	//NOTE : unique_ptr은 유사시 가리키는 대상의 소멸을 호출하므로, HeapNode의 소멸자에 접근할 수 있어야 함
	friend struct std::default_delete<SplayNode<DataType>>;

	friend std::ostream& operator <<(std::ostream& out, const SplayNode<DataType>& printedNode)
	{
		std::cout << "키 : " << printedNode.m_key << " / 데이터 : " << printedNode.m_data;

		return out;
	}

private:

	//NOTE : 데이터가 lvalue인 경우와 rvalue인 경우를 모두 각 참조로 받을 수 있도록 포워딩을 사용함
	template <typename NewDataType = DataType>
	SplayNode(int newKey, NewDataType&& newData) : m_key(newKey), m_data(std::forward<NewDataType>(newData)), m_pLeftChild(nullptr), m_pRightChild(nullptr)
	{

	}

	SplayNode(const SplayNode& sourceNode) : m_key(sourceNode.m_key), m_data(sourceNode.m_data), m_pLeftChild(nullptr), m_pRightChild(nullptr)
	{

	}

	//NOTE : 이진 탐색 트리 템플릿 클래스에 소멸자가 정의되어있으므로, 별도의 노드 소멸자 정의는 필요 없음
	~SplayNode() noexcept = default;

	//NOTE : 쓰이지 않는 노드 생성, 할당 방식들
	SplayNode() = delete;
	SplayNode(SplayNode&& sourceNode) = delete;
	SplayNode& operator = (const SplayNode& sourceNode) = delete;
	SplayNode& operator = (SplayNode&& sourceNode) = delete;

private:

	int						m_key;
	DataType				m_data;
	SplayNode<DataType>*	m_pLeftChild;
	SplayNode<DataType>*	m_pRightChild;
};

template <typename DataType>
class SplayTree : public BstTemplate<SplayNode, DataType>
{
public:

	SplayTree() = default;
	SplayTree(const SplayTree& sourceTree) = default;
	SplayTree(SplayTree&& sourceTree) noexcept = default;
	SplayTree& operator = (const SplayTree& sourceTree) = default;
	SplayTree& operator = (SplayTree&& sourceTree) noexcept = default;
	~SplayTree() noexcept = default;

	//RETURN : targetKey와 같은 키를 가진 노드가 존재하지 않는 경우에 false를 반환함
	bool Retrieve(int targetKey, DataType& outData)
	{
		LogPrint("retrieve with splay");

		SplayNode<DataType>* pSearchNode = this->m_pHead;
		Stack<SplayNode<DataType>*> searchPathStack;
		while (pSearchNode != nullptr)
		{
			if (targetKey < pSearchNode->m_key)
			{
				searchPathStack.Push(pSearchNode);
				pSearchNode = pSearchNode->m_pLeftChild;
			}
			else if (pSearchNode->m_key < targetKey)
			{
				searchPathStack.Push(pSearchNode);
				pSearchNode = pSearchNode->m_pRightChild;
			}
			else
			{
				outData = pSearchNode->m_data;

				//TODO : 반복문으로 루트 노드까지 올리도록 로직을 수정하자 (+삽입, 삭제에도 splay를 적용하기)
				SplayNode<DataType>* pParentOfSearchNode = nullptr;
				SplayNode<DataType>* pGrandParentOfSearchNode = nullptr;
				SplayNode<DataType>* pGreatGrandParentOfSearchNode = nullptr;
				searchPathStack.Pop(pParentOfSearchNode);
				searchPathStack.Pop(pGrandParentOfSearchNode);
				searchPathStack.Pop(pGreatGrandParentOfSearchNode);
				SplayTarget(pGreatGrandParentOfSearchNode, pGrandParentOfSearchNode, pParentOfSearchNode, pSearchNode);
				
				return true;
			}
		}

		WarningPrint("cannot retrieve because there is no same key in tree!");

		return false;
	}

private:

	//NOTE : 조건문이 깊고 코드의 반복이 많아 좋지 못한 코드지만, 아래처럼 조정을 분기하고 하위 조정 메소드에 첫 매개변수를 레퍼런스로 전달하는 방식이 최선이라고 판단함
	void SplayTarget(SplayNode<DataType>* pGreatGrandParentOfTarget, SplayNode<DataType>* pGrandParentOfTarget, SplayNode<DataType>* pParentOfTarget, SplayNode<DataType>* pTarget)
	{
		LogPrint("splay target");

		if (pTarget == this->m_pHead)
		{

		}
		else if (pParentOfTarget == this->m_pHead)
		{
			if (pParentOfTarget->m_pLeftChild == pTarget)
			{
				ZigL(this->m_pHead, pTarget);
			}
			else
			{
				ZigR(this->m_pHead, pTarget);
			}
		}
		else if (pGrandParentOfTarget == this->m_pHead)
		{
			if (pGrandParentOfTarget->m_pLeftChild == pParentOfTarget)
			{
				if (pParentOfTarget->m_pLeftChild == pTarget)
				{
					ZigZigLL(this->m_pHead, pParentOfTarget, pTarget);
				}
				else
				{
					ZigZagLR(this->m_pHead, pParentOfTarget, pTarget);
				}
			}
			else
			{
				if (pParentOfTarget->m_pLeftChild == pTarget)
				{
					ZigZagRL(this->m_pHead, pParentOfTarget, pTarget);
				}
				else
				{
					ZigZigRR(this->m_pHead, pParentOfTarget, pTarget);
				}
			}
		}
		else
		{
			if (pGreatGrandParentOfTarget->m_pLeftChild == pGrandParentOfTarget)
			{
				if (pGrandParentOfTarget->m_pLeftChild == pParentOfTarget)
				{
					if (pParentOfTarget->m_pLeftChild == pTarget)
					{
						ZigZigLL(pGreatGrandParentOfTarget->m_pLeftChild, pParentOfTarget, pTarget);
					}
					else
					{
						ZigZagLR(pGreatGrandParentOfTarget->m_pLeftChild, pParentOfTarget, pTarget);
					}
				}
				else
				{
					if (pParentOfTarget->m_pLeftChild == pTarget)
					{
						ZigZagRL(pGreatGrandParentOfTarget->m_pLeftChild, pParentOfTarget, pTarget);
					}
					else
					{
						ZigZigRR(pGreatGrandParentOfTarget->m_pLeftChild, pParentOfTarget, pTarget);
					}
				}
			}
			else
			{
				if (pGrandParentOfTarget->m_pLeftChild == pParentOfTarget)
				{
					if (pParentOfTarget->m_pLeftChild == pTarget)
					{
						ZigZigLL(pGreatGrandParentOfTarget->m_pRightChild, pParentOfTarget, pTarget);
					}
					else
					{
						ZigZagLR(pGreatGrandParentOfTarget->m_pRightChild, pParentOfTarget, pTarget);
					}
				}
				else
				{
					if (pParentOfTarget->m_pLeftChild == pTarget)
					{
						ZigZagRL(pGreatGrandParentOfTarget->m_pRightChild, pParentOfTarget, pTarget);
					}
					else
					{
						ZigZigRR(pGreatGrandParentOfTarget->m_pRightChild, pParentOfTarget, pTarget);
					}
				}
			}
		}
	}

	//NOTE : ZIG는 부모를 가르키는 조부의 자식 포인터를 직접 수정할 수 있도록 레퍼런스 인자를 사용함
	void ZigL(SplayNode<DataType>*& pFatherOfTarget, SplayNode<DataType>* pTarget)
	{
		LogPrint("zig left");

		pFatherOfTarget->m_pLeftChild = pTarget->m_pRightChild;
		pTarget->m_pRightChild = pFatherOfTarget;
		pFatherOfTarget = pTarget;
	}

	void ZigR(SplayNode<DataType>*& pFatherOfTarget, SplayNode<DataType>* pTarget)
	{
		LogPrint("zig right");

		pFatherOfTarget->m_pRightChild = pTarget->m_pLeftChild;
		pTarget->m_pLeftChild = pFatherOfTarget;
		pFatherOfTarget = pTarget;
	}

	//NOTE : ZIG_ZIG나 ZIG_ZAG와 같이 조부까지 변화가 일어나는 경우를 위해, 조부를 가리키는 증조부의 자식 포인터를 직접 수정할 수 있도록 레퍼런스 인자를 사용하였음
	void ZigZigLL(SplayNode<DataType>*& pGrandFatherOfTarget, SplayNode<DataType>* pFatherOfTarget, SplayNode<DataType>* pTarget)
	{
		LogPrint("zig zig left left");

		pGrandFatherOfTarget->m_pLeftChild = pFatherOfTarget->m_pRightChild;
		pFatherOfTarget->m_pRightChild = pGrandFatherOfTarget;
		pGrandFatherOfTarget = pFatherOfTarget;

		pFatherOfTarget->m_pLeftChild = pTarget->m_pRightChild;
		pTarget->m_pRightChild = pFatherOfTarget;
		pGrandFatherOfTarget = pTarget;
	}

	void ZigZagLR(SplayNode<DataType>*& pGrandFatherOfTarget, SplayNode<DataType>* pFatherOfTarget, SplayNode<DataType>* pTarget)
	{
		LogPrint("zig zag left right");

		pGrandFatherOfTarget->m_pLeftChild = pTarget;
		pFatherOfTarget->m_pRightChild = pTarget->m_pLeftChild;
		pTarget->m_pLeftChild = pFatherOfTarget;

		pGrandFatherOfTarget->m_pLeftChild = pTarget->m_pRightChild;
		pTarget->m_pRightChild = pGrandFatherOfTarget;
		pGrandFatherOfTarget = pTarget;
	}

	void ZigZagRL(SplayNode<DataType>*& pGrandFatherOfTarget, SplayNode<DataType>* pFatherOfTarget, SplayNode<DataType>* pTarget)
	{
		LogPrint("zig zag right left");

		pGrandFatherOfTarget->m_pRightChild = pTarget;
		pFatherOfTarget->m_pLeftChild = pTarget->m_pRightChild;
		pTarget->m_pRightChild = pFatherOfTarget;

		pGrandFatherOfTarget->m_pRightChild = pTarget->m_pLeftChild;
		pTarget->m_pLeftChild = pGrandFatherOfTarget;
		pGrandFatherOfTarget = pTarget;
	}

	void ZigZigRR(SplayNode<DataType>*& pGrandFatherOfTarget, SplayNode<DataType>* pFatherOfTarget, SplayNode<DataType>* pTarget)
	{
		LogPrint("zig zig right right");

		pGrandFatherOfTarget->m_pRightChild = pFatherOfTarget->m_pLeftChild;
		pFatherOfTarget->m_pLeftChild = pGrandFatherOfTarget;
		pGrandFatherOfTarget = pFatherOfTarget;

		pFatherOfTarget->m_pRightChild = pTarget->m_pLeftChild;
		pTarget->m_pLeftChild = pFatherOfTarget;
		pGrandFatherOfTarget = pTarget;
	}
};

#endif //SPLAY_TREE_H