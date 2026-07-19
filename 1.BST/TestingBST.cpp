//로그 출력문들을 활성화하고 싶을 시 아래 구문의 주석을 해제할 것
//#define TREE_LOG	
//#define TREE_WARNING
//#define TREE_ERROR

#include "BST.h"
#include <crtdbg.h>
#include <iostream>
#include <chrono>
#include <string>
#include <numeric>
#include <random>
#include <algorithm>
#include <vector>
#include <map>
#include <utility>

using namespace std;
using namespace chrono;

template <typename DataType>
void PrintKeyAndData(int key, const DataType& retrievedData);

void RandomWorkloadSpeedTest(int workloadNum, int workloadPerDataLen);
void RandomLocalWorkloadTest(int workloadNum, int workloadPerDataLen, int localBlockSize);
void LinearIncreaseWorkloadTest(int workloadNum, int workloadPerDataLen);
void LinearDecreaseWorkloadTest(int workloadNum, int workloadPerDataLen);

//NOTE	: insertDataWorkload는 복사 비용이 크지만, 그럼에도 하나의 워크로드를 BST와 map에 반복해서 사용할 수 있도록 값복사 형식의 매개변수를 사용함
time_point<steady_clock> TestBST(steady_clock& clock, int workloadNum, vector<string> insertDataWorkload, const vector<int>& insertKeyWorkload, const vector<int>& retrieveKeyWorkload, const vector<int>& removeKeyWorkload);
time_point<steady_clock> TestMap(steady_clock& clock, int workloadNum, vector<string> insertDataWorkload, const vector<int>& insertKeyWorkload, const vector<int>& retrieveKeyWorkload, const vector<int>& removeKeyWorkload);

