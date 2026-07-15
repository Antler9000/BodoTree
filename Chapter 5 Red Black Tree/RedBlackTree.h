#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include "../Common/BstUsingWhileTemplate.h"
#include "../Common/Debug.h"
#include <cstdint>

enum NodeColor { RED, BLACK };

//여러 번역 단위에 include되는 헤더 파일에다가 함수 정의를 했을 때 생기는 중복 정의 문제를 피하기 위해 inline 키워드를 사용함
inline std::ostream& operator <<(std::ostream& out, NodeColor color)
{
	if (color == RED)
	{
		out << "RED";
	}
	else
	{
		out << "BLACK";
	}
	
	return out;
}

template <typename DataType>
class RedBlackTree;

template <typename DataType>
class RedBlackNode
{
	friend class BstTemplate<RedBlackNode, DataType>;
	friend class RedBlackTree<DataType>;

	friend std::ostream& operator <<(std::ostream& out, const RedBlackNode<DataType>& targetNode)
	{
		out << "키 : " << targetNode.m_key << " / 데이터 : " << targetNode.m_data << " / 컬러 : " << targetNode.m_color;

		return out;
	}

private:

	RedBlackNode(std::int32_t newKey, DataType newData)
	{
		m_key = newKey;
		m_data = newData;
		m_color = BLACK;
		m_pLeftChild = NULL;
		m_pRightChild = NULL;
	}

	RedBlackNode(const RedBlackNode& sourceNode)
	{
		m_data = sourceNode.m_data;
		m_key = sourceNode.m_key;
		m_color = sourceNode.m_color;
		m_pLeftChild = nullptr;
		m_pRightChild = nullptr;
	}

private:

	std::int32_t	m_key;
	DataType		m_data;
	NodeColor		m_color;
	RedBlackNode*	m_pLeftChild;
	RedBlackNode*	m_pRightChild;
};

template <typename DataType>
class RedBlackTree : public BstTemplate<RedBlackNode, DataType>
{
public:

	RedBlackTree() : BstTemplate<RedBlackNode, DataType>() {}

	void Insert(std::int32_t newKey, DataType newData)
	{
		if (this->m_pHead == NULL)
		{
			this->m_pHead = new RedBlackNode<DataType>(newKey, newData);

			return;
		}

		RedBlackNode<DataType>* pTraverse = this->m_pHead;
		Stack<RedBlackNode<DataType>*> pRouteStack;
		while (true)
		{
			pRouteStack.Push(pTraverse);
			CheckAndResolve4NodesWhileDescent(&pRouteStack);

			if (newKey < pTraverse->m_key)
			{
				if (pTraverse->m_pLeftChild != NULL)
				{
					pTraverse = pTraverse->m_pLeftChild;
				}
				else
				{
					pTraverse->m_pLeftChild = new RedBlackNode<DataType>(newKey, newData);
					pTraverse->m_pLeftChild->m_color = RED;
					pRouteStack.Push(pTraverse->m_pLeftChild);
					CheckAndResolve4NodesOnDestination(&pRouteStack);

					return;
				}
			}
			else if (pTraverse->m_key < newKey)
			{
				if (pTraverse->m_pRightChild != NULL)
				{
					pTraverse = pTraverse->m_pRightChild;
				}
				else
				{
					pTraverse->m_pRightChild = new RedBlackNode<DataType>(newKey, newData);
					pTraverse->m_pRightChild->m_color = RED;
					pRouteStack.Push(pTraverse->m_pRightChild);
					CheckAndResolve4NodesOnDestination(&pRouteStack);

					return;
				}
			}
			else
			{
				std::cout << "Cannot Insert node to tree! There are same newKey node already!" << std::endl;

				return;
			}
		}
	}

