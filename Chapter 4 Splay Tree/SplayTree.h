#ifndef SplayTree_H
#define SplayTree_H

#include "../Common/BstUsingWhileTemplate.h"
#include "../Common/Debug.h"

template <typename DataType>
class SplayTree;

template <typename DataType>
class SplayNode
{
	friend class SplayTree<DataType>;

	friend class BstTemplate<SplayNode, DataType>;

	friend ostream& operator <<(ostream& out, const SplayNode<DataType>& printedNode)
	{
		cout << "키 : " << printedNode.m_key << " / 데이터 : " << printedNode.m_data;

		return out;
	}

private:

	SplayNode(int newKey, DataType newData)
	{
		m_key = newKey;
		m_data = newData;
		m_pLeftChild = NULL;
		m_pRightChild = NULL;
	}

	SplayNode(const SplayNode& sourceNode)
	{
		m_data = sourceNode.m_data;
		m_key = sourceNode.m_key;
		m_pLeftChild = nullptr;
		m_pRightChild = nullptr;
	}

private:

	int m_key;
	DataType m_data;
	SplayNode<DataType>* m_pLeftChild;
	SplayNode<DataType>* m_pRightChild;
};

template <typename DataType>
class SplayTree : public BstTemplate<SplayNode, DataType>
{
private:
	void SplayTarget(SplayNode<DataType>* pGreatGrandFatherOfTarget, SplayNode<DataType>* pGrandFatherOfTarget, SplayNode<DataType>* pFatherOfTarget, SplayNode<DataType>* pTarget);

	//ZIG의 경우에는 부모까지 변화가 일어나는 경우이므로, 부모를 가르키는 조부의 자식 포인터를 직접 수정할 수 있도록 레퍼런스 인자를 사용하였다.
	//ZIG_ZIG나 ZIG_ZAG와 같이 조부까지 변화가 일어나는 경우를 위해, 조부를 가리키는 증조부의 자식 포인터를 직접 수정할 수 있도록 레퍼런스 인자를 사용하였다.
	void ZigL(SplayNode<DataType>*& pFatherOfTarget, SplayNode<DataType>* pTarget)
	{
		cout << "ZigL" << endl;
		pFatherOfTarget->m_pLeftChild = pTarget->m_pRightChild;
		pTarget->m_pRightChild = pFatherOfTarget;
		pFatherOfTarget = pTarget;
	}

	void ZigR(SplayNode<DataType>*& pFatherOfTarget, SplayNode<DataType>* pTarget)
	{
		cout << "ZigR" << endl;
		pFatherOfTarget->m_pRightChild = pTarget->m_pLeftChild;
		pTarget->m_pLeftChild = pFatherOfTarget;
		pFatherOfTarget = pTarget;
	}

	void ZigZigLL(SplayNode<DataType>*& pGrandFatherOfTarget, SplayNode<DataType>* pFatherOfTarget, SplayNode<DataType>* pTarget)
	{
		cout << "ZigZigLL" << endl;
		pGrandFatherOfTarget->m_pLeftChild = pFatherOfTarget->m_pRightChild;
		pFatherOfTarget->m_pRightChild = pGrandFatherOfTarget;
		pGrandFatherOfTarget = pFatherOfTarget;

		pFatherOfTarget->m_pLeftChild = pTarget->m_pRightChild;
		pTarget->m_pRightChild = pFatherOfTarget;
		pGrandFatherOfTarget = pTarget;
	}

	void ZigZagLR(SplayNode<DataType>*& pGrandFatherOfTarget, SplayNode<DataType>* pFatherOfTarget, SplayNode<DataType>* pTarget)
	{
		cout << "ZigZagLR" << endl;
		cout << pGrandFatherOfTarget->m_key << " " << pFatherOfTarget->m_key << " " << pTarget->m_key << endl;
		pGrandFatherOfTarget->m_pLeftChild = pTarget;
		pFatherOfTarget->m_pRightChild = pTarget->m_pLeftChild;
		pTarget->m_pLeftChild = pFatherOfTarget;

		pGrandFatherOfTarget->m_pLeftChild = pTarget->m_pRightChild;
		pTarget->m_pRightChild = pGrandFatherOfTarget;
		pGrandFatherOfTarget = pTarget;
	}

	void ZigZagRL(SplayNode<DataType>*& pGrandFatherOfTarget, SplayNode<DataType>* pFatherOfTarget, SplayNode<DataType>* pTarget)
	{
		cout << "ZigZagRL" << endl;
		pGrandFatherOfTarget->m_pRightChild = pTarget;
		pFatherOfTarget->m_pLeftChild = pTarget->m_pRightChild;
		pTarget->m_pRightChild = pFatherOfTarget;

		pGrandFatherOfTarget->m_pRightChild = pTarget->m_pLeftChild;
		pTarget->m_pLeftChild = pGrandFatherOfTarget;
		pGrandFatherOfTarget = pTarget;
	}

