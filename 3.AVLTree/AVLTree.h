#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "../Common/BSTTemplate.h"
#include "../Common/Debug.h"
#include <cstdint>

template <typename DataType>
class AvlTree;

template <typename DataType>
class AVL_Node
{	
	friend class BstTemplate<AVL_Node, DataType>;
	friend class AvlTree<DataType>;

	friend std::ostream& operator <<(std::ostream& out, const AVL_Node<DataType>& printedNode)
	{
		std::cout << "키 : " << printedNode.m_key << " / 데이터 : " << printedNode.m_data << " / 높이 : " << printedNode.m_height;

		return out;
	}

private:

	AVL_Node(std::int32_t newKey, DataType newData)
	{
		m_key = newKey;
		m_data = newData;
		m_height = 0;
		m_pLeftChild = NULL;
		m_pRightChild = NULL;
	}

	AVL_Node(const AVL_Node& sourceNode)
	{
		m_key = sourceNode.m_key;
		m_data = sourceNode.m_data;
		m_height = sourceNode.m_height;
		m_pLeftChild = nullptr;
		m_pRightChild = nullptr;
	}

private:

	std::int32_t		m_key;
	DataType			m_data;
	std::int32_t		m_height;
	AVL_Node<DataType>*	m_pLeftChild;
	AVL_Node<DataType>*	m_pRightChild;
};

template <typename DataType>
class AvlTree : public BstTemplate<AVL_Node, DataType>
{
public:
	AvlTree() : BstTemplate<AVL_Node, DataType>()
	{
	
	}

	void Insert(std::int32_t newKey, DataType newData);

	void Remove(std::int32_t targetKey);

private:
	void RemoveTarget(AVL_Node<DataType>*& pTarget, Stack<AVL_Node<DataType>*>* pRouteStack);
	void ReplaceWithInorderPredecessor(AVL_Node<DataType>*& pTarget, Stack<AVL_Node<DataType>*>* pRouteStack);
	void ReplaceWithInorderSuccessor(AVL_Node<DataType>*& pTarget, Stack<AVL_Node<DataType>*>* pRouteStack);

	void BalancingAllTargetToRoot(Stack<AVL_Node<DataType>*>* pRouteStack);

	void BalancingTargetNode(AVL_Node<DataType>* pTarget, AVL_Node<DataType>* pParent);

	void RotationLL(AVL_Node<DataType>* pTarget, AVL_Node<DataType>* pParent);
	void RotationLR(AVL_Node<DataType>* pTarget, AVL_Node<DataType>* pParent);
	void RotationRL(AVL_Node<DataType>* pTarget, AVL_Node<DataType>* pParent);
	void RotationRR(AVL_Node<DataType>* pTarget, AVL_Node<DataType>* pParent);

	void UpdateHeight(AVL_Node<DataType>* pTarget)
	{
		std::int32_t heightFromLChild = 0;
		std::int32_t heightFromRChild = 0;
		if (pTarget->m_pLeftChild != NULL) heightFromLChild = 1 + pTarget->m_pLeftChild->m_height;
		if (pTarget->m_pRightChild != NULL) heightFromRChild = 1 + pTarget->m_pRightChild->m_height;
		pTarget->m_height = Max(heightFromLChild, heightFromRChild);
	}

	std::int32_t Max(std::int32_t a, std::int32_t b)
	{
		return (a > b) ? a : b;
	}
};

template <typename DataType>
inline void AvlTree<DataType>::RemoveTarget(AVL_Node<DataType>*& pTarget, Stack<AVL_Node<DataType>*>* pRouteStack)
{
	if (pTarget->m_pLeftChild != NULL && pTarget->m_pRightChild != NULL) //두 자식 모두 있는 경우엔, 중위선행자와 중위후속자 중에서 그냥 중위후속자(오른쪽 자식 트리에서 제일 작은 키 값의 노드)를 없애기로함
	{
		ReplaceWithInorderSuccessor(pTarget, pRouteStack);
	}
	else if (pTarget->m_pLeftChild == NULL && pTarget->m_pRightChild != NULL) {
		ReplaceWithInorderSuccessor(pTarget, pRouteStack);
	}
	else if (pTarget->m_pLeftChild != NULL && pTarget->m_pRightChild == NULL) {
		ReplaceWithInorderPredecessor(pTarget, pRouteStack);
	}
	else {
		delete pTarget;
		pTarget = NULL;
	}

	BalancingAllTargetToRoot(pRouteStack);
}

template <typename DataType>
inline void AvlTree<DataType>::ReplaceWithInorderPredecessor(AVL_Node<DataType>*& pTarget, Stack<AVL_Node<DataType>*>* pRouteStack)
{
	AVL_Node<DataType>* pPrevious = NULL;
	AVL_Node<DataType>* pTraverse = pTarget->m_pLeftChild;
	pRouteStack->Push(pTarget);
	while (pTraverse->m_pRightChild != NULL)
	{
		pPrevious = pTraverse;
		pTraverse = pTraverse->m_pRightChild;
		pRouteStack->Push(pPrevious);
	}

	if (pPrevious != NULL) pPrevious->m_pRightChild = pTraverse->m_pLeftChild;
	else pTarget->m_pLeftChild = pTraverse->m_pLeftChild;

	pTarget->m_key = pTraverse->m_key;
	pTarget->m_data = pTraverse->m_data;
	delete pTraverse;
}

