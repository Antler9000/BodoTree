//로그 출력문들을 활성화하고 싶을 시 아래 구문의 주석을 해제할 것
//#define TREE_LOG	
//#define TREE_WARNING
//#define TREE_ERROR

#include "SplayTree.h"			//직접 정의한 SplayTree
#include "iostream"				//std::cout, std::endl
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

void LinearIncreaseWorkloadSpeedTest(int workloadNum, int workloadPerDataLen);

void LinearDecreaseWorkloadSpeedTest(int workloadNum, int workloadPerDataLen);

//insertDataWorkload는 복사 비용이 크지만, 그럼에도 하나의 워크로드를 SplayTree와 map에 반복해서 사용할 수 있도록 값복사 형식의 매개변수를 사용함
time_point<steady_clock> SpeedTestSplayTree(steady_clock& clock, int workloadNum, vector<string> insertDataWorkload, const vector<int>& insertKeyWorkload, const vector<int>& retrieveKeyWorkload, const vector<int>& removeKeyWorkload);

time_point<steady_clock> SpeedTestMap(steady_clock& clock, int workloadNum, vector<string> insertDataWorkload, const vector<int>& insertKeyWorkload, const vector<int>& retrieveKeyWorkload, const vector<int>& removeKeyWorkload);

int main()
{
	//디버깅 실행이 종료될 시점에도 해제되지 않은 동적 메모리 누수가 존재할 시, Visual Studio의 하단의 출력창(output)에 해당 누수에 대한 정보가 출력됨
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	cout << endl << "디버그 구성 테스트 1/3 : SplayTree<int>--------------------------------------------------------------------------" << endl;

	SplayTree<int> intTestSplayTree;

	cout << endl << "삽입 (트리 A)" << endl;
	intTestSplayTree.Insert(5, 55);
	intTestSplayTree.Insert(7, 77);
	intTestSplayTree.Insert(3, 33);
	intTestSplayTree.Insert(4, 44);
	intTestSplayTree.Insert(6, 66);
	intTestSplayTree.Insert(9, 99);
	intTestSplayTree.Insert(2, 22);
	intTestSplayTree.PreorderPrint();

	cout << endl << "중위 순회 출력 (트리 A)" << endl;
	intTestSplayTree.InorderPrint();

	cout << endl << "후위 순회 출력 (트리 A)" << endl;
	intTestSplayTree.PostorderPrint();

	cout << endl << "검색 (트리 A)" << endl;
	int intRetrievedData = 0;
	intTestSplayTree.Retrieve(3, intRetrievedData);
	PrintKeyAndData(3, intRetrievedData);

	cout << endl << "명시적 복사 (트리 A -> B)" << endl;
	SplayTree<int> intExplicitCopyTestSplayTree;
	intExplicitCopyTestSplayTree.CopyTree(intTestSplayTree);
	intExplicitCopyTestSplayTree.PreorderPrint();

	cout << endl << "복사 생성자 (트리 A -> C)" << endl;
	SplayTree<int> intCopyConstructorTestSplayTree = intTestSplayTree;
	intCopyConstructorTestSplayTree.PreorderPrint();

	cout << endl << "복사 할당 연산자 (트리 A -> D)" << endl;
	SplayTree<int> intCopyAssignmentTestSplayTree;
	intCopyAssignmentTestSplayTree = intTestSplayTree;
	intCopyAssignmentTestSplayTree.PreorderPrint();

	cout << endl << "이동 생성자 (트리 C -> E)" << endl;
	SplayTree<int> intMoveConstructorTestSplayTree = move(intCopyConstructorTestSplayTree);
	intMoveConstructorTestSplayTree.PreorderPrint();

	cout << endl << "이동 할당 연산자 (트리 D -> F)" << endl;
	SplayTree<int> intMoveAssignmentTestSplayTree;
	intMoveAssignmentTestSplayTree = move(intCopyAssignmentTestSplayTree);
	intMoveAssignmentTestSplayTree.PreorderPrint();

	cout << endl << "이동 후 소스 트리는 비워짐 (트리 C, D)" << endl;
	intCopyConstructorTestSplayTree.PreorderPrint();
	intCopyAssignmentTestSplayTree.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	intTestSplayTree.Remove(7);
	intTestSplayTree.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	intTestSplayTree.Remove(3);
	intTestSplayTree.PreorderPrint();

	cout << endl << "전체 삭제 (트리 A)" << endl;
	intTestSplayTree.RemoveTree();
	intTestSplayTree.PreorderPrint();

	cout << endl << "복사한 트리는 원본과 독립적임 (트리 B)" << endl;
	intExplicitCopyTestSplayTree.PreorderPrint();

	cout << endl << "디버그 구성 테스트 2/3 : SplayTree<float>--------------------------------------------------------------------------" << endl;

	SplayTree<float> floatTestSplayTree;

	cout << endl << "삽입 (트리 A)" << endl;
	floatTestSplayTree.Insert(5, 0.55f);
	floatTestSplayTree.Insert(7, 0.77f);
	floatTestSplayTree.Insert(3, 0.33f);
	floatTestSplayTree.Insert(4, 0.44f);
	floatTestSplayTree.Insert(6, 0.66f);
	floatTestSplayTree.Insert(9, 0.99f);
	floatTestSplayTree.Insert(2, 0.22f);
	floatTestSplayTree.PreorderPrint();

	cout << endl << "중위 순회 출력 (트리 A)" << endl;
	floatTestSplayTree.InorderPrint();

	cout << endl << "후위 순회 출력 (트리 A)" << endl;
	floatTestSplayTree.PostorderPrint();

	cout << endl << "검색 (트리 A)" << endl;
	float floatRetrievedData = 0.0f;
	floatTestSplayTree.Retrieve(3, floatRetrievedData);
	PrintKeyAndData(3, floatRetrievedData);

	cout << endl << "명시적 복사 (트리 A -> B)" << endl;
	SplayTree<float> floatExplicitCopyTestSplayTree;
	floatExplicitCopyTestSplayTree.CopyTree(floatTestSplayTree);
	floatExplicitCopyTestSplayTree.PreorderPrint();

	cout << endl << "복사 생성자 (트리 A -> C)" << endl;
	SplayTree<float> floatCopyConstructorTestSplayTree = floatTestSplayTree;
	floatCopyConstructorTestSplayTree.PreorderPrint();

	cout << endl << "복사 할당 연산자 (트리 A -> D)" << endl;
	SplayTree<float> floatCopyAssignmentTestSplayTree;
	floatCopyAssignmentTestSplayTree = floatTestSplayTree;
	floatCopyAssignmentTestSplayTree.PreorderPrint();

	cout << endl << "이동 생성자 (트리 C -> E)" << endl;
	SplayTree<float> floatMoveConstructorTestSplayTree = move(floatCopyConstructorTestSplayTree);
	floatMoveConstructorTestSplayTree.PreorderPrint();

	cout << endl << "이동 할당 연산자 (트리 D -> F)" << endl;
	SplayTree<float> floatMoveAssignmentTestSplayTree;
	floatMoveAssignmentTestSplayTree = move(floatCopyAssignmentTestSplayTree);
	floatMoveAssignmentTestSplayTree.PreorderPrint();

	cout << endl << "이동 후 소스 트리는 비워짐 (트리 C, D)" << endl;
	floatCopyConstructorTestSplayTree.PreorderPrint();
	floatCopyAssignmentTestSplayTree.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	floatTestSplayTree.Remove(7);
	floatTestSplayTree.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	floatTestSplayTree.Remove(3);
	floatTestSplayTree.PreorderPrint();

	cout << endl << "전체 삭제 (트리 A)" << endl;
	floatTestSplayTree.RemoveTree();
	floatTestSplayTree.PreorderPrint();

	cout << endl << "복사한 트리는 원본과 독립적임 (트리 B)" << endl;
	floatExplicitCopyTestSplayTree.PreorderPrint();

	cout << endl << "디버그 구성 테스트 3/3 : SplayTree<string>--------------------------------------------------------------------------" << endl;

	SplayTree<string> stringTestSplayTree;

	cout << endl << "삽입 (트리 A)" << endl;
	stringTestSplayTree.Insert(5, "Panther");
	stringTestSplayTree.Insert(7, "Comet");
	stringTestSplayTree.Insert(3, "Crusader");
	stringTestSplayTree.Insert(4, "Sherman");
	stringTestSplayTree.Insert(6, "Tiger");
	stringTestSplayTree.Insert(9, "Mouse");
	stringTestSplayTree.Insert(2, "Stuart");
	stringTestSplayTree.PreorderPrint();

	cout << endl << "중위 순회 출력 (트리 A)" << endl;
	stringTestSplayTree.InorderPrint();

	cout << endl << "후위 순회 출력 (트리 A)" << endl;
	stringTestSplayTree.PostorderPrint();

	cout << endl << "검색 (트리 A)" << endl;
	string stringRetrievedData = "";
	stringTestSplayTree.Retrieve(3, stringRetrievedData);
	PrintKeyAndData(3, stringRetrievedData);

	cout << endl << "명시적 복사 (트리 A -> B)" << endl;
	SplayTree<string> stringExplicitCopyTestSplayTree;
	stringExplicitCopyTestSplayTree.CopyTree(stringTestSplayTree);
	stringExplicitCopyTestSplayTree.PreorderPrint();

	cout << endl << "복사 생성자 (트리 A -> C)" << endl;
	SplayTree<string> stringCopyConstructorTestSplayTree = stringTestSplayTree;
	stringCopyConstructorTestSplayTree.PreorderPrint();

	cout << endl << "복사 할당 연산자 (트리 A -> D)" << endl;
	SplayTree<string> stringCopyAssignmentTestSplayTree;
	stringCopyAssignmentTestSplayTree = stringTestSplayTree;
	stringCopyAssignmentTestSplayTree.PreorderPrint();

	cout << endl << "이동 생성자 (트리 C -> E)" << endl;
	SplayTree<string> stringMoveConstructorTestSplayTree = move(stringCopyConstructorTestSplayTree);
	stringMoveConstructorTestSplayTree.PreorderPrint();

	cout << endl << "이동 할당 연산자 (트리 D -> F)" << endl;
	SplayTree<string> stringMoveAssignmentTestSplayTree;
	stringMoveAssignmentTestSplayTree = move(stringCopyAssignmentTestSplayTree);
	stringMoveAssignmentTestSplayTree.PreorderPrint();

	cout << endl << "이동 후 소스 트리는 비워짐 (트리 C, D)" << endl;
	stringCopyConstructorTestSplayTree.PreorderPrint();
	stringCopyAssignmentTestSplayTree.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	stringTestSplayTree.Remove(7);
	stringTestSplayTree.PreorderPrint();

	cout << endl << "요소별 삭제 (트리 A)" << endl;
	stringTestSplayTree.Remove(3);
	stringTestSplayTree.PreorderPrint();

	cout << endl << "전체 삭제 (트리 A)" << endl;
	stringTestSplayTree.RemoveTree();
	stringTestSplayTree.PreorderPrint();

	cout << endl << "복사한 트리는 원본과 독립적임 (트리 B)" << endl;
	stringExplicitCopyTestSplayTree.PreorderPrint();
#endif

#ifndef _DEBUG
	cout << fixed << setprecision(2);

	cout << endl << "릴리즈 구성 테스트 1/3 : 랜덤 워크로드 속도 테스트---------------------------------------------------------" << endl;

	/*	(테스팅 방법)
		randomWorkloadNum 횟수만큼 복사 삽입(트리 A), 이동 삽입(트리 B), 검색(트리 A), 삭제(트리 A)와 소멸(트리 B)을 수행함
		키는 [0,randomWorkloadNum-1] 의 중복되지 않는 키 값들을 랜덤하게 셔플해놓고 사용함
		데이터는 randomWorkloadPerDataLen 으로 지정된 길이의 string 객체를 randomWorkloadNum 개 만들어놓고 사용함
	*/

	/*	(테스팅 결과)
		[randomWorkloadNum = 1,000,000  |  randomWorkloadPerDataLen = 30]
		복사 삽입	: SplayTree = 0.80초	|	std::map = 0.79초
		이동 삽입	: SplayTree = 0.74초	|	std::map = 0.72초
		검색		: SplayTree = 67.1초	|	std::map = 0.94초
		삭제		: SplayTree = 88.2초	|	std::map = 1.02초
		소멸		: SplayTree = 0.29초	|	std::map = 0.27초
	*/

	/*	(테스팅 해석)
		SplayTree는 이전 단원과 달리 반복문으로 구현되었기에, 컴파일러의 최적화에 상관없이 재귀 호출은 일어나지 않았음
		다만 빌드된 어셈블리 코드를 보았을 때 삽입, 삭제 메소드에서 하위 작업 메소드 호출이 인라이닝되지 못하고 있는 것이 우선 원인으로 추정됨
	*/

	const int randomWorkloadNum = 1000000;
	const int randomWorkloadPerDataLen = 30;
	RandomWorkloadSpeedTest(randomWorkloadNum, randomWorkloadPerDataLen);

	cout << endl << "릴리즈 구성 테스트 2/3 : 선형 증가 워크로드 속도 테스트----------------------------------------------" << endl;

	/*	(테스팅 방법)
		앞선 테스트와 비슷하나,키값들을 뒤섞지 않고 선형 그대로 사용함
	*/

	/*	(테스팅 결과)
		[linearIncreaseWorkloadNum = 100,000  |  linearIncreaseWorkloadPerDataLen = 앞선 테스트와 동일]
		복사 삽입	: SplayTree = 18.7초	|	std::map = 0.02초
		이동 삽입	: SplayTree = 19.1초	|	std::map = 0.01초
		검색		: SplayTree = 0.01초	|	std::map = 0.01초
		삭제		: SplayTree = 15.0초	|	std::map = 0.01초
		소멸		: SplayTree = 0.01초	|	std::map = 0.01초
	*/

	/*	(테스팅 해석)
		트리 균형이 유지되는 std::map과 달리, 여기서 구현된 SplayTree는 균형을 유지하지 않으므로,
		테스트에 nlog(n)의 시간복잡도를 가지는 std::map과 달리, SplayTree는 n^2의 시간 복잡도가 걸리기에 큰 차이가 남
		다만 SplayTree의 삭제는 헤드에서 바로 삭제가 일어나기에 트리의 높이에 영향을 받지 않으므로 빠른 속도를 보임
	*/

	const int linearIncreaseWorkloadNum = 100000;
	const int linearIncreaseWorkloadPerDataLen = randomWorkloadPerDataLen;
	LinearIncreaseWorkloadSpeedTest(linearIncreaseWorkloadNum, linearIncreaseWorkloadPerDataLen);

	cout << endl << "릴리즈 구성 테스트 3/3 : 선형 감소 워크로드 속도 테스트----------------------------------------------" << endl;

	/*	(테스팅 방법)
		앞선 테스트와 동일하나 키를 역순으로 사용함
	*/

	/*	(테스팅 결과)
		[linearDecreaseWorkloadNum = 앞선 테스트와 동일  |  linearDecreaseWorkloadPerDataLen = 앞선 테스트와 동일]
		복사 삽입	: SplayTree = 19.2초	|	std::map = 0.02초
		이동 삽입	: SplayTree = 18.1초	|	std::map = 0.01초
		검색		: SplayTree = 0.01초	|	std::map = 0.01초
		삭제		: SplayTree = 14.4초	|	std::map = 0.01초
		소멸		: SplayTree = 0.01초	|	std::map = 0.01초
	*/

	/*	(테스팅 해석)
		앞선 테스트와 동일
	*/

	const int linearDecreaseWorkloadNum = linearIncreaseWorkloadNum;
	const int linearDecreaseWorkloadPerDataLen = linearIncreaseWorkloadPerDataLen;
	LinearDecreaseWorkloadSpeedTest(linearDecreaseWorkloadNum, linearDecreaseWorkloadPerDataLen);
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

	vector<string> insertTestDatum;
	insertTestDatum.reserve(workloadNum);
	for (int i = 0; i < workloadNum; i++)
	{
		insertTestDatum.emplace_back(string(workloadPerDataLen, 'A'));
	}

	vector<int> insertTestKeys(workloadNum);
	iota(insertTestKeys.begin(), insertTestKeys.end(), 0);
	mt19937 insertTestRng(123456);
	shuffle(insertTestKeys.begin(), insertTestKeys.end(), insertTestRng);

	vector<int> retrieveTestKeys(workloadNum);
	iota(retrieveTestKeys.begin(), retrieveTestKeys.end(), 0);
	mt19937 retrieveTestRng(654321);
	shuffle(retrieveTestKeys.begin(), retrieveTestKeys.end(), retrieveTestRng);

	vector<int> removeTestKeys(workloadNum);
	iota(removeTestKeys.begin(), removeTestKeys.end(), 0);
	mt19937 removeTestRng(162534);
	shuffle(removeTestKeys.begin(), removeTestKeys.end(), removeTestRng);

	steady_clock clock;
	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "랜덤 워크로드 복사 중...." << endl;
	timeBegin = SpeedTestSplayTree(clock, workloadNum, insertTestDatum, insertTestKeys, retrieveTestKeys, removeTestKeys);

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "SplayTree : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "-----------------------------------------------------------" << endl;

	cout << endl << "랜덤 워크로드 복사 중...." << endl;
	timeBegin = SpeedTestMap(clock, workloadNum, insertTestDatum, insertTestKeys, retrieveTestKeys, removeTestKeys);

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "map : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
}

