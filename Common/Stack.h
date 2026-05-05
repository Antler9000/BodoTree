#ifndef STACK_H
#define STACK_H

#include <utility>					//이동 시맨틱을 사용함
using namespace std;				//..

template <class DataType>
class Stack
{
public:

	Stack()
		: m_pData(nullptr), m_size(0), m_capacity(0)
	{

	}

	Stack(const Stack& sourceStack)
	{
		CopyStack(sourceStack);
	}

	Stack(Stack&& sourceStack) noexcept : m_pData(sourceStack.m_pData), m_size(sourceStack.m_size), m_capacity(sourceStack.m_capacity)
	{
		sourceStack.m_pData = nullptr;
		sourceStack.m_size = 0;
		sourceStack.m_capacity = 0;
	}

	Stack& operator = (const Stack& sourceStack)
	{
		CopyStack(sourceStack);

		return *this;
	}

	Stack& operator = (Stack&& sourceStack) noexcept
	{
		if (this == &sourceStack)
		{
			return *this;
		}

		RemoveStack();

		m_pData = sourceStack.m_pData;
		m_size = sourceStack.m_size;
		m_capacity = sourceStack.m_capacity;
		sourceStack.m_pData = nullptr;
		sourceStack.m_size = 0;
		sourceStack.m_capacity = 0;

		return *this;
	}

	~Stack() noexcept
	{
		RemoveStack();
	}

	template <typename InsertDataType = DataType>
	void Push(InsertDataType&& data)
	{
		if (m_capacity == 0)
		{
			m_pData = DBG_NEW DataType[1];
			m_capacity = 1;
		}
		else if (m_capacity == m_size)
		{
			unique_ptr<DataType[]> upNewData = unique_ptr<DataType[]>(DBG_NEW DataType[2 * m_capacity]);
			for (int i = 0; i < m_size; i++)
			{
				upNewData[i] = m_pData[i];	//DataType의 이동 할당 연산자가 noexcept임이 보장되지 않기에 move(..)를 사용하지 않았다
			}

			delete[] m_pData;
			m_pData = upNewData.release();
			m_capacity *= 2;
		}
		
		m_pData[m_size] = forward<InsertDataType>(data);
		m_size++;
	}

	//bool 반환값이 false인 경우 : 내부에 데이터가 하나도 없는 경우 
	bool Pop(DataType& outData)
	{
		if (m_size == 0)
		{
			return false;
		}

		outData = m_pData[m_size - 1];
		m_size--;

		if (m_size <= (m_capacity / 2))
		{
			if (m_capacity / 2 == 0)
			{
				delete[] m_pData;
				m_pData = nullptr;
				m_capacity = 0;
			}
			else
			{
				unique_ptr<DataType[]> upNewData = unique_ptr<DataType[]>(DBG_NEW DataType[m_capacity / 2]);
				for (int i = 0; i < m_size; i++)
				{
					upNewData[i] = m_pData[i];	//DataType의 이동 할당 연산자가 noexcept임이 보장되지 않기에 move(..)를 사용하지 않았다
				}

				delete[] m_pData;
				m_pData = upNewData.release();
				m_capacity /= 2;
			}
		}

		return true;
	}

	//bool 반환값이 false인 경우 : 내부에 데이터가 하나도 없는 경우 
	bool GetTop(DataType& outData)
	{
		if (m_size == 0)
		{
			return false;
		}

		outData = m_pData[m_size - 1];

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

	void RemoveStack() noexcept
	{
		delete[] m_pData;
		m_pData = nullptr;
		m_size = 0;
		m_capacity = 0;
	}

	//bool 반환값이 false인 경우 : 내부에 데이터가 하나도 없는 경우 
	bool CopyStack(const Stack<DataType>& sourceStack)
	{
		if (sourceStack.m_size == 0)
		{
			return false;
		}

		RemoveStack();

		m_pData = DBG_NEW DataType[sourceStack.m_capacity];
		m_size = sourceStack.m_size;
		m_capacity = sourceStack.m_capacity;
		for (int i = 0; i < m_size; i++)
		{
			m_pData[i] = sourceStack.m_pData[i];
		}

		return true;
	}

private:

	DataType* m_pData;
	int m_size;
	int m_capacity;
};

#endif //STACK_H