	void Remove(std::int32_t targetKey)
	{
		Stack<RedBlackNode<DataType>*> pRouteStack;

		if (this->m_pHead->m_key == targetKey)
		{
			pRouteStack.Push(this->m_pHead);
			RemoveTarget(this->m_pHead, &pRouteStack);

			return;
		}

		RedBlackNode<DataType>* pTraverse = this->m_pHead;
		while (true)
		{
			if (targetKey < pTraverse->m_key)
			{
				if (pTraverse->m_pLeftChild != NULL)
				{
					pRouteStack.Push(pTraverse);
					pTraverse = pTraverse->m_pLeftChild;
				}
				else
				{
					std::cout << "Cannot Remove! Cannot find such target node!" << std::endl;

					return;
				}
			}
			else if (pTraverse->m_key < targetKey)
			{
				if (pTraverse->m_pRightChild != NULL)
				{
					pRouteStack.Push(pTraverse);
					pTraverse = pTraverse->m_pRightChild;
				}
				else
				{
					std::cout << "Cannot Remove! Cannot find such target node!" << std::endl;

					return;
				}
			}
			else
			{
				RedBlackNode<DataType>* pParent = nullptr;
				pRouteStack.GetTop(pParent);
				pRouteStack.Push(pTraverse);
				if (pParent->m_pLeftChild == pTraverse)
				{
					RemoveTarget(pParent->m_pLeftChild, &pRouteStack);
				}
				else
				{
					RemoveTarget(pParent->m_pRightChild, &pRouteStack);
				}

				return;
			}
		}
	}

private:

	//삽입 메소드에서 삽입 위치를 찾기 위해 빈 리프노드 자리로 탐색하는 과정에서 매번 호출되는 메소드다.
	//4노드가 있으면 이를 쪼개놓고 내려가는 로직을 수행한다.
	void CheckAndResolve4NodesWhileDescent(Stack<RedBlackNode<DataType>*>* pRouteStack)
	{
		RedBlackNode<DataType>* pTarget = nullptr;
		pRouteStack->Pop(pTarget);
		RedBlackNode<DataType>* pParent = nullptr;
		pRouteStack->Pop(pParent);

		if (Is4Node(pTarget) == true)
		{
			pTarget->m_pLeftChild->m_color = BLACK;
			pTarget->m_pRightChild->m_color = BLACK;
			if (pParent != NULL && pParent->m_color == RED)
			{
				RedBlackNode<DataType>* pGrandParent = nullptr;
				pRouteStack->Pop(pGrandParent);
				RedBlackNode<DataType>* pGreatGrandParent = nullptr;
				pRouteStack->Pop(pGreatGrandParent);

				DoProperRotation(true, pTarget, pParent, pGrandParent, pGreatGrandParent);

				if (pGreatGrandParent != NULL) pRouteStack->Push(pGreatGrandParent);
				pRouteStack->Push(pGrandParent);
			}
			else Split4Node(pTarget);
		}

		pRouteStack->Push(pParent);
		pRouteStack->Push(pTarget);
	}

	bool Is4Node(RedBlackNode<DataType>* pTarget)
	{
		if (pTarget->m_pLeftChild != NULL && pTarget->m_pRightChild != NULL)
		{
			if (pTarget->m_pLeftChild->m_color == RED && pTarget->m_pRightChild->m_color == RED)
			{
				return true;
			}
		}
		return false;
	}

	void Split4Node(RedBlackNode<DataType>* pTarget)
	{
		if (pTarget != this->m_pHead) pTarget->m_color = RED;
	}

	//삽입 메소드에서는 빈 리프노드에 레드 노드의 형태로 삽입이 일어난다.
	//그렇게 삽입이 일어났는데 "조부노드-부모노드-삽입노드"가 "블랙-레드-레드"이렇게 연속적인 레드로 구성될 경우,
	//이를 확인하고 회전으로 해결하기 위한 메소드다.
	void CheckAndResolve4NodesOnDestination(Stack<RedBlackNode<DataType>*>* pRouteStack)
	{
		RedBlackNode<DataType>* targetNode = nullptr;
		pRouteStack->Pop(targetNode);
		RedBlackNode<DataType>* parentNode = nullptr;
		pRouteStack->Pop(parentNode);

		if (parentNode->m_color == RED)
		{
			RedBlackNode<DataType>* pGrandParent = nullptr;
			pRouteStack->Pop(pGrandParent);
			RedBlackNode<DataType>* pGreatGrandParent = nullptr;
			pRouteStack->Pop(pGreatGrandParent);
			DoProperRotation(true, targetNode, parentNode, pGrandParent, pGreatGrandParent);
		}
	}

