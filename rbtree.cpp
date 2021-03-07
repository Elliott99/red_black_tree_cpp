using namespace std;
#include "tree_node.h"
#include <iostream>
#include <cstddef>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <stdexcept>

//Nil Black Node plays the role of the external nodes in my tree
//Subclass of Tree_node() superclass, instantiated with the default constructor definition of the Tree_node class
//Default constructor definition for the Tree_node() class means the Nil Black Node objects all will be black, point to themselves, and have no value
Nil_Black_Node::Nil_Black_Node():Tree_node(){}


//Defining constructor for the parameterized non-external node Tree_node objects
//By default, a new tree node's l/r children are two external nodes whose parent pointer both point to the node they have been instantiated with
//Therefore, instantiating one non-external Tree_node object puts not one but 3 objects on the heap
//color=0 means it is a black node, color=1 means it is a red node
Tree_node::Tree_node(int v){
	val=v;
	left=new Nil_Black_Node();
	right=new Nil_Black_Node();
	parent=NULL;
	left->parent=this;
	right->parent=this;
	color=1;
	visited=0;
	visited_deletion=0;
	visited_double_red=0;
}

//Defining the constructor for the Tree_node class
Tree_node::~Tree_node(){
	cout<<"Deleted node with value "<<this->val<<endl;
}

//Defining the constructor for the Nil_Black_Node class. I override the virtual destructor of the Tree_node destructor to give this
//unique behavior since nil black nodes have no value and therefore it makes no sense to print "Deleted node with X value"
Nil_Black_Node::~Nil_Black_Node(){
	cout<<"Deleted external node";
}
//return const int reference to get the int value of any tree_node (const so we can't modify member value after returning)
const int& Tree_node::get_val(){
	return this->val;
}

//checks if node is external node: All external nodes are sentinel nodes that point to themselves
bool Tree_node::is_external_node(){
	if (this==this->left && this==this->right){
		return true;
	}
	return false;
}

//Tree_node class setter method for the color 
void Tree_node::set_color(int color){
	this->color=color;
}


//Tree_node member function which checks if caller object is a root
//If caller object has no parent and is black, it's a root
bool Tree_node::is_root(){
	bool tree_bool=false;
	if (this->parent==NULL && this->color==0){
		tree_bool=true;
	}
	return tree_bool;
}



//return const reference to color of node: Const so reference to color value is immutable 
const int& Tree_node::get_color(){
	return this->color;
}

//class member function which returns parent of caller object
Tree_node * Tree_node::get_parent(){
	return this->parent;
}

/*Tree_node * Tree_node::placeholder_init(){
	Tree_node *placeholderptr=&this->placeholder;
	return placeholderptr;
}*/


s
//class member functions which return constant reference to value of leftchild and rightchild
const int& Tree_node::get_lchild_val(){
	return this->left->val;
}

const int& Tree_node::get_rchild_val(){
	return this->right->val;
}

//class member functions that return pointers to left and right child of caller object
Tree_node * Tree_node::get_lchild_pointer(){
	return this->left;
}

Tree_node * Tree_node::get_rchild_pointer(){
	return this->right;
}

//checks if the caller object is a left child of it's parent node
bool Tree_node::is_left_child(){
	bool left_child_bool=false;
	if (this->parent->left==this){
		left_child_bool=true;
	}
	return left_child_bool;
}

//checks if caller object is a right child
bool Tree_node::is_right_child(){
	bool right_child_bool=false;
	if (this->parent->right==this){
		right_child_bool=true;
	}
	return right_child_bool;
}

//resets the color attribute of the caller object to the opposite of what it currently is
void Tree_node::recolor(){
	if (this->color==0){
		this->color=1;
	}
	else if (this->color==1){
		this->color=0;
	}
}

//recurs backwards through parent nodes until the current node's parent is null, indicating a root node
Tree_node* Tree_node::get_root(){
	if (this->parent==NULL){
		return this;
	}
	return this->parent->get_root();	
}


//get black depth gives me back the black depth of the path taken by only going down the left pointers
//Red-Black Tree rules state each path must have an equal length black depth, or the number of black nodes encountered on any path
int get_black_depth(Tree_node *root){
	root=root->get_root();
	int count=1;
	while(!root->get_lchild_pointer()->is_external_node()){
		if (root->get_lchild_pointer()->get_color()==0){
			count++;
		}
		root=root->get_lchild_pointer();
	}
	return count;
}

