//로그 출력문들을 활성화하고 싶을 시 아래 구문의 주석을 해제할 것
//#define TREE_LOG	
//#define TREE_ERROR
//#define TREE_WARNING

#include "heap.h"				//직접 정의한 클래스 Heap
#include <crtdbg.h>				//_CrtSetDbgFlag
#include <iostream>				//std::cout, std::endl
#include <iomanip>				//std::fixed, std::setprecision
#include <chrono>;				//chrono::clock, chrono::time_point, chrono::steady_clock, chrono::duration, chrono::duration::count
#include <string>;				//std::string
#include <numeric>;				//std::iota
#include <random>;				//std::mt19937
#include <algorithm>			//std::shuffle
#include <queue>;				//std::priority_queue

using namespace chrono;

template <typename DataType>
void PrintData(const DataType& poppedData);

void RandomWorkloadTest(int workloadNum, int workloadPerDataLen);

void RandomLocalWorkloadTest(int workloadNum, int workloadPerDataLen, int localBlockSize);

void LinearIncreaseWorkloadTest(int workloadNum, int workloadPerDataLen);

void LinearDecreaseWorkloadTest(int workloadNum, int workloadPerDataLen);

//pushDataWorkload는 복사 비용이 크지만, 그럼에도 하나의 워크로드를 Heap와 priority_queue에 반복해서 사용할 수 있도록 값복사 형식의 매개변수를 사용함
time_point<steady_clock> TestHeap(steady_clock& clock, int workloadNum, vector<string> pushDataWorkload, const vector<int>& pushKeyWorkload);

time_point<steady_clock> TestPriorityQueue(steady_clock& clock, int workloadNum, vector<string> pushDataWorkload, const vector<int>& pushKeyWorkload);

