#ifndef BST_USING_WHILE_H
#define BST_USING_WHILE_H

#include "../Common/BstUsingWhileTemplate.h"
#include <iostream>
#include <utility>

template <typename DataType>
class Bst;

template <typename DataType>
class BstNode
{
	friend class BstTemplate<BstNode, DataType>;
	friend class Bst<DataType>;

	//NOTE : unique_ptr은 유사시 가리키는 대상의 소멸을 호출하므로, HeapNode의 소멸자에 접근할 수 있어야 함
	friend struct std::default_delete<BstNode<DataType>>;

	friend std::ostream& operator <<(std::ostream& out, const BstNode<DataType>& printedNode)
	{
		std::cout << "키 : " << printedNode.m_key << " / 데이터 : " << printedNode.m_data;

		return out;
	}

private:

	//NOTE : 데이터가 lvalue인 경우와 rvalue인 경우를 모두 각 참조로 받을 수 있도록 포워딩을 사용함
	template <typename NewDataType = DataType>
	BstNode(int newKey, NewDataType&& newData) : m_key(newKey), m_data(std::forward<NewDataType>(newData)), m_pLeftChild(nullptr), m_pRightChild(nullptr)
	{

	}

	BstNode(const BstNode& sourceNode) : m_key(sourceNode.m_key), m_data(sourceNode.m_data), m_pLeftChild(nullptr), m_pRightChild(nullptr)
	{

	}

	//NOTE : 이진 탐색 트리 템플릿 클래스에 소멸자가 정의되어있으므로, 별도의 노드 소멸자 정의는 필요 없음
	~BstNode() noexcept
	{

	}

	//NOTE : 쓰이지 않는 노드 생성, 할당 방식들
	BstNode() = delete;
	BstNode(BstNode&& sourceNode) = delete;
	BstNode& operator = (const BstNode& sourceNode) = delete;
	BstNode& operator = (BstNode&& sourceNode) = delete;

private:

	int			m_key;
	DataType	m_data;
	BstNode*	m_pLeftChild;
	BstNode*	m_pRightChild;
};

template <typename DataType>
class Bst : public BstTemplate<BstNode, DataType>
{
public:

	Bst() = default;
	Bst(const Bst<DataType>& sourceTree) = default;
	Bst(Bst<DataType>&& sourceTree) noexcept = default;
	Bst<DataType>& operator = (const Bst<DataType>& sourceTree) = default;
	Bst<DataType>& operator = (Bst<DataType>&& sourceTree) noexcept = default;
	~Bst() noexcept = default;
};

#endif //BST_USING_WHILE_H