//modified DFS tree traversal which tells me if all of the paths in tree have equal black depth
//I initialize my static member variables before the method begins
int Tree_node::black_depth_count=0;
bool Tree_node::is_balanced_tree=true;
void Tree_node::uniform_black_depth(int black_depth){
	//case where we have reached the end of a path
	//if we've hit an external node and the static black_depth_count value is not equal to the black depth of the path from get_black_depth
	//set the static is_balanced_tree boolean to false- we have a tree in which not all paths have equal length
	if (this->is_external_node()){
		if (Tree_node::black_depth_count!=black_depth){
			Tree_node::is_balanced_tree=false;
		}
		return;
	}
	//root case: Increment black_depth_count by 1
	else if (this->visited==0 && this->parent==NULL){
		Tree_node::black_depth_count++;
	}
	//case where our sibling (therefore, our sibling has the same black depth) has been visited and is black. Therefore, the black depth count value 
	//for this depth has already been updated. We won't increment again if our sibling is black and has already been visited and is black
	else if (this->get_sibling()->color==1 && this->get_sibling()->visited==1 && this->visited==0){
		return;
	}
	//case where we are switching over to the right subtree from the left. We reset our black_depth_count value to 1, the only black node this subtree has so far is the root node
	else if (this->parent->is_root() && this->parent->left->visited==1 && this->visited==0){
		Tree_node::black_depth_count=1;
		return;
	}
	//case where we've already visited a node and it's not the root (parent isn't null)
	//we simply return to wherever it was this was called from
	else if (this->visited==1 && this->parent!=NULL){
		return;
	}
	//if we encounter a black non-visited node, then increment counter and return to where this was called from
	else if (this->color==0 && this->visited==0){
		Tree_node::black_depth_count++;
		return;
	}

	this->visited=1;
	this->left->uniform_black_depth(black_depth);
	this->right->uniform_black_depth(black_depth);
}

//checks for double red in tree
//modified DFS traversal with conditional checking
//initialize static member boolean double_red_in_tree
bool Tree_node::double_red_in_tree=false;
void Tree_node::check_for_double_red(){
	//we've hit the end of a path-return to where this was called from
	if (this->is_external_node()){
		return;
	}
	//if we are at the root and have not yet visited it: set to visited, and recurisvely call function with left child (in-order traversal)
	if (this->is_root() && this->visited_double_red==0){
		this->visited=1;
		this->left->check_for_double_red();
	}
	//if we've already visited the current caller object just return to where this was called from 
	else if (this->visited_double_red==1){
		return;
	}
	//if our current caller object is red, and it's parent is red, we have a double red
	else if (this->color==1 && this->parent->color==1){
		Tree_node::double_red_in_tree=true;
		cout<<"Double red discovered: Node with value "<<this->val<<" is red and parent node with value "<<this->parent->val<<" is also red"<<endl;
		return;
	}
	this->visited_double_red=1;
	this->left->check_for_double_red();
	this->right->check_for_double_red();
}

//member getter method for the static double_red_in_tree boolean which indicates if our tree has a double red case
bool Tree_node::get_double_red_boolean(){
	return Tree_node::double_red_in_tree;
}

//insertion method for when we have just inserted a node into the tree and must re-balance
//returns the root node of the tree after we do some balancing
Tree_node * Tree_node::balance_insert(){
	Tree_node *uncle=NULL;
	Tree_node *root=NULL;
	//if the node we are balancing is a root, make it black, and return the pointer to it
	if (this->parent==NULL){
		root=this;
		root->set_color(0);
		return this;
	}
	//if node we are balancing is child of root, then simply set the root pointer to the node's parent, and return the pointer to the node's parent
	else if (this->parent->is_root()){
		root=this->parent;
		return root;
	}
	else{
	//root case
	//null parent indicates root. Set color to black and return the pointer to this node since it is the root
		while (this->parent->color==1){
			if (this->parent==this->parent->parent->left){
			//uncle is right child, parent is left child
				uncle=this->parent->parent->right;
			//if we have an uncle that is red, and it is the right child of our grandfather:
				//if we have a red uncle then recolor the uncle, parent, and grand parent (case 1)
				//once we've done this, make a recursive call to balance with our new grandparent node that has been recolored
				if (!uncle->is_external_node() && uncle->get_color()==1){
					this->parent->recolor();
					uncle->recolor();
					this->parent->parent->recolor();
					Tree_node *grandpa=this->parent->parent;
				//once we do the recoloring, we then perform the same balancing algorithm on the current node's grandfather
					grandpa->balance_insert();
				}
				//Triangle case: Uncle is right child of grandfather, parent must be left child: We have a "triangle" case where parent is left child and our current node is the right child
				//Triangle cases lead to a rotation of the parent
				//Grandfather's left child is current node, and current node's new right child is the previous parent node
				else if (this==this->parent->right && (uncle->color==0)){
					Tree_node *tmp=this->parent;
					this->parent->parent->left=this;
					this->left=tmp;
					this->parent=tmp->parent;
					tmp->parent=this;
				}
				//line case: Parent is left child, current node is left child of parent, leads to recoloring and rotation:
				//in the case of a "left line", we must rotate the grandparent to the right and put the parent in it's place
				//if our current node has a "brother" (the right child of the parent) then we must make it the left child of the rotated grandfather
				//recolor original parent and grandparent after rotation
				else if (this==this->parent->left && uncle->color==0){
					Tree_node *tmp_grandpa=this->parent->parent;
					if (!tmp_grandpa->is_root()){
						if (this->parent->parent==this->parent->parent->parent->left){
						tmp_grandpa->parent->left=this->parent;
						}
						else if (this->parent->parent==this->parent->parent->parent->right){
							tmp_grandpa->parent->right=this->parent;
						}
						this->parent->parent=tmp_grandpa->parent;
					}
					else if (tmp_grandpa->is_root()){
						this->parent->parent=NULL;
					}

					if (!this->parent->right->is_external_node()){
						Tree_node *brother=parent->right;
						tmp_grandpa->left=brother;
					}

					this->parent->right=tmp_grandpa;
					tmp_grandpa->parent=this->parent;
					tmp_grandpa->recolor();
					this->parent->recolor();

				}
			}
			//case where parent is a right child
			else if (this->parent==this->parent->parent->right){
				uncle=this->parent->parent->left;
				//case where we have a red uncle: recolor uncle, parent, grandparent, perform algorithm again on recolored grandparent 
				if (uncle->get_color()==1){
					uncle->recolor();
					this->parent->recolor();
					this->parent->parent->recolor();
					this->parent->parent->balance_insert();
				}
			//Triangle case w/black or null uncle, (parent is right child, current node is leftchild of parent)
			//We set the current node to exist in the place of it's parent, set parent to be right child of parent
				else if (this==this->parent->left && uncle->get_color()==0){
					Tree_node *tmp=this->parent;
					this->parent->parent->right=this;
					this->right=tmp;
					this->parent=(tmp->parent);
					tmp->parent=this;
				}
			//line case which requires recolorng and rotation
				else if (this==this->parent->right && uncle->get_color()==0){
					Tree_node *tmp_grandpa=this->parent->parent;
					//tmp_grandpa->parent->right=this->parent;
					if (!tmp_grandpa->is_root()){
						if (this->parent->parent==this->parent->parent->parent->left){
							tmp_grandpa->parent->left=this->parent;
						}
						else if (this->parent->parent==this->parent->parent->parent->right){
							tmp_grandpa->parent->right=this->parent;
						}
						this->parent->parent=tmp_grandpa->parent;
					}
					else if (tmp_grandpa->is_root()){
						this->parent=NULL;
					}

					if (!this->parent->left->is_external_node()){
						Tree_node *brother=this->parent->left;
						tmp_grandpa->right=brother;
					}

					this->parent->left=tmp_grandpa;
					this->parent->parent=tmp_grandpa->parent;
					tmp_grandpa->recolor();
					this->parent->recolor();
				}
			}
		}
	}
	//at end of method, if we did not already hit condition that triggered an early return, then get the current tree root using this->get_root() and color black
	root=this->get_root();
	root->set_color(0);
	return root;
	}



