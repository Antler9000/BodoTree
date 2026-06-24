#ifndef STACK_H
#define STACK_H

#include "Debug.h"
#include <memory>
#include <utility>

template <typename DataType>
class Stack
{
public:

	Stack() : m_pDatum(nullptr), m_size(0), m_capacity(0)
	{

	}

	Stack(const Stack& sourceStack) : m_pDatum(nullptr), m_size(0), m_capacity(0)
	{
		CopyStack(sourceStack);
	}

	Stack(Stack&& sourceStack) noexcept : m_pDatum(sourceStack.m_pDatum), m_size(sourceStack.m_size), m_capacity(sourceStack.m_capacity)
	{
		sourceStack.m_pDatum = nullptr;
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

		m_pDatum = sourceStack.m_pDatum;
		m_size = sourceStack.m_size;
		m_capacity = sourceStack.m_capacity;
		sourceStack.m_pDatum = nullptr;
		sourceStack.m_size = 0;
		sourceStack.m_capacity = 0;

		return *this;
	}

	~Stack() noexcept
	{
		RemoveStack();
	}

	//NOTE : 데이터가 lvalue인 경우와 rvalue인 경우를 모두 각 참조로 받을 수 있도록 포워딩을 사용함
	template <typename PushDataType = DataType>
	void Push(PushDataType&& newData)
	{
		if (m_capacity == 0)
		{
			m_pDatum = DBG_NEW DataType[1];
			m_capacity = 1;
		}
		else if (m_capacity == m_size)
		{
			std::unique_ptr<DataType[]> upNewData = std::unique_ptr<DataType[]>(DBG_NEW DataType[m_capacity * 2]);	//NOTE : DataType의 기본 생성자가 제공된다고 가정함
			for (int i = 0; i < m_size; i++)
			{
				upNewData[i] = m_pDatum[i];		//NOTE : DataType의 이동 할당 연산자가 noexcept임이 보장되지 않기에 move(..)를 사용하지 않았음
			}

			delete[] m_pDatum;
			m_pDatum = upNewData.release();
			m_capacity *= 2;
		}
		
		m_pDatum[m_size] = std::forward<PushDataType>(newData);
		m_size++;
	}

	//RETURN : 내부에 데이터가 하나도 없는 경우 false를 반환
	bool Pop(DataType& outData)
	{
		if (m_size == 0)
		{
			return false;
		}

		outData = m_pDatum[m_size - 1];
		m_size--;

		if (m_size <= (m_capacity / 2))
		{
			if (m_capacity / 2 == 0)
			{
				delete[] m_pDatum;
				m_pDatum = nullptr;
				m_capacity = 0;
			}
			else
			{
				std::unique_ptr<DataType[]> upNewData = std::unique_ptr<DataType[]>(DBG_NEW DataType[m_capacity / 2]);	//NOTE : DataType의 기본 생성자가 제공된다고 가정함
				for (int i = 0; i < m_size; i++)
				{
					upNewData[i] = m_pDatum[i];		//NOTE : DataType의 이동 할당 연산자가 noexcept임이 보장되지 않기에 move(..)를 사용하지 않았음
				}

				delete[] m_pDatum;
				m_pDatum = upNewData.release();
				m_capacity /= 2;
			}
		}

		return true;
	}

	//RETURN : 내부에 데이터가 하나도 없는 경우 false를 반환함
	bool GetTop(DataType& outData)
	{
		if (m_size == 0)
		{
			return false;
		}

		outData = m_pDatum[m_size - 1];

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
		delete[] m_pDatum;
		m_pDatum = nullptr;
		m_size = 0;
		m_capacity = 0;
	}

	//RETURN : 내부에 데이터가 하나도 없는 경우 false를 반환함
	bool CopyStack(const Stack<DataType>& sourceStack)
	{
		if (sourceStack.m_size == 0)
		{
			return false;
		}

		RemoveStack();

		m_pDatum = DBG_NEW DataType[sourceStack.m_capacity];
		m_size = sourceStack.m_size;
		m_capacity = sourceStack.m_capacity;
		for (int i = 0; i < m_size; i++)
		{
			m_pDatum[i] = sourceStack.m_pDatum[i];
		}

		return true;
	}

private:

	DataType* m_pDatum;
	int m_size;
	int m_capacity;
};

#endif //STACK_H