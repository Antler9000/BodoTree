#ifndef HEAP_H
#define HEAP_H

#include "../Common/Debug.h"
#include <iostream>
#include <memory>
#include <utility>
#include <cstdint>

template <typename DataType>
class Heap;

template <typename DataType>
class MinHeap;

template <typename DataType>
class MaxHeap;

template <typename DataType>
class HeapNode
{
	//NOTE : MinHeap과 MaxHeap 까지만 구현할 예정이므로, 여기에 friend를 일일이 선언하는 방식을 사용했음
	friend class Heap <DataType>;
	friend class MinHeap<DataType>;
	friend class MaxHeap<DataType>;

	//NOTE : unique_ptr은 유사시 가리키는 대상의 소멸을 호출하므로, HeapNode의 소멸자에 접근할 수 있어야 함
	friend struct std::default_delete<HeapNode<DataType>[]>;

private:

	HeapNode() : m_key(0), m_data()
	{

	}

	//NOTE : 데이터가 lvalue인 경우와 rvalue인 경우를 모두 각 참조로 받을 수 있도록 포워딩을 사용함
	template <typename NewDataType = DataType>
	HeapNode(std::int32_t key, NewDataType&& data) : m_key(key), m_data(std::forward<NewDataType>(data))
	{

	}

	HeapNode(const HeapNode& sourceHeapNode) : m_key(sourceHeapNode.m_key), m_data(sourceHeapNode.m_data)
	{

	}

	HeapNode(HeapNode&& sourceHeapNode) noexcept : m_key(sourceHeapNode.m_key), m_data(std::move(sourceHeapNode.m_data))
	{

	}

	HeapNode& operator = (const HeapNode& sourceNode)
	{
		if (this == &sourceNode)
		{
			return *this;
		}

		m_key = sourceNode.m_key;
		m_data = sourceNode.m_data;

		return *this;
	}

	HeapNode& operator = (HeapNode&& sourceNode) noexcept
	{
		if (this == &sourceNode)
		{
			return *this;
		}

		m_key = sourceNode.m_key;
		m_data = std::move(sourceNode.m_data);

		sourceNode.m_key = 0;
		//NOTE : source 측의 m_data는 DataType의 이동 생성자를 통해 이미 빈 상태가 되었다고 가정함

		return *this;
	}

	~HeapNode() noexcept = default;

private:
	
	std::int32_t	m_key;
	DataType		m_data;
};

template <typename DataType>
class Heap
{
public:

	Heap() : m_pNodes(DBG_NEW HeapNode<DataType>[50]), m_size(0), m_capacity(50)
	{
		LogPrint("empty constructor");
	}

	Heap(const Heap& sourceHeap) : m_pNodes(nullptr), m_size(0), m_capacity(0)
	{
		LogPrint("copy constructor");

		CopyHeap(sourceHeap);
	}

	Heap(Heap&& sourceHeap) noexcept : m_pNodes(sourceHeap.m_pNodes), m_size(sourceHeap.m_size), m_capacity(sourceHeap.m_capacity)
	{
		LogPrint("move constructor");

		sourceHeap.m_pNodes = nullptr;
		sourceHeap.m_size = 0;
		sourceHeap.m_capacity = 0;
	}

	Heap& operator = (const Heap& sourceHeap)
	{
		LogPrint("copy assignment");

		if (this == &sourceHeap)
		{
			return *this;
		}

		CopyHeap(sourceHeap);

		return *this;
	}

	Heap& operator = (Heap&& sourceHeap) noexcept
	{
		LogPrint("move assignment");

		if (this == &sourceHeap)
		{
			return *this;
		}

		RemoveHeap();

		m_pNodes = sourceHeap.m_pNodes;
		sourceHeap.m_pNodes = nullptr;

		m_size = sourceHeap.m_size;
		sourceHeap.m_size = 0;

		m_capacity = sourceHeap.m_capacity;
		sourceHeap.m_capacity = 0;

		return *this;
	}

	~Heap() noexcept
	{
		LogPrint("destructor");

		delete[] m_pNodes;
		m_pNodes = nullptr;
		m_size = 0;
		m_capacity = 0;
	}

	//PARAMTER : 데이터가 lvalue인 경우와 rvalue인 경우를 모두 각 참조로 받을 수 있도록 포워딩을 사용함
	template <typename PushDataType = DataType>
	void Push(std::int32_t newKey, PushDataType&& newData)
	{
		LogPrint("push");

		if (m_size >= m_capacity)
		{
			GiveLargerMemorySpace();
		}

		m_pNodes[m_size] = HeapNode<DataType>(newKey, std::forward<PushDataType>(newData));
		m_size++;

		ReorderByPromoting();
	}

