#include "rbtree.cpp"
//A testing file for the rbtree driver file
//Checks for various deletion cases, and that tree construction works effectively
//These examples were pulled from Alena Chang's youtube video on red black tree insertions and deletion: Video can be found at https://www.youtube.com/watch?v=eO3GzpCCUSg
void test_2(){
	cout<<endl<<"Testing case 2: Tree with black depth 2: Deleting red node that has black child"<<endl;
	Tree_node *og_root_ptr1=insert(5,NULL);
	og_root_ptr1=insert(2,og_root_ptr1);
	og_root_ptr1=insert(4,og_root_ptr1);
	og_root_ptr1=insert(8,og_root_ptr1);
	og_root_ptr1=insert(1,og_root_ptr1);
	og_root_ptr1=insert(7,og_root_ptr1);
	og_root_ptr1=insert(9,og_root_ptr1);
	og_root_ptr1=delete_node(2,og_root_ptr1);
	og_root_ptr1->is_legitimate_red_black_tree();
	std::cout<<std::endl;
	Tree_node::clean_tree(og_root_ptr1);
	cout<<endl;
}

void test_3(){
	cout<<"Testing Root Deletion of Tree: Deleting root of tree which leads to Case 4 deletion"<<endl;
	Tree_node *og_root_ptr2=insert(13,NULL);
	og_root_ptr2=insert(8,og_root_ptr2);
	og_root_ptr2=insert(17,og_root_ptr2);
	og_root_ptr2=insert(1,og_root_ptr2);
	og_root_ptr2=insert(11,og_root_ptr2);
	og_root_ptr2=insert(15,og_root_ptr2);
	og_root_ptr2=insert(25,og_root_ptr2);
	og_root_ptr2=insert(6,og_root_ptr2);
	og_root_ptr2=insert(22,og_root_ptr2);
	og_root_ptr2=insert(27,og_root_ptr2);
	og_root_ptr2=delete_node(13,og_root_ptr2);
	og_root_ptr2->is_legitimate_red_black_tree();
	std::cout<<std::endl;
	Tree_node::clean_tree(og_root_ptr2);
	cout<<endl;
}

void test_4(){
	cout<<endl<<"Testing Case 3 Deletion Case Which Leads to Case 4 Deletion Case"<<endl;
	Tree_node *og_root_ptr3=insert(13,NULL);
	og_root_ptr3=insert(8,og_root_ptr3);
	og_root_ptr3=insert(17,og_root_ptr3);
	og_root_ptr3=insert(11,og_root_ptr3);
	og_root_ptr3=insert(1,og_root_ptr3);
	og_root_ptr3=insert(6,og_root_ptr3);
	og_root_ptr3=insert(15,og_root_ptr3);
	og_root_ptr3=insert(25,og_root_ptr3);
	og_root_ptr3=insert(22,og_root_ptr3);
	og_root_ptr3=insert(27,og_root_ptr3);
	og_root_ptr3=delete_node(8,og_root_ptr3);
	og_root_ptr3->is_legitimate_red_black_tree();
	std::cout<<std::endl;
	Tree_node::clean_tree(og_root_ptr3);
}

void test_5(){
	cout<<endl<<"Testing Case 1 Deletion Case Which Leads to Case 4 Deletion"<<endl;
	Tree_node *og_root_ptr4=insert(7,NULL);
	og_root_ptr4=insert(3,og_root_ptr4);
	og_root_ptr4=insert(18,og_root_ptr4);
	og_root_ptr4=insert(10,og_root_ptr4);
	og_root_ptr4=insert(22,og_root_ptr4);
	og_root_ptr4=insert(8,og_root_ptr4);
	og_root_ptr4=insert(11,og_root_ptr4);
	og_root_ptr4=insert(26,og_root_ptr4);
	og_root_ptr4=delete_node(3,og_root_ptr4);
	og_root_ptr4->is_legitimate_red_black_tree();
	std::cout<<std::endl;
	Tree_node::clean_tree(og_root_ptr4);
	cout<<endl;
}

void test_6(){
	cout<<endl<<"Testing for case of deleting a black node with 2 external node children. Leads to Case 3, and then Case 4 deletion"<<endl;
	Tree_node *og_root_ptr5=insert(13,NULL);
	og_root_ptr5=insert(8,og_root_ptr5);
	og_root_ptr5=insert(17,og_root_ptr5);
	og_root_ptr5=insert(1,og_root_ptr5);
	og_root_ptr5=insert(11,og_root_ptr5);
	og_root_ptr5=insert(15,og_root_ptr5);
	og_root_ptr5=insert(25,og_root_ptr5);
	og_root_ptr5=insert(6,og_root_ptr5);
	og_root_ptr5=insert(22,og_root_ptr5);
	og_root_ptr5=insert(27,og_root_ptr5);
	og_root_ptr5=delete_node(11,og_root_ptr5);
	og_root_ptr5->is_legitimate_red_black_tree();
	cout<<std::endl;
	Tree_node::clean_tree(og_root_ptr5);
	cout<<endl;
}

void test_7(){
	cout<<endl<<"Testing case of an illegitimate red black tree: Tree has double red and inconsistent black depth throughout all paths"<<endl;
	Tree_node *og_root_ptr6=insert(10,NULL);
	og_root_ptr6=insert(5,og_root_ptr6);
	og_root_ptr6=insert(12,og_root_ptr6);
	og_root_ptr6=insert(3,og_root_ptr6);
	og_root_ptr6->get_lchild_pointer()->recolor();
	og_root_ptr6->is_legitimate_red_black_tree();
	std::cout<<std::endl;
	Tree_node::clean_tree(og_root_ptr6);
	std::cout<<endl;
}

void test_8(){
	Tree_node *root=insert(10,NULL);
	root=insert(8,root);
	root=insert(6,root);
	root->is_legitimate_red_black_tree();
	std::cout<<std::endl;
	Tree_node::clean_tree(root);
}
int main(){
	test_2();
	cout<<endl;
	test_3();
	cout<<endl;
	test_4();
	cout<<endl;
	test_5();
	cout<<endl;
	test_6();
	cout<<endl;
	test_7();
	cout<<endl;
	test_8();
	cout<<std::endl;
}
