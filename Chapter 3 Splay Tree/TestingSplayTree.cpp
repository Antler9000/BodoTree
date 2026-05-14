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

void RandomWorkloadTest(int workloadNum, int workloadPerDataLen);

void RandomLocalWorkloadTest(int workloadNum, int workloadPerDataLen, int localBlockSize);

void LinearIncreaseWorkloadTest(int workloadNum, int workloadPerDataLen);

void LinearDecreaseWorkloadTest(int workloadNum, int workloadPerDataLen);

//insertDataWorkload는 복사 비용이 크지만, 그럼에도 하나의 워크로드를 SplayTree와 map에 반복해서 사용할 수 있도록 값복사 형식의 매개변수를 사용함
time_point<steady_clock> TestSplayTree(steady_clock& clock, int workloadNum, vector<string> insertDataWorkload, const vector<int>& insertKeyWorkload, const vector<int>& retrieveKeyWorkload, const vector<int>& removeKeyWorkload);

time_point<steady_clock> TestMap(steady_clock& clock, int workloadNum, vector<string> insertDataWorkload, const vector<int>& insertKeyWorkload, const vector<int>& retrieveKeyWorkload, const vector<int>& removeKeyWorkload);

int main()
{
	//디버깅 실행이 종료될 시점에도 해제되지 않은 동적 메모리 누수가 존재할 시, Visual Studio의 하단의 출력창(output)에 해당 누수에 대한 정보가 출력됨
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	cout << endl << "디버그 구성 테스트 1/3 : SplayTree<int>-------------------------------------------------------------" << endl;

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

	cout << endl << "디버그 구성 테스트 2/3 : SplayTree<float>-----------------------------------------------------------" << endl;

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

	cout << endl << "디버그 구성 테스트 3/3 : SplayTree<string>----------------------------------------------------------" << endl;

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

	cout << endl << "릴리즈 구성 테스트 1/4 : 랜덤 워크로드 테스트-------------------------------------------------------" << endl;

	/*	(테스팅 방법)
		randomWorkloadNum 횟수만큼 복사 삽입(트리 A), 이동 삽입(트리 B), 검색(트리 A), 삭제(트리 A)와 소멸(트리 B)을 수행함
		키는 [0,randomWorkloadNum-1] 의 중복되지 않는 키 값들을 랜덤하게 셔플해놓고 사용함(삽입, 검색, 삭제의 키 값들은 각각 독립으로 셔플됨)
		데이터는 randomWorkloadPerDataLen 으로 지정된 길이의 string 객체를 randomWorkloadNum 개 만들어놓고 사용함
	*/

	/*	(테스팅 결과)
		[randomWorkloadNum			= 10,000,000]
		[randomWorkloadPerDataLen	= 30]
		복사 삽입	: SplayTree = 18.20초	|	std::map = 16.47초
		이동 삽입	: SplayTree = 17.20초	|	std::map = 16.06초
		검색		: SplayTree = 시간초과	|	std::map = 17.01초
		삭제		: SplayTree = 시간초과	|	std::map = 21.99초
		소멸		: SplayTree =  9.42초	|	std::map =  5.03초
	*/

	/*	(테스팅 해석)
		현재 구현에서는 splay가 검색 메소드에서 1~2 높이로만 적용되고 있음

		조정이 일어나지 않는 삭제 메소드도 느린 것으로 보아,
		앞선 검색 메소드 수행 과정이 트리를 편향시켜서 속도를 느리게 하는 것으로 추정함

		따라서, 우선은 편향 문제가 해결하기 위해서 검색 과정에 splay를 보다 온전히 적용해보고자 함
		그리고 만일 편향 문제가 해결되어 삭제 메소드의 속도는 개선되었지만 여전히 검색 메소드가 느리다면, stack 삽입 과정을 최적화해볼 예정
		그리고 편향이 해결되지 않더라도, 삭제에도 splay가 적용되도록 하여 이를 극복할 수 있는지 확인할 예정
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

		복사 삽입	: SplayTree =  7.88초	|	std::map =  4.02초
		이동 삽입	: SplayTree =  7.34초	|	std::map =  3.35초
		검색		: SplayTree = 시간초과	|	std::map =  3.65초
		삭제		: SplayTree = 시간초과	|	std::map =  4.21초
		소멸		: SplayTree =  6.81초	|	std::map =  3.18초
	*/

	/*	(테스팅 해석)
		우선 워크로드가 지역 선형성을 띄면서 트리의 높이가 1번 랜덤 워크로드 테스트에 비해 높아질 것임에도 불구하고,
		Bst와 std::map 둘 다 1번 랜덤 워크로드 테스트에서보다 더 빠른 속도를 보이는 이유는 지역성을 통한 캐시 히트율 상승 때문으로 추정함

		삽입에서의 속도보다 검색의 속도가 느리다는 점, 그리고 검색 이후 삭제 메소드에서도 속도가 느려졌다는 점 등을 보았을 때,
		앞선 1번 랜덤 워크로드 테스트와 마찬가지로 검색 메소드 중 수행하는 splay 조정이 트리를 편향시키는 것으로 추정함

		그리고 그러한 트리의 모양이 3번 선형 증가 워크로드에서 검색 이후 트리의 상태처럼 좌하향 편향 트리에 비슷한 모양이 되기에
		3번 선형 증가 워크로드에서의 삭제 메소드의 속도와 비슷한 속도로 현 테스트에서 검색과 삭제가 이뤄지는 것으로 추정함
		
		다만 3번 선형 증가 워크로드에 테스트와 달리 검색에서도 느린 속도가 나타나는 이유는
		삽입과 검색의 키가 동일한 3번 테스트와 달리 현 테스트는 삽입과 검색 메소드가 각각 셔플되기에 검색 대상 노드가 루트에 미리 대기하지 못하기 때문으로 추정함
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

		복사 삽입	: SplayTree = 시간초과	|	std::map =  1.98초
		이동 삽입	: SplayTree = 시간초과	|	std::map =  1.46초
		검색		: SplayTree =  0.28초	|	std::map =  0.72초
		삭제		: SplayTree = 40.67초	|	std::map =  1.78초
		소멸		: SplayTree =  0.40초	|	std::map =  1.30초
	*/

	/*	(테스팅 해석)
		검색에서는 splay로 인해 다음 검색 대상이 항상 루트의 자식 노드로 오게 되므로 매우 빠른 속도를 보임
		반대로 삭제는 앞선 검색으로 인해 트리가 조정되어 삭제할 노드가 루트 노드에 위치하지 않게 되므로 검색보다 느린 것으로 추정함
		삭제 메소드에도 검색처럼 splay 조정을 수행하도록 할 시 속도가 개선될 거라 생각함
		삽입이 느린 이유는 균형 트리가 아닌 이진 탐색 트리의 단점을 그대로 가진 것이나, 삽입에도 splay 조정이 수행되도록 하여 이것이 개선되는지 확인해볼 예정
	
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

		복사 삽입	: SplayTree = 시간초과	|	std::map =  2.11초
		이동 삽입	: SplayTree = 시간초과	|	std::map =  1.74초
		검색		: SplayTree =  0.31초	|	std::map =  0.74초
		삭제		: SplayTree = 시간초과	|	std::map =  1.82초
		소멸		: SplayTree =  0.42초	|	std::map =  2.46초
	*/

	/*	(테스팅 해석)
		앞선 3번 선형 증가 워크로드 테스트와 동일
	*/

	const int linearDecreaseWorkloadNum = randomWorkloadNum;
	const int linearDecreaseWorkloadPerDataLen = randomWorkloadPerDataLen;
	LinearDecreaseWorkloadTest(linearDecreaseWorkloadNum, linearDecreaseWorkloadPerDataLen);
#endif

	cout << endl << "테스트 종료-----------------------------------------------------------------------------------------" << endl;

	return 0;
}