void LinearIncreaseWorkloadSpeedTest(int workloadNum, int workloadPerDataLen)
{
	cout << endl << "선형 증가 워크로드 준비 중...." << endl;

	vector<string> insertTestDatum;
	insertTestDatum.reserve(workloadNum);
	for (int i = 0; i < workloadNum; i++)
	{
		insertTestDatum.emplace_back(string(workloadPerDataLen, 'A'));
	}

	vector<int> insertTestKeys(workloadNum);
	iota(insertTestKeys.begin(), insertTestKeys.end(), 0);

	vector<int> retrieveTestKeys = insertTestKeys;

	vector<int> removeTestKeys = insertTestKeys;

	steady_clock clock;
	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "선형 증가 워크로드 복사 중...." << endl;
	timeBegin = SpeedTestSplayTree(clock, workloadNum, insertTestDatum, insertTestKeys, retrieveTestKeys, removeTestKeys);

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "SplayTree : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "-----------------------------------------------------------" << endl;

	cout << endl << "선형 증가 워크로드 복사 중...." << endl;
	timeBegin = SpeedTestMap(clock, workloadNum, insertTestDatum, insertTestKeys, retrieveTestKeys, removeTestKeys);

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "map : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
}

void LinearDecreaseWorkloadSpeedTest(int workloadNum, int workloadPerDataLen)
{
	cout << endl << "선형 감소 워크로드 준비 중...." << endl;

	vector<string> insertTestDatum;
	insertTestDatum.reserve(workloadNum);
	for (int i = 0; i < workloadNum; i++)
	{
		insertTestDatum.emplace_back(string(workloadPerDataLen, 'A'));
	}

	vector<int> insertTestKeys(workloadNum);
	iota(insertTestKeys.rbegin(), insertTestKeys.rend(), 0);

	vector<int> retrieveTestKeys = insertTestKeys;

	vector<int> removeTestKeys = insertTestKeys;

	steady_clock clock;
	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "선형 감소 워크로드 복사 중...." << endl;
	timeBegin = SpeedTestSplayTree(clock, workloadNum, insertTestDatum, insertTestKeys, retrieveTestKeys, removeTestKeys);

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "SplayTree : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "-----------------------------------------------------------" << endl;

	cout << endl << "선형 감소 워크로드 복사 중...." << endl;
	timeBegin = SpeedTestMap(clock, workloadNum, insertTestDatum, insertTestKeys, retrieveTestKeys, removeTestKeys);

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "map : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
}