	void DoProperRotation(bool isDoubleRed, RedBlackNode<DataType>* pTarget, RedBlackNode<DataType>* pParent, RedBlackNode<DataType>* pGrandParent, RedBlackNode<DataType>* pGreatGrandParent)
	{
		if (pGreatGrandParent == NULL)
		{
			SelectProperRotation(isDoubleRed, pTarget, pParent, this->m_pHead);
		}
		else if (pGreatGrandParent->m_pLeftChild == pGrandParent)
		{
			SelectProperRotation(isDoubleRed, pTarget, pParent, pGreatGrandParent->m_pLeftChild);
		}
		else
		{
			SelectProperRotation(isDoubleRed, pTarget, pParent, pGreatGrandParent->m_pRightChild);
		}
	}

	//회전으로 인해 조부 노드의 위치가 변하면, 증조부 노드의 자식 포인터도 그에 맞게 업데이트해줘야 함
	//따라서 조부 노드의 경우에는 단순히 해당 노드의 포인터를 값으로 받아오지 않고, 증조부 노드의 자식 포인터의 레퍼런스 매개변수로 받아왔음
	void SelectProperRotation(bool isDoubleRed, RedBlackNode<DataType>* pTarget, RedBlackNode<DataType>* pParent, RedBlackNode<DataType>*& pGrandParent)
	{
		if (pGrandParent->m_pLeftChild == pParent)
		{
			if (pParent->m_pLeftChild == pTarget)
			{
				RotationLL(isDoubleRed, pTarget, pParent, pGrandParent);
			}
			else
			{
				RotationLR(isDoubleRed, pTarget, pParent, pGrandParent);
			}
		}
		else
		{
			if (pParent->m_pLeftChild == pTarget)
			{
				RotationRL(isDoubleRed, pTarget, pParent, pGrandParent);
			}
			else
			{
				RotationRR(isDoubleRed, pTarget, pParent, pGrandParent);
			}
		}
	}

	void RotationLL(bool isDoubleRed, RedBlackNode<DataType>* pTarget, RedBlackNode<DataType>* pParent, RedBlackNode<DataType>*& pGrandParent)
	{
		if (isDoubleRed == true)
		{
			pGrandParent->m_color = RED;
			pParent->m_color = BLACK;
			pTarget->m_color = RED;
		}

		pGrandParent->m_pLeftChild = pParent->m_pRightChild;
		pParent->m_pRightChild = pGrandParent;
		pGrandParent = pParent;
	}

	void RotationLR(bool isDoubleRed, RedBlackNode<DataType>* pTarget, RedBlackNode<DataType>* pParent, RedBlackNode<DataType>*& pGrandParent)
	{
		pGrandParent->m_pLeftChild = pTarget;
		pParent->m_pRightChild = pTarget->m_pLeftChild;
		pTarget->m_pLeftChild = pParent;
		RotationLL(isDoubleRed, pParent, pTarget, pGrandParent);
	}

	void RotationRL(bool isDoubleRed, RedBlackNode<DataType>* pTarget, RedBlackNode<DataType>* pParent, RedBlackNode<DataType>*& pGrandParent)
	{
		pGrandParent->m_pRightChild = pTarget;
		pParent->m_pLeftChild = pTarget->m_pRightChild;
		pTarget->m_pRightChild = pParent;
		RotationRR(isDoubleRed, pParent, pTarget, pGrandParent);
	}