int main()
{
	//NOTE : 디버깅 실행이 종료될 시점에도 해제되지 않은 동적 메모리 누수가 존재할 시, Visual Studio의 하단의 출력창(output)에 해당 누수에 대한 정보가 출력됨
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	cout << endl << "기능 테스트 1/3 : BST<int>-------------------------------------------------------------------" << endl;

	BST<int> intTestBST;

	cout << endl << "삽입 (트리 A)" << endl;
	intTestBST.Insert(5, 55);
	intTestBST.Insert(7, 77);
	intTestBST.Insert(3, 33);
	intTestBST.Insert(4, 44);
	intTestBST.Insert(6, 66);
	intTestBST.Insert(9, 99);
	intTestBST.Insert(2, 22);
	intTestBST.PreorderPrint();

	cout << endl << "중위 순회 출력 (트리 A)" << endl;
	intTestBST.InorderPrint();

	cout << endl << "후위 순회 출력 (트리 A)" << endl;
	intTestBST.PostorderPrint();

	cout << endl << "검색 (트리 A)" << endl;
	int intRetrievedData = 0;
	intTestBST.Retrieve(3, intRetrievedData);
	PrintKeyAndData(3, intRetrievedData);

	cout << endl << "명시적 복사 (트리 A -> B)" << endl;
	BST<int> intExplicitCopyTestBST;
	intExplicitCopyTestBST.CopyTree(intTestBST);
	intExplicitCopyTestBST.PreorderPrint();

	cout << endl << "복사 생성자 (트리 A -> C)" << endl;
	BST<int> intCopyConstructorTestBST = intTestBST;
	intCopyConstructorTestBST.PreorderPrint();

	cout << endl << "복사 할당 연산자 (트리 A -> D)" << endl;
	BST<int> intCopyAssignmentTestBST;
	intCopyAssignmentTestBST = intTestBST;
	intCopyAssignmentTestBST.PreorderPrint();

	cout << endl << "이동 생성자 (트리 C -> E)" << endl;
	BST<int> intMoveConstructorTestBST = move(intCopyConstructorTestBST);
	intMoveConstructorTestBST.PreorderPrint();

	cout << endl << "이동 할당 연산자 (트리 D -> F)" << endl;
	BST<int> intMoveAssignmentTestBST;
	intMoveAssignmentTestBST = move(intCopyAssignmentTestBST);
	intMoveAssignmentTestBST.PreorderPrint();

	cout << endl << "이동 후 소스 트리는 비워짐 (트리 C, D)" << endl;
	intCopyConstructorTestBST.PreorderPrint();
	intCopyAssignmentTestBST.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	intTestBST.Remove(7);
	intTestBST.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	intTestBST.Remove(3);
	intTestBST.PreorderPrint();

	cout << endl << "전체 삭제 (트리 A)" << endl;
	intTestBST.RemoveTree();
	intTestBST.PreorderPrint();

	cout << endl << "복사한 트리는 원본과 독립적임 (트리 B)" << endl;
	intExplicitCopyTestBST.PreorderPrint();

	cout << endl << "기능 테스트 2/3 : BST<float>-----------------------------------------------------------------" << endl;

	BST<float> floatTestBST;

	cout << endl << "삽입 (트리 A)" << endl;
	floatTestBST.Insert(5, 0.55f);
	floatTestBST.Insert(7, 0.77f);
	floatTestBST.Insert(3, 0.33f);
	floatTestBST.Insert(4, 0.44f);
	floatTestBST.Insert(6, 0.66f);
	floatTestBST.Insert(9, 0.99f);
	floatTestBST.Insert(2, 0.22f);
	floatTestBST.PreorderPrint();

	cout << endl << "중위 순회 출력 (트리 A)" << endl;
	floatTestBST.InorderPrint();

	cout << endl << "후위 순회 출력 (트리 A)" << endl;
	floatTestBST.PostorderPrint();

	cout << endl << "검색 (트리 A)" << endl;
	float floatRetrievedData = 0.0f;
	floatTestBST.Retrieve(3, floatRetrievedData);
	PrintKeyAndData(3, floatRetrievedData);

	cout << endl << "명시적 복사 (트리 A -> B)" << endl;
	BST<float> floatExplicitCopyTestBST;
	floatExplicitCopyTestBST.CopyTree(floatTestBST);
	floatExplicitCopyTestBST.PreorderPrint();

	cout << endl << "복사 생성자 (트리 A -> C)" << endl;
	BST<float> floatCopyConstructorTestBST = floatTestBST;
	floatCopyConstructorTestBST.PreorderPrint();

	cout << endl << "복사 할당 연산자 (트리 A -> D)" << endl;
	BST<float> floatCopyAssignmentTestBST;
	floatCopyAssignmentTestBST = floatTestBST;
	floatCopyAssignmentTestBST.PreorderPrint();

	cout << endl << "이동 생성자 (트리 C -> E)" << endl;
	BST<float> floatMoveConstructorTestBST = move(floatCopyConstructorTestBST);
	floatMoveConstructorTestBST.PreorderPrint();

	cout << endl << "이동 할당 연산자 (트리 D -> F)" << endl;
	BST<float> floatMoveAssignmentTestBST;
	floatMoveAssignmentTestBST = move(floatCopyAssignmentTestBST);
	floatMoveAssignmentTestBST.PreorderPrint();

	cout << endl << "이동 후 소스 트리는 비워짐 (트리 C, D)" << endl;
	floatCopyConstructorTestBST.PreorderPrint();
	floatCopyAssignmentTestBST.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	floatTestBST.Remove(7);
	floatTestBST.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	floatTestBST.Remove(3);
	floatTestBST.PreorderPrint();

	cout << endl << "전체 삭제 (트리 A)" << endl;
	floatTestBST.RemoveTree();
	floatTestBST.PreorderPrint();

	cout << endl << "복사한 트리는 원본과 독립적임 (트리 B)" << endl;
	floatExplicitCopyTestBST.PreorderPrint();

	cout << endl << "기능 테스트 3/3 : BST<string>----------------------------------------------------------------" << endl;

	BST<string> stringTestBST;

	cout << endl << "삽입 (트리 A)" << endl;
	stringTestBST.Insert(5, "Panther");
	stringTestBST.Insert(7, "Comet");
	stringTestBST.Insert(3, "Crusader");
	stringTestBST.Insert(4, "Sherman");
	stringTestBST.Insert(6, "Tiger");
	stringTestBST.Insert(9, "Mouse");
	stringTestBST.Insert(2, "Stuart");
	stringTestBST.PreorderPrint();

	cout << endl << "중위 순회 출력 (트리 A)" << endl;
	stringTestBST.InorderPrint();

	cout << endl << "후위 순회 출력 (트리 A)" << endl;
	stringTestBST.PostorderPrint();

	cout << endl << "검색 (트리 A)" << endl;
	string stringRetrievedData = "";
	stringTestBST.Retrieve(3, stringRetrievedData);
	PrintKeyAndData(3, stringRetrievedData);

	cout << endl << "명시적 복사 (트리 A -> B)" << endl;
	BST<string> stringExplicitCopyTestBST;
	stringExplicitCopyTestBST.CopyTree(stringTestBST);
	stringExplicitCopyTestBST.PreorderPrint();

	cout << endl << "복사 생성자 (트리 A -> C)" << endl;
	BST<string> stringCopyConstructorTestBST = stringTestBST;
	stringCopyConstructorTestBST.PreorderPrint();

	cout << endl << "복사 할당 연산자 (트리 A -> D)" << endl;
	BST<string> stringCopyAssignmentTestBST;
	stringCopyAssignmentTestBST = stringTestBST;
	stringCopyAssignmentTestBST.PreorderPrint();

	cout << endl << "이동 생성자 (트리 C -> E)" << endl;
	BST<string> stringMoveConstructorTestBST = move(stringCopyConstructorTestBST);
	stringMoveConstructorTestBST.PreorderPrint();

	cout << endl << "이동 할당 연산자 (트리 D -> F)" << endl;
	BST<string> stringMoveAssignmentTestBST;
	stringMoveAssignmentTestBST = move(stringCopyAssignmentTestBST);
	stringMoveAssignmentTestBST.PreorderPrint();

	cout << endl << "이동 후 소스 트리는 비워짐 (트리 C, D)" << endl;
	stringCopyConstructorTestBST.PreorderPrint();
	stringCopyAssignmentTestBST.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	stringTestBST.Remove(7);
	stringTestBST.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	stringTestBST.Remove(3);
	stringTestBST.PreorderPrint();

	cout << endl << "전체 삭제 (트리 A)" << endl;
	stringTestBST.RemoveTree();
	stringTestBST.PreorderPrint();

	cout << endl << "복사한 트리는 원본과 독립적임 (트리 B)" << endl;
	stringExplicitCopyTestBST.PreorderPrint();
#endif

#ifndef _DEBUG
	cout << fixed << setprecision(2);

	cout << endl << "속도 테스트 1/4 : 랜덤 워크로드 테스트-------------------------------------------------------" << endl;

	/*	(테스팅 방법)
		randomWorkloadNum 횟수만큼 복사 삽입(트리 A), 이동 삽입(트리 B), 검색(트리 A), 삭제(트리 A)와 소멸(트리 B)을 수행함
		키는 [0,randomWorkloadNum-1] 의 중복되지 않는 키 값들을 랜덤하게 셔플해놓고 사용함(삽입, 검색, 삭제의 키 값들은 각각 독립으로 셔플됨)
		데이터는 randomWorkloadPerDataLen 으로 지정된 길이의 string 객체를 randomWorkloadNum 개 만들어놓고 사용함
	*/

	/*	(테스팅 결과)
		[randomWorkloadNum			= 10,000,000]
		[randomWorkloadPerDataLen	= 30]

		복사 삽입	: BST = 16.46초	|	std::map = 15.83초 -4%
		이동 삽입	: BST = 15.77초	|	std::map = 14.58초 -8%
		검색		: BST = 16.97초	|	std::map = 17.64초
		삭제		: BST = 21.92초	|	std::map = 18.61초
		소멸		: BST =  4.51초	|	std::map =  4.38초
	*/

	/*	(테스팅 해석)
		본 트리는 반복문으로 구현되었기에, 컴파일러의 최적화에 상관없이 재귀 호출은 무조건 일어나지 않음
		
		빌드된 어셈블리 코드를 보았을 때 삽입과 검색 메소드는 모두 인라이닝 되었고 이로 인해 std::map과 속도 차이가 적음
		그럼에도 상대적으로 10% 정도 느린 속도는 상세적인 최적화가 부족하기 때문으로 추정함
		
		삭제 메소드의 하위 작업 호출이 인라이닝되지 못한 것을 확인하였고 std::map과의 속도 차이도 크게 나타남
		삭제 메소드는 하위 작업 메소드의 크기가 커서 인라이닝되지 못한 것으로 추정함 
	*/

	constexpr int randomWorkloadNum = 10000000;
	constexpr int randomWorkloadPerDataLen = 30;
	RandomWorkloadSpeedTest(randomWorkloadNum, randomWorkloadPerDataLen);

	cout << endl << "속도 테스트 2/4 : 랜덤 로컬 워크로드 테스트--------------------------------------------------" << endl;

	/*	(테스팅 방법)
		앞선 1번 랜덤 워크로드 테스트와 비슷하나, 키 값들이 localBlockSize 단위로 내부에서 선형 증가 연속성을 가지도록 하였음
	*/

	/*	(테스팅 결과)
		[randomLocalWorkloadNum			= 앞선 1번 랜덤 워크로드 테스트와 동일]
		[randomLocalWorkloadPerDataLen	= 앞선 1번 랜덤 워크로드 테스트와 동일]
		[localBlockSize					= 10]

		복사 삽입	: BST =  7.75초	|	std::map =  4.09초
		이동 삽입	: BST =  6.74초	|	std::map =  3.34초
		검색		: BST =  6.60초	|	std::map =  3.65초
		삭제		: BST =  7.70초	|	std::map =  4.11초
		소멸		: BST =  3.51초	|	std::map =  3.38초
	*/

	/*	(테스팅 해석)
		워크로드가 지역 선형성을 가지게 되므로 1번 랜덤 워크로드 테스트에 비해서 BST의 트리 높이는 더 높아짐
		그럼에도 1번 랜덤 워크로드 테스트에 비해서 BST의 삽입, 검색, 삭제 메소드가 더 빨라진 이유는 캐시 히트율이 높아졌기 때문으로 추정함
		본 테스트에서 BST와 std::map의 속도 차이가 벌어진 이유는 설계와 구현상에서 캐시 히트율 고려에 대한 수준 차이 때문으로 추정함
	*/

	constexpr int randomLocalWorkloadNum = randomWorkloadNum;
	constexpr int randomLocalWorkloadPerDataLen = randomWorkloadPerDataLen;
	constexpr int localBlockSize = 10;
	RandomLocalWorkloadTest(randomLocalWorkloadNum, randomLocalWorkloadPerDataLen, localBlockSize);

	cout << endl << "속도 테스트 3/4 : 선형 증가 워크로드 테스트--------------------------------------------------" << endl;

	/*	(테스팅 방법)
		앞선 1번 랜덤 워크로드 테스트와 비슷하나, 키값들을 뒤섞지 않고 선형 그대로 사용함
	*/

	/*	(테스팅 결과)
		[linearIncreaseWorkloadNum			= 앞선 1번 랜덤 워크로드 테스트와 동일]
		[linearIncreaseWorkloadPerDataLen	= 앞선 1번 랜덤 워크로드 테스트와 동일]

		복사 삽입	: BST = 시간초과|	std::map =  1.79초
		이동 삽입	: BST = 시간초과|	std::map =  1.51초
		검색		: BST = 시간초과|	std::map =  0.66초
		삭제		: BST =  0.02초	|	std::map =  1.84초
		소멸		: BST =  0.36초 |	std::map =  1.32초
	*/

	/*	(테스팅 해석)
		선형 워크로드는 BST를 편향시켜 리스트와 같은 형상이 되도록 함
		
		삽입 메소드와 검색 메소드는 리스트와 같은 모양이 되어서 나타나는 O(N^2)의 시간 복잡도 때문에 시간 초과가 남
		시간초과로 삽입이 덜 이루어졌으므로, 이후의 '삭제', '소멸'의 측정 시간은 신뢰할 수 없음
		
		BST와 달리 std::map은 균형을 유지하는 트리이기 때문에 선형 워크로드에 대해서도 O(NlogN)의 시간 복잡도를 가져 매우 빠른 속도를 보임
		게다가 선형 워크로드가 가진 지역성으로 캐시 히트율이 증가해 1번 랜덤 워크로드 테스트보다 훨씬 빠른 속도를 보이는 것으로 추정함
	*/

	constexpr int linearIncreaseWorkloadNum = randomWorkloadNum;
	constexpr int linearIncreaseWorkloadPerDataLen = randomWorkloadPerDataLen;
	LinearIncreaseWorkloadTest(linearIncreaseWorkloadNum, linearIncreaseWorkloadPerDataLen);

	cout << endl << "속도 테스트 4/4 : 선형 감소 워크로드 테스트--------------------------------------------------" << endl;

	/*	(테스팅 방법)
		앞선 3번 선형 증가 워크로드 테스트와 비슷하나, 키를 역순으로 사용함
	*/

	/*	(테스팅 결과)
		[linearDecreaseWorkloadNum			= 앞선 1번 랜덤 워크로드 테스트와 동일]
		[linearDecreaseWorkloadPerDataLen	= 앞선 1번 랜덤 워크로드 테스트와 동일]

		복사 삽입	: BST = 시간초과|	std::map =  1.89초
		이동 삽입	: BST = 시간초과|	std::map =  1.46초
		검색		: BST = 시간초과|	std::map =  0.68초
		삭제		: BST =  0.02초	|	std::map =  1.64초
		소멸		: BST =  0.35초 |	std::map =  1.98초
	*/

	/*	(테스팅 해석)
		앞선 3번 선형 증가 워크로드 테스트와 같음
	*/

	constexpr int linearDecreaseWorkloadNum = randomWorkloadNum;
	constexpr int linearDecreaseWorkloadPerDataLen = randomWorkloadPerDataLen;
	LinearDecreaseWorkloadTest(linearDecreaseWorkloadNum, linearDecreaseWorkloadPerDataLen);
#endif

	cout << endl << "테스트 종료----------------------------------------------------------------------------------" << endl;

	return 0;
}