time_point<steady_clock> SpeedTestSplayTree(steady_clock& clock, int workloadNum, vector<string> insertDataWorkload, const vector<int>& insertKeyWorkload, const vector<int>& retrieveKeyWorkload, const vector<int>& removeKeyWorkload)
{
	SplayTree<string> copyInsertTestSplayTree;
	SplayTree<string> moveInsertTestSplayTree;

	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "[SplayTree 복사 삽입 측정 시작]" << endl;
	cout << endl << "|------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < workloadNum; i++)
	{
		if (i % ((workloadNum / 20) + 1) == 0) cout << "*";

		copyInsertTestSplayTree.Insert(insertKeyWorkload[i], insertDataWorkload[i]);
	}
	cout << endl;

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "SplayTree : " << workloadNum << "번의 복사 삽입 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "[SplayTree 이동 삽입 측정 시작]" << endl;
	cout << endl << "|------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < workloadNum; i++)
	{
		if (i % ((workloadNum / 20) + 1) == 0) cout << "*";

		moveInsertTestSplayTree.Insert(insertKeyWorkload[i], move(insertDataWorkload[i]));
	}
	cout << endl;

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "SplayTree : " << workloadNum << "번의 이동 삽입 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	string retrievedData;

	cout << endl << "[SplayTree 검색 측정 시작]" << endl;
	cout << endl << "|------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < workloadNum; i++)
	{
		if (i % ((workloadNum / 20) + 1) == 0) cout << "*";

		copyInsertTestSplayTree.Retrieve(insertKeyWorkload[i], retrievedData);
		retrievedData += 'a';			//컴파일, 링킹 최적화로 테스트 중의 검색 메소드 호출이 건너뛰어지는 경우가 없도록 하기 위한 추가 명령문임
	}
	cout << endl;

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "SplayTree : " << workloadNum << "번의 검색 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "[SplayTree 삭제 측정 시작]" << endl;
	cout << endl << "|------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < workloadNum; i++)
	{
		if (i % ((workloadNum / 20) + 1) == 0) cout << "*";

		copyInsertTestSplayTree.Remove(removeKeyWorkload[i]);
	}
	cout << endl;

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "SplayTree : " << workloadNum << "번의 삭제 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "[SplayTree 소멸자 측정 시작]" << endl;
	return clock.now();
}