//the private class member insert function called by the public insert method
void Tree_node::insert(Tree_node *root){
	//NULL root argument means we are inserting into empty tree
	if (root==NULL){
		this->color=0;
	}
	//base case of inserting a left child
	else if (root->left->is_external_node() && this->val<root->val){
		root->left=this;
		this->parent=root;
		return;
	}
	//base case of inserting a right child
	else if (root->right->is_external_node()  && this->val>root->val){
		this->parent=root;
		root->right=this;
		return;
	}
	//recursive case: Our caller object is greater in value than the root, so it must be in right subtree, but the right child of the root is not null
	//if this is the case, we mus traverse down the right subtree and find an appropriate insertion location
	else if (!root->right->is_external_node() && this->val > root->val){
		 this->insert(root->right);
	}
	//recursive left subtree case
	else if (!root->left->is_external_node() && this->val < root->val){
		 this->insert(root->left);
	}

}

//public insert method: Calls the private class method after allocating space on the heap for a Node with the integer value passed as an argument
//inserts heap allocated node using private insert Tree_node member method
//returns the root of the tree we've inserted into
//Takes in as argument integer value of new node we wish to insert, and pointer to the current root node
Tree_node* insert(int m,Tree_node *root){
	Tree_node *new_node;
	new_node=new Tree_node(m);
	if (root==NULL){
		root=new_node;
		root->set_color(0);
		return new_node;
	}

	new_node->insert(root);
	root=new_node->balance_insert();
	return root;
}
//must initialize static member attribute outside of class as static attributes do not share the same memory location as class objects
//therefore we do not initiailize the static member when the class object is instantiated but instead must do it outside scope of class
Tree_node* Tree_node::node_to_delete=NULL;
//setter method for static node_to_delete attribute which will be the node we currently plan to delete off our tree
void Tree_node::set_node_for_deletion(Tree_node *node){
	Tree_node::node_to_delete=node;
}
//getter method
Tree_node* Tree_node::get_node_for_deletion(){
	return Tree_node::node_to_delete;
}

//the node_exists class member function does a DFS-traversal of the tree and once it encounters a node with equivalent value
//to the integer argument sets the static member function node_to_delete to that node
void Tree_node::node_exists(int m){
	if (this->is_external_node()){
		return;
	}
	else if (this->visited_deletion==1){
		return;
	}
	else if (this->val==m && this->visited==0){
		Tree_node::set_node_for_deletion(this);
		return;
	}
	this->visited_deletion=1;
	this->left->node_exists(m);
	this->right->node_exists(m);
}

//get_sibling is the class method which returns pointer to a the caller object's "sibling", or the other node at the same depth which shares a common parent
Tree_node * Tree_node::get_sibling(){
	if (this==this->parent->right){
		return this->parent->left;
	}
	return this->parent->right;
}