	void RotationRR(bool isDoubleRed, RedBlackNode<DataType>* pTarget, RedBlackNode<DataType>* pParent, RedBlackNode<DataType>*& pGrandParent)
	{
		if (isDoubleRed == true)
		{
			pGrandParent->m_color = RED;
			pParent->m_color = BLACK;
			pTarget->m_color = RED;
		}

		pGrandParent->m_pRightChild = pParent->m_pLeftChild;
		pParent->m_pLeftChild = pGrandParent;
		pGrandParent = pParent;
	}

	//삭제 대상이 리프 노드여서 아에 삭제되는 경우,
	//그 노드를 가리키는 부모노드의 자식 포인터를 null로 해야하기에 레퍼런스 매개변수로 두었음
	void RemoveTarget(RedBlackNode<DataType>*& pTarget, Stack<RedBlackNode<DataType>*>* pRouteStack)
	{
		if (pTarget->m_pLeftChild != NULL && pTarget->m_pRightChild != NULL)	//두 자식 모두 있는 경우엔, 중위선행자와 중위후속자 중에서 그냥 중위후속자(오른쪽 자식 트리에서 제일 작은 키 값의 노드)를 없애기로함
		{
			ReplaceWithInorderSuccessor(pTarget, pRouteStack);
		}
		else if (pTarget->m_pLeftChild == NULL && pTarget->m_pRightChild != NULL)
		{
			ReplaceWithInorderSuccessor(pTarget, pRouteStack);
		}
		else if (pTarget->m_pLeftChild != NULL && pTarget->m_pRightChild == NULL)
		{
			ReplaceWithInorderPredecessor(pTarget, pRouteStack);
		}
		else  //자식이 없는 경우에도 블랙 노드 균형을 맞추기 위한 과정이 필요하므로, 중위후속자 대체 메소드를 호출하도록 하자. 중위후속자가 없으므로 target_node의 자리에서 삭제가 일어나지만 균형을 맞추는 과정은 수행할 것이다.
		{
			ReplaceWithInorderSuccessor(pTarget, pRouteStack);
		}
	}

	//이진탐색트리에서의 replace_with_inorder_predecessor(...)과 차이점은,
	//중위 선행자나 후속자의 삭제 문제로 대체할 때, 트리의 블랙 노드의 균형이 깨지는 것을 방지하기 위해서 다음을 수행한다는 점이다.
	void ReplaceWithInorderPredecessor(RedBlackNode<DataType>*& pTarget, Stack<RedBlackNode<DataType>*>* pRouteStack)
	{
		if (pTarget->m_pLeftChild != NULL)
		{
			RedBlackNode<DataType>* traversePtr = pTarget->m_pLeftChild;
			pRouteStack->Push(traversePtr);
			while (traversePtr->m_pRightChild != NULL)
			{
				traversePtr = traversePtr->m_pRightChild;
				pRouteStack->Push(traversePtr);
			}
		}
		
		RedBlackNode<DataType>* pPredecessor = nullptr;
		pRouteStack->Pop(pPredecessor);
		RedBlackNode<DataType>* pParentOfPredecessor = nullptr;
		pRouteStack->Pop(pParentOfPredecessor);
		RedBlackNode<DataType>* pGrandParentOfPredecessor = nullptr;
		pRouteStack->Pop(pGrandParentOfPredecessor);
		pTarget->m_key = pPredecessor->m_key;
		pTarget->m_data = pPredecessor->m_data;

		//트리에 헤드 노드만 하나 남은 경우
		if (pParentOfPredecessor == NULL)
		{
			delete this->m_pHead;
			this->m_pHead = NULL;

			return;
		}

		//1.중위 선행자 자신이 레드 노드인가?
		// -> 그렇다면 그냥 삭제하면 된다
		if (pPredecessor->m_color == RED)
		{
			if (pParentOfPredecessor->m_pLeftChild == pPredecessor) pParentOfPredecessor->m_pLeftChild = NULL;
			else pParentOfPredecessor->m_pRightChild = NULL;
			delete pPredecessor;

			return;
		}
		//2.위 경우가 아니라면, 중위 선행자에게 자식으로 레드 리프 노드가 달려있는가?
		// -> 그렇다면 중위 선행자의 부모에 해당 자식을 붙이며, 해당 빨간 자식 노드의 색깔을 검은색으로 바꾸면 된다
		if ((pPredecessor->m_pLeftChild != NULL) && (pPredecessor->m_pLeftChild->m_color == RED))
		{
			pPredecessor->m_key = pPredecessor->m_pLeftChild->m_key;
			pPredecessor->m_data = pPredecessor->m_pLeftChild->m_data;
			delete pPredecessor->m_pLeftChild;
			pPredecessor->m_pLeftChild = NULL;

			return;
		}
		//3~5. 선행자 본인과 그 자식들로는 블랙 노드의 균형을 맞출 수 없다. 부모와 형제자매 노드를 이용해보자.
		BalanceTheBlackNodeByFamily(pPredecessor, pParentOfPredecessor, pGrandParentOfPredecessor, pRouteStack);

		return;
	}

