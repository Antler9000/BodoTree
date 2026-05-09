//로그 출력문들을 활성화하고 싶을 시 아래 구문의 주석을 해제할 것
//#define TREE_LOG	
//#define TREE_ERROR
//#define TREE_WARNING

#include "BstUsingWhile.h"		//정의한 Bst를 테스팅함
#include "iostream"				//콘솔 출력을 위해 사용함
#include <iomanip>				//소숫점 출력 지정을 위해 사용함
#include <chrono>;				//속도 테스트를 위해 사용함
#include <string>;				//..
#include <numeric>;				//..
#include <random>;				//..
#include <map>;					//..
using namespace chrono;			//..

template <typename DataType>
void PrintKeyAndData(int key, const DataType& retrievedData);

void RandomWorkloadSpeedTest(int workloadNum, int workloadPerDataLen);

void LinearIncreaseWorkloadSpeedTest(int workloadNum, int workloadPerDataLen);

void LinearDecreaseWorkloadSpeedTest(int workloadNum, int workloadPerDataLen);

//insertDataWorkload는 복사 비용이 크지만, 그럼에도 하나의 워크로드를 Bst와 map에 반복해서 사용할 수 있도록 값복사 형식의 매개변수를 사용함
time_point<steady_clock> SpeedTestBst(steady_clock& clock, int workloadNum, vector<string> insertDataWorkload, const vector<int>& insertKeyWorkload, const vector<int>& retrieveKeyWorkload, const vector<int>& removeKeyWorkload);

time_point<steady_clock> SpeedTestMap(steady_clock& clock, int workloadNum, vector<string> insertDataWorkload, const vector<int>& insertKeyWorkload, const vector<int>& retrieveKeyWorkload, const vector<int>& removeKeyWorkload);