	//RETURN : 힙이 비어져 있는 경우 false를 반환함
	bool Pop(DataType& outData)
	{
		LogPrint("pop");

		if (m_size <= 0)
		{
			WarningPrint("cannot pop from heap because heap is empty.");

			return false;
		}

		outData = std::move(m_pNodes[0].m_data);

		m_pNodes[0] = m_pNodes[m_size-1];	//NOTE : 노드 내부에 저장되는 DataType의 이동 할당자가 noexcept임이 보장되지 않아 move(..)를 사용하지 않았음
		m_size--;
		ReorderByDemoting();

		return true;
	}

	//RETURN : 힙이 비어져 있는 경우 false를 반환함
	bool GetTop(DataType& outData)
	{
		LogPrint("get top");

		if (m_size <= 0)
		{
			WarningPrint("can't get top from heap because heap is empty.");

			return false;
		}

		outData = m_pNodes[0].m_data;

		return true;
	}

	//RETURN : 힙에 하나의 데이터라도 있으면 false를 반환함
	bool IsEmpty()
	{
		LogPrint("is empty?");

		if (m_size <= 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//NOTE : Heap의 소멸자에서 사용되므로 예외를 던지는 경우가 없도록 하였음
	void RemoveHeap() noexcept
	{
		LogPrint("remove heap");

		delete[] m_pNodes;
		m_pNodes = nullptr;
		m_size = 0;
		m_capacity = 0;
	}

	void CopyHeap(const Heap& sourceHeap)
	{
		LogPrint("copy heap");

		std::unique_ptr<HeapNode<DataType>[]> upTempData = std::unique_ptr<HeapNode<DataType>[]>(DBG_NEW HeapNode<DataType>[sourceHeap.m_capacity]);
		for (std::int32_t i = 0; i < sourceHeap.m_size; i++)
		{
			upTempData[i] = sourceHeap.m_pNodes[i];
		}

		delete[] m_pNodes;
		m_pNodes = upTempData.release();
		m_capacity = sourceHeap.m_capacity;
		m_size = sourceHeap.m_size;
	}

	//NOTE : 디버깅용
	void PrintHeap()
	{
		LogPrint("print heap");

		for (std::int32_t i = 0; i < m_size; i++)
		{
			std::cout << "키 : " << m_pNodes[i].m_key << " / 데이터 : " << m_pNodes[i].m_data << std::endl;
		}
	}

private:

	void GiveLargerMemorySpace()
	{
		LogPrint("give larget memory space");

		std::int32_t newCapacity = (m_capacity == 0) ? (50) : (2 * m_capacity);
		std::unique_ptr<HeapNode<DataType>[]> upTempData = std::unique_ptr<HeapNode<DataType>[]>(DBG_NEW HeapNode<DataType>[newCapacity]);
		for (std::int32_t i = 0; i < m_size; i++)
		{
			upTempData[i] = m_pNodes[i];	//NOTE : 노드 내부에 저장되는 DataType의 이동 할당자가 noexcept임이 보장되지 않아 move(..)를 사용하지 않았음
		}

		delete[] m_pNodes;
		m_pNodes = upTempData.release();
		m_capacity = newCapacity;
	}

	//NOTE : Swap(..)으로부터 올라오는 예외에 안전하지 못한 구현이지만, 더 나은 방식이 생각나지 않아 그대로 두려고 함
	void ReorderByPromoting()
	{
		LogPrint("reorder by promoting");

		std::int32_t targetIndex = m_size - 1;
		std::int32_t parentIndex = GetParentIndex(targetIndex);
		while (targetIndex != parentIndex && IsNotOrdered(parentIndex, targetIndex))
		{
			Swap(m_pNodes[targetIndex], m_pNodes[parentIndex]);

			targetIndex = parentIndex;
			parentIndex = GetParentIndex(targetIndex);
		}
	}

	//NOTE : Swap(..)으로부터 올라오는 예외에 안전하지 못한 구현이지만, 더 나은 방식이 생각나지 않아 그대로 두려고 함
	void ReorderByDemoting()
	{
		LogPrint("reorder by demoting");

		std::int32_t targetIndex = 0;
		std::int32_t leftChildIndex = GetLeftChildIndex(targetIndex);
		std::int32_t rightChildIndex = GetRightChildIndex(targetIndex);
		std::int32_t targetChildIndex;
		while (leftChildIndex < m_size)
		{
			if (rightChildIndex < m_size)
			{
				if (IsLeftChildTarget(leftChildIndex, rightChildIndex) == true)
				{
					targetChildIndex = leftChildIndex;
				}
				else
				{
					targetChildIndex = rightChildIndex;
				}
			}
			else
			{
				targetChildIndex = leftChildIndex;
			}

			if (IsNotOrdered(targetIndex, targetChildIndex) == true)
			{
				Swap(m_pNodes[targetIndex], m_pNodes[targetChildIndex]);

				targetIndex = targetChildIndex;
				leftChildIndex = GetLeftChildIndex(targetIndex);
				rightChildIndex = GetRightChildIndex(targetIndex);
			}
			else
			{
				break;
			}
		}
	}

	void Swap(HeapNode<DataType>& nodeA, HeapNode<DataType>& nodeB)
	{
		LogPrint("swap");

		HeapNode<DataType> temp = nodeA;	//NOTE : 노드 내부에 저장되는 DataType의 이동 할당자가 noexcept임이 보장되지 않아 move(..)를 사용하지 않았음
		nodeA = nodeB;						//NOTE : 여기부터 예외가 올라오면 nodeA나 nodeB가 훼손된 채로 끝나게 된다는 문제점이 있으나, 더 나은 방식을 생각해내지 못함
		nodeB = temp;
	}

	std::int32_t GetLeftChildIndex(std::int32_t dataIndex)
	{
		LogPrint("get left child index");

		return (dataIndex * 2 + 1);
	}

	std::int32_t GetRightChildIndex(std::int32_t dataIndex)
	{
		LogPrint("get right child index");

		return (dataIndex * 2 + 2);
	}

	std::int32_t GetParentIndex(std::int32_t dataIndex)
	{
		LogPrint("get parent index");

		return ((dataIndex - 1) / 2);
	}

	//NOTE : 상속된 최소힙 or 최대힙에서 각기 방식으로 구체화함
	virtual bool IsNotOrdered(std::int32_t parentIndex, std::int32_t childIndex) = 0;
	virtual bool IsLeftChildTarget(std::int32_t leftChildIndex, std::int32_t rightChildIndex) = 0;

protected:

	HeapNode<DataType>*	m_pNodes;
	std::int32_t		m_size;
	std::int32_t		m_capacity;
};

template <typename DataType>
class MinHeap : public Heap<DataType>
{
public:

	MinHeap() = default;
	MinHeap(const MinHeap& sourceTree) = default;
	MinHeap(MinHeap&& sourceTree) noexcept = default;
	MinHeap& operator = (const MinHeap& sourceTree) = default;
	MinHeap& operator = (MinHeap&& sourceTree) noexcept = default;
	~MinHeap() noexcept = default;

private:

	bool IsNotOrdered(std::int32_t parentIndex, std::int32_t childIndex)
	{
		LogPrint("is not ordered? in MinHeap");

		if (this->m_pNodes[parentIndex].m_key > this->m_pNodes[childIndex].m_key)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsLeftChildTarget(std::int32_t leftChildIndex, std::int32_t rightChildIndex)
	{
		LogPrint("is left child target? in MinHeap");

		if (this->m_pNodes[leftChildIndex].m_key < this->m_pNodes[rightChildIndex].m_key)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

template <typename DataType>
class MaxHeap : public Heap<DataType>
{
public:

	MaxHeap() = default;
	MaxHeap(const MaxHeap& sourceTree) = default;
	MaxHeap(MaxHeap&& sourceTree) noexcept = default;
	MaxHeap& operator = (const MaxHeap& sourceTree) = default;
	MaxHeap& operator = (MaxHeap&& sourceTree) noexcept = default;
	~MaxHeap() noexcept = default;

private:

	bool IsNotOrdered(std::int32_t parentIndex, std::int32_t childIndex)
	{
		LogPrint("is not ordered? in MaxHeap");

		if (this->m_pNodes[parentIndex].m_key < this->m_pNodes[childIndex].m_key)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsLeftChildTarget(std::int32_t leftChildIndex, std::int32_t rightChildIndex)
	{
		LogPrint("is left child target? in MaxHeap");

		if (this->m_pNodes[leftChildIndex].m_key > this->m_pNodes[rightChildIndex].m_key)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

#endif //HEAP_H