int main()
{
	//디버깅 실행이 종료될 시점에도 해제되지 않은 동적 메모리 누수가 존재할 시, Visual Studio의 하단의 출력창(output)에 해당 누수에 대한 정보가 출력됨
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	cout << endl << "디버그 구성 테스트 1/3 : MinHeap<int>---------------------------------------------------------------" << endl;

	MinHeap<int> intTestHeap;

	cout << endl << "푸시 (힙 A)" << endl;
	intTestHeap.Push(5, 55);
	intTestHeap.Push(7, 77);
	intTestHeap.Push(3, 33);
	intTestHeap.Push(4, 44);
	intTestHeap.Push(6, 66);
	intTestHeap.Push(9, 99);
	intTestHeap.Push(2, 22);
	intTestHeap.PrintHeap();

	cout << endl << "명시적 복사 (힙 A -> B)" << endl;
	MinHeap<int> intExplicitCopyTestHeap;
	intExplicitCopyTestHeap.CopyHeap(intTestHeap);
	intExplicitCopyTestHeap.PrintHeap();

	cout << endl << "복사 생성자 (힙 A -> C)" << endl;
	MinHeap<int> intCopyConstructorTestHeap = intTestHeap;
	intCopyConstructorTestHeap.PrintHeap();

	cout << endl << "복사 할당 연산자 (힙 A -> D)" << endl;
	MinHeap<int> intCopyAssignmentTestHeap;
	intCopyAssignmentTestHeap = intTestHeap;
	intCopyAssignmentTestHeap.PrintHeap();

	cout << endl << "이동 생성자 (힙 C -> E)" << endl;
	MinHeap<int> intMoveConstructorTestHeap = move(intCopyConstructorTestHeap);
	intMoveConstructorTestHeap.PrintHeap();

	cout << endl << "이동 할당 연산자 (힙 D -> F)" << endl;
	MinHeap<int> intMoveAssignmentTestHeap;
	intMoveAssignmentTestHeap = move(intCopyAssignmentTestHeap);
	intMoveAssignmentTestHeap.PrintHeap();

	cout << endl << "이동 후 소스 힙는 비워짐 (힙 C, D)" << endl;
	intCopyConstructorTestHeap.PrintHeap();
	intCopyAssignmentTestHeap.PrintHeap();

	cout << endl << "팝 (힙 A)" << endl;
	int intPoppedData1 = 0;
	intTestHeap.Pop(intPoppedData1);
	PrintData(intPoppedData1);

	int intPoppedData2 = 0;
	intTestHeap.Pop(intPoppedData2);
	PrintData(intPoppedData2);

	int intPoppedData3 = 0;
	intTestHeap.Pop(intPoppedData3);
	PrintData(intPoppedData3);

	int intPoppedData4 = 0;
	intTestHeap.Pop(intPoppedData4);
	PrintData(intPoppedData4);

	int intPoppedData5 = 0;
	intTestHeap.Pop(intPoppedData5);
	PrintData(intPoppedData5);

	int intPoppedData6 = 0;
	intTestHeap.Pop(intPoppedData6);
	PrintData(intPoppedData6);

	int intPoppedData7 = 0;
	intTestHeap.Pop(intPoppedData7);
	PrintData(intPoppedData7);

	cout << endl << "비워진 상태에서 팝 (힙 A)" << endl;
	int intPoppedData8 = 0;
	intTestHeap.Pop(intPoppedData8);
	PrintData(intPoppedData8);

	cout << endl << "복사한 힙는 원본과 독립적임 (힙 B)" << endl;
	intExplicitCopyTestHeap.PrintHeap();

	cout << endl << "디버그 구성 테스트 2/3 : MinHeap<float>-------------------------------------------------------------" << endl;

	MinHeap<float> floatTestHeap;

	cout << endl << "푸시 (힙 A)" << endl;
	floatTestHeap.Push(5, 0.55f);
	floatTestHeap.Push(7, 0.77f);
	floatTestHeap.Push(3, 0.33f);
	floatTestHeap.Push(4, 0.44f);
	floatTestHeap.Push(6, 0.66f);
	floatTestHeap.Push(9, 0.99f);
	floatTestHeap.Push(2, 0.22f);
	floatTestHeap.PrintHeap();

	cout << endl << "명시적 복사 (힙 A -> B)" << endl;
	MinHeap<float> floatExplicitCopyTestHeap;
	floatExplicitCopyTestHeap.CopyHeap(floatTestHeap);
	floatExplicitCopyTestHeap.PrintHeap();

	cout << endl << "복사 생성자 (힙 A -> C)" << endl;
	MinHeap<float> floatCopyConstructorTestHeap = floatTestHeap;
	floatCopyConstructorTestHeap.PrintHeap();

	cout << endl << "복사 할당 연산자 (힙 A -> D)" << endl;
	MinHeap<float> floatCopyAssignmentTestHeap;
	floatCopyAssignmentTestHeap = floatTestHeap;
	floatCopyAssignmentTestHeap.PrintHeap();

	cout << endl << "이동 생성자 (힙 C -> E)" << endl;
	MinHeap<float> floatMoveConstructorTestHeap = move(floatCopyConstructorTestHeap);
	floatMoveConstructorTestHeap.PrintHeap();

	cout << endl << "이동 할당 연산자 (힙 D -> F)" << endl;
	MinHeap<float> floatMoveAssignmentTestHeap;
	floatMoveAssignmentTestHeap = move(floatCopyAssignmentTestHeap);
	floatMoveAssignmentTestHeap.PrintHeap();

	cout << endl << "이동 후 소스 힙는 비워짐 (힙 C, D)" << endl;
	floatCopyConstructorTestHeap.PrintHeap();
	floatCopyAssignmentTestHeap.PrintHeap();

	cout << endl << "팝 (힙 A)" << endl;
	float floatPoppedData1 = 0.0f;
	floatTestHeap.Pop(floatPoppedData1);
	PrintData(floatPoppedData1);

	float floatPoppedData2 = 0.0f;
	floatTestHeap.Pop(floatPoppedData2);
	PrintData(floatPoppedData2);

	float floatPoppedData3 = 0.0f;
	floatTestHeap.Pop(floatPoppedData3);
	PrintData(floatPoppedData3);

	float floatPoppedData4 = 0.0f;
	floatTestHeap.Pop(floatPoppedData4);
	PrintData(floatPoppedData4);

	float floatPoppedData5 = 0.0f;
	floatTestHeap.Pop(floatPoppedData5);
	PrintData(floatPoppedData5);

	float floatPoppedData6 = 0.0f;
	floatTestHeap.Pop(floatPoppedData6);
	PrintData(floatPoppedData6);

	float floatPoppedData7 = 0.0f;
	floatTestHeap.Pop(floatPoppedData7);
	PrintData(floatPoppedData7);

	cout << endl << "비워진 상태에서 팝 (힙 A)" << endl;
	float floatPoppedData8 = 0.0f;
	floatTestHeap.Pop(floatPoppedData8);
	PrintData(floatPoppedData8);

	cout << endl << "복사한 힙는 원본과 독립적임 (힙 B)" << endl;
	floatExplicitCopyTestHeap.PrintHeap();

	cout << endl << "디버그 구성 테스트 3/3 : MinHeap<string>------------------------------------------------------------" << endl;

	MinHeap<string> stringTestHeap;

	cout << endl << "푸시 (힙 A)" << endl;
	stringTestHeap.Push(5, "Panther");
	stringTestHeap.Push(7, "Comet");
	stringTestHeap.Push(3, "Crusader");
	stringTestHeap.Push(4, "Sherman");
	stringTestHeap.Push(6, "Tiger");
	stringTestHeap.Push(9, "Mouse");
	stringTestHeap.Push(2, "Stuart");
	stringTestHeap.PrintHeap();

	cout << endl << "명시적 복사 (힙 A -> B)" << endl;
	MinHeap<string> stringExplicitCopyTestHeap;
	stringExplicitCopyTestHeap.CopyHeap(stringTestHeap);
	stringExplicitCopyTestHeap.PrintHeap();

	cout << endl << "복사 생성자 (힙 A -> C)" << endl;
	MinHeap<string> stringCopyConstructorTestHeap = stringTestHeap;
	stringCopyConstructorTestHeap.PrintHeap();

	cout << endl << "복사 할당 연산자 (힙 A -> D)" << endl;
	MinHeap<string> stringCopyAssignmentTestHeap;
	stringCopyAssignmentTestHeap = stringTestHeap;
	stringCopyAssignmentTestHeap.PrintHeap();

	cout << endl << "이동 생성자 (힙 C -> E)" << endl;
	MinHeap<string> stringMoveConstructorTestHeap = move(stringCopyConstructorTestHeap);
	stringMoveConstructorTestHeap.PrintHeap();

	cout << endl << "이동 할당 연산자 (힙 D -> F)" << endl;
	MinHeap<string> stringMoveAssignmentTestHeap;
	stringMoveAssignmentTestHeap = move(stringCopyAssignmentTestHeap);
	stringMoveAssignmentTestHeap.PrintHeap();

	cout << endl << "이동 후 소스 힙는 비워짐 (힙 C, D)" << endl;
	stringCopyConstructorTestHeap.PrintHeap();
	stringCopyAssignmentTestHeap.PrintHeap();

	cout << endl << "팝 (힙 A)" << endl;
	string stringPoppedData1 = "";
	stringTestHeap.Pop(stringPoppedData1);
	PrintData(stringPoppedData1);

	string stringPoppedData2 = "";
	stringTestHeap.Pop(stringPoppedData2);
	PrintData(stringPoppedData2);

	string stringPoppedData3 = "";
	stringTestHeap.Pop(stringPoppedData3);
	PrintData(stringPoppedData3);

	string stringPoppedData4 = "";
	stringTestHeap.Pop(stringPoppedData4);
	PrintData(stringPoppedData4);

	string stringPoppedData5 = "";
	stringTestHeap.Pop(stringPoppedData5);
	PrintData(stringPoppedData5);

	string stringPoppedData6 = "";
	stringTestHeap.Pop(stringPoppedData6);
	PrintData(stringPoppedData6);

	string stringPoppedData7 = "";
	stringTestHeap.Pop(stringPoppedData7);
	PrintData(stringPoppedData7);

	cout << endl << "비워진 상태에서 팝 (힙 A)" << endl;
	string stringPoppedData8 = "";
	stringTestHeap.Pop(stringPoppedData8);
	PrintData(stringPoppedData8);

	cout << endl << "복사한 힙는 원본과 독립적임 (힙 B)" << endl;
	stringExplicitCopyTestHeap.PrintHeap();
#endif

#ifndef _DEBUG
	cout << fixed << setprecision(2);

	cout << endl << "릴리즈 구성 테스트 1/4 : 랜덤 워크로드 테스트-------------------------------------------------------" << endl;
	
	/*	(테스팅 방법)
		randomWorkloadNum 횟수만큼 복사 푸시(힙 A), 이동 푸시(힙 B), 팝(힙 A), 소멸(힙 B)을 수행함
		키는 [0,randomWorkloadNum-1] 의 중복되지 않는 키 값들을 랜덤하게 셔플해놓고 사용함(삽입, 검색, 삭제의 키 값들은 각각 독립으로 셔플됨)
		데이터는 randomWorkloadPerDataLen 으로 지정된 길이의 string 객체를 randomWorkloadNum 개 만들어놓고 사용함
	*/

	/*	(테스팅 결과)
		[randomWorkloadNum			= 10,000,000]
		[randomWorkloadPerDataLen	= 30]

		복사 푸시	: Heap =  2.25초	|	std::priority_queue =  1.25초
		이동 푸시	: Heap =  1.61초	|	std::priority_queue =  1.04초
		팝			: Heap = 13.41초	|	std::priority_queue = 14.47초
		소멸		: Heap =  1.37초	|	std::priority_queue =  0.75초
	*/

	/*	(테스팅 해석)
		리프에 삽입되어서 부모와 대소 관계를 비교해 루트노드까지 거슬러 올라가는 푸시와 달리,
		팝은 루트노드에서 시작해서 자식 둘과 모두 비교를 수행하며 내려가기에 속도가 느린 것으로 추정함

		직접 정의한 힙이 stl의 우선순위 큐보다 팝은 다소 빠르게 나타났으나,
		우선순위큐는 힙의 팝에 직접 대응되는 메소드가 없어 pop(..)과 top(..)을 나누어 호출했음을 감안하면 사실 거의 비긴셈임

		푸시 메소드에선 우선순위 큐가 직접 정의한 힙보다 훨씬 빠른 속도를 보이고 있음.
		우선 폴리몰피즘을 사용한 현재 힙의 구현 방식을 좀 더 단단한 방식으로 개선해서 속도를 올릴 필요가 있음.
	*/

	const int randomWorkloadNum = 10000000;
	const int randomWorkloadPerDataLen = 30;
	RandomWorkloadTest(randomWorkloadNum, randomWorkloadPerDataLen);

	cout << endl << "릴리즈 구성 테스트 2/4 : 랜덤 로컬 워크로드 테스트--------------------------------------------------" << endl;

	/*	(테스팅 방법)
		앞선 1번 랜덤 워크로드 테스트와 비슷하나, 키 값들이 localBlockSize 단위로 내부에서 선형 증가 연속성을 가지도록 하였음
	*/

	/*	(테스팅 결과)
		[randomLocalWorkloadNum			= 앞선 1번 랜덤 워크로드 테스트와 동일]
		[randomLocalWorkloadPerDataLen	= 앞선 1번 랜덤 워크로드 테스트와 동일]
		[localBlockSize					= 10]

		복사 푸시	: Heap =  2.06초	|	std::priority_queue =  1.19초
		이동 푸시	: Heap =  1.44초	|	std::priority_queue =  0.68초
		팝			: Heap =  9.28초	|	std::priority_queue =  8.40초
		소멸		: Heap =  1.49초	|	std::priority_queue =  0.83초
	*/

	/*	(테스팅 해석)
		블럭 안의 지역성이 캐시 히트율을 높여주어 앞선 1번 랜덤 워크로드 테스트보다 전반적으로 빠른 속도가 나온 것으로 보임.
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

		복사 푸시	: Heap =  1.48초	|	std::priority_queue =  1.14초
		이동 푸시	: Heap =  1.03초	|	std::priority_queue =  0.66초
		팝			: Heap =  2.78초	|	std::priority_queue =  1.63초
		소멸		: Heap =  0.94초	|	std::priority_queue =  0.41초
	*/

	/*	(테스팅 해석)
		힙과 우선순위큐 둘 다 선형 증가 워크로드에서 푸시가 팝보다 더 빠른 속도를 보임
		오름차순으로 푸시되면, 최소힙은 별다른 조정을 해줄 필요가 없기 때문에 최선의 시간이 걸리게 되기에 이런 결과가 나오는 듯함
		
		이러한 공통적인 경향성을 지니지만, 여전히 Heap의 최적화 수준은 우선수위큐에 비해 크게 떨어짐
		앞서 1번 랜덤 워크로드 테스트에서 말했듯이 현재 힙의 구현에 사용된 폴리몰피즘 방식부터 탈피하고서 최적화를 해나갸아할 듯함
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

		복사 푸시	: Heap =  3.59초	|	std::priority_queue =  1.50초
		이동 푸시	: Heap =  3.01초	|	std::priority_queue =  1.01초
		팝			: Heap =  1.99초	|	std::priority_queue =  0.95초
		소멸		: Heap =  1.05초	|	std::priority_queue =  0.55초
	*/

	/*	(테스팅 해석)
		선형 감소 워크로드에서는 앞선 3번 선형 증가 워크로드와 반대로 팝이 푸시보다 더 빠른 속도를 보임
		푸시 과정에서 조정이 일어나지 않았으므로, 항상 힙의 마지막 요소가 최대값이 존재하여서 팝 과정에서는 최악의 시간이 걸리게 되는 듯함
	
		이러한 공통적인 경향성을 지니지만, 여전히 Heap의 최적화 수준은 우선수위큐에 비해 크게 떨어짐
		앞서 1번 랜덤 워크로드 테스트에서 말했듯이 현재 힙의 구현에 사용된 폴리몰피즘 방식부터 탈피하고서 최적화를 해나갸아할 듯함
	*/

	const int linearDecreaseWorkloadNum = linearIncreaseWorkloadNum;
	const int linearDecreaseWorkloadPerDataLen = linearIncreaseWorkloadPerDataLen;
	LinearDecreaseWorkloadTest(linearDecreaseWorkloadNum, linearDecreaseWorkloadPerDataLen);
#endif

	cout << endl << "테스트 종료-----------------------------------------------------------------------------------------" << endl;

	return 0;
}