//class member method which returns caller object's "left nephew": Nephew being the left child of a caller object's sibling
//will be utilized in delettion cases
Tree_node * Tree_node::get_left_nephew(){
	if (this==this->parent->right){
		return this->parent->left->left;
	}
	return this->parent->right->left;
}

//gets right nephew
Tree_node * Tree_node::get_right_nephew(){
	if (this==this->parent->right){
		return this->parent->left->right;
	}
	return this->parent->right->right;
}

//class method for left rotation
void Tree_node::left_rotation(){
	//if we left rotating a root, we must set the new root node's parent to null, 
	//the new root's left child to the root, and make the right child of the old root node the new root's old left child
	if (this->parent==NULL){
		this->parent=this->right;
		this->right=this->right->left;
		this->parent->parent=NULL;
		this->parent->left=this;
		return;
	}
	//non-root rotation: do same steps as for root but keeping tracking of the parent two levels up from the caller object (grandfather)
	else{
		this->right->parent=this->parent;
		this->parent=this->right;
		this->parent->parent->left=this->parent;
		this->right=this->parent->left;
		this->right->parent=this;
		this->parent->left=this;
	}
	
}

//class method for right rotation: Same as left, just in reverse
void Tree_node::right_rotation(){
	if (this->parent==NULL){
		this->parent=this->left;
		this->left=this->parent->right;
		this->left->parent=this;
		this->parent->right=this;
		this->parent->parent=NULL;
		return;
	}

	this->left->parent=this->parent;
	this->parent=this->left;
	this->parent->parent->left=this->left;
	this->left=this->right->left;
	this->parent->right=this;
}

//class method for choosing deletion cases:
	//we have 5 deletion cases:
		//Case 0: Our caller object is red: simply recolor and return. We are done
		//Case 1: Our caller object is black, and it's sibling is red
		//Case 2: Our caller object is black, it's sibling is black, and both of our caller object's nephews are black
		//Case 3: Our caller object is black, it's sibling is is black:
				//if our caller object is left child, and left nephew is black and right nephew is red, or vice versa, perform case 3 deletion
		//Case 4: Our caller object is black, it's sibling is black:
					//if our caller object is left child, and it's left sibling is red, perform case 4 deletion
	//Because of the importance the role the sibling node plays, we must pass it as a parameter. Furthermore, the sibling node pointer may be changed over the course of some deletion cases
void Tree_node::choose_deletion_case(Tree_node *sibling){
	if (this->color==1){
		this->recolor();
		return;
	}
	else if (this==this->parent->right && this->color==0){
			if (sibling->color==1){
				this->deletion_case_1(sibling);
			}
			else if (sibling->color==0){
				if (sibling->left->color==0 && sibling->right->color==0){
					this->deletion_case_2(sibling);
				}
				else if (sibling->right->color==1 && sibling->left->color==0){
					this->deletion_case_3(sibling);
				}
				else if (sibling->left->color==1){
					this->deletion_case_4(sibling);
				}
		}
	}
	else if (this==this->parent->left && this->color==0){
		if (sibling->color==1){
			this->deletion_case_1(sibling);
		}
		else if (sibling->color==0){
			if (sibling->left->color==0 && sibling->right->color==0){
				this->deletion_case_2(sibling);
			}
			else if (sibling->left->color==1 && sibling->right->color==0){
				this->deletion_case_3(sibling);
			}
			else if (sibling->right->color==1){
				this->deletion_case_4(sibling);
			}
		}
	}

}

//case when sibling is red
//do some rotation and recoloring and then decide on next case
//We must make sure to change the sibling node after doing rotations
void Tree_node::deletion_case_1(Tree_node *sibling){
	sibling->set_color(0);
	this->parent->set_color(1);
	if (this==this->parent->left){
		this->parent->left_rotation();
		sibling=this->parent->right;
	}
	else{
		this->parent->right_rotation();
		sibling=this->parent->left;
	}
	this->choose_deletion_case(sibling);
}
	
//case when we have a black node with a black sibling and both of it's children are black
//deletion case 2: recolor sibling to be red
//recolor parent to be black if it's red (re-balances black depth). We are done after doin this
//If parent is not black, set sibling to parent's sibling, use the parent and it's sibling as the caller object and parameter in call to choose_deletion_case: We are not yet done
void Tree_node::deletion_case_2(Tree_node *sibling){
	sibling->recolor();
	if (this->parent->color==1){
		this->parent->recolor();
		return;
	}
	else{
		sibling=this->parent->get_sibling();
		this->parent->choose_deletion_case(sibling);
	}
}
//case when our node is left child/black and the left nephew is red and vice versa and the sibling node is black
	//current node is lchild: recolor left nephew to black and recolor sibling to red, then do right rotation on newly red sibling
	//current node is rchild: recolor right nephew to red, and recolor sibling to red, and do left rotation on newly red sibling
	//proceed to deletion case 4 as we now have a sibling node that exists in the opposite case: left child has black left nephew and right red sibling