time_point<steady_clock> SpeedTestMap(steady_clock& clock, int speedTestRepeat, vector<string> insertTestDatum, const vector<int>& insertTestKeys, const vector<int>& retrieveTestKeys, const vector<int>& removeTestKeys)
{
	map<int, string> copyInsertTestMap;
	map<int, string> moveInsertTestMap;

	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "[map 복사 삽입 측정 시작]" << endl;
	cout << endl << "|------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < speedTestRepeat; i++)
	{
		if (i % ((speedTestRepeat / 20) + 1) == 0) cout << "*";

		copyInsertTestMap.emplace(insertTestKeys[i], insertTestDatum[i]);
	}
	cout << endl;

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "map : " << speedTestRepeat << "번의 삽입 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "[map 이동 삽입 측정 시작]" << endl;
	cout << endl << "|------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < speedTestRepeat; i++)
	{
		if (i % ((speedTestRepeat / 20) + 1) == 0) cout << "*";

		moveInsertTestMap.emplace(insertTestKeys[i], move(insertTestDatum[i]));
	}
	cout << endl;

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "map : " << speedTestRepeat << "번의 이동 삽입 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "[map 검색 측정 시작]" << endl;
	cout << endl << "|------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < speedTestRepeat; i++)
	{
		if (i % ((speedTestRepeat / 20) + 1) == 0) cout << "*";

		auto iterator = copyInsertTestMap.find(retrieveTestKeys[i]);
		iterator->second += 'a';			//컴파일, 링킹 최적화로 테스트 중의 검색 메소드 호출이 건너뛰어지는 경우가 없도록 하기 위한 추가 명령문임
	}
	cout << endl;

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "map : " << speedTestRepeat << "번의 검색 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "[map 삭제 측정 시작]" << endl;
	cout << endl << "|------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < speedTestRepeat; i++)
	{
		if (i % ((speedTestRepeat / 20) + 1) == 0) cout << "*";

		copyInsertTestMap.erase(insertTestKeys[i]);
	}
	cout << endl;

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "map : " << speedTestRepeat << "번의 삭제 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "[map 소멸자 측정 시작]" << endl;
	return clock.now();
}