#ifndef HEAP_H
#define HEAP_H

#include "../Common/Debug.h"	//정의한 로그 출력 매크로를 사용함
#include <iostream>				//콘솔 출력을 사용함
#include <memory>				//유니크 포인터를 사용함
#include <utility>				//이동 시맨틱을 사용함
using namespace std;			//..

template <typename DataType>
class Heap;

template <typename DataType>
class MinHeap;

template <typename DataType>
class MaxHeap;

template <typename DataType>
class HeapNode
{
	friend class Heap <DataType>;

	//MinHeap과 MaxHeap 까지만 구현할 예정이므로, 굳이 추가클래스 구현에 열려있을 필요가 없어 여기에 friend를 일일이 선언하는 방식을 사용했음
	friend class MinHeap<DataType>;
	friend class MaxHeap<DataType>;

	//unique_ptr은 유사시 가리키는 대상의 소멸을 호출하므로, HeapNode의 소멸자에 접근할 수 있어야 한다
	friend struct default_delete<HeapNode<DataType>[]>;

private:

	HeapNode() : m_key(0), m_data()
	{

	}

	template <typename NewDataType = DataType>
	HeapNode(int key, NewDataType&& data) : m_key(key), m_data(forward<NewDataType>(data))
	{

	}

	HeapNode(const HeapNode& sourceHeapNode) : m_key(sourceHeapNode.m_key), m_data(sourceHeapNode.m_data)
	{

	}

	HeapNode(HeapNode&& sourceHeapNode) noexcept : m_key(sourceHeapNode.m_key), m_data(move(sourceHeapNode.m_data))
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
		m_data = move(sourceNode.m_data);

		sourceNode.m_key = 0;
		//source 측의 m_data는 DataType의 이동 생성자를 통해 이미 빈 상태가 되었다고 가정함

		return *this;
	}

	~HeapNode() noexcept
	{

	}

private:
	
	int			m_key;
	DataType	m_data;
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

	template <typename PushDataType = DataType>
	void Push(int newKey, PushDataType&& newData)
	{
		LogPrint("push");

		if (m_size >= m_capacity)
		{
			GiveLargerMemorySpace();
		}

		m_pNodes[m_size] = HeapNode<DataType>(newKey, forward<PushDataType>(newData));
		m_size++;

		ReorderByPromoting();
	}

	//bool 반환값이 false인 경우 : 힙이 비어져 있는 경우
	bool Pop(DataType& outData)
	{
		LogPrint("pop");

		if (m_size <= 0)
		{
			WarningPrint("cannot pop from heap because heap is empty.");

			return false;
		}

		outData = m_pNodes[0].m_data;		//DataType의 이동 생성자가 noexcept임이 보장되지 않기에 move(..)를 사용하지 않았다

		m_pNodes[0] = m_pNodes[m_size-1];
		m_size--;
		ReorderByDemoting();

		return true;
	}

	//bool 반환값이 false인 경우 : 힙이 비어져 있는 경우
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

		unique_ptr<HeapNode<DataType>[]> upTempData = unique_ptr<HeapNode<DataType>[]>(DBG_NEW HeapNode<DataType>[sourceHeap.m_capacity]);

		for (int i = 0; i < sourceHeap.m_size; i++)
		{
			upTempData[i] = sourceHeap.m_pNodes[i];
		}

		delete[] m_pNodes;
		m_pNodes = upTempData.release();
		m_capacity = sourceHeap.m_capacity;
		m_size = sourceHeap.m_size;
	}

	//디버깅용 메소드임
	void PrintHeap()
	{
		LogPrint("print heap");

		for (int i = 0; i < m_size; i++)
		{
			cout << "키 : " << m_pNodes[i].m_key << " / 데이터 : " << m_pNodes[i].m_data << endl;
		}
	}

