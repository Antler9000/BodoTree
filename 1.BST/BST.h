#ifndef BST_USING_WHILE_H
#define BST_USING_WHILE_H

#include "BSTTemplate.h"
#include <iostream>
#include <utility>
#include <cstdint>

template <typename DataType>
class BST;

template <typename DataType>
class BSTNode
{
	friend class BSTTemplate<BSTNode, DataType>;
	friend class BST<DataType>;

	//NOTE : unique_ptr은 scope를 벗어날 때 가리키는 대상의 소멸을 호출하므로, HeapNode의 소멸자에 접근할 수 있어야 함
	friend struct std::default_delete<BSTNode<DataType>>;

	friend std::ostream& operator <<(std::ostream& out, const BSTNode<DataType>& printedNode)
	{
		std::cout << "키 : " << printedNode.m_key << " / 데이터 : " << printedNode.m_data;

		return out;
	}

private:

	//NOTE : 데이터가 lvalue인 경우와 rvalue인 경우를 모두 각 참조로 받을 수 있도록 포워딩을 사용함
	template <typename NewDataType = DataType>
	BSTNode(std::int32_t newKey, NewDataType&& newData) : m_key(newKey), m_data(std::forward<NewDataType>(newData)), m_pLeftChild(nullptr), m_pRightChild(nullptr)
	{

	}

	BSTNode(const BSTNode& sourceNode) : m_key(sourceNode.m_key), m_data(sourceNode.m_data), m_pLeftChild(nullptr), m_pRightChild(nullptr)
	{

	}

	//NOTE : 이진 탐색 트리 템플릿 클래스에 소멸자가 정의되어있으므로, 별도의 노드 소멸자 정의는 필요 없음
	~BSTNode() noexcept = default;

	//NOTE : 쓰이지 않는 노드 생성, 할당 방식들
	BSTNode() = delete;
	BSTNode(BSTNode&& sourceNode) = delete;
	BSTNode& operator = (const BSTNode& sourceNode) = delete;
	BSTNode& operator = (BSTNode&& sourceNode) = delete;

private:

	std::int32_t	m_key;
	DataType		m_data;
	BSTNode*		m_pLeftChild;
	BSTNode*		m_pRightChild;
};

template <typename DataType>
class BST : public BSTTemplate<BSTNode, DataType>
{
public:

	BST() = default;
	BST(const BST<DataType>& sourceTree) = default;
	BST(BST<DataType>&& sourceTree) noexcept = default;
	BST<DataType>& operator = (const BST<DataType>& sourceTree) = default;
	BST<DataType>& operator = (BST<DataType>&& sourceTree) noexcept = default;
	~BST() noexcept = default;
};

#endif //BST_USING_WHILE_H