template <typename DataType>
inline void AvlTree<DataType>::ReplaceWithInorderSuccessor(AVL_Node<DataType>*& pTarget, Stack<AVL_Node<DataType>*>* pRouteStack)
{
	AVL_Node<DataType>* pPrevious = NULL;
	AVL_Node<DataType>* pTraverse = pTarget->m_pRightChild;
	pRouteStack->Push(pTarget);
	while (pTraverse->m_pLeftChild != NULL)
	{
		pPrevious = pTraverse;
		pTraverse = pTraverse->m_pLeftChild;
		pRouteStack->Push(pPrevious);
	}

	if (pPrevious != NULL) pPrevious->m_pLeftChild = pTraverse->m_pRightChild;
	else pTarget->m_pRightChild = pTraverse->m_pRightChild;

	pTarget->m_key = pTraverse->m_key;
	pTarget->m_data = pTraverse->m_data;
	delete pTraverse;
}

template <typename DataType>
inline void AvlTree<DataType>::BalancingAllTargetToRoot(Stack<AVL_Node<DataType>*>* pRouteStack)
{
	while (pRouteStack->IsEmpty() == false)
	{
		AVL_Node<DataType>* pRetraverse = nullptr;
		pRouteStack->Pop(pRetraverse);
		AVL_Node<DataType>* pParentOfRetraverse = nullptr;
		pRouteStack->GetTop(pParentOfRetraverse);
		UpdateHeight(pRetraverse);

		std::cout << "node's height : " << pRetraverse->m_height << std::endl;

		BalancingTargetNode(pRetraverse, pParentOfRetraverse);
	}
}

template <typename DataType>
inline void AvlTree<DataType>::BalancingTargetNode(AVL_Node<DataType>* pTarget, AVL_Node<DataType>* pParent)
{
	std::int32_t leftHeight = 0;
	std::int32_t rightHeight = 0;
	if (pTarget->m_pLeftChild != NULL) leftHeight = 1 + pTarget->m_pLeftChild->m_height;
	if (pTarget->m_pRightChild != NULL) rightHeight = 1 + pTarget->m_pRightChild->m_height;

	if (leftHeight - rightHeight >= 2) {
		if (pTarget->m_pLeftChild->m_pRightChild == NULL)
		{
			RotationLL(pTarget, pParent);
		}
		else if (pTarget->m_pLeftChild->m_pLeftChild == NULL)
		{
			RotationLR(pTarget, pParent);
		}
		else
		{
			if (pTarget->m_pLeftChild->m_pLeftChild->m_height > pTarget->m_pLeftChild->m_pRightChild->m_height)
			{
				RotationLL(pTarget, pParent);
			}
			else
			{
				RotationLR(pTarget, pParent);
			}
		}
	}
	else if (rightHeight - leftHeight >= 2)
	{
		if (pTarget->m_pRightChild->m_pRightChild == NULL)
		{
			RotationRL(pTarget, pParent);
		}
		else if (pTarget->m_pRightChild->m_pLeftChild == NULL)
		{
			RotationRR(pTarget, pParent);
		}
		else
		{
			if (pTarget->m_pRightChild->m_pLeftChild->m_height > pTarget->m_pRightChild->m_pRightChild->m_height)
			{
				RotationRL(pTarget, pParent);
			}
			else
			{
				RotationRR(pTarget, pParent);
			}
		}
	}
}

template <typename DataType>
inline void AvlTree<DataType>::RotationLL(AVL_Node<DataType>* pTarget, AVL_Node<DataType>* pParent)
{
	std::cout << "LL 회전" << std::endl;

	if (pParent == NULL)
	{
		this->m_pHead = pTarget->m_pLeftChild;
		pTarget->m_pLeftChild = pTarget->m_pLeftChild->m_pRightChild;
		this->m_pHead->m_pRightChild = pTarget;
		this->m_pHead->m_height = pTarget->m_height - 1;
	}
	else if (pParent->m_pLeftChild == pTarget)
	{
		pParent->m_pLeftChild = pTarget->m_pLeftChild;
		pTarget->m_pLeftChild = pTarget->m_pLeftChild->m_pRightChild;
		pParent->m_pLeftChild->m_pRightChild = pTarget;
		pParent->m_pLeftChild->m_height = pTarget->m_height - 1;
	}
	else
	{
		pParent->m_pRightChild = pTarget->m_pLeftChild;
		pTarget->m_pLeftChild = pTarget->m_pLeftChild->m_pRightChild;
		pParent->m_pRightChild->m_pRightChild = pTarget;
		pParent->m_pRightChild->m_height = pTarget->m_height - 1;
	}

	UpdateHeight(pTarget);
}