void Tree_node::deletion_case_3(Tree_node *sibling){
	if (this==this->parent->left){
		sibling->left->color=0;
		sibling->recolor();
		sibling->right_rotation();
		sibling=this->parent->right;
	}
	else if (this==this->parent->right){
		sibling->right->color=0;
		sibling->recolor();
		sibling->left_rotation();
		sibling=this->parent->left;
	}
	this->deletion_case_4(sibling);
}

//case when our node is right child/black and right newphew is black and vice versa
//reset sibling color to parent color
//set parent color to black
//perform recoloring on right nephew if caller object is left child and left rotation on parent node, and vice versa
//After this, we are done with deletion
void Tree_node::deletion_case_4(Tree_node *sibling){
	int parent_color=this->parent->color;
	sibling->set_color(parent_color);
	this->parent->set_color(0);
	if (this==this->parent->left){
		sibling->right->recolor();
		this->parent->left_rotation();
	}
	else if (this==this->parent->right){
		sibling->left->recolor();
		this->parent->right_rotation();
	}
}

//class method for deleting a node
//takes in root as argument
//we must keep track of the following nodes
		//the root: Each call to delete_node returns a pointer to the root as the root location may be modified because of deletion
		//node_to_balance: the inorder successor of the current node: Comes into play when caller object has two non-external node children
		//spliced: the right child of the node_to_balance node: Will be utilized as the caller object for choose_deletion_case
		//node: a pointer to static class attribute node_to_delete. We are only deleting one node at once, so we store the current node_to_delete
				//pointer as a static attribute, since each node object will share a common node_to_delete value
				//I found it easier to implement it this way than setting some kind of flag for the actual individual node we want delete