template <typename DataType>
void PrintKeyAndData(int key, const DataType& retrievedData)
{
	cout << "검색한 키 : " << key << " / 검색된 데이터 : " << retrievedData << endl;
}

void RandomWorkloadTest(int workloadNum, int workloadPerDataLen)
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
	timeBegin = TestSplayTree(clock, workloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "SplayTree : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

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

	vector<int> insertTestKeyBlocks(workloadNum/localBlockSize);
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
	timeBegin = TestSplayTree(clock, realWorkloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "SplayTree : " << realWorkloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

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
	timeBegin = TestSplayTree(clock, workloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "SplayTree : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

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
	timeBegin = TestSplayTree(clock, workloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "SplayTree : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << "---------------------------------------------------------------------------" << endl;

	cout << endl << "선형 감소 워크로드 복사 중...." << endl;
	timeBegin = TestMap(clock, workloadNum, insertDataWorkload, insertKeyWorkload, retrieveKeyWorkload, removeKeyWorkload);

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	cout << endl << "map : " << workloadNum << "번의 소멸자 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;

	cout << endl << endl << endl << endl << endl;
}

time_point<steady_clock> TestSplayTree(steady_clock& clock, int workloadNum, vector<string> insertDataWorkload, const vector<int>& insertKeyWorkload, const vector<int>& retrieveKeyWorkload, const vector<int>& removeKeyWorkload)
{
	SplayTree<string> copyInsertTestSplayTree;
	SplayTree<string> moveInsertTestSplayTree;

	time_point<steady_clock> timeBegin;
	time_point<steady_clock> timeEnd;
	duration<double> timeDiff;

	cout << endl << "[SplayTree 복사 삽입 측정 시작]" << endl;
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

		copyInsertTestSplayTree.Insert(insertKeyWorkload[i], insertDataWorkload[i]);
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "SplayTree : " << workloadNum << "번의 복사 삽입 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "SplayTree : " << workloadNum << "번의 복사 삽입 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[SplayTree 이동 삽입 측정 시작]" << endl;
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

		moveInsertTestSplayTree.Insert(insertKeyWorkload[i], move(insertDataWorkload[i]));
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "SplayTree : " << workloadNum << "번의 이동 삽입 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "SplayTree : " << workloadNum << "번의 이동 삽입 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	string retrievedData;

	cout << endl << "[SplayTree 검색 측정 시작]" << endl;
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

		copyInsertTestSplayTree.Retrieve(insertKeyWorkload[i], retrievedData);
		retrievedData += 'a';			//컴파일, 링킹 최적화로 테스트 중의 검색 메소드 호출이 건너뛰어지는 경우가 없도록 하기 위한 추가 명령문임
	}
	cout << endl;

	if (timeDiff.count() < 50)
	{
		cout << endl << "SplayTree : " << workloadNum << "번의 검색 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "SplayTree : " << workloadNum << "번의 검색 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[SplayTree 삭제 측정 시작]" << endl;
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

		copyInsertTestSplayTree.Remove(removeKeyWorkload[i]);
	}
	cout << endl;

	timeEnd = clock.now();
	timeDiff = timeEnd - timeBegin;

	if (timeDiff.count() < 50)
	{
		cout << endl << "SplayTree : " << workloadNum << "번의 삭제 동안 흐른 시간은 : " << timeDiff.count() << " 초" << endl;
	}
	else
	{
		cout << endl << "SplayTree : " << workloadNum << "번의 삭제 동안 흐른 시간은 : 50+ 초(시간 초과)" << endl;
	}

	cout << endl << "[SplayTree 소멸자 측정 시작]" << endl;
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
		iterator->second += 'a';			//컴파일, 링킹 최적화로 테스트 중의 검색 메소드 호출이 건너뛰어지는 경우가 없도록 하기 위한 추가 명령문임
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