	void ReplaceWithInorderSuccessor(RedBlackNode<DataType>*& pTarget, Stack<RedBlackNode<DataType>*>* pRouteStack)
	{
		if (pTarget->m_pRightChild != NULL)
		{
			RedBlackNode<DataType>* traversePtr = pTarget->m_pRightChild;
			pRouteStack->Push(traversePtr);
			while (traversePtr->m_pLeftChild != NULL)
			{
				traversePtr = traversePtr->m_pLeftChild;
				pRouteStack->Push(traversePtr);
			}
		}

		RedBlackNode<DataType>* pSuccessor = nullptr;
		pRouteStack->Pop(pSuccessor);
		RedBlackNode<DataType>* pParentOfSuccessor = nullptr;
		pRouteStack->Pop(pParentOfSuccessor);
		RedBlackNode<DataType>* pGrandParentOfSuccessor = nullptr;
		pRouteStack->Pop(pGrandParentOfSuccessor);
		pTarget->m_key = pSuccessor->m_key;
		pTarget->m_data = pSuccessor->m_data;

		//트리에 헤드 노드만 하나 남은 경우
		if (pParentOfSuccessor == NULL) {
			delete this->m_pHead;
			this->m_pHead = NULL;

			return;
		}

		//1.중위 후속자 자신이 레드 노드인가?
		// -> 그렇다면 그냥 삭제하면 된다
		if (pSuccessor->m_color == RED) {
			if (pParentOfSuccessor->m_pRightChild == pSuccessor) pParentOfSuccessor->m_pRightChild = NULL;
			else pParentOfSuccessor->m_pLeftChild = NULL;
			delete pSuccessor;

			return;
		}

		//2.위 경우가 아니라면, 중위 후속자에게 자식으로 레드 리프 노드가 달려있는가?
		// -> 그렇다면 중위 후속자의 부모에 해당 자식을 붙이며, 해당 빨간 자식 노드의 색깔을 검은색으로 바꾸면 된다
		if ((pSuccessor->m_pRightChild != NULL) && (pSuccessor->m_pRightChild->m_color == RED)) {
			pSuccessor->m_key = pSuccessor->m_pRightChild->m_key;
			pSuccessor->m_data = pSuccessor->m_pRightChild->m_data;
			delete pSuccessor->m_pRightChild;
			pSuccessor->m_pRightChild = NULL;

			return;
		}

		//3~5.후속자 본인과 그 자식들로는 블랙 노드의 균형을 맞출 수 없다. 부모와 형제자매 노드를 이용해보자.
		if (pParentOfSuccessor->m_pLeftChild == pSuccessor) pParentOfSuccessor->m_pLeftChild = NULL;
		else pParentOfSuccessor->m_pRightChild = NULL;
		delete pSuccessor;
		BalanceTheBlackNodeByFamily(pSuccessor, pParentOfSuccessor, pGrandParentOfSuccessor, pRouteStack);
	}

