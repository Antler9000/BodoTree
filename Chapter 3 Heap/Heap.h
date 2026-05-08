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

	HeapNode& operator = (const HeapNode& sourceHeapNode)
	{
		if (this == &sourceHeapNode)
		{
			return *this;
		}

		m_key = sourceHeapNode.m_key;
		m_data = sourceHeapNode.m_data;

		return *this;
	}

	HeapNode& operator = (HeapNode&& sourceHeapNode) noexcept
	{
		if (this == &sourceHeapNode)
		{
			return *this;
		}

		m_key = sourceHeapNode.m_key;
		m_data = move(sourceHeapNode.m_data);

		sourceHeapNode.m_key = 0;
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

	Heap() : m_pNodeArr(DBG_NEW HeapNode<DataType>[50]), m_size(0), m_capacity(50)
	{
		LogPrint("empty constructor");
	}

	Heap(const Heap& sourceHeap) : m_pNodeArr(nullptr), m_size(0), m_capacity(0)
	{
		LogPrint("copy constructor");

		CopyHeap(sourceHeap);
	}

	Heap(Heap&& sourceHeap) noexcept : m_pNodeArr(sourceHeap.m_pNodeArr), m_size(sourceHeap.m_size), m_capacity(sourceHeap.m_capacity)
	{
		LogPrint("move constructor");

		sourceHeap.m_pNodeArr = nullptr;
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

		m_pNodeArr = sourceHeap.m_pNodeArr;
		sourceHeap.m_pNodeArr = nullptr;

		m_size = sourceHeap.m_size;
		sourceHeap.m_size = 0;

		m_capacity = sourceHeap.m_capacity;
		sourceHeap.m_capacity = 0;

		return *this;
	}

	~Heap() noexcept
	{
		LogPrint("destructor");

		delete[] m_pNodeArr;
		m_pNodeArr = nullptr;
		m_size = 0;
		m_capacity = 0;
	}

	template <typename InsertDataType = DataType>
	void Push(int newKey, InsertDataType&& newData)
	{
		LogPrint("push");

		if (m_size >= m_capacity)
		{
			GiveLargerMemorySpace();
		}

		m_pNodeArr[m_size] = HeapNode<DataType>(newKey, forward<InsertDataType>(newData));
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

		outData = m_pNodeArr[0].m_data;		//DataType의 이동 생성자가 noexcept임이 보장되지 않기에 move(..)를 사용하지 않았다

		m_pNodeArr[0] = m_pNodeArr[m_size-1];
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

		outData = m_pNodeArr[0].m_data;

		return true;
	}

	bool IsEmpty()
	{
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

		delete[] m_pNodeArr;
		m_pNodeArr = nullptr;
		m_size = 0;
		m_capacity = 0;
	}

	void CopyHeap(const Heap& sourceHeap)
	{
		LogPrint("copy heap");

		unique_ptr<HeapNode<DataType>[]> upTempData = unique_ptr<HeapNode<DataType>[]>(DBG_NEW HeapNode<DataType>[sourceHeap.m_capacity]);

		for (int i = 0; i < sourceHeap.m_size; i++)
		{
			upTempData[i] = sourceHeap.m_pNodeArr[i];
		}

		delete[] m_pNodeArr;
		m_pNodeArr = upTempData.release();
		m_capacity = sourceHeap.m_capacity;
		m_size = sourceHeap.m_size;
	}

	//디버깅용 메소드임
	void PrintHeap()
	{
		LogPrint("print heap");

		for (int i = 0; i < m_size; i++)
		{
			cout << "키 : " << m_pNodeArr[i].m_key << " / 데이터 : " << m_pNodeArr[i].m_data << endl;
		}
	}

protected:

	void GiveLargerMemorySpace()
	{
		int newCapacity = (m_capacity == 0) ? (50) : (2 * m_capacity);
		unique_ptr<HeapNode<DataType>[]> upTempData = unique_ptr<HeapNode<DataType>[]>(DBG_NEW HeapNode<DataType>[newCapacity]);

		for (int i = 0; i < m_size; i++)
		{
			upTempData[i] = m_pNodeArr[i];	//DataType의 이동 생성자가 noexcept임이 보장되지 않기에 move(..)를 사용하지 않았다
		}

		delete[] m_pNodeArr;
		m_pNodeArr = upTempData.release();
		m_capacity = newCapacity;
	}

	//Swap(..)으로부터 올라오는 예외에 안전하지 못한 구현이지만, 더 나은 방식이 생각나지 않아 그대로 두려고 함
	void ReorderByPromoting()
	{
		int targetIndex = m_size - 1;
		int parentIndex = GetParentIndex(targetIndex);

		while (targetIndex != parentIndex && IsNotOrdered(parentIndex, targetIndex))
		{
			Swap(m_pNodeArr[targetIndex], m_pNodeArr[parentIndex]);

			targetIndex = parentIndex;
			parentIndex = GetParentIndex(targetIndex);
		}
	}

	//Swap(..)으로부터 올라오는 예외에 안전하지 못한 구현이지만, 더 나은 방식이 생각나지 않아 그대로 두려고 함
	void ReorderByDemoting()
	{
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
				Swap(m_pNodeArr[targetIndex], m_pNodeArr[targetChildIndex]);

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
	void Swap(HeapNode<DataType>& dataA, HeapNode<DataType>& dataB)
	{
		HeapNode<DataType> temp = move(dataA);
		dataA = move(dataB);
		dataB = move(temp);
	}

	int GetLeftChildIndex(int dataIndex)
	{
		return (dataIndex * 2 + 1);
	}

	int GetRightChildIndex(int dataIndex)
	{
		return (dataIndex * 2 + 2);
	}

	int GetParentIndex(int dataIndex)
	{
		return ((dataIndex - 1) / 2);
	}

	virtual bool IsNotOrdered(int parentIndex, int childIndex) = 0;					//상속된 최소힙 or 최대힙에서 각기 방식으로 구체화함
	virtual bool IsLeftChildTarget(int leftChildIndex, int rightChildIndex) = 0;

protected:

	HeapNode<DataType>* m_pNodeArr;
	int m_size;
	int m_capacity;
};

template <typename DataType>
class MinHeap : public Heap<DataType>
{
public:

	MinHeap() = default;
	MinHeap(const MinHeap& sourceMinHeap) = default;
	MinHeap(MinHeap&& sourceMinHeap) = default;
	MinHeap& operator = (const MinHeap& sourceMinHeap) = default;
	MinHeap& operator = (MinHeap&& sourceMinHeap) = default;

private:

	bool IsNotOrdered(int parentIndex, int childIndex)
	{
		if (this->m_pNodeArr[parentIndex].m_key > this->m_pNodeArr[childIndex].m_key)
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
		if (this->m_pNodeArr[leftChildIndex].m_key < this->m_pNodeArr[rightChildIndex].m_key)
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
	MaxHeap(const MaxHeap& sourceMaxHeap) = default;
	MaxHeap(MaxHeap&& sourceMaxHeap) = default;
	MaxHeap& operator = (const MaxHeap& sourceMaxHeap) = default;
	MaxHeap& operator = (MaxHeap&& sourceMaxHeap) = default;

private:

	bool IsNotOrdered(int parentIndex, int childIndex)
	{
		if (this->m_pNodeArr[parentIndex].m_key < this->m_pNodeArr[childIndex].m_key)
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
		if (this->m_pNodeArr[leftChildIndex].m_key > this->m_pNodeArr[rightChildIndex].m_key)
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