Tree_node* Tree_node::delete_node(Tree_node * root){
	Tree_node *node=Tree_node::node_to_delete;
	Tree_node *node_to_balance=NULL;
	Tree_node *spliced=NULL;
	Tree_node *sibling=NULL;
	//deleting a node that is red and it's children are 
	//if it's replacement is red or nill, we are done
	//if it's replacement is black, recolor
	if (node->color==1){
		if (node==node->parent->right){
			if (node->right->is_external_node() && node->left->is_external_node()){
					root=node->get_root();
					node->parent->right=node->right;
					delete(node);
					Tree_node::node_to_delete=NULL;
					return NULL;
				}
				//if we have one black non-external node left child: recolor replacement,replace node with non-nil child, make call to choose_deletion_case with the non-nil child
				//if our replacement is a red child: delete node off of heap, set the static class attribute node_to_delete to NULL, and return a pointer to the root
			else if (node->right->is_external_node() && !node->left->is_external_node()){
				node->parent->right=node->left;
				node->left->parent=node->parent;
				spliced=node->left;
				if (spliced->color==1){
					root=spliced->get_root();
					delete(node);
					Tree_node::node_to_delete=NULL;
					return root;
				}
				//to do: replacing red node with black node
				spliced->recolor();
				sibling=spliced->get_sibling();
				spliced->choose_deletion_case(sibling);
			}
			//same specifications as previous case, just for one external node right child
			else if (!node->right->is_external_node() && node->left->is_external_node()){
				node->parent->right=node->right;
				node->right->parent=node->parent;
				spliced=node->right;
				if (spliced->color==1){
					root=spliced->get_root();
					delete(node);
					Tree_node::node_to_delete=NULL;
					return root;
				}
				spliced->recolor();
				sibling=spliced->get_sibling();
				spliced->choose_deletion_case(sibling);
			}
			//two non-external node kids:
					//find in order successor (node_to_balance)
					//set spliced to the original node_to_balance right child
					//replace node with node_to_balance (but do not alter the location of the spliced pointer)
					//if node_to_balance is red: just recolor, delete node we want to delete (node pointer) off heap, and reset static class node_to_delete attribute to NULL
					//else if node_to_balance is black:
							//call choose_deletion_case with spliced as caller object and spliced's sibling as parameter
			else if (!node->right->is_external_node() && !node->left->is_external_node()){
				node->inorder_successor();
				node_to_balance=get_inorder_successor();
				spliced=node_to_balance->right;
				node->parent->right=node_to_balance;
				node->left->parent=node_to_balance;
				
				if (node->right->parent!=node_to_balance){
					node->right->parent=node_to_balance;
				}
				if (node->val < node->get_root()->val){
					node_to_balance->parent->right=node_to_balance->right;
					node_to_balance->left=node->left;
				}
				else if (node->val > node->get_root()->val){
					node_to_balance->parent->left=node_to_balance->left;
					node_to_balance->right=node->right;
				}
				node_to_balance->parent=node->parent;
				if (node_to_balance->color==1){
					root=node_to_balance->get_root();
					delete(node);
					Tree_node::node_to_delete=NULL;
					return root;
				}
				//to do: replacing red with black in order successor
				node_to_balance->recolor();
				sibling=spliced->get_sibling();
				spliced->choose_deletion_case(sibling);
			}

		}
		//same as red right child deletion cases, just reversed
		else if (node==node->parent->left){
			if (node->right->is_external_node() && node->left->is_external_node()){
				root=node->get_root();
				node->parent->left=node->left;
				delete(node);
				Tree_node::node_to_delete=NULL;
				return root;
			}
			else if (node->right->is_external_node() && !node->left->is_external_node()){
				node->parent->left=node->left;
				node->left->parent=node->parent;
				spliced=node->left;
				if (node_to_balance->color==1){
					root=spliced->get_root();
					delete(node);
					Tree_node::node_to_delete=NULL;
					return root;
				}
				//todo:
				spliced->recolor();
				sibling=spliced->get_sibling();
				spliced->choose_deletion_case(sibling);
			}
			else if (!node->right->is_external_node() && node->left->is_external_node()){
				node->parent->left=node->right;
				node->right->parent=node->parent;
				spliced=node->right;
				if (spliced->color==1){
					root=spliced->get_root();
					delete(node);
					Tree_node::node_to_delete=NULL;
					return root;
				}
				//todo:
				spliced->recolor();
				sibling=spliced->get_sibling();
				spliced->choose_deletion_case(sibling);
			}
			else if (!node->right->is_external_node() && !node->left->is_external_node()){
				node->inorder_successor();
				node_to_balance=get_inorder_successor();
				spliced=node_to_balance->right;
				node->parent->left=node_to_balance;
				node->left->parent=node_to_balance;
				if (node->right->parent!=node_to_balance){
					node->right->parent=node_to_balance;
				}
				if (node->val < node->get_root()->val){
					node_to_balance->parent->right=node_to_balance->right;
					node_to_balance->left=node->left;
				}
				else if (node->val > node->get_root()->val){
					node_to_balance->parent->left=node_to_balance->left;
					node_to_balance->right=node->right;
				}
				node_to_balance->parent=node->parent;
				if (node_to_balance->color==1){
					root=node_to_balance->get_root();
					delete(node);
					Tree_node::node_to_delete=NULL;
					return root;
				}

				//recolor replacement, proceed to next case based on replacement's original right child
				node_to_balance->recolor();
				sibling=spliced->get_sibling();
				spliced->choose_deletion_case(spliced);
			}
			}
		}
		//replacing a black node
		else if (node->color==0){
			//cases of deleting a root node:deleting singular root node: return NULL as tree has no nodes anymore

			if (node->is_root()){
				if (node->right->is_external_node() && node->left->is_external_node()){
					root=node;
					delete(node);
					return NULL;
				}
				//one non-external node left child: replace node with left child
						//if left chiild is red: recolor, delete node we wish to delete off of heap, set static node_to_delete attribute to NULL, return pointer to new root
						//if left child is black: skip recoloring, do same instruvtions as above
				else if (node->right->is_external_node() && !node->left->is_external_node()){
					if (node->left->color==1){
						node->left->parent=NULL;
						node->left->recolor();
						root=node->left;
						delete(node);
						Tree_node::node_to_delete=NULL;
						return root;
					}
			
					else{
						node->left->parent=NULL;
						root=node->left;
						delete(node);
						Tree_node::node_to_delete=NULL;
						return root;
					}
				}
				//reverse case as above case: just reversed to account for dealing with right child
				else if (!node->right->is_external_node() && node->left->is_external_node()){
				  	if (node->right->color==1){
						node->right->parent=NULL;
						node->right->recolor();
						root=node->right;
						delete(node);
						Tree_node::node_to_delete=NULL;
						return root;
					}
					else{
						node->right->parent=NULL;
						root=node->right;
						return root;
					}
				}
				//two non-external node children: inorder succcessor is red: Replace node with successor, recolor
												//inorder successor is black: perform previously specified deletions steps when node has two non-external node kids
				else if (!node->right->is_external_node() && !node->left->is_external_node()){
					node->inorder_successor();
					node_to_balance=get_inorder_successor();
					spliced=node_to_balance->right;
					if (node_to_balance->color==1){
						node_to_balance->left=node->left;
						node_to_balance->right=node->right;
						if (node->right->parent!=node_to_balance){
							node->right->parent=node_to_balance;
						}
						if (node->left->parent=node_to_balance){
							node->left->parent=node_to_balance;
						}
						node_to_balance->parent->left=spliced;
						spliced->parent=node_to_balance->parent;
						node_to_balance->parent=NULL;
						node_to_balance->recolor();
					}
					else{
						//todo replacing root with black node
						node_to_balance->left=node->left;
						node_to_balance->right=node->right;
						node->left->parent=node_to_balance;
						node->right->parent=node_to_balance;
						node_to_balance->parent->left=spliced;
						spliced->parent=node_to_balance->parent;
						node_to_balance->parent=NULL;
						sibling=spliced->get_sibling();
						spliced->choose_deletion_case(sibling);
					}
				}
			}
			//black non-root deletion cases: If replacement is red, just recolor replacement- you are done
				//if replacement is red: make call to choose_deletion_case with either replacement's right child (current node to delete has two non-external node children)
					//or make call to choose_deletion_case with non-external node child (when node only one non-external node child)
			else if (node==node->parent->right){
				if (node->right->is_external_node() && node->left->is_external_node()){
					spliced=node->right;
					node->parent->right=spliced;
					spliced->parent=node->parent;
					sibling=spliced->get_sibling();
					spliced->choose_deletion_case(sibling);
				}
				else if (node->right->is_external_node() && !node->left->is_external_node()){
					node->parent->right=node->left;
					node->left->parent=node->parent;
					spliced=node->left;
					if (spliced->color==1){
						spliced->recolor();
						root=spliced->get_root();
						delete(node);
						Tree_node::node_to_delete=NULL;
						return root;
					}
					sibling=spliced->get_sibling();
					spliced->choose_deletion_case(sibling);
				}
				else if (!node->right->is_external_node() && node->left->is_external_node()){
					node->parent->right=node->right;
					node->right->parent=node->parent;
					spliced=node->right;
					if (spliced->color==1){
						spliced->recolor();
						root=spliced->get_root();
						delete(node);
						Tree_node::node_to_delete=NULL;
						return root;
					}
				sibling=spliced->get_sibling();
				spliced->choose_deletion_case(sibling);
				}
				else if (!node->right->is_external_node() && !node->left->is_external_node()){
					node->inorder_successor();
					node_to_balance=get_inorder_successor();
					spliced=node_to_balance->right;
					node->parent->right=node_to_balance;
					if (node->right!=node_to_balance){
						node->right->parent=node_to_balance;
					}
					node->left!=node_to_balance;
					if (node->val < node->get_root()->val){
						node_to_balance->parent->right=node_to_balance->right;
						node_to_balance->left=node->left;
						node_to_balance->parent->right=spliced;
					}
					else if (node->val > node->get_root()->val){
						node_to_balance->parent->left=node_to_balance->left;
						node_to_balance->right=node->right;
						node_to_balance->parent->left=spliced;
					}
					node_to_balance->parent=node->parent;
					if (node_to_balance->color==1){
						node_to_balance->recolor();
						root=node_to_balance->get_root();
						delete(node);
						Tree_node::node_to_delete=NULL;
						return root;
					}
					sibling=spliced->get_sibling();
					spliced->choose_deletion_case(sibling);
				}

			}
		else if (node==node->parent->left){
			if (node->right->is_external_node() && node->left->is_external_node()){
				spliced=node->right;
				node->parent->left=spliced;
				spliced->parent=node->parent;
				sibling=spliced->get_sibling();
				spliced->choose_deletion_case(sibling);
			}
			else if (node->right->is_external_node() && !node->left->is_external_node()){
				node->parent->left=node->left;
				node->left->parent=node->parent;
				spliced=node->left;
				if (spliced->color==1){
					spliced->recolor();
					root=spliced->get_root();
					delete(node);
					Tree_node::node_to_delete=NULL;
					return root;
				}
				sibling=spliced->get_sibling();
				spliced->choose_deletion_case(sibling);
			}
			else if (!node->right->is_external_node() && node->left->is_external_node()){
				node->parent->left=node->right;
				node->right->parent=node->parent;
				spliced=node->right;
				if (spliced->color==1){
					spliced->recolor();
					root=spliced->get_root();
					delete(node);
					Tree_node::node_to_delete=NULL;
					return root;
				}
				sibling=spliced->get_sibling();
				spliced->choose_deletion_case(sibling);
			}
			else if (!node->right->is_external_node() && !node->left->is_external_node()){
				node->inorder_successor();
				node_to_balance=get_inorder_successor();
				spliced=node_to_balance->right;
				node->parent->left=node_to_balance;
				if (node->right!=node_to_balance){
					node->right->parent=node_to_balance;
				}
				if (node->val < node->get_root()->val){
					node_to_balance->parent->right=node_to_balance->right;
					node_to_balance->left=node->left;
					node_to_balance->parent->right=spliced;
				}
				else if (node->val > node->get_root()->val){
					node_to_balance->parent->left=NULL;
					node_to_balance->right=node->right;
					node_to_balance->parent->left=spliced;
				}
				node_to_balance->parent=node->parent;
				if (node_to_balance->color==1){
					node_to_balance->recolor();
					root=node_to_balance->get_root();
					delete(node);
					Tree_node::node_to_delete=NULL;
					return root;
				}
				sibling=spliced->get_sibling();
				spliced->choose_deletion_case(sibling);
			}
		}
	}
	delete(node);
	Tree_node::node_to_delete=NULL;
	root=spliced->get_root();
	return root;
}

