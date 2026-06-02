//로그 출력문들을 활성화하고 싶을 시 아래 구문의 주석을 해제할 것
//#define TREE_LOG	
//#define TREE_WARNING
//#define TREE_ERROR

#include "BstUsingWhile.h"		//직접 정의한 클래스 
#include <crtdbg.h>				//_CrtSetDbgFlagBst
#include <iostream>				//std::cout, std::endl
#include <iomanip>				//std::fixed, std::setprecision
#include <chrono>;				//chrono::clock, chrono::time_point, chrono::steady_clock, chrono::duration, chrono::duration::count
#include <string>;				//std::string
#include <numeric>;				//std::iota
#include <random>;				//std::mt19937
#include <algorithm>			//std::shuffle
#include <map>;					//std::map
#include <utility>				//std::move

using namespace std;
using namespace chrono;

template <typename DataType>
void PrintKeyAndData(int key, const DataType& retrievedData);

void RandomWorkloadSpeedTest(int workloadNum, int workloadPerDataLen);

void RandomLocalWorkloadTest(int workloadNum, int workloadPerDataLen, int localBlockSize);

void LinearIncreaseWorkloadTest(int workloadNum, int workloadPerDataLen);

void LinearDecreaseWorkloadTest(int workloadNum, int workloadPerDataLen);

//parameter	: insertDataWorkload는 복사 비용이 크지만, 그럼에도 하나의 워크로드를 Bst와 map에 반복해서 사용할 수 있도록 값복사 형식의 매개변수를 사용함
time_point<steady_clock> TestBst(steady_clock& clock, int workloadNum, vector<string> insertDataWorkload, const vector<int>& insertKeyWorkload, const vector<int>& retrieveKeyWorkload, const vector<int>& removeKeyWorkload);

time_point<steady_clock> TestMap(steady_clock& clock, int workloadNum, vector<string> insertDataWorkload, const vector<int>& insertKeyWorkload, const vector<int>& retrieveKeyWorkload, const vector<int>& removeKeyWorkload);

