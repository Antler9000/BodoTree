#include "AvlTree.h"

int main()
{
	AvlTree<int> testTree;

	cout << endl << "insert 5 test" << endl;
	testTree.Insert(5, 55);
	testTree.PreorderPrint();

	cout << endl << "insert 9 test" << endl;
	testTree.Insert(9, 99);
	testTree.PreorderPrint();

	cout << endl << "insert 8 test" << endl;
	testTree.Insert(8, 88);
	testTree.PreorderPrint();

	cout << endl << "insert 7 test" << endl;
	testTree.Insert(7, 77);
	testTree.PreorderPrint();

	cout << endl << "insert 1 test" << endl;
	testTree.Insert(1, 11);
	testTree.PreorderPrint();

	cout << endl << "insert 2 test" << endl;
	testTree.Insert(2, 22);
	testTree.PreorderPrint();

	cout << endl << "insert 3 test" << endl;
	testTree.Insert(3, 33);
	testTree.PreorderPrint();

	cout << endl << "copy test" << endl;
	AvlTree<int> copied_test_tree;
	copied_test_tree.CopyTree(testTree);
	copied_test_tree.PreorderPrint();

	cout << endl << "remove 1 test" << endl;
	testTree.Remove(1);
	testTree.PreorderPrint();

	cout << endl << "remove 2 test" << endl;
	testTree.Remove(2);
	testTree.PreorderPrint();

	cout << endl << "remove 3 test" << endl;
	testTree.Remove(3);
	testTree.PreorderPrint();

	cout << endl << "remove 5 test" << endl;
	testTree.Remove(5);
	testTree.PreorderPrint();

	cout << endl << "remove 7 test" << endl;
	testTree.Remove(7);
	testTree.PreorderPrint();

	cout << endl << "remove 8 test" << endl;
	testTree.Remove(8);
	testTree.PreorderPrint();

	cout << endl << "remove 9 test" << endl;
	testTree.Remove(9);
	testTree.PreorderPrint();

	return 0;
}