template <typename DataType>
inline void AvlTree<DataType>::RotationLR(AVL_Node<DataType>* pTarget, AVL_Node<DataType>* pParent)
{
	std::cout << "LR 회전" << std::endl;

	AVL_Node<DataType>* pLR_Location = pTarget->m_pLeftChild->m_pRightChild;
	pTarget->m_pLeftChild->m_pRightChild = pLR_Location->m_pLeftChild;
	pLR_Location->m_pLeftChild = pTarget->m_pLeftChild;
	pTarget->m_pLeftChild = pLR_Location;

	UpdateHeight(pLR_Location->m_pLeftChild);
	UpdateHeight(pLR_Location);
	UpdateHeight(pTarget);

	RotationLL(pTarget, pParent);
}

template <typename DataType>
inline void AvlTree<DataType>::RotationRL(AVL_Node<DataType>* pTarget, AVL_Node<DataType>* pParent)
{
	std::cout << "RL 회전" << std::endl;

	AVL_Node<DataType>* pRL_Location = pTarget->m_pRightChild->m_pLeftChild;
	pTarget->m_pRightChild->m_pLeftChild = pRL_Location->m_pRightChild;
	pRL_Location->m_pRightChild = pTarget->m_pRightChild;
	pTarget->m_pRightChild = pRL_Location;

	UpdateHeight(pRL_Location->m_pRightChild);
	UpdateHeight(pRL_Location);
	UpdateHeight(pTarget);

	RotationRR(pTarget, pParent);
}

template <typename DataType>
inline void AvlTree<DataType>::RotationRR(AVL_Node<DataType>* pTarget, AVL_Node<DataType>* pParent)
{
	std::cout << "RR 회전" << std::endl;

	if (pParent == NULL)
	{
		this->m_pHead = pTarget->m_pRightChild;
		pTarget->m_pRightChild = pTarget->m_pRightChild->m_pLeftChild;
		this->m_pHead->m_pLeftChild = pTarget;
		this->m_pHead->m_height = pTarget->m_height - 1;
	}
	else if (pParent->m_pLeftChild == pTarget)
	{
		pParent->m_pLeftChild = pTarget->m_pRightChild;
		pTarget->m_pRightChild = pTarget->m_pRightChild->m_pLeftChild;
		pParent->m_pLeftChild->m_pLeftChild = pTarget;
		pParent->m_pLeftChild->m_height = pTarget->m_height - 1;
	}
	else
	{
		pParent->m_pRightChild = pTarget->m_pRightChild;
		pTarget->m_pRightChild = pTarget->m_pRightChild->m_pLeftChild;
		pParent->m_pRightChild->m_pLeftChild = pTarget;
		pParent->m_pRightChild->m_height = pTarget->m_height - 1;
	}

	UpdateHeight(pTarget);
}

template <typename DataType>
inline void AvlTree<DataType>::Insert(std::int32_t newKey, DataType newData)
{
	if (this->m_pHead == NULL)
	{
		this->m_pHead = new AVL_Node<DataType>(newKey, newData);
		return;
	}

	AVL_Node<DataType>* pTraverse = this->m_pHead;
	Stack<AVL_Node<DataType>*> pRouteStack;
	while (true)
	{
		if (newKey < pTraverse->m_key)
		{
			if (pTraverse->m_pLeftChild == NULL)
			{
				pTraverse->m_pLeftChild = new AVL_Node<DataType>(newKey, newData);
				pRouteStack.Push(pTraverse);
				BalancingAllTargetToRoot(&pRouteStack);
				return;
			}
			else
			{
				pRouteStack.Push(pTraverse);
				pTraverse = pTraverse->m_pLeftChild;
			}
		}
		else
		{
			if (pTraverse->m_pRightChild == NULL)
			{
				pTraverse->m_pRightChild = new AVL_Node<DataType>(newKey, newData);
				pRouteStack.Push(pTraverse);
				BalancingAllTargetToRoot(&pRouteStack);
				return;
			}
			else
			{
				pRouteStack.Push(pTraverse);
				pTraverse = pTraverse->m_pRightChild;
			}
		}
	}
}

template <typename DataType>
inline void AvlTree<DataType>::Remove(std::int32_t targetKey)
{
	if (this->m_pHead == NULL)
	{
		std::cout << "Cannot Remove! tree is emptied" << std::endl;
		return;
	}

	Stack<AVL_Node<DataType>*> pRouteStack;

	if (this->m_pHead->m_key == targetKey)
	{
		RemoveTarget(this->m_pHead, &pRouteStack);
		return;
	}

	AVL_Node<DataType>* pTraverse = this->m_pHead;
	while (true)
	{
		if (targetKey < pTraverse->m_key)
		{
			pRouteStack.Push(pTraverse);
			if (pTraverse->m_pLeftChild->m_key == targetKey)
			{
				RemoveTarget(pTraverse->m_pLeftChild, &pRouteStack);
				return;
			}
			else pTraverse = pTraverse->m_pLeftChild;
		}
		else
		{
			pRouteStack.Push(pTraverse);
			if (pTraverse->m_pRightChild->m_key == targetKey)
			{
				RemoveTarget(pTraverse->m_pRightChild, &pRouteStack);
				return;
			}
			else pTraverse = pTraverse->m_pRightChild;
		}
	}
}

#endif //AVL_TREE_H