	void BalanceTheBlackNodeByFamily(RedBlackNode<DataType>* pTarget, RedBlackNode<DataType>* pParent, RedBlackNode<DataType>* pGrandParent, Stack<RedBlackNode<DataType>*>* pRouteStack)
	{
		//3.앞선 경우들도 아니라면, 균형을 맞추기 위해 댕겨올 자매노드의 자식(=조카) 레드 노드가 있는가?
		// -> 그렇다면 회전이나 조정을 통해서 블랙 노드의 균형을 맞추면 된다.
		RedBlackNode<DataType>* pBrother;
		if (pParent->m_pLeftChild != NULL && pParent->m_pLeftChild != pTarget) pBrother = pParent->m_pLeftChild;
		else if (pParent->m_pRightChild != NULL && pParent->m_pRightChild != pTarget) pBrother = pParent->m_pRightChild;
		else pBrother = NULL;

		if (pBrother != NULL)
		{
			if (pBrother->m_color == RED)
			{
				//형제 노드가 레드 노드라면, 균형 조건에 의하면 무조건 두 개의 검은색 조카 노드가 존재한다.
				//그리고 이 조카들과 그것에 달려있을 수 있는 자식 레드 노드들을 사용해서 균형을 맞출 수 있다.
				//이 과정에서는 기존의 LL, LR, RL, RR 회전으로 수행할 수 없으므로, 여기서는 복잡하더라도 직접 노드 관계를 조정한다.
				//조정에 사용한 규칙은 다음과 같다.
				//-우선 삭제되는 중위 선행자나 후속자의 위치를 부모 노드로 채운다.
				//-그리고 이로 인해 비게 되는 부모 노드의 위치를 형제 노드의 자식이나 손자 중에 가장 값이 낮거나(형제가 우측일시), 높은(형제가 좌측일시) 녀석으로 채운다.
				//-여전히 트리 중간에 빈 노드가 존재하는 경우 앞선 방법처럼 가장 높거나 낮은 녀석을 이용해서 채운다.
				if (pGrandParent != NULL && pGrandParent->m_pLeftChild == pParent)
				{
					DoProperAdjustment(pTarget, pBrother, pGrandParent->m_pLeftChild);

					return;
				}
				else if (pGrandParent != NULL && pGrandParent->m_pRightChild == pParent)
				{
					DoProperAdjustment(pTarget, pBrother, pGrandParent->m_pRightChild);

					return;
				}
				else if (pGrandParent == NULL)
				{
					DoProperAdjustment(pTarget, pBrother, this->m_pHead);

					return;
				}
				else
				{
					std::cout << "Cannot be here! The pParent should be one of pGrandParent's childs" << std::endl;

					return;
				}
			}
			else if ((pBrother->m_pLeftChild != NULL) && (pBrother->m_pLeftChild->m_color == RED))
			{	
				if (pParent->m_pLeftChild == pBrother)
				{
					pBrother->m_pLeftChild->m_color = pParent->m_color;
					pParent->m_color = BLACK;
					pBrother->m_color = BLACK;
					
				}
				else if (pParent->m_pRightChild == pBrother)
				{
					pBrother->m_pLeftChild->m_color = pParent->m_color;
					pParent->m_color = BLACK;
					pBrother->m_color = BLACK;
				}
				else
				{
					std::cout << "Cannot be here! The pBrother should be one of pParent's childs" << std::endl;

					return;
				}

				DoProperRotation(false, pBrother->m_pLeftChild, pBrother, pParent, pGrandParent);

				return;
			}
			else if ((pBrother->m_pRightChild != NULL) && (pBrother->m_pRightChild->m_color == RED))
			{
				if (pParent->m_pLeftChild == pBrother)
				{
					pBrother->m_pRightChild->m_color = pParent->m_color;
					pParent->m_color = BLACK;
					pBrother->m_color = BLACK;
				}
				else if (pParent->m_pRightChild == pBrother)
				{
					pBrother->m_pRightChild->m_color = pParent->m_color;
					pParent->m_color = BLACK;
					pBrother->m_color = BLACK;
				}
				else
				{
					std::cout << "Cannot be here! The pBrother should be one of pParent's childs" << std::endl;

					return;
				}
				
				DoProperRotation(false, pBrother->m_pRightChild, pBrother, pParent, pGrandParent);

				return;
			}
			else
			{
				//댕겨올 조카 노드들이 없으므로, 블럭을 나와 다음 4번 과정으로 진행한다.
			}
		}

		//4.위 경우도 아니라면, 부모노드가 레드노드인가?
		// -> 부모노드는 블랙노드가 되도록 하고, 자매 노드가 있으면 레드 노드가 되도록 하여 균형을 맞추자
		if (pParent->m_color == RED)
		{
			pParent->m_color = BLACK;
			if (pBrother != NULL) pBrother->m_color = RED;

			return;
		}


		//5.위 경우도 아니라면
		// -> 자매 노드가 있으면 레드 노드가 되도록 하자. 그리고 부모노드 위치의 삭제 문제로 변환하여 다시 3번부터 수행하도록 하자
		if (pBrother != NULL) pBrother->m_color = RED;

		if (pGrandParent != NULL)
		{
			pTarget = pParent;
			pParent = pGrandParent;
			pGrandParent = nullptr;
			pRouteStack->Pop(pGrandParent);

			BalanceTheBlackNodeByFamily(pTarget, pParent, pGrandParent, pRouteStack);
		}

		return;
	}
	