//public get node method which returns the node we will be deleting
//pass in node value we wish to delete as parameter
//find node in tree that has value passed as parameter. The call to node_exists sets the static node_to_delete attribute to this node's location
//make call to the static class method delete_node with current tree's root as parameter. A call to this method returns the new root of the tree after deletion and re-balancing
//return pointer to new root we got from static class method
Tree_node* delete_node(int m,Tree_node *root){
	root->node_exists(m);
	//get_node(m,root);
	root=Tree_node::delete_node(root);
	return root;
}

//method to get inorder successor of current node
//sets the static class attribute successor
//if we've reached a NULL pointer, we couldn't find an inorder successor,
//if we have a right child and no left tree below that, the right child is our successor
//if we have a right child with a left tree, recursively call method with this->right->left
//if this->left is NULL we have reached the end of the left subtree in the right subtree, showing we have found the smallest value in the right subtree
//if we are in the left subtree, and our caller object's right child is not an external node it's successor is the right child
	//If we are in left subtree and caller object's right child IS an external node, it's successor is it's parent node
Tree_node * Tree_node::successor=NULL;
void Tree_node::inorder_successor(){
	if (this->is_root()){
		if (this->right->left->is_external_node()){
			Tree_node::successor=this->right;
			return;
		}
		else{
			return this->right->left->inorder_successor();
		}
	}

	else if (this==this->parent->left && this->val > this->get_root()->val){
		if (!this->left->is_external_node()){
			this->left->inorder_successor();
		}
		else{
			Tree_node::successor=this;
			return;
		}
	}
	else if (this==this->parent->right && this->val > this->get_root()->val){
		if (this->left->is_external_node()){
			Tree_node::successor= this;
		}
		else{
			return this->left->inorder_successor();
		}
	}
	else if (this->val < this->get_root()->val && !this->right->is_external_node()){
		Tree_node::successor=this->right;
		return;
	}
	else if (this->val < this->get_root()->val && this->right->is_external_node()){
		Tree_node::successor=this->parent;
		return;
	}
}