int main()
{
	//디버깅 실행이 종료될 시점에도 해제되지 않은 동적 메모리 누수가 존재할 시, Visual Studio의 하단의 출력창(output)에 해당 누수에 대한 정보가 출력됨
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	cout << endl << "디버그 구성 테스트 1/3 : Bst<int>--------------------------------------------------------------------------" << endl;

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

	cout << endl << "디버그 구성 테스트 2/3 : Bst<float>--------------------------------------------------------------------------" << endl;

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

	cout << endl << "디버그 구성 테스트 3/3 : Bst<string>--------------------------------------------------------------------------" << endl;

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

	cout << endl << "릴리즈 구성 테스트 1/3 : 랜덤 워크로드 속도 테스트---------------------------------------------------------" << endl;

	/*	(테스팅 방법)
		randomWorkloadNum 횟수만큼 복사 삽입(트리 A), 이동 삽입(트리 B), 검색(트리 A), 삭제(트리 A)와 소멸(트리 B)을 수행함
		키는 [0,randomWorkloadNum-1] 의 중복되지 않는 키 값들을 랜덤하게 셔플해놓고 사용함
		데이터는 randomWorkloadPerDataLen 으로 지정된 길이의 string 객체를 randomWorkloadNum 개 만들어놓고 사용함
	*/

	/*	(테스팅 결과)
		[randomWorkloadNum = 10,000,000  |  randomWorkloadPerDataLen = 30]
		복사 삽입	: Bst = 16.4초	|	std::map = 14.5초
		이동 삽입	: Bst = 14.6초	|	std::map = 15.0초
		검색		: Bst = 14.3초	|	std::map = 15.8초
		삭제		: Bst = 20.3초	|	std::map = 19.3초
		소멸		: Bst = 4.2초	|	std::map = 4.3초
	*/

	/*	(테스팅 해석)
		Bst는 이전 단원과 달리 반복문으로 구현되었기에, 컴파일러의 최적화에 상관없이 재귀 호출은 일어나지 않았음
		다만 빌드된 어셈블리 코드를 보았을 때 삽입, 삭제 메소드에서 하위 작업 메소드 호출이 인라이닝되지 못하고 있는 것이 우선 원인으로 추정됨
	*/

	const int randomWorkloadNum = 10000000;
	const int randomWorkloadPerDataLen = 30;
	RandomWorkloadSpeedTest(randomWorkloadNum, randomWorkloadPerDataLen);

	cout << endl << "릴리즈 구성 테스트 2/3 : 선형 증가 워크로드 속도 테스트----------------------------------------------" << endl;

	/*	(테스팅 방법)
		앞선 테스트와 비슷하나,키값들을 뒤섞지 않고 선형 그대로 사용함
	*/

	/*	(테스팅 결과)
		[linearIncreaseWorkloadNum = 100,000  |  linearIncreaseWorkloadPerDataLen = 앞선 테스트와 동일]
		복사 삽입	: Bst = 18.6초	|	std::map = 0.01초
		이동 삽입	: Bst = 18.7초	|	std::map = 0.00초
		검색		: Bst = 18.6초	|	std::map = 0.00초
		삭제		: Bst = 0.00초	|	std::map = 0.00초
		소멸		: Bst = 0.00초	|	std::map = 0.00초
	*/

	/*	(테스팅 해석)
		트리 균형이 유지되는 std::map과 달리, 여기서 구현된 Bst는 균형을 유지하지 않으므로,
		테스트에 nlog(n)의 시간복잡도를 가지는 std::map과 달리, Bst는 n^2의 시간 복잡도가 걸리기에 큰 차이가 남
		다만 Bst의 삭제는 헤드에서 바로 삭제가 일어나기에 트리의 높이에 영향을 받지 않으므로 빠른 속도를 보임 
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
		복사 삽입	: Bst = 21.0초	|	std::map = 0.01초
		이동 삽입	: Bst = 18.3초	|	std::map = 0.00초
		검색		: Bst = 20.7초	|	std::map = 0.00초
		삭제		: Bst = 0.00초	|	std::map = 0.01초
		소멸		: Bst = 0.00초	|	std::map = 0.00초
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
	timeBegin = SpeedTestBst(clock, workloadNum, insertTestDatum, insertTestKeys, retrieveTestKeys, removeTestKeys);

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "Bst : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

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
	timeBegin = SpeedTestBst(clock, workloadNum, insertTestDatum, insertTestKeys, retrieveTestKeys, removeTestKeys);

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "Bst : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

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
	timeBegin = SpeedTestBst(clock, workloadNum, insertTestDatum, insertTestKeys, retrieveTestKeys, removeTestKeys);

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "Bst : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "-----------------------------------------------------------" << endl;

	cout << endl << "선형 감소 워크로드 복사 중...." << endl;
	timeBegin = SpeedTestMap(clock, workloadNum, insertTestDatum, insertTestKeys, retrieveTestKeys, removeTestKeys);

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "map : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
}

time_point<steady_clock> SpeedTestBst(steady_clock& clock, int workloadNum, vector<string> insertDataWorkload, const vector<int>& insertKeyWorkload, const vector<int>& retrieveKeyWorkload, const vector<int>& removeKeyWorkload)
{
	Bst<string> copyInsertTestBst;
	Bst<string> moveInsertTestBst;

	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "[Bst 복사 삽입 측정 시작]" << endl;
	cout << endl << "|------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < workloadNum; i++)
	{
		if (i % ((workloadNum / 20) + 1) == 0) cout << "*";

		copyInsertTestBst.Insert(insertKeyWorkload[i], insertDataWorkload[i]);
	}
	cout << endl;

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "Bst : " << workloadNum << "번의 복사 삽입 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "[Bst 이동 삽입 측정 시작]" << endl;
	cout << endl << "|------------------|" << endl;

	timeBegin = clock.now();
	
	for (int i = 0; i < workloadNum; i++)
	{
		if (i % ((workloadNum / 20) + 1) == 0) cout << "*";

		moveInsertTestBst.Insert(insertKeyWorkload[i], move(insertDataWorkload[i]));
	}
	cout << endl;

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "Bst : " << workloadNum << "번의 이동 삽입 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	string retrievedData;

	cout << endl << "[Bst 검색 측정 시작]" << endl;
	cout << endl << "|------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < workloadNum; i++)
	{
		if (i % ((workloadNum / 20) + 1) == 0) cout << "*";

		copyInsertTestBst.Retrieve(insertKeyWorkload[i], retrievedData);
		retrievedData += 'a';			//컴파일, 링킹 최적화로 테스트 중의 검색 메소드 호출이 건너뛰어지는 경우가 없도록 하기 위한 추가 명령문임
	}
	cout << endl;

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "Bst : " << workloadNum << "번의 검색 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "[Bst 삭제 측정 시작]" << endl;
	cout << endl << "|------------------|" << endl;

	timeBegin = clock.now();

	for (int i = 0; i < workloadNum; i++)
	{
		if (i % ((workloadNum / 20) + 1) == 0) cout << "*";

		copyInsertTestBst.Remove(removeKeyWorkload[i]);
	}
	cout << endl;

	timeEnd = clock.now();

	timeDiff = timeEnd - timeBegin;

	cout << endl << "Bst : " << workloadNum << "번의 삭제 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "[Bst 소멸자 측정 시작]" << endl;
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