	//삭제되는 중위 선행자나 후속자의 부모 노드의 교체를 원활히 교체하기 위해서, 일반 변수가 아니라 레퍼런스 변수로 이를 지칭하였다.
	void DoProperAdjustment(RedBlackNode<DataType>* pTarget, RedBlackNode<DataType>* pBrother, RedBlackNode<DataType>*& pParent)
	{
		RedBlackNode<DataType>* pOriginalParent = pParent;

		if (pOriginalParent->m_pLeftChild == pBrother)
		{
			if (pBrother->m_pRightChild->m_pRightChild != NULL && pBrother->m_pRightChild->m_pRightChild->m_color == RED)
			{
				pBrother->m_pRightChild->m_pRightChild->m_color = BLACK;

				pOriginalParent->m_pLeftChild = NULL;
				pParent = pBrother->m_pRightChild->m_pRightChild;
				pBrother->m_pRightChild->m_pRightChild = NULL;
				pParent->m_pRightChild = pOriginalParent;
				pParent->m_pLeftChild = pBrother;

				return;
			}
			else if (pBrother->m_pRightChild->m_pLeftChild != NULL && pBrother->m_pRightChild->m_pLeftChild->m_color == RED)
			{
				pBrother->m_pRightChild->m_pLeftChild->m_color = BLACK;

				pOriginalParent->m_pLeftChild = NULL;
				pParent = pBrother->m_pRightChild;
				pBrother->m_pRightChild = pBrother->m_pRightChild->m_pLeftChild;
				pParent->m_pRightChild = pOriginalParent;
				pParent->m_pLeftChild = pBrother;

				return;
			}
			else if (pBrother->m_pLeftChild->m_pRightChild != NULL && pBrother->m_pLeftChild->m_pRightChild->m_color == RED)
			{
				pBrother->m_color = BLACK;

				pOriginalParent->m_pLeftChild = NULL;
				pParent = pBrother->m_pRightChild;
				pBrother->m_pRightChild = NULL;
				pParent->m_pRightChild = pOriginalParent;
				pParent->m_pLeftChild = pBrother->m_pLeftChild->m_pRightChild;
				pBrother->m_pLeftChild->m_pRightChild = NULL;
				pParent->m_pLeftChild->m_pRightChild = pBrother;
				pParent->m_pLeftChild->m_pLeftChild = pBrother->m_pLeftChild;
				pBrother->m_pLeftChild = NULL;

				return;
			}
			else if (pBrother->m_pLeftChild->m_pLeftChild != NULL && pBrother->m_pLeftChild->m_pLeftChild->m_color == RED)
			{
				pOriginalParent->m_pLeftChild = NULL;
				pParent = pBrother->m_pRightChild;
				pBrother->m_pRightChild = NULL;
				pParent->m_pRightChild = pOriginalParent;
				pParent->m_pLeftChild = pBrother->m_pLeftChild;
				pParent->m_pLeftChild->m_pRightChild = pBrother;
				pBrother->m_pLeftChild = NULL;

				return;
			}
			//균형을 맞출 조카의 레드 노드 자식들이 없는 경우에도 조정이 가능하다.
			else
			{
				pBrother->m_color = BLACK;
				pBrother->m_pLeftChild->m_color = RED;

				pOriginalParent->m_pLeftChild = NULL;
				pParent = pBrother->m_pRightChild;
				pBrother->m_pRightChild = NULL;
				pParent->m_pRightChild = pOriginalParent;
				pParent->m_pLeftChild = pBrother;
				return;
			}
		}
		else if (pOriginalParent->m_pRightChild == pBrother)
		{
			if (pBrother->m_pLeftChild->m_pLeftChild != NULL && pBrother->m_pLeftChild->m_pLeftChild->m_color == RED)
			{
				pBrother->m_pLeftChild->m_pLeftChild->m_color = BLACK;

				pOriginalParent->m_pRightChild = NULL;
				pParent = pBrother->m_pLeftChild->m_pLeftChild;
				pBrother->m_pLeftChild->m_pLeftChild = NULL;
				pParent->m_pLeftChild = pOriginalParent;
				pParent->m_pRightChild = pBrother;

				return;
			}
			else if (pBrother->m_pLeftChild->m_pRightChild != NULL && pBrother->m_pLeftChild->m_pRightChild->m_color == RED)
			{
				pBrother->m_pLeftChild->m_pRightChild->m_color = BLACK;

				pOriginalParent->m_pRightChild = NULL;
				pParent = pBrother->m_pLeftChild;
				pBrother->m_pLeftChild = pBrother->m_pLeftChild->m_pRightChild;
				pParent->m_pLeftChild = pOriginalParent;
				pParent->m_pRightChild = pBrother;

				return;
			}
			else if (pBrother->m_pRightChild->m_pLeftChild != NULL && pBrother->m_pRightChild->m_pLeftChild->m_color == RED)
			{
				pBrother->m_color = BLACK;

				pOriginalParent->m_pRightChild = NULL;
				pParent = pBrother->m_pLeftChild;
				pBrother->m_pLeftChild = NULL;
				pParent->m_pLeftChild = pOriginalParent;
				pParent->m_pRightChild = pBrother->m_pRightChild->m_pLeftChild;
				pBrother->m_pRightChild->m_pLeftChild = NULL;
				pParent->m_pRightChild->m_pLeftChild = pBrother;
				pParent->m_pRightChild->m_pRightChild = pBrother->m_pRightChild;
				pBrother->m_pRightChild = NULL;

				return;
			}
			else if (pBrother->m_pRightChild->m_pRightChild != NULL && pBrother->m_pRightChild->m_pRightChild->m_color == RED)
			{
				pOriginalParent->m_pRightChild = NULL;
				pParent = pBrother->m_pLeftChild;
				pBrother->m_pLeftChild = NULL;
				pParent->m_pLeftChild = pOriginalParent;
				pParent->m_pRightChild = pBrother->m_pRightChild;
				pParent->m_pRightChild->m_pLeftChild = pBrother;
				pBrother->m_pRightChild = NULL;


				return;
			}
			//균형을 맞출 조카의 레드 노드 자식들이 없는 경우에도 조정이 가능하다.
			else
			{
				pBrother->m_color = BLACK;
				pBrother->m_pRightChild->m_color = RED;

				pOriginalParent->m_pRightChild = NULL;
				pParent = pBrother->m_pLeftChild;
				pBrother->m_pLeftChild = NULL;
				pParent->m_pLeftChild = pOriginalParent;
				pParent->m_pRightChild = pBrother;
				return;
			}
		}
		else
		{
			std::cout << "Cannot be here! The pBrother should be one of ParentOfTarget's childs" << std::endl;

			return;
		}
	}
};

#endif //RED_BLACK_TREE_H