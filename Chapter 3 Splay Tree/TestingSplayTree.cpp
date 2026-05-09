//로그 출력문들을 활성화하고 싶을 시 아래 구문의 주석을 해제할 것
//#define TREE_LOG	
//#define TREE_ERROR
//#define TREE_WARNING

#include "SplayTree.h"

int main()
{
	SplayTree<int> testTree;

	testTree.Insert(5, 1515);
	testTree.Insert(7, 2727);
	testTree.Insert(3, 1313);
	testTree.Insert(4, 2424);
	testTree.Insert(6, 3636);
	testTree.Insert(1, 5151);
	testTree.Insert(8, 5858);
	testTree.PreorderPrint();

	cout << endl << "Retrieve test 8" << endl;
	int retrievedData = 0;
	testTree.Retrieve(8, retrievedData);
	testTree.PreorderPrint();

	cout << endl << "Retrieve test 4" << endl;
	testTree.Retrieve(4, retrievedData);
	testTree.PreorderPrint();

	cout << endl << "Retrieve test 3" << endl;
	testTree.Retrieve(3, retrievedData);
	testTree.PreorderPrint();

	cout << endl << "Retrieve test 3" << endl;
	testTree.Retrieve(3, retrievedData);
	testTree.PreorderPrint();

	cout << endl << "Retrieve test 7" << endl;
	testTree.Retrieve(7, retrievedData);
	testTree.PreorderPrint();

	cout << endl << "Retrieve test 5" << endl;
	testTree.Retrieve(5, retrievedData);
	testTree.PreorderPrint();

	SplayTree<int> copied_test_tree;
	copied_test_tree.CopyTree(testTree);
	copied_test_tree.PreorderPrint();

	cout << endl << "Remove test 7" << endl;
	testTree.Remove(7);
	testTree.PreorderPrint();

	cout << endl << "Remove test 3" << endl;
	testTree.Remove(3);
	testTree.PreorderPrint();

	cout << endl << "Remove test 5" << endl;
	testTree.Remove(5);
	testTree.PreorderPrint();

	cout << endl << "Remove test 4" << endl;
	testTree.Remove(4);
	testTree.PreorderPrint();

	cout << endl << "Remove test 6" << endl;
	testTree.Remove(6);
	testTree.PreorderPrint();

	cout << endl << "Remove test 1" << endl;
	testTree.Remove(1);
	testTree.PreorderPrint();

	cout << endl << "Remove test 8" << endl;
	testTree.Remove(8);
	testTree.PreorderPrint();

	return 0;
}