int main()
{
	//note : 디버깅 실행이 종료될 시점에도 해제되지 않은 동적 메모리 누수가 존재할 시, Visual Studio의 하단의 출력창(output)에 해당 누수에 대한 정보가 출력됨
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	cout << endl << "디버그 구성 테스트 1/3 : Bst<int>-------------------------------------------------------------------" << endl;

	Bst<int> intTestBst;

	cout << endl << "삽입 (트리 A)" << endl;
	intTestBst.Insert(5, 55);
	intTestBst.Insert(7, 77);
	intTestBst.Insert(3, 33);
	intTestBst.Insert(4, 44);
	intTestBst.Insert(6, 66);
	intTestBst.Insert(9, 99);
	intTestBst.Insert(2, 22);
	intTestBst.PreorderPrint();

	cout << endl << "중위 순회 출력 (트리 A)" << endl;
	intTestBst.InorderPrint();

	cout << endl << "후위 순회 출력 (트리 A)" << endl;
	intTestBst.PostorderPrint();

	cout << endl << "검색 (트리 A)" << endl;
	int intRetrievedData = 0;
	intTestBst.Retrieve(3, intRetrievedData);
	PrintKeyAndData(3, intRetrievedData);

	cout << endl << "명시적 복사 (트리 A -> B)" << endl;
	Bst<int> intExplicitCopyTestBst;
	intExplicitCopyTestBst.CopyTree(intTestBst);
	intExplicitCopyTestBst.PreorderPrint();

	cout << endl << "복사 생성자 (트리 A -> C)" << endl;
	Bst<int> intCopyConstructorTestBst = intTestBst;
	intCopyConstructorTestBst.PreorderPrint();

	cout << endl << "복사 할당 연산자 (트리 A -> D)" << endl;
	Bst<int> intCopyAssignmentTestBst;
	intCopyAssignmentTestBst = intTestBst;
	intCopyAssignmentTestBst.PreorderPrint();

	cout << endl << "이동 생성자 (트리 C -> E)" << endl;
	Bst<int> intMoveConstructorTestBst = move(intCopyConstructorTestBst);
	intMoveConstructorTestBst.PreorderPrint();

	cout << endl << "이동 할당 연산자 (트리 D -> F)" << endl;
	Bst<int> intMoveAssignmentTestBst;
	intMoveAssignmentTestBst = move(intCopyAssignmentTestBst);
	intMoveAssignmentTestBst.PreorderPrint();

	cout << endl << "이동 후 소스 트리는 비워짐 (트리 C, D)" << endl;
	intCopyConstructorTestBst.PreorderPrint();
	intCopyAssignmentTestBst.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	intTestBst.Remove(7);
	intTestBst.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	intTestBst.Remove(3);
	intTestBst.PreorderPrint();

	cout << endl << "전체 삭제 (트리 A)" << endl;
	intTestBst.RemoveTree();
	intTestBst.PreorderPrint();

	cout << endl << "복사한 트리는 원본과 독립적임 (트리 B)" << endl;
	intExplicitCopyTestBst.PreorderPrint();

	cout << endl << "디버그 구성 테스트 2/3 : Bst<float>-----------------------------------------------------------------" << endl;

	Bst<float> floatTestBst;

	cout << endl << "삽입 (트리 A)" << endl;
	floatTestBst.Insert(5, 0.55f);
	floatTestBst.Insert(7, 0.77f);
	floatTestBst.Insert(3, 0.33f);
	floatTestBst.Insert(4, 0.44f);
	floatTestBst.Insert(6, 0.66f);
	floatTestBst.Insert(9, 0.99f);
	floatTestBst.Insert(2, 0.22f);
	floatTestBst.PreorderPrint();

	cout << endl << "중위 순회 출력 (트리 A)" << endl;
	floatTestBst.InorderPrint();

	cout << endl << "후위 순회 출력 (트리 A)" << endl;
	floatTestBst.PostorderPrint();

	cout << endl << "검색 (트리 A)" << endl;
	float floatRetrievedData = 0.0f;
	floatTestBst.Retrieve(3, floatRetrievedData);
	PrintKeyAndData(3, floatRetrievedData);

	cout << endl << "명시적 복사 (트리 A -> B)" << endl;
	Bst<float> floatExplicitCopyTestBst;
	floatExplicitCopyTestBst.CopyTree(floatTestBst);
	floatExplicitCopyTestBst.PreorderPrint();

	cout << endl << "복사 생성자 (트리 A -> C)" << endl;
	Bst<float> floatCopyConstructorTestBst = floatTestBst;
	floatCopyConstructorTestBst.PreorderPrint();

	cout << endl << "복사 할당 연산자 (트리 A -> D)" << endl;
	Bst<float> floatCopyAssignmentTestBst;
	floatCopyAssignmentTestBst = floatTestBst;
	floatCopyAssignmentTestBst.PreorderPrint();

	cout << endl << "이동 생성자 (트리 C -> E)" << endl;
	Bst<float> floatMoveConstructorTestBst = move(floatCopyConstructorTestBst);
	floatMoveConstructorTestBst.PreorderPrint();

	cout << endl << "이동 할당 연산자 (트리 D -> F)" << endl;
	Bst<float> floatMoveAssignmentTestBst;
	floatMoveAssignmentTestBst = move(floatCopyAssignmentTestBst);
	floatMoveAssignmentTestBst.PreorderPrint();

	cout << endl << "이동 후 소스 트리는 비워짐 (트리 C, D)" << endl;
	floatCopyConstructorTestBst.PreorderPrint();
	floatCopyAssignmentTestBst.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	floatTestBst.Remove(7);
	floatTestBst.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	floatTestBst.Remove(3);
	floatTestBst.PreorderPrint();

	cout << endl << "전체 삭제 (트리 A)" << endl;
	floatTestBst.RemoveTree();
	floatTestBst.PreorderPrint();

	cout << endl << "복사한 트리는 원본과 독립적임 (트리 B)" << endl;
	floatExplicitCopyTestBst.PreorderPrint();

	cout << endl << "디버그 구성 테스트 3/3 : Bst<string>----------------------------------------------------------------" << endl;

	Bst<string> stringTestBst;

	cout << endl << "삽입 (트리 A)" << endl;
	stringTestBst.Insert(5, "Panther");
	stringTestBst.Insert(7, "Comet");
	stringTestBst.Insert(3, "Crusader");
	stringTestBst.Insert(4, "Sherman");
	stringTestBst.Insert(6, "Tiger");
	stringTestBst.Insert(9, "Mouse");
	stringTestBst.Insert(2, "Stuart");
	stringTestBst.PreorderPrint();

	cout << endl << "중위 순회 출력 (트리 A)" << endl;
	stringTestBst.InorderPrint();

	cout << endl << "후위 순회 출력 (트리 A)" << endl;
	stringTestBst.PostorderPrint();

	cout << endl << "검색 (트리 A)" << endl;
	string stringRetrievedData = "";
	stringTestBst.Retrieve(3, stringRetrievedData);
	PrintKeyAndData(3, stringRetrievedData);

	cout << endl << "명시적 복사 (트리 A -> B)" << endl;
	Bst<string> stringExplicitCopyTestBst;
	stringExplicitCopyTestBst.CopyTree(stringTestBst);
	stringExplicitCopyTestBst.PreorderPrint();

	cout << endl << "복사 생성자 (트리 A -> C)" << endl;
	Bst<string> stringCopyConstructorTestBst = stringTestBst;
	stringCopyConstructorTestBst.PreorderPrint();

	cout << endl << "복사 할당 연산자 (트리 A -> D)" << endl;
	Bst<string> stringCopyAssignmentTestBst;
	stringCopyAssignmentTestBst = stringTestBst;
	stringCopyAssignmentTestBst.PreorderPrint();

	cout << endl << "이동 생성자 (트리 C -> E)" << endl;
	Bst<string> stringMoveConstructorTestBst = move(stringCopyConstructorTestBst);
	stringMoveConstructorTestBst.PreorderPrint();

	cout << endl << "이동 할당 연산자 (트리 D -> F)" << endl;
	Bst<string> stringMoveAssignmentTestBst;
	stringMoveAssignmentTestBst = move(stringCopyAssignmentTestBst);
	stringMoveAssignmentTestBst.PreorderPrint();

	cout << endl << "이동 후 소스 트리는 비워짐 (트리 C, D)" << endl;
	stringCopyConstructorTestBst.PreorderPrint();
	stringCopyAssignmentTestBst.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	stringTestBst.Remove(7);
	stringTestBst.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	stringTestBst.Remove(3);
	stringTestBst.PreorderPrint();

	cout << endl << "전체 삭제 (트리 A)" << endl;
	stringTestBst.RemoveTree();
	stringTestBst.PreorderPrint();

	cout << endl << "복사한 트리는 원본과 독립적임 (트리 B)" << endl;
	stringExplicitCopyTestBst.PreorderPrint();
#endif

#ifndef _DEBUG
	cout << fixed << setprecision(2);

	cout << endl << "릴리즈 구성 테스트 1/4 : 랜덤 워크로드 테스트-------------------------------------------------------" << endl;

	/*	(테스팅 방법)
		randomWorkloadNum 횟수만큼 복사 삽입(트리 A), 이동 삽입(트리 B), 검색(트리 A), 삭제(트리 A)와 소멸(트리 B)을 수행함
		키는 [0,randomWorkloadNum-1] 의 중복되지 않는 키 값들을 랜덤하게 셔플해놓고 사용함(삽입, 검색, 삭제의 키 값들은 각각 독립으로 셔플됨)
		데이터는 randomWorkloadPerDataLen 으로 지정된 길이의 string 객체를 randomWorkloadNum 개 만들어놓고 사용함
	*/

	/*	(테스팅 결과)
		[randomWorkloadNum			= 10,000,000]
		[randomWorkloadPerDataLen	= 30]

		복사 삽입	: Bst = 17.63초	|	std::map = 20.49초
		이동 삽입	: Bst = 17.19초	|	std::map = 16.48초
		검색		: Bst = 18.83초	|	std::map = 16.96초
		삭제		: Bst = 34.07초	|	std::map = 26.50초
		소멸		: Bst =  5.21초	|	std::map =  4.97초
	*/

	/*	(테스팅 해석)
		본 단원의 Bst는 반복문으로 구현되었기에, 컴파일러의 최적화에 상관없이 재귀 호출은 무조건 일어나지 않음
		다만 빌드된 어셈블리 코드를 보았을 때 삽입, 삭제 메소드에서 하위 작업 메소드 호출이 인라이닝되지 못하고 있는 것이 느린 속도의 원인으로 추정함
	*/

	const int randomWorkloadNum = 10000000;
	const int randomWorkloadPerDataLen = 30;
	RandomWorkloadSpeedTest(randomWorkloadNum, randomWorkloadPerDataLen);

	cout << endl << "릴리즈 구성 테스트 2/4 : 랜덤 로컬 워크로드 테스트--------------------------------------------------" << endl;

	/*	(테스팅 방법)
		앞선 1번 랜덤 워크로드 테스트와 비슷하나, 키 값들이 localBlockSize 단위로 내부에서 선형 증가 연속성을 가지도록 하였음
	*/

	/*	(테스팅 결과)
		[randomLocalWorkloadNum			= 앞선 1번 랜덤 워크로드 테스트와 동일]
		[randomLocalWorkloadPerDataLen	= 앞선 1번 랜덤 워크로드 테스트와 동일]
		[localBlockSize					= 10]

		복사 삽입	: Bst =  9.25초	|	std::map =  4.09초
		이동 삽입	: Bst =  9.01초	|	std::map =  3.42초
		검색		: Bst =  7.03초	|	std::map =  3.56초
		삭제		: Bst =  7.97초	|	std::map =  4.39초
		소멸		: Bst =  3.57초	|	std::map =  3.68초
	*/

	/*	(테스팅 해석)
		워크로드가 지역 선형성을 가지게 되므로 1번 랜덤 워크로드 테스트에 비해서 Bst의 트리 높이는 더 높아질 것임
		그럼에도 1번 랜덤 워크로드 테스트에 비해서 Bst의 삽입, 검색, 삭제 메소드가 더 빨라진 이유는 캐시 히트율이 높아졌기 때문으로 추정함
		본 테스트에서 Bst와 std::map의 속도 차이가 벌어진 이유는 설계와 구현상에서 캐시 히트율 고려에 대한 수준 차이 때문으로 추정함
	*/

	const int randomLocalWorkloadNum = randomWorkloadNum;
	const int randomLocalWorkloadPerDataLen = randomWorkloadPerDataLen;
	const int localBlockSize = 10;
	RandomLocalWorkloadTest(randomLocalWorkloadNum, randomLocalWorkloadPerDataLen, localBlockSize);

	cout << endl << "릴리즈 구성 테스트 3/4 : 선형 증가 워크로드 테스트--------------------------------------------------" << endl;

	/*	(테스팅 방법)
		앞선 1번 랜덤 워크로드 테스트와 비슷하나, 키값들을 뒤섞지 않고 선형 그대로 사용함
	*/

	/*	(테스팅 결과)
		[linearIncreaseWorkloadNum			= 앞선 1번 랜덤 워크로드 테스트와 동일]
		[linearIncreaseWorkloadPerDataLen	= 앞선 1번 랜덤 워크로드 테스트와 동일]

		복사 삽입	: Bst = 시간초과|	std::map =  1.94초
		이동 삽입	: Bst = 시간초과|	std::map =  1.44초
		검색		: Bst = 시간초과|	std::map =  0.69초
		삭제		: Bst =  0.02초	|	std::map =  1.84초
		소멸		: Bst =  0.39초 |	std::map =  1.44초
	*/

	/*	(테스팅 해석)
		선형 워크로드는 Bst를 편향시켜 리스트와 같은 모양이 되도록 함
		
		삽입 메소드와 검색 메소드는 리스트와 같은 모양이 되어서 나타나는 O(N^2)의 시간 복잡도 때문에 시간 초과가 남

		삭제 메소드의 속도가 매우 빠른 이유는 선형 삽입과 똑같은 키로 삭제를 수행하니 매번 삭제 대상이 루트에 위치해서임

		소멸의 경우에도 회전을 이용해서 루트노드에서만 작업이 이뤄지므로, 선형 워크로드를 랜덤 워크로드보다 빠른 속도를 보임
		우하향 리스트와 같은 트리를 그저 좌하향 리스트로 길이 N 횟수만 회전 시키고서 계속 루트에서 소멸을 진행하니 아래 4번 선형 감소 워크로드와도 큰 차이가 없음

		Bst와 달리 std::map은 균형을 유지하는 트리이기 때문에 선형 워크로드에 대해서도 O(NlogN)의 시간 복잡도를 가져 매우 빠른 속도를 보임
		게다가 선형 워크로드가 가진 지역성으로 캐시 히트율이 증가해 1번 랜덤 워크로드 테스트보다 훨씬 빠른 속도를 보이는 것으로 추정함
	*/

	const int linearIncreaseWorkloadNum = randomWorkloadNum;
	const int linearIncreaseWorkloadPerDataLen = randomWorkloadPerDataLen;
	LinearIncreaseWorkloadTest(linearIncreaseWorkloadNum, linearIncreaseWorkloadPerDataLen);

	cout << endl << "릴리즈 구성 테스트 4/4 : 선형 감소 워크로드 테스트--------------------------------------------------" << endl;

	/*	(테스팅 방법)
		앞선 3번 선형 증가 워크로드 테스트와 비슷하나, 키를 역순으로 사용함
	*/

	/*	(테스팅 결과)
		[linearDecreaseWorkloadNum			= 앞선 1번 랜덤 워크로드 테스트와 동일]
		[linearDecreaseWorkloadPerDataLen	= 앞선 1번 랜덤 워크로드 테스트와 동일]

		복사 삽입	: Bst = 시간초과|	std::map =  2.13초
		이동 삽입	: Bst = 시간초과|	std::map =  1.64초
		검색		: Bst = 시간초과|	std::map =  0.83초
		삭제		: Bst =  0.03초	|	std::map =  2.06초
		소멸		: Bst =  0.32초 |	std::map =  2.87초
	*/

	/*	(테스팅 해석)
		앞선 3번 선형 증가 워크로드 테스트와 같음
	*/

	const int linearDecreaseWorkloadNum = randomWorkloadNum;
	const int linearDecreaseWorkloadPerDataLen = randomWorkloadPerDataLen;
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
	timeBegin = TestBst(clock, workloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "Bst : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

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
	timeBegin = TestBst(clock, realWorkloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "Bst : " << realWorkloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

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
	timeBegin = TestBst(clock, workloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "Bst : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

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
	timeBegin = TestBst(clock, workloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "Bst : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "---------------------------------------------------------------------------" << endl;

	cout << endl << "선형 감소 워크로드 복사 중...." << endl;
	timeBegin = TestMap(clock, workloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "map : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << endl << endl << endl << endl;
}


time_point<steady_clock> TestBst(steady_clock& clock, int workloadNum, vector<string> insertDataWorkload, const vector<int>& insertKeyWorkload, const vector<int>& retrieveKeyWorkload, const vector<int>& removeKeyWorkload)
{
	Bst<string> copyInsertTestBst;
	Bst<string> moveInsertTestBst;

	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "[Bst 복사 삽입 측정 시작]" << endl;
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

		copyInsertTestBst.Insert(insertKeyWorkload[i], insertDataWorkload[i]);
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "Bst : " << workloadNum << "번의 복사 삽입 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "Bst : " << workloadNum << "번의 복사 삽입 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[Bst 이동 삽입 측정 시작]" << endl;
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

		moveInsertTestBst.Insert(insertKeyWorkload[i], move(insertDataWorkload[i]));
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "Bst : " << workloadNum << "번의 이동 삽입 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "Bst : " << workloadNum << "번의 이동 삽입 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	string retrievedData;

	cout << endl << "[Bst 검색 측정 시작]" << endl;
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

		copyInsertTestBst.Retrieve(insertKeyWorkload[i], retrievedData);
		retrievedData += 'a';			//note : 컴파일, 링킹 최적화로 테스트 중의 검색 메소드 호출이 건너뛰어지는 경우가 없도록 하기 위한 추가 명령문임
	}
	cout << endl;

	if (timeDiff.count() < 50)
	{
		cout << endl << "Bst : " << workloadNum << "번의 검색 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "Bst : " << workloadNum << "번의 검색 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[Bst 삭제 측정 시작]" << endl;
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

		copyInsertTestBst.Remove(removeKeyWorkload[i]);
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "Bst : " << workloadNum << "번의 삭제 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "Bst : " << workloadNum << "번의 삭제 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[Bst 소멸자 측정 시작]" << endl;
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
		iterator->second += 'a';			//note : 컴파일, 링킹 최적화로 테스트 중의 검색 메소드 호출이 건너뛰어지는 경우가 없도록 하기 위한 추가 명령문임
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