protected:

	void GiveLargerMemorySpace()
	{
		LogPrint("give larget memory space");

		int newCapacity = (m_capacity == 0) ? (50) : (2 * m_capacity);
		unique_ptr<HeapNode<DataType>[]> upTempData = unique_ptr<HeapNode<DataType>[]>(DBG_NEW HeapNode<DataType>[newCapacity]);

		for (int i = 0; i < m_size; i++)
		{
			upTempData[i] = m_pNodes[i];	//DataType의 이동 생성자가 noexcept임이 보장되지 않기에 move(..)를 사용하지 않았다
		}

		delete[] m_pNodes;
		m_pNodes = upTempData.release();
		m_capacity = newCapacity;
	}

	//Swap(..)으로부터 올라오는 예외에 안전하지 못한 구현이지만, 더 나은 방식이 생각나지 않아 그대로 두려고 함
	void ReorderByPromoting()
	{
		LogPrint("reorder by promoting");

		int targetIndex = m_size - 1;
		int parentIndex = GetParentIndex(targetIndex);

		while (targetIndex != parentIndex && IsNotOrdered(parentIndex, targetIndex))
		{
			Swap(m_pNodes[targetIndex], m_pNodes[parentIndex]);

			targetIndex = parentIndex;
			parentIndex = GetParentIndex(targetIndex);
		}
	}

	//Swap(..)으로부터 올라오는 예외에 안전하지 못한 구현이지만, 더 나은 방식이 생각나지 않아 그대로 두려고 함
	void ReorderByDemoting()
	{
		LogPrint("reorder by demoting");

		int targetIndex = 0;
		int leftChildIndex = GetLeftChildIndex(targetIndex);
		int rightChildIndex = GetRightChildIndex(targetIndex);
		int targetChildIndex;

		while (leftChildIndex < m_size)
		{
			if (rightChildIndex < m_size)
			{
				if (IsLeftChildTarget(leftChildIndex, rightChildIndex))
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

			if (IsNotOrdered(targetIndex, targetChildIndex))
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

protected:

	//데이터 타입의 이동 할당 연산자가 noexcept임이 보장되지 않기에, Swap(..)에 noexcept를 붙이지 않았다
	//같은 이유로 Swap(..) 내부도 예외 안전성이 강하지 못하나, 더 나은 방식이 생각나지 않아 이대로 두려고 한다
	void Swap(HeapNode<DataType>& nodeA, HeapNode<DataType>& nodeB)
	{
		LogPrint("swap");

		HeapNode<DataType> temp = move(nodeA);
		nodeA = move(nodeB);
		nodeB = move(temp);
	}

	int GetLeftChildIndex(int dataIndex)
	{
		LogPrint("get left child index");

		return (dataIndex * 2 + 1);
	}

	int GetRightChildIndex(int dataIndex)
	{
		LogPrint("get right child index");

		return (dataIndex * 2 + 2);
	}

	int GetParentIndex(int dataIndex)
	{
		LogPrint("get parent index");

		return ((dataIndex - 1) / 2);
	}

	virtual bool IsNotOrdered(int parentIndex, int childIndex) = 0;					//상속된 최소힙 or 최대힙에서 각기 방식으로 구체화함
	virtual bool IsLeftChildTarget(int leftChildIndex, int rightChildIndex) = 0;

protected:

	HeapNode<DataType>* m_pNodes;
	int m_size;
	int m_capacity;
};

template <typename DataType>
class MinHeap : public Heap<DataType>
{
public:

	MinHeap() = default;
	MinHeap(const MinHeap& sourceTree) = default;
	MinHeap(MinHeap&& sourceTree) = default;
	MinHeap& operator = (const MinHeap& sourceTree) = default;
	MinHeap& operator = (MinHeap&& sourceTree) = default;

private:

	bool IsNotOrdered(int parentIndex, int childIndex)
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

	bool IsLeftChildTarget(int leftChildIndex, int rightChildIndex)
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
	MaxHeap(MaxHeap&& sourceTree) = default;
	MaxHeap& operator = (const MaxHeap& sourceTree) = default;
	MaxHeap& operator = (MaxHeap&& sourceTree) = default;

private:

	bool IsNotOrdered(int parentIndex, int childIndex)
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

	bool IsLeftChildTarget(int leftChildIndex, int rightChildIndex)
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