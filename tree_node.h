using namespace std;
#include <string>
#include <vector>

//the external nil black nodes that have no value are null in every sense EXCEPT for the fact that they are black

class Tree_node{
public:
	Tree_node(int m);
	const int& get_val();
	const int& get_color();
	const int& get_lchild_val();
	const int& get_rchild_val();
	Tree_node * get_parent();
	Tree_node * get_lchild_pointer();
	Tree_node * get_rchild_pointer();
	bool is_left_child();
	bool is_right_child();
	bool is_root();
	void set_color(int m);
	void recolor();
	bool is_leaf();
	void insert(Tree_node *root);
	void node_exists(int m);
	Tree_node * get_root();
	Tree_node * balance_insert();
	void uniform_black_depth(int black_depth);
	static Tree_node* delete_node(Tree_node *root);
	void inorder_successor();
	void check_for_double_red();
	static Tree_node* get_inorder_successor();
	static void set_node_for_deletion(Tree_node *node);
	void is_legitimate_red_black_tree();
	static Tree_node * get_node_for_deletion();
	static bool get_balanced_boolean();
	static bool get_double_red_boolean();
	static void print_deletion_vector();
	static void clean_tree(Tree_node * root);
	Tree_node * get_sibling();
	Tree_node * get_left_nephew();
	Tree_node * get_right_nephew();
	void deletion_case_1(Tree_node *sibling);
	void deletion_case_2(Tree_node *sibling);
	void deletion_case_3(Tree_node *sibling);
	void deletion_case_4(Tree_node *sibling);
	void choose_deletion_case(Tree_node *sibling);
	void right_rotation();
	void left_rotation();
	bool is_external_node();
	void prepare_tree_for_pruning();
	void set_val(int v){
		val=v;
	}
	~Tree_node();
	//definition of the default constructor
	//This constructor initializes no "val" value: This is because it will play the role of an external node
	//External nodes are black and have value -1
	//The Nil Black Node subclass is derived from this constructor definition
	Tree_node(){
		parent=NULL;
		val=-1;
		left=this;
		right=this;
		color=0;
		visited=0;
		visited_deletion=0;
		visited_clean_tree=0;
}


//color is red/black color
//left is left child pointer
//right is right child pointer
//parent is parent pointer
//val is integer value of node
//visited is used for the DFS traversal of the tree
//visited_deletion is used for finding if node we wish to delete exists in tree
//visited_double_red is flag for checking for existence of double reds in any path (uses DFS traversal, must keep track of visited)
protected:
	Tree_node *left;
	Tree_node *right;
	Tree_node *parent;
	static Tree_node *node_to_delete;
	static Tree_node *successor;
	static int black_depth_count;
	static bool is_balanced_tree;
	static bool double_red_in_tree;
	static vector <Tree_node*> deletion_vector;
	int val;
	int color;
	int visited;
	int visited_deletion;
	int visited_double_red;
	int visited_clean_tree;
};

class Nil_Black_Node:public Tree_node{
public:
	Nil_Black_Node();
	~Nil_Black_Node();
};
