#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <crtdbg.h>

#ifdef TREE_LOG
	#define LogPrint(statement) std::cout << "(Log : " << __func__ << ") " << statement << std::endl
#else
	#define LogPrint(statement)
#endif

#ifdef TREE_WARNING
	#define WarningPrint(statement) std::cout << "(Warning : " << __func__ << ") " << statement << std::endl
#else
	#define WarningPrint(statement)
#endif

#ifdef TREE_ERROR
	#define ErrorPrint(statement) std::cout << "(Error : " << __func__ << ") " << statement << std::endl
#else
	#define ErrorPrint(statement)
#endif

//NOTE :	메모리 누수로 이어진 new 할당을 추적하기 위해선 기존 new가 아니라 오버로딩된 특별한 new를 사용하여야 함
//			이를 명시적으로 사용하여야 하므로, make_unique를 사용하지 말고 unique_ptr<T>(DBG_NEW T(argument))와 같은 방식을 사용하기로 함
#ifdef _DEBUG
	#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
	#define DBG_NEW new
#endif

#endif //DEBUG_H