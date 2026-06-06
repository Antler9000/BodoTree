# TreeStructure
"C와 C++로 구현하는 자료구조" 책에서 소개된 트리 자료구조들을 C++로 구현한 리포지토리입니다[1].  

현재 Splay Tree, AVL Tree, Red Black Tree의 테스팅 및 다듬기 과정에 있습니다.

<br>




## 목차
1. [제공 대상](#1-제공-대상)
2. [폴더 구조](#2-폴더-구조)
3. [실행 방법](#3-실행-방법)
4. [성능 측정](#4-성능-측정)
5. [구현 예정](#5-구현-예정)
6. [구현 제외](#6-구현-제외)
8. [참고 자료](#7-참고-자료)

<br>




## 1. 제공 대상

### 1.1. 재귀 호출을 이용한 이진 탐색 트리
이진 탐색 트리(Binary Search Tree, BST)는 각 노드의 키 값을 기준으로 자식 노드들의 대소 관계가 지켜지는 트리입니다. 부모노드의 키(key)는 좌측 자식 노드의 키보다는 크고, 우측 자식의 키보다는 작아야 한다는 조건을 통해 빠르게(평균적으로 O(log(n))) 특정 키의 노드를 찾을 수 있습니다. 

이러한 이진 탐색 트리의 부모 자식 간의 반복되는 관계를 재귀로 구현한 버전입니다.

- ### 생성자, 할당 연산자, 소멸자
```cpp
    Bst()

    Bst(const Bst<DataType>& sourceTree)

    Bst(Bst<DataType>&& sourceTree) noexcept

    Bst<DataType>& operator = (const Bst<DataType>& sourceTree)

    Bst<DataType>& operator = (Bst<DataType>&& sourceTree) noexcept

    ~Bst() noexcept
```

- ### 퍼블릭 메소드
```cpp
    //RETURN : newKey와 같은 키의 노드가 이미 존재하는 경우 false를 반환함
    template <typename InsertDataType = DataType>  
    bool Insert(int newKey, InsertDataType&& newData)

    //RETURN : targetKey와 같은 키를 가진 노드가 존재하지 않는 경우 false를 반환함
    bool Retrieve(int targetKey, DataType& outData) const

    //RETURN : targetKey와 같은 키를 가진 노드가 존재하지 않는 경우 false를 반환함
    bool Remove(int targetKey)

    void RemoveTree() noexcept

    void CopyTree(const Bst<DataType>& sourceTree)

    //NOTE : 디버깅 용
    void PreorderPrint() const

    void InorderPrint() const

    void PostorderPrint() const
```
 
<br>

### 1.2. 반복문을 이용한 이진 탐색 트리
재귀로 인해 발생하는 함수 호출 비용과, 스택 오버 플로우의 위험성을 제거한 버전의 이진 탐색 트리입니다. 함수 호출 기능에 의존하는 재귀호출 대신, 직접 스택과 반복문을 사용해 똑같은 기능을 구현하였습니다.

- ### 생성자, 할당 연산자, 소멸자
```cpp
    Bst()

    Bst(const Bst<DataType>& sourceTree)

    Bst(Bst<DataType>&& sourceTree) noexcept

    Bst<DataType>& operator = (const Bst<DataType>& sourceTree)

    Bst<DataType>& operator = (Bst<DataType>&& sourceTree) noexcept

    ~Bst() noexcept
```

- ### 퍼블릭 메소드
```cpp
    //RETURN : newKey와 같은 키의 노드가 이미 존재하는 경우 false를 반환
    template <typename InsertDataType = DataType>
    bool Insert(int newKey, InsertDataType&& newData)

    //RETURN : targetKey와 같은 키를 가진 노드가 존재하지 않는 경우 false를 반환
    bool Retrieve(int targetKey, DataType& outData) const

    //RETURN : targetKey와 같은 키를 가진 노드가 존재하지 않는 경우 false를 반환
    bool Remove(int targetKey)

    void RemoveTree() noexcept

    void CopyTree(const BstTemplate<NodeType, DataType>& sourceBst)

    //NOTE : 디버깅 용
    void PreorderPrint() const

    void InorderPrint() const

    void PostorderPrint() const
```

<br>

### 1.3. 스플레이 트리
스플레이 트리(Splay Tree)는 이진 탐색 트리의 일종으로, 특정 노드를 탐색할 때마다 해당 노드를 루트로 올려, 자주 탐색되는 노드와 그 인접 노드들이 더 빨리 탐색되도록 합니다.

- ### 생성자, 할당 연산자, 소멸자
```cpp
    SplayTree()

    SplayTree(const SplayTree& sourceTree)

    SplayTree(SplayTree&& sourceTree) noexcept

    SplayTree& operator = (const SplayTree& sourceTree)

    SplayTree& operator = (SplayTree&& sourceTree) noexcept

    ~SplayTree() noexcept
```

- ### 퍼블릭 메소드
```cpp
    //RETURN : newKey와 같은 키의 노드가 이미 존재하는 경우 false를 반환
    template <typename InsertDataType = DataType>
    bool Insert(int newKey, InsertDataType&& newData)

    //RETURN : targetKey와 같은 키를 가진 노드가 존재하지 않는 경우에 false를 반환
    bool Retrieve(int targetKey, DataType& outData)

    //RETURN : targetKey와 같은 키를 가진 노드가 존재하지 않는 경우 false를 반환
    bool Remove(int targetKey)

    void RemoveTree() noexcept

    void CopyTree(const BstTemplate<NodeType, DataType>& sourceBst)

    //NOTE : 디버깅 용
    void PreorderPrint() const

    void InorderPrint() const

    void PostorderPrint() const
```

<br>

### 1.4. AVL 트리
AVL 트리는 키가 정렬된 노드들이 삽입될 때 이진 탐색 트리에서 발생하는 편향 문제를 해결한 균형 트리구조입니다. 노드의 왼쪽 하위 트리의 높이와 우측 하위 트리의 높이의 차가 2 이상이 될 경우에, 해당 위치를 기준으로 노드들을 회전하여 트리의 균형을 조절합니다.

- ### 생성자, 할당 연산자, 소멸자
```cpp
    단원의 다듬기가 끝나고 작성할 예정
```

- ### 퍼블릭 메소드
```cpp
    단원의 다듬기가 끝나고 작성할 예정
```

<br>

### 1.5. 레드-블랙 트리
레드-블랙 트리(Red-Black Tree)는 완벽하게 균형이 지켜지는 2-3-4 트리의 개념을, 이진 탐색 트리로 모방 구현한 균형 추종 트리입니다.  

2-3-4 트리는 기존에 한 개의 레코드가 들어가는 노드(2-노드) 외에도, 한 노드 안에 레코드가 2개 들어가는 노드(3-노드)와, 3개 들어가는 노드(4-노드)들을 함께 사용하여 삽입과 삭제에도 균형이 완벽하게 유지되도록 한 트리입니다.

이를 편하게 이진 탐색 트리로 구현하기 위해 레드-블랙 트리는 3-노드나 4-노드 대신, 기존의 2-노드에 '레드'와 '블랙'이라는 색깔 멤버 변수를 추가하고, 3-노드를 인접한 두 개의 2-노드로 구현하고(블랙 노드 1개, 레드 노드 1개), 4-노드를 인접한 3개의 2-노드로 구현합니다(블랙 노드 1개, 레드 노드 2개).

하나의 3-노드나 4-노드가 부모 자식의 관계인 여러 노드들로 구성되므로, 레드-블랙 트리는 2-3-4 트리와 달리 균형을 완벽하게 유지하지는 못합니다. 하지만 레드-블랙 트리는 2-3-4 트리의 삽입-삭제 로직을 구현하여 균형을 어느정도 유지해내며, 이진 탐색 트리의 로직을 재활용해 비교적 쉽게 구현이 가능하다는 장점을 가집니다.

- ### 생성자, 할당 연산자, 소멸자
```cpp
    단원의 다듬기가 끝나고 작성할 예정
```

- ### 퍼블릭 메소드
```cpp
    단원의 다듬기가 끝나고 작성할 예정
```

<br>

### 1.6. 힙
힙(Heap)은 부모 노드와 자식 노드 사이에 대소 관계가 지켜지는 완전 이진 트리입니다. 부모가 자식보다 커야 한다면 MaxHeap이라 하고, 작아야 한다면 MinHeap이라고 합니다. 가장 크거나(MaxHeap) 작은(MinHeap) 요소를 얻을 수 있으므로 우선순위 큐로 사용할 수 있습니다.

- ### 생성자, 할당 연산자, 소멸자
```cpp
    MinHeap()
    MaxHeap()

    MinHeap(const MinHeap& sourceTree)
    MaxHeap(const MaxHeap& sourceTree)

    MinHeap(MinHeap&& sourceTree) noexcept
    MaxHeap(MaxHeap&& sourceTree) noexcept

    MinHeap& operator = (const MinHeap& sourceTree)
    MaxHeap& operator = (const MaxHeap& sourceTree)

    MinHeap& operator = (MinHeap&& sourceTree) noexcept
    MaxHeap& operator = (MaxHeap&& sourceTree) noexcept

    ~MinHeap() noexcept
    ~MaxHeap() noexcept
```

- ### 퍼블릭 메소드
```cpp
    template <typename PushDataType = DataType>
    void Push(int newKey, PushDataType&& newData)

    //RETURN : 힙이 비어져 있는 경우 false를 반환
    bool Pop(DataType& outData)

    //RETURN : 힙이 비어져 있는 경우 false를 반환
    bool GetTop(DataType& outData)

    //RETURN : 힙에 하나의 데이터라도 있으면 false를 반환
    bool IsEmpty()

    void RemoveHeap() noexcept

    void CopyHeap(const Heap& sourceHeap)

    //NOTE : 디버깅 용
    void PrintHeap()
```

<br>




## 2. 폴더 구조
Chapter 번호는 책에서 해당 트리 자료구조가 소개된 단원과는 무관합니다[1].

- **Chapter 1 BST Using Recurse** 
    - **TestingBstUsingRecurse.cpp**  

    - **BstUsingRecurse.h**  

- **Chapter 2 BST Using While**
    - **TestingBstUsingWhile.cpp**

    - **BstUsingWhile.h**

- **Chapter 3 Splay Tree**
    - **TestingSplayTree.cpp**

    - **SplayTree.h**

- **Chapter 4 AVL Tree**
    - **TestingAvlTree.cpp**

    - **AvlTree.h**

- **Chapter 5 Red Black Tree**
    - **TestingRedBlackTree.cpp**

    - **RedBlackTree.h**

- **Chapter 6 Heap**
    - **TestingHeap.cpp**

    - **Heap.h**

- **Common**
    - **BstUsingWhileTemplate.h**  
        2 ~ 5 단원의 트리를 구현하기 위한 베이스 이진 탐색 트리 클래스가 정의되어있습니다.

    - **Stack.h**  
        BstUsingWhileTemplate.h, SplayTree.h, AvlTree.h, RedBlackTree.h 에서 로직 구현을 위해 사용하는 스택입니다.

    - **Debug.h**  
        로그 출력 매크로, 누수 추적용 new 함수의 매크로가 정의되어있습니다.

<br>




## 3. 실행 방법
이 리포지토리는 Visual Studio 2022, 2026을 기준으로 작성되었습니다.

1. 리포지토리를 다운로드 받습니다.

2. 로컬의 리포지토리 디렉토리에 포함된 'TreeStructure.sln' 솔루션 파일을 더블 클릭합니다.

3. Visual Studio가 실행되면 우측에 '솔루션 탐색기' 바를 확인합니다.
(해당 '솔루션 탐색기'바가 없을시 상단의 '보기(V)'바에서 '솔루션 탐색기(P)'를 누르면 됩니다)

4. 해당 '솔루션 탐색기' 바에서 실행해보고 싶은 단원의 트리 프로젝트를 우클릭합니다.

5. 우클릭시 나오는 메뉴에서 '시작 프로젝트로 설정'을 누르거나, 단축키 'A'를 누릅니다.

6. 예제를 실행하고자 한다면, Visual Studio 상단의 구성(configuration)을 "Debug" "x64"로 설정하고, 디버깅 시작키인 'F5'를 누르면 됩니다. 실행 결과가 검은 명렁 프롬프트에 출력될 것입니다. 또한 메모리 누수가 존재한다면 Visual Studio 하단의 출력(output) 창에 누수 정보가 출력될 것입니다.

7. 테스트를 실행하고자 한다면, 상단의 구성을 "Release"로 변경하고, 디버깅 없이 실행('Ctrl+F5')을 누르면 됩니다. 테스트 과정과 결과가 검은 명령 프롬프트에 출력될 것입니다.

<br>




## 4. 성능 측정

### 4.1. 측정 환경
[기본]
|||
|-------------------|---------------------------------------------------------------|    
|CPU                |i5-13600KF, 3500Mhz, 14 코어, 20 논리 프로세서                 |
|RAM                |32GB, DDR4                                                     |
|OS                 |Windows 11, 버전 25H2, 빌드 26200.8246                         |
|IDE                |Microsoft Visual Studio Community 2026 (64 - bit) 버전 18.5.2  |
|플랫폼 도구 집합   |v145 for Microsoft C++ Build Tools                             |
|컴파일러 버전      |x86용 Microsoft (R) C/C++ 최적화 컴파일러 버전 19.50.35730     |

<br>

[상세]
|||||
|---------------|-----------|-----------------------|---------------------------------------------------|
|구성 선택      |           |					    |Release x64                                        |
|디버깅 여부    |           |                       |디버깅하지 않고 시작(Ctrl + F5)                    |
|C / C++        |일반       |디버그 정보 형식       |프로그램 데이터베이스(/Zi)                         |
|..             |최적화     |최적화			        |최대 최적화(속도 우선)(/O2)                        |
|..             |..         |인라인 함수 확장	    |적합한 것 모두 확장(/Ob2)                          |
|..             |..         |내장 함수 사용         |예(/Oi)                                            |
|..             |..         |크기 또는 속도	        |코드 속도 우선(/Ot)                                |
|..             |..         |전체 프로그램 최적화   |예(/GL)                                            |
|..             |전처리기   |전처리기 정의          |NDEBUG;_CONSOLE;%(PreprocessorDefinitions)         |
|..             |코드 생성  |기본 런타임 검사	    |기본값                                             |
|..             |코드 생성  |런타임 라이브러리      |다중 스레드 DLL(/MD)                               |
|..             |언어       |C++ 언어 표준			|기본값(ISO C++ 14 표준)                            |
|..             |출력 파일  |어셈블러 출력		    |소스 코드로 구성된 어셈블리(/FAs)                  |
|링커           |최적화     |링크 타임 코드 생성    |빠른 링크 타임 코드 생성 사용(/LTCG:incremental)   |

<br>

### 4.2. 측정 방식
모든 단원은 '랜덤 워크로드', '랜덤 지역 워크로드', '선형 증가 워크로드', '선형 감소 워크로드'로 나누어 릴리즈 성능 테스트를 진행합니다.

세부 사항은 임시로 'Chapter 1 BST Using Recurse\TestingBstUsingRecurse.cpp', 'Chapter 2 BST Using While\TestingBstUsingWhile.cpp', 'Chapter 3 Splay Tree\TestingSplayTree.cpp'에 동일한 내용으로 작성되어있습니다.

다만 힙은 Push와 Pop 만이 일어나는 어댑터이므로, 워크로드의 종류는 비슷하나 확인하는 메소드 종류가 다릅니다. 해당 사항은 'Chapter 6 Heap\TestingHeap.cpp'에 임시로 작성되어있습니다.

<br>

### 4.3. 측정 결과

#### 4.1. 재귀 호출을 이용한 이진 탐색 트리
임시로 "Chapter 1 BST Using Recurse\TestingBstUsingRecurse.cpp"의 주석에 업데이트 되고 있습니다.

<br>

#### 4.2. 반복문을 이용한 이진 탐색 트리
임시로 "Chapter 2 BST Using While\TestingBstUsingWhile.cpp"의 주석에 업데이트 되고 있습니다.

<br>

#### 4.3. 스플레이 트리
임시로 "Chapter 3 Splay Tree\TestingSplayTree.cpp"의 주석에 업데이트 되고 있습니다.

<br>

#### 4.4. AVL 트리
제대로 된 테스트가 아직 구현되지 않았습니다.

<br>

#### 4.5. 레드-블랙 트리
제대로 된 테스트가 아직 구현되지 않았습니다.

<br>

#### 4.6. 힙
임시로 "Chapter 6 Heap\TestingHeap.cpp"의 주석에 업데이트 되고 있습니다.

<br>




## 5. 구현 예정

### 5.0. 공통
- 예외 안전성은 포기하더라도, 스택을 이용한 중위 순회 Bst Iterator 제공하기

<br>

### 5.1. 재귀를 이용한 이진 탐색 트리
- 없음

<br>

### 5.2. 반복문을 이용한 이진 탐색 트리
- 없음

<br>

### 5.3. 스플레이 트리
- 검색 메소드에서 splay 조정이 루트 노드까지 쭉 거슬러 올라가며 적용되도록 하기

- 삽입과 삭제 메소드에도 splay 조정이 일어나도록 하기

- 비정상적으로 느린 속도 개선하기

<br>

### 5.4. AVL 트리
- 앞선 단원들에 적용된 개선 사항 적용하고 테스팅해보기

<br>

### 5.5. 레드-블랙 트리
- 앞선 단원들에 적용된 개선 사항 적용하고 테스팅해보기

<br>

### 5.6. 힙
- 최소힙과 최대힙을 폴리몰피즘으로 구현하지 말고 더 정적인 방식으로 구현하기

- priority_queue 대비 2배 느린 연산들을 개선하기

<br>




## 6. 구현 제외
- **B+ 트리, 트라이 자료구조**  
BST 단원들의 트리와 다른 별도의 테스팅을 요구하는 트리들이라 구현과 유지 보수가 힘드리라 생각해 제외하였습니다.  
힙 단원은 테스팅을 고려하기 전에 이미 초안을 구현해버렸기에 구현 대상에 포함하였습니다.

<br>

- **노드의 부모 포인터**  
현재 구현에선 각 노드들이 자식 노드만을 가리키고 있어서 순회가 어렵고, 대신 스택을 통해서 순회를 구현하고 있습니다.  
스택을 이용하는 방식은 스택 삽입에서 일어나는 동적 할당으로 인해 예외에 안전하지 않습니다.  
다만 부모를 가리키는 포인터를 각 노드에 포함시키려면 수정해야 할 코드가 많아지기에, 불안전하지만 스택을 이용한 순회를 이용하기로 하였습니다.

<br>

- **제너릭 메소드 개선**  
현재 BstTemplateUsingWhile에서 사용한 제너릭 메소드 방식은, 하위 작업 메소드의 호출(ex. InsertNode(..))이 인라이닝 되지 않는 경우가 있고,  
제너릭 메소드가 const 메소드 키워드 여부에 따라 두 버전으로 작성되어야 한다는 점,  
하위 작업 메소드에 넘겨지는 인자의 개수를 유동적일 수 없다는 점 등의 단점을 지닙니다.  
그러나 이보다 나은 제너릭 메소드를 작성하기 위한 모던 C++ 문법을 아직 익히지 못하여 현 상태 그대로 두려고 합니다.

<br>

- **값복사 매개변수 방식의 메소드**  
데이터 타입이 단순한 경우에는 참조 전달이 아니라 값복사 방식으로 전달하는 것이 더 효율적입니다.  
그러나 데이터 타입이 기본 데이터 타입인지를 확인하는 문법을 아직 이해하지 못하여 구현 대상에서 제외하였습니다.

<br>

- **데이터 타입의 예외 안전성 확인**  
현재 구현에서는 기존 어레이를 확장 할 때, 이동 연산이 안전하지 못하는 경우를 고려하여 복사를 통해 이를 수행합니다.  
따로 이동을 사용하는 버전의 메소드를 같이 정의하는 것이 효율적이겠지만,  
앞선 항목과 마찬가지로 이를 위한 문법에 익숙치 못하고, 여러 메소드를 중복 구현해야 하는 것이 부담스러워 구현 대상에서 제외하였습니다.

<br>

- **스왑 메소드의 예외 안전성 개선**  
현재 힙에서 사용하는 스왑 메소드는 아래에서 예외가 발생할 시 두 인자를 보존하지 못하고,  
이를 사용하는 메소드도 스왑에서 예외가 발생할 시 데이터가 훼손되도록 로직이 구현되어있습니다.  
그러나 데이터 타입이 그대로 어레이에 저장되는 현 방식에서 더 안전한 스왑 메소드를 제공하긴 힘드리라 생각하여 개선을 제외하였습니다.

<br>

- **emplace 메소드**  
emplace가 제공된다면 생성과 동시에 보관될 데이터를 자료구조에서 직접 생성하여, 할당 연산 과정을 건너뛸 수 있습니다.   
그러나 emplace로 넘어오는 가변적인 인자를 받는 포워딩 구문이, 아직 이해가 안 가는 모던 C++ 문법에 의존하고 있어 제외하였습니다.

<br>

- **메모리 풀**  
메모리 풀을 사용한다면 삽입 메소드 호출마다 일어나던 new 연산을 줄일 수 있습니다.  
그러나 노드의 사용이 해제된 경우 필요한 가비지 콜렉션까지 구현하기엔 부담스러워 제외하였습니다.

<br>




## 7. 참고 자료
[1] "C C++로 구현하는 자료구조" (주우석 지음 / 한빛아카데미 출판 / 2015년 발행)

[2] "SW코딩 역량을 위한 C++ 프로그래밍" (홍봉희 지음 / 정익사 출판 / 2017년 발행)

[3] "Move Constructors and Move Assignment Operators (C++)" (Microsoft Learn / 2026년 6월 재방문)  
https://learn.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-170

[4] "Rvalue reference declarator: &&" (Microsoft Learn / 2026년 6월 재방문)  
https://learn.microsoft.com/en-us/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-170

[5] "How to: Design for exception safety" (Microsoft Learn / 2026년 6월 재방문)  
https://learn.microsoft.com/en-us/cpp/cpp/how-to-design-for-exception-safety?view=msvc-170

[5] "How to: Create and use unique_ptr instances" (Microsoft Learn / 2026년 6월 재방문)  
https://learn.microsoft.com/en-us/cpp/cpp/how-to-create-and-use-unique-ptr-instances?view=msvc-170

[6] "Find memory leaks with the CRT library" (Microsoft Learn / 2026년 6월 재방문)  
https://learn.microsoft.com/en-us/cpp/c-runtime-library/find-memory-leaks-using-the-crt-library?view=msvc-170