template <typename DataType>
void PrintData(const DataType& poppedData)
{
	cout << "팝 된 데이터 : " << poppedData << endl;
}

void RandomWorkloadTest(int workloadNum, int workloadPerDataLen)
{
	cout << endl << "랜덤 워크로드 준비 중...." << endl;

	vector<string> pushDataWorkload;
	pushDataWorkload.reserve(workloadNum);
	for (int i = 0; i < workloadNum; i++)
	{
		pushDataWorkload.emplace_back(string(workloadPerDataLen, 'A'));
	}

	vector<int> pushKeyWorkload(workloadNum);
	iota(pushKeyWorkload.begin(), pushKeyWorkload.end(), 0);
	mt19937 pushTestRng(123456);
	shuffle(pushKeyWorkload.begin(), pushKeyWorkload.end(), pushTestRng);

	steady_clock clock;
	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "랜덤 워크로드 복사 중...." << endl;
	timeBegin = TestHeap(clock, workloadNum, pushDataWorkload, pushKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "Heap : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "---------------------------------------------------------------------------" << endl;

	cout << endl << "랜덤 워크로드 복사 중...." << endl;
	timeBegin = TestPriorityQueue(clock, workloadNum, pushDataWorkload, pushKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "priority_queue : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << endl << endl << endl << endl;
}

void RandomLocalWorkloadTest(int workloadNum, int workloadPerDataLen, int localBlockSize)
{
	cout << endl << "랜덤 로컬 워크로드 준비 중...." << endl;

	vector<string> pushDataWorkload;
	pushDataWorkload.reserve(workloadNum);
	for (int i = 0; i < workloadNum; i++)
	{
		pushDataWorkload.emplace_back(string(workloadPerDataLen, 'A'));
	}

	vector<int> pushKeyWorkload(workloadNum / localBlockSize);
	iota(pushKeyWorkload.begin(), pushKeyWorkload.end(), 0);
	mt19937 insertTestRng(123456);
	shuffle(pushKeyWorkload.begin(), pushKeyWorkload.end(), insertTestRng);
	vector<int> insertKeyWorkload;
	for (int blockIndex = 0; blockIndex < pushKeyWorkload.size(); blockIndex++)
	{
		for (int offset = 0; offset < localBlockSize; offset++)
		{
			insertKeyWorkload.push_back(pushKeyWorkload[blockIndex] * localBlockSize + offset);
		}
	}

	steady_clock clock;
	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	int realWorkloadNum = (workloadNum / localBlockSize) * localBlockSize;

	cout << endl << "랜덤 로컬 워크로드 복사 중...." << endl;
	timeBegin = TestHeap(clock, realWorkloadNum, pushDataWorkload, insertKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "Heap : " << realWorkloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "---------------------------------------------------------------------------" << endl;

	cout << endl << "랜덤 로컬 워크로드 복사 중...." << endl;
	timeBegin = TestPriorityQueue(clock, realWorkloadNum, pushDataWorkload, insertKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "priority_queue : " << realWorkloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << endl << endl << endl << endl;
}

void LinearIncreaseWorkloadTest(int workloadNum, int workloadPerDataLen)
{
	cout << endl << "선형 증가 워크로드 준비 중...." << endl;

	vector<string> pushDataWorkload;
	pushDataWorkload.reserve(workloadNum);
	for (int i = 0; i < workloadNum; i++)
	{
		pushDataWorkload.emplace_back(string(workloadPerDataLen, 'A'));
	}

	vector<int> pushKeyWorkload(workloadNum);
	iota(pushKeyWorkload.begin(), pushKeyWorkload.end(), 0);

	steady_clock clock;
	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "선형 증가 워크로드 복사 중...." << endl;
	timeBegin = TestHeap(clock, workloadNum, pushDataWorkload, pushKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "Heap : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "---------------------------------------------------------------------------" << endl;

	cout << endl << "선형 증가 워크로드 복사 중...." << endl;
	timeBegin = TestPriorityQueue(clock, workloadNum, pushDataWorkload, pushKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "priority_queue : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << endl << endl << endl << endl;
}

void LinearDecreaseWorkloadTest(int workloadNum, int workloadPerDataLen)
{
	cout << endl << "선형 감소 워크로드 준비 중...." << endl;

	vector<string> pushDataWorkload;
	pushDataWorkload.reserve(workloadNum);
	for (int i = 0; i < workloadNum; i++)
	{
		pushDataWorkload.emplace_back(string(workloadPerDataLen, 'A'));
	}

	vector<int> pushKeyWorkload(workloadNum);
	iota(pushKeyWorkload.rbegin(), pushKeyWorkload.rend(), 0);

	steady_clock clock;
	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "선형 감소 워크로드 복사 중...." << endl;
	timeBegin = TestHeap(clock, workloadNum, pushDataWorkload, pushKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "Heap : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "---------------------------------------------------------------------------" << endl;

	cout << endl << "선형 감소 워크로드 복사 중...." << endl;
	timeBegin = TestPriorityQueue(clock, workloadNum, pushDataWorkload, pushKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "priority_queue : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << endl << endl << endl << endl;
}

time_point<steady_clock> TestHeap(steady_clock& clock, int workloadNum, vector<string> pushDataWorkload, const vector<int>& pushKeyWorkload)
{
	MinHeap<string> copyPushTestHeap;
	MinHeap<string> movePushTestHeap;

	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "[Heap 복사 푸시 측정 시작]" << endl;
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

		copyPushTestHeap.Push(pushKeyWorkload[i], pushDataWorkload[i]);
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "Heap : " << workloadNum << "번의 복사 푸시 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "Heap : " << workloadNum << "번의 복사 푸시 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[Heap 이동 푸시 측정 시작]" << endl;
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

		movePushTestHeap.Push(pushKeyWorkload[i], move(pushDataWorkload[i]));
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "Heap : " << workloadNum << "번의 이동 푸시 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "Heap : " << workloadNum << "번의 이동 푸시 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[Heap 팝 측정 시작]" << endl;
	cout << endl << "|------------------------------------------------|" << endl;

	timeBegin = clock.now();

	string retrievedData;
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

		copyPushTestHeap.Pop(retrievedData);
		retrievedData += 'a';			//컴파일, 링킹 최적화로 테스트 중의 검색 메소드 호출이 건너뛰어지는 경우가 없도록 하기 위한 추가 명령문임
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "Heap : " << workloadNum << "번의 팝 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "Heap : " << workloadNum << "번의 팝 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "Heap 소멸자 측정 시작" << endl;
	return clock.now();
}

time_point<steady_clock> TestPriorityQueue(steady_clock& clock, int workloadNum, vector<string> pushTestDatum, const vector<int>& pushTestKeys)
{
	struct PriorityQueueNode
	{
		int m_key;
		string m_data;

		PriorityQueueNode(int newKey, const string& newData) : m_key(newKey), m_data(newData)
		{

		}

		PriorityQueueNode(int newKey, string&& newData) noexcept : m_key(newKey), m_data(move(newData))
		{

		}
	};

	struct LessCompare
	{
		bool operator()(const PriorityQueueNode& a, const PriorityQueueNode& b) const noexcept
		{
			return a.m_key > b.m_key;
		}
	};

	priority_queue<PriorityQueueNode, vector<PriorityQueueNode>, LessCompare> copyPushTestPriorityQueue;
	priority_queue<PriorityQueueNode, vector<PriorityQueueNode>, LessCompare> movePushTestPriorityQueue;

	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "[priority_queue 복사 푸시 측정 시작]" << endl;
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

		copyPushTestPriorityQueue.emplace( pushTestKeys[i], pushTestDatum[i] );
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "priority_queue : " << workloadNum << "번의 복사 푸시 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "priority_queue : " << workloadNum << "번의 복사 푸시 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[priority_queue 이동 푸시 측정 시작]" << endl;
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

		movePushTestPriorityQueue.emplace(pushTestKeys[i], move(pushTestDatum[i]));
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "priority_queue : " << workloadNum << "번의 이동 푸시 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "priority_queue : " << workloadNum << "번의 이동 푸시 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[priority_queue 팝 측정 시작]" << endl;
	cout << endl << "|------------------------------------------------|" << endl;

	timeBegin = clock.now();

	string retrievedData;
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

		//priority_queue는 pop 메소드가 top 아이템을 반환하지 않기 때문에, 동일한 기능을 수행하는 시간을 비교하기 위해 top()과 pop()을 연달아 호출하도록 함
		retrievedData = copyPushTestPriorityQueue.top().m_data;
		copyPushTestPriorityQueue.pop();
		retrievedData += 'a';		//컴파일, 링킹 최적화로 테스트 중의 검색 메소드 호출이 건너뛰어지는 경우가 없도록 하기 위한 추가 명령문임
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "priority_queue : " << workloadNum << "번의 팝 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "priority_queue : " << workloadNum << "번의 팝 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[priority_queue 소멸자 측정 시작]" << endl;
	return clock.now();
}