//getter method for getting the static successor attribute 
Tree_node * Tree_node::get_inorder_successor(){
	return Tree_node::successor;
}


//getter method for getting static is_balanced_tree, which specifies is current tree has equal black depth amongst all paths
bool Tree_node::get_balanced_boolean(){
	return Tree_node::is_balanced_tree;
}

//method for checking if red black tree properties are respected
//caller object must be root: invalid argument exception is thrown if not
void Tree_node::is_legitimate_red_black_tree(){
	if (!this->is_root()){
		throw invalid_argument("Must invoke this method with Tree node object that is a root");
	}

	//get black depth of the path from following all left pointers from root: If our tree follows the red black tree properties, all of it's paths will have the same black length as this
	int black_depth_of_one_path=get_black_depth(this);
	//make call to uniform black depth with root as caller object, which sets static boolean that specifies if tree has equal black depth
	this->uniform_black_depth(black_depth_of_one_path);
	//checks for existencec of double reds in any path: This would also violate the red black tree properties
	this->check_for_double_red();
	//print out a number of different messages based on tree's adherence to double red, uniform black depth, and black root properties
	if (Tree_node::get_balanced_boolean() && !Tree_node::get_double_red_boolean() && this->get_color()==0){
		cout<<"This is a balanced red black tree. All paths have equal black depth of length "<<black_depth_of_one_path<<" there are no double reds and the root is black";
	}
	else if (!Tree_node::get_balanced_boolean() && !Tree_node::get_double_red_boolean() && this->get_color()==0){
		cout<<"This is not a balanced red black tree. Not all paths have an equal black depth of length "<<black_depth_of_one_path<<" there are no double reds, and the root is black";
	}
	else if (Tree_node::get_balanced_boolean() && !Tree_node::get_double_red_boolean() && this->get_color()==1){
		cout<<"This is not a balanced red black tree. All paths have an equal black depth of length "<<black_depth_of_one_path<<" and there are no double reds but the root is red";
	}
	else if (!Tree_node::get_balanced_boolean() && !Tree_node::get_double_red_boolean() && this->get_color()==1){
		cout<<"This is not a balanced red black tree. Not all paths have an equal black depth of length "<<black_depth_of_one_path<<" there are no double reds and root is red";
	}
	else if (Tree_node::get_balanced_boolean() && Tree_node::get_double_red_boolean() && this->get_color()==0){
		cout<<"This is not a balanced red black tree. All paths have equal black depth of length "<<black_depth_of_one_path<<" but there is a double red and the root is black";
	}
	else if (!Tree_node::get_balanced_boolean() && Tree_node::get_double_red_boolean() && this->get_color()==0){
		cout<<"This is not a balanced red black tree. Not all paths have equal black depth of length "<<black_depth_of_one_path<<" ,there is a double red and the root is black";
	}

	else if (!Tree_node::get_balanced_boolean() && Tree_node::get_double_red_boolean() && this->get_color()==1){
		cout<<"This is not a balanced red black tree. Not all paths have equal black depth of length "<<black_depth_of_one_path<<" ,there is a double red and the root is red";
	}
	else if (Tree_node::get_balanced_boolean() && Tree_node::get_double_red_boolean() && this->get_color()==1){
		cout<<"This is not a balanced red black tree. All paths have equal black depth of length "<<black_depth_of_one_path<<" ,but there is a double red and the root is red";
	}

	//reset all static class attributes: A call to this method essentially indicates we are done adding or deleting to this tree so we reset all static attributes to their original values
	Tree_node::black_depth_count=0;
	Tree_node::is_balanced_tree=true;
	Tree_node::double_red_in_tree=false;

}







//return const * to a map on the heap( must be deleted at end)
//const so nothing in map can be modified-it's sole purpose is to link 0 to black, and 1 to red