	void ZigZigRR(SplayNode<DataType>*& pGrandFatherOfTarget, SplayNode<DataType>* pFatherOfTarget, SplayNode<DataType>* pTarget)
	{
		cout << "ZigZigRR" << endl;
		pGrandFatherOfTarget->m_pRightChild = pFatherOfTarget->m_pLeftChild;
		pFatherOfTarget->m_pLeftChild = pGrandFatherOfTarget;
		pGrandFatherOfTarget = pFatherOfTarget;

		pFatherOfTarget->m_pRightChild = pTarget->m_pLeftChild;
		pTarget->m_pLeftChild = pFatherOfTarget;
		pGrandFatherOfTarget = pTarget;
	}


public :
	SplayTree() : BstTemplate<SplayNode, DataType>() {}

	DataType Retrieve(int targetKey);
};

template <typename DataType>
DataType SplayTree<DataType>::Retrieve(int targetKey)
{
	SplayNode<DataType>* pTraversePtr = this->m_pHead;
	SplayNode<DataType>* pFatherOfTraversePtr = NULL;
	SplayNode<DataType>* pGrandFatherOfTraversePtr = NULL;
	SplayNode<DataType>* pGreatGrandFatherOfTraversePtr = NULL;
	while (pTraversePtr != NULL)
	{
		if (targetKey < pTraversePtr->m_key)
		{
			pGreatGrandFatherOfTraversePtr = pGrandFatherOfTraversePtr;
			pGrandFatherOfTraversePtr = pFatherOfTraversePtr;
			pFatherOfTraversePtr = pTraversePtr;
			pTraversePtr = pTraversePtr->m_pLeftChild;
		}
		else if (pTraversePtr->m_key < targetKey)
		{
			pGreatGrandFatherOfTraversePtr = pGrandFatherOfTraversePtr;
			pGrandFatherOfTraversePtr = pFatherOfTraversePtr;
			pFatherOfTraversePtr = pTraversePtr;
			pTraversePtr = pTraversePtr->m_pRightChild;
		}
		else
		{
			SplayTarget(pGreatGrandFatherOfTraversePtr, pGrandFatherOfTraversePtr, pFatherOfTraversePtr, pTraversePtr);

			return pTraversePtr->m_data;
		}
	}
}

template <typename DataType>
void SplayTree<DataType>::SplayTarget(SplayNode<DataType>* pGreatGrandFatherOfTarget, SplayNode<DataType>* pGrandFatherOfTarget, SplayNode<DataType>* pFatherOfTarget, SplayNode<DataType>* pTarget)
{
	if (pGreatGrandFatherOfTarget != NULL)
	{
		if (pGreatGrandFatherOfTarget->m_pLeftChild == pGrandFatherOfTarget)
		{
			if (pGrandFatherOfTarget->m_pLeftChild == pFatherOfTarget)
			{
				if (pFatherOfTarget->m_pLeftChild == pTarget) ZigZigLL(pGreatGrandFatherOfTarget->m_pLeftChild, pFatherOfTarget, pTarget);
				else ZigZagLR(pGreatGrandFatherOfTarget->m_pLeftChild, pFatherOfTarget, pTarget);
			}
			else
			{
				if (pFatherOfTarget->m_pLeftChild == pTarget) ZigZagRL(pGreatGrandFatherOfTarget->m_pLeftChild, pFatherOfTarget, pTarget);
				else ZigZigRR(pGreatGrandFatherOfTarget->m_pLeftChild, pFatherOfTarget, pTarget);
			}
		}
		else
		{
			if (pGrandFatherOfTarget->m_pLeftChild == pFatherOfTarget)
			{
				if (pFatherOfTarget->m_pLeftChild == pTarget) ZigZigLL(pGreatGrandFatherOfTarget->m_pRightChild, pFatherOfTarget, pTarget);
				else ZigZagLR(pGreatGrandFatherOfTarget->m_pRightChild, pFatherOfTarget, pTarget);
			}
			else
			{
				if (pFatherOfTarget->m_pLeftChild == pTarget) ZigZagRL(pGreatGrandFatherOfTarget->m_pRightChild, pFatherOfTarget, pTarget);
				else ZigZigRR(pGreatGrandFatherOfTarget->m_pRightChild, pFatherOfTarget, pTarget);
			}
		}
	}
	else if (pGrandFatherOfTarget != NULL)
	{
		if (pGrandFatherOfTarget->m_pLeftChild == pFatherOfTarget)
		{
			if (pFatherOfTarget->m_pLeftChild == pTarget) ZigZigLL(this->m_pHead, pFatherOfTarget, pTarget);
			else ZigZagLR(this->m_pHead, pFatherOfTarget, pTarget);
		}
		else
		{
			if (pFatherOfTarget->m_pLeftChild == pTarget) ZigZagRL(this->m_pHead, pFatherOfTarget, pTarget);
			else ZigZigRR(this->m_pHead, pFatherOfTarget, pTarget);
		}
	}
	else if (pFatherOfTarget != NULL)
	{
		if (pFatherOfTarget->m_pLeftChild == pTarget) ZigL(this->m_pHead, pTarget);
		else ZigR(this->m_pHead, pTarget);
	}
	else
	{
		//head를 스플레이 할 수는 없으므로, 아무것도 하지 말자
	}
}

#endif //SPLAY_TREE_H