template <typename DataType>
void PrintKeyAndData(int key, const DataType& retrievedData)
{
	cout << "검색한 키 : " << key << " / 검색된 데이터 : " << retrievedData << endl;
}

void RandomWorkloadSpeedTest(int workloadNum, int workloadPerDataLen)
{
	cout << endl << "랜덤 워크로드 준비 중...." << endl;

	vector<string> insertDataWorkload;
	insertDataWorkload.reserve(workloadNum);
	for (int i = 0; i < workloadNum; i++)
	{
		insertDataWorkload.emplace_back(string(workloadPerDataLen, 'A'));
	}

	vector<int> insertKeyWorkload(workloadNum);
	iota(insertKeyWorkload.begin(), insertKeyWorkload.end(), 0);
	mt19937 insertTestRng(123456);
	shuffle(insertKeyWorkload.begin(), insertKeyWorkload.end(), insertTestRng);

	vector<int> retrieveKeyWorkload(workloadNum);
	iota(retrieveKeyWorkload.begin(), retrieveKeyWorkload.end(), 0);
	mt19937 retrieveTestRng(654321);
	shuffle(retrieveKeyWorkload.begin(), retrieveKeyWorkload.end(), retrieveTestRng);

	vector<int> removeKeyWorkload(workloadNum);
	iota(removeKeyWorkload.begin(), removeKeyWorkload.end(), 0);
	mt19937 removeTestRng(162534);
	shuffle(removeKeyWorkload.begin(), removeKeyWorkload.end(), removeTestRng);

	steady_clock clock;
	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "랜덤 워크로드 복사 중...." << endl;
	timeBegin = TestBST(clock, workloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "BST : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "---------------------------------------------------------------------------" << endl;

	cout << endl << "랜덤 워크로드 복사 중...." << endl;
	timeBegin = TestMap(clock, workloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "map : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << endl << endl << endl << endl;
}

void RandomLocalWorkloadTest(int workloadNum, int workloadPerDataLen, int localBlockSize)
{
	cout << endl << "랜덤 로컬 워크로드 준비 중...." << endl;

	vector<string> insertDataWorkload;
	insertDataWorkload.reserve(workloadNum);
	for (int i = 0; i < workloadNum; i++)
	{
		insertDataWorkload.emplace_back(string(workloadPerDataLen, 'A'));
	}

	vector<int> insertTestKeyBlocks(workloadNum / localBlockSize);
	iota(insertTestKeyBlocks.begin(), insertTestKeyBlocks.end(), 0);
	mt19937 insertTestRng(123456);
	shuffle(insertTestKeyBlocks.begin(), insertTestKeyBlocks.end(), insertTestRng);
	vector<int> insertKeyWorkload;
	for (int blockIndex = 0; blockIndex < insertTestKeyBlocks.size(); blockIndex++)
	{
		for (int offset = 0; offset < localBlockSize; offset++)
		{
			insertKeyWorkload.push_back(insertTestKeyBlocks[blockIndex] * localBlockSize + offset);
		}
	}

	vector<int> retrieveTestKeyBlocks(workloadNum / localBlockSize);
	iota(retrieveTestKeyBlocks.begin(), retrieveTestKeyBlocks.end(), 0);
	mt19937 retrieveTestRng(654321);
	shuffle(retrieveTestKeyBlocks.begin(), retrieveTestKeyBlocks.end(), retrieveTestRng);
	vector<int> retrieveKeyWorkload;
	for (int blockIndex = 0; blockIndex < retrieveTestKeyBlocks.size(); blockIndex++)
	{
		for (int offset = 0; offset < localBlockSize; offset++)
		{
			retrieveKeyWorkload.push_back(retrieveTestKeyBlocks[blockIndex] * localBlockSize + offset);
		}
	}

	vector<int> removeTestKeyBlocks(workloadNum / localBlockSize);
	iota(removeTestKeyBlocks.begin(), removeTestKeyBlocks.end(), 0);
	mt19937 removeTestRng(162534);
	shuffle(removeTestKeyBlocks.begin(), removeTestKeyBlocks.end(), removeTestRng);
	vector<int> removeKeyWorkload;
	for (int blockIndex = 0; blockIndex < removeTestKeyBlocks.size(); blockIndex++)
	{
		for (int offset = 0; offset < localBlockSize; offset++)
		{
			removeKeyWorkload.push_back(removeTestKeyBlocks[blockIndex] * localBlockSize + offset);
		}
	}

	steady_clock clock;
	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	int realWorkloadNum = (workloadNum / localBlockSize) * localBlockSize;

	cout << endl << "랜덤 로컬 워크로드 복사 중...." << endl;
	timeBegin = TestBST(clock, realWorkloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "BST : " << realWorkloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "---------------------------------------------------------------------------" << endl;

	cout << endl << "랜덤 로컬 워크로드 복사 중...." << endl;
	timeBegin = TestMap(clock, realWorkloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "map : " << realWorkloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << endl << endl << endl << endl;
}

void LinearIncreaseWorkloadTest(int workloadNum, int workloadPerDataLen)
{
	cout << endl << "선형 증가 워크로드 준비 중...." << endl;

	vector<string> insertDataWorkload;
	insertDataWorkload.reserve(workloadNum);
	for (int i = 0; i < workloadNum; i++)
	{
		insertDataWorkload.emplace_back(string(workloadPerDataLen, 'A'));
	}

	vector<int> insertKeyWorkload(workloadNum);
	iota(insertKeyWorkload.begin(), insertKeyWorkload.end(), 0);

	vector<int> retrieveKeyWorkload = insertKeyWorkload;

	vector<int> removeKeyWorkload = insertKeyWorkload;

	steady_clock clock;
	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "선형 증가 워크로드 복사 중...." << endl;
	timeBegin = TestBST(clock, workloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "BST : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "---------------------------------------------------------------------------" << endl;

	cout << endl << "선형 증가 워크로드 복사 중...." << endl;
	timeBegin = TestMap(clock, workloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "map : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << endl << endl << endl << endl;
}

void LinearDecreaseWorkloadTest(int workloadNum, int workloadPerDataLen)
{
	cout << endl << "선형 감소 워크로드 준비 중...." << endl;

	vector<string> insertDataWorkload;
	insertDataWorkload.reserve(workloadNum);
	for (int i = 0; i < workloadNum; i++)
	{
		insertDataWorkload.emplace_back(string(workloadPerDataLen, 'A'));
	}

	vector<int> insertKeyWorkload(workloadNum);
	iota(insertKeyWorkload.rbegin(), insertKeyWorkload.rend(), 0);

	vector<int> retrieveKeyWorkload = insertKeyWorkload;

	vector<int> removeKeyWorkload = insertKeyWorkload;

	steady_clock clock;
	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "선형 감소 워크로드 복사 중...." << endl;
	timeBegin = TestBST(clock, workloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "BST : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "---------------------------------------------------------------------------" << endl;

	cout << endl << "선형 감소 워크로드 복사 중...." << endl;
	timeBegin = TestMap(clock, workloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "map : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << endl << endl << endl << endl;
}


time_point<steady_clock> TestBST(steady_clock& clock, int workloadNum, vector<string> insertDataWorkload, const vector<int>& insertKeyWorkload, const vector<int>& retrieveKeyWorkload, const vector<int>& removeKeyWorkload)
{
	BST<string> copyInsertTestBST;
	BST<string> moveInsertTestBST;

	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "[BST 복사 삽입 측정 시작]" << endl;
	cout << endl << "|------------------------------------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < workloadNum; i++)
	{
		if (i % (workloadNum / 50) == 0)
		{
			cout << "*";
		}

		if (i % 10000 == 0)
		{
			timeEnd = clock.now();
			timeDiff = timeEnd - timeBegin;
			if (timeDiff.count() > 50)
			{
				break;
			}
		}

		copyInsertTestBST.Insert(insertKeyWorkload[i], insertDataWorkload[i]);
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "BST : " << workloadNum << "번의 복사 삽입 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "BST : " << workloadNum << "번의 복사 삽입 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[BST 이동 삽입 측정 시작]" << endl;
	cout << endl << "|------------------------------------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < workloadNum; i++)
	{
		if (i % (workloadNum / 50) == 0)
		{
			cout << "*";
		}

		if (i % 10000 == 0)
		{
			timeEnd = clock.now();
			timeDiff = timeEnd - timeBegin;
			if (timeDiff.count() > 50)
			{
				break;
			}
		}

		moveInsertTestBST.Insert(insertKeyWorkload[i], move(insertDataWorkload[i]));
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "BST : " << workloadNum << "번의 이동 삽입 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "BST : " << workloadNum << "번의 이동 삽입 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	string retrievedData;

	cout << endl << "[BST 검색 측정 시작]" << endl;
	cout << endl << "|------------------------------------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < workloadNum; i++)
	{
		if (i % (workloadNum / 50) == 0)
		{
			cout << "*";
		}

		if (i % 10000 == 0)
		{
			timeEnd = clock.now();
			timeDiff = timeEnd - timeBegin;
			if (timeDiff.count() > 50)
			{
				break;
			}
		}

		copyInsertTestBST.Retrieve(insertKeyWorkload[i], retrievedData);
		retrievedData += 'a';			//NOTE : 컴파일, 링킹 최적화로 테스트 중의 검색 메소드 호출이 건너뛰어지는 경우가 없도록 하기 위한 추가 명령문임
	}
	cout << endl;

	if (timeDiff.count() < 50)
	{
		cout << endl << "BST : " << workloadNum << "번의 검색 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "BST : " << workloadNum << "번의 검색 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[BST 삭제 측정 시작]" << endl;
	cout << endl << "|------------------------------------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < workloadNum; i++)
	{
		if (i % (workloadNum / 50) == 0)
		{
			cout << "*";
		}

		if (i % 10000 == 0)
		{
			timeEnd = clock.now();
			timeDiff = timeEnd - timeBegin;
			if (timeDiff.count() > 50)
			{
				break;
			}
		}

		copyInsertTestBST.Remove(removeKeyWorkload[i]);
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "BST : " << workloadNum << "번의 삭제 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "BST : " << workloadNum << "번의 삭제 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[BST 소멸자 측정 시작]" << endl;
	return clock.now();
}

time_point<steady_clock> TestMap(steady_clock& clock, int workloadNum, vector<string> insertDataWorkload, const vector<int>& insertKeyWorkload, const vector<int>& retrieveKeyWorkload, const vector<int>& removeKeyWorkload)
{
	map<int, string> copyInsertTestMap;
	map<int, string> moveInsertTestMap;

	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "[map 복사 삽입 측정 시작]" << endl;
	cout << endl << "|------------------------------------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < workloadNum; i++)
	{
		if (i % (workloadNum / 50) == 0)
		{
			cout << "*";
		}

		if (i % 10000 == 0)
		{
			timeEnd = clock.now();
			timeDiff = timeEnd - timeBegin;
			if (timeDiff.count() > 50)
			{
				break;
			}
		}

		copyInsertTestMap.emplace(insertKeyWorkload[i], insertDataWorkload[i]);
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "map : " << workloadNum << "번의 복사 삽입 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "map : " << workloadNum << "번의 복사 삽입 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[map 이동 삽입 측정 시작]" << endl;
	cout << endl << "|------------------------------------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < workloadNum; i++)
	{
		if (i % (workloadNum / 50) == 0)
		{
			cout << "*";
		}

		if (i % 10000 == 0)
		{
			timeEnd = clock.now();
			timeDiff = timeEnd - timeBegin;
			if (timeDiff.count() > 50)
			{
				break;
			}
		}

		moveInsertTestMap.emplace(insertKeyWorkload[i], move(insertDataWorkload[i]));
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "map : " << workloadNum << "번의 이동 삽입 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "map : " << workloadNum << "번의 이동 삽입 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[map 검색 측정 시작]" << endl;
	cout << endl << "|------------------------------------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < workloadNum; i++)
	{
		if (i % (workloadNum / 50) == 0)
		{
			cout << "*";
		}

		if (i % 10000 == 0)
		{
			timeEnd = clock.now();
			timeDiff = timeEnd - timeBegin;
			if (timeDiff.count() > 50)
			{
				break;
			}
		}

		auto iterator = copyInsertTestMap.find(retrieveKeyWorkload[i]);
		iterator->second += 'a';			//NOTE : 컴파일, 링킹 최적화로 테스트 중의 검색 메소드 호출이 건너뛰어지는 경우가 없도록 하기 위한 추가 명령문임
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "map : " << workloadNum << "번의 검색 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "map : " << workloadNum << "번의 검색 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[map 삭제 측정 시작]" << endl;
	cout << endl << "|------------------------------------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < workloadNum; i++)
	{
		if (i % (workloadNum / 50) == 0)
		{
			cout << "*";
		}

		if (i % 10000 == 0)
		{
			timeEnd = clock.now();
			timeDiff = timeEnd - timeBegin;
			if (timeDiff.count() > 50)
			{
				break;
			}
		}

		copyInsertTestMap.erase(insertKeyWorkload[i]);
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "map : " << workloadNum << "번의 삭제 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "map : " << workloadNum << "번의 삭제 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[map 소멸자 측정 시작]" << endl;
	return clock.now();
}