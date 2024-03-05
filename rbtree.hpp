#include <iostream>
#include <utility>
// RED - 0
// BLACK - 1

using namespace std;

template<typename T>
struct RBTreeNode
{
	RBTreeNode<T>* lChild;
	RBTreeNode<T>* rChild;
	RBTreeNode<T>* parent;
	T data;
	bool clr;

    //Constructor
	RBTreeNode(const T& data)
		: lChild(nullptr), rChild(nullptr), parent(nullptr), data(data), clr(0){}
};

template<typename T, typename Ref, typename Ptr>
class RBTreeIterator
{

public:
	typedef RBTreeNode<T>                 Node;
	typedef RBTreeIterator<T, Ref, Ptr>   Self;
	Node*                                 node;

	RBTreeIterator(Node* node)
		: node(node){}
	RBTreeIterator(const RBTreeIterator<T, T&, T*>& it)
		: node(it.node) {}

	Ref operator*()  { return node->data; }
	Ptr operator->() { return &node->data; }
	bool operator!=(const Self& self) { return node != self.node; }
	bool operator==(const Self& self) { return node == self.node; }
	Self& operator++() {
        if (node->rChild) {
			Node* subLeft = node->rChild;
			while (subLeft->lChild){
				subLeft = subLeft->lChild;
			}
			node = subLeft;
		}
		else{
			Node* cur = node;
			Node* parent = cur->parent;
			while (parent && cur == parent->rChild){
				cur = parent;
				parent = parent->parent;
			}
            node = parent;
		}
		return *this;
	}
	Self& operator--() {
		if (node->lChild) {
            Node* subRight = node->lChild;
			while (subRight->rChild) {
                subRight = subRight->rChild;
			}
			node = subRight;
		}
		else {
			Node* cur = node;
			Node* parent = cur->parent;
			while (parent && cur == parent->lChild){
				cur = parent;
				parent = parent->parent;
			}
			node = parent;
		}
		return *this;
	}
};

template<typename T, typename Ref, typename Ptr>
class RBTreeRevIterator
{

public:
	typedef RBTreeNode<T>                    Node;
	typedef RBTreeRevIterator<T, Ref, Ptr>   Self;
	Node*                                    node;

	RBTreeRevIterator(Node* node)
		: node(node){}
	RBTreeRevIterator(const RBTreeRevIterator<T, T&, T*>& it)
		: node(it.node) {}

	Ref operator*()  { return node->data; }
	Ptr operator->() { return &node->data; }
	bool operator!=(const Self& self) { return node != self.node; }
	bool operator==(const Self& self) { return node == self.node; }
	Self& operator--() {
        if (node->rChild) {
			Node* subLeft = node->rChild;
			while (subLeft->lChild){
				subLeft = subLeft->lChild;
			}
			node = subLeft;
		}
		else{
			Node* cur = node;
			Node* parent = cur->parent;
			while (parent && cur == parent->rChild){
				cur = parent;
				parent = parent->parent;
			}
            node = parent;
		}
		return *this;
	}
	Self& operator++() {
		if (node->lChild) {
            Node* subRight = node->lChild;
			while (subRight->rChild) {
                subRight = subRight->rChild;
			}
			node = subRight;
		}
		else {
			Node* cur = node;
			Node* parent = cur->parent;
			while (parent && cur == parent->lChild){
				cur = parent;
				parent = parent->parent;
			}
			node = parent;
		}
		return *this;
	}
};



template<class K, class T, class KeyOfT>
class RBTree
{
	typedef RBTreeNode<T>            Node;
	Node* root                  = nullptr;
public:
	class StackIterator;

	//typedef T                  value_type;
    //typedef size_t             size_type;
    //typedef ptrdiff_t         difference_type;
    //typedef value_type &       reference;
    //typedef value_type *       pointer;
    //typedef RBTreeIterator     iter;

	

	void Destroy(Node* root){
		if (root == nullptr){
			return;
		}
		Destroy(root->lChild);
		Destroy(root->rChild);
		delete root;
	}

	
	typedef RBTreeIterator<T, T&, T*>                             iter;
	typedef RBTreeIterator<T, const T&, const T*>       const_iterator;
	typedef RBTreeRevIterator<T, T&, T*>                       reviter;
	typedef RBTreeRevIterator<T, const T&, const T*> const_reviterator;


	iter Erase(iter pos){
			Node* node = pos.node;
			if (node == nullptr)
				return end();
			// 0 cld
			if (node->lChild == nullptr && node->rChild == nullptr) {
				std::cout << "0 child\n";
				if (node->parent == nullptr) { //root
					std::cout << "this\n";
					delete node;
					std::cout << "this\n";
					root = nullptr;
					
					return end();
				} 
				else {
					Node* parent = node->parent;
					if (parent->lChild == node) 
						parent->lChild = nullptr;
					else 
						parent->rChild = nullptr;
					delete node;
					return iter(parent);
				}
				
				
			}
			// 1 chld
			Node* child = nullptr;
			if (node->lChild != nullptr) 
				child = node->lChild;
			else 
				child = node->rChild;

			if (child != nullptr) 
				child->parent = node->parent; // усыновляем внука
			
			if (node->parent == nullptr)  //сиротка....
				root = child;
			else {
				Node* parent = node->parent;
				if (parent->lChild == node) 
					parent->lChild = child;
				else 
					parent->rChild = child;
			}
			// 2 cld
			if (node->lChild != nullptr && node->rChild != nullptr) {
				std::cout << "2 child\n";
				Node* replace_node = node->rChild;
				while (replace_node->lChild != nullptr) {
					replace_node = replace_node->lChild;
				}
				//node->data = std::move(replace_node->data);
				//node->data = replace_node->data;
				//node = replace_node;
				replace_node-> parent = node->parent;
				replace_node-> rChild = node->rChild;
				replace_node-> rChild = node-> lChild;
				(replace_node->parent)->lChild = nullptr;
			}
			delete node;
			//сурово балансим....
			Node* uncle = nullptr;
			Node* parent = node->parent;
			Node* sibling = nullptr;
			while (parent != nullptr && parent->clr == 0) {
				if (parent == parent->parent->lChild) {
					uncle = parent->parent->rChild;
					sibling = uncle ? uncle->lChild : uncle->rChild;
					if (sibling != nullptr && sibling->clr == 0) {
						// red uncle
						parent->clr = 1;
						uncle->clr = 1;
						sibling->clr = 1;
						node = parent->parent;
						parent = node->parent;
					} else {
						if (node == parent->rChild) {
							// node rCh
							node = parent;
							RotateL(node);
							parent = node->parent;
							uncle = parent->rChild;
							sibling = uncle ? uncle->lChild : uncle->rChild;
						}
						// node lCh
						parent->clr = 1;
						uncle->clr = 0;
						RotateR(parent);
						node = parent->parent;
					}
				} else {
					uncle = parent->parent->lChild;
					sibling = uncle ? uncle->rChild : uncle->lChild;
					if (sibling != nullptr && sibling->clr == 0) {
						// red uncle
						parent->clr = 1;
						uncle->clr = 1;
						sibling->clr = 1;
						node = parent->parent;
						parent = node->parent;
					} else {
						if (node == parent->lChild) {
							// node lCh
							node = parent;
							RotateR(node);
						parent = node->parent;
						uncle = parent->lChild;
						sibling = uncle ? uncle->lChild : uncle->rChild;
					}
					// Node rCh
					parent->clr = 1;
					uncle->clr = 0;
					RotateL(parent);
					node = parent->parent;
				}

			}

		}
		if (node != nullptr) {
			node->clr = 0;
		}
		return begin();
	}


	// всякое бегиновоэдное 
	iter begin(){
		Node* cur = root;
		while (cur && cur->lChild){
			cur = cur->lChild;
		}
		return iter(cur);
	}

	iter end(){ return iter(nullptr); }

	reviter rbegin(){
		Node* cur = root;
		while (cur && cur->rChild){
			cur = cur->rChild;
		}
		return iter(cur);
	}

	reviter rend(){
		Node* cur = root;
		while (cur && cur->lChild){
			cur = cur->lChild;
		}
		return iter(cur->parent);
	}

	const_iterator begin() const{
		Node* cur = root;
		while (cur && cur->lChild){
			cur = cur->lChild;
		}
		return const_iterator(cur);
	}

	const_iterator end() const{ return const_iterator(nullptr); }

	const_reviterator rbegin() const{
		Node* cur = root;
		while (cur && cur->rChild){
			cur = cur->rChild;
		}
		return iter(cur);
	}

	const_reviterator rend() const{
		Node* cur = root;
		while (cur && cur->lChild){
			cur = cur->lChild;
		}
		return iter(cur->parent);
	}
	
	
	//кошмар и тихий ужас...но прикольно)
	pair<iter, bool> Insert(const T& data){
		if (root == nullptr){
			root = new Node(data);
			root->clr = 1;
			return make_pair(iter(root), true);
		}

		KeyOfT kot;
		Node* parent = nullptr;
		Node* cur = root;
		while (cur){    
			if (kot(cur->data) < kot(data)){
				parent = cur;
				cur = cur->rChild;
			}
			else if (kot(cur->data) > kot(data)){
				parent = cur;
				cur = cur->lChild;
			}
			else{
				return make_pair(iter(cur), false);
			}
		}

		cur = new Node(data);
		Node* newnode = cur;
		if (kot(parent->data) > kot(data)) 
			parent->lChild = cur;
		else
            parent->rChild = cur;
		cur->parent = parent;
		while (parent && parent->clr == 0){
			Node* grandfather = parent->parent;
			if (grandfather->lChild == parent){
				Node* uncle = grandfather->rChild;
				if (uncle && uncle->clr == 0){
					parent->clr = 1;
					uncle->clr = 1;
					grandfather->clr = 0;					
					cur = grandfather;
					parent = cur->parent;
				}
				else {
					//     g
					//   p   u
					// c 
					if (cur == parent->lChild){
						RotateR(grandfather);
						parent->clr = 1;
						grandfather->clr = 0;
					}
					else{
						//     g
						//   p   u
						//     c
						RotateL(parent);
						RotateR(grandfather);
						cur->clr = 1;
						//parent->clr = 0;
						grandfather->clr = 0;
					}
					break;
				}
			}
			else { // grandfather->rChild == parent
				//    g
				//  u   p
				//        c
				Node* uncle = grandfather->lChild;
				if (uncle && uncle->clr == 0){
					parent->clr = 1;
					uncle->clr = 1;
					grandfather->clr = 0;

					cur = grandfather;
					parent = cur->parent;
				}
				else { 
					//    g
					//  u   p
					//        c
					if (cur == parent->rChild){
						RotateL(grandfather);
						grandfather->clr = 0;
						parent->clr = 1;
					}
					else{
						//    g
						//  u   p
						//    c
						RotateR(parent);
						RotateL(grandfather);
						cur->clr = 1;
						grandfather->clr = 0;
					}
					break;
				}
			}
		}
		root->clr = 1;
		return make_pair(iter(newnode), true);;
	}

	Node* Find(const K& key){
		Node* cur = root;
		KeyOfT kot;
		while (cur){
			if (kot(cur->data) < key)
				cur = cur->rChild;
			else if (kot(cur->data) > key)
				cur = cur->lChild;
			else
				return cur;
		}
		return nullptr;
	}


	iter lower_bound(const K& key ){
		Node* cur = root;
		KeyOfT kot;
		while (cur){
			if (kot(cur->data) < key)
				cur = cur->rChild;
			else if (kot(cur->data) > key)
				return cur;
			else
				return cur;
		}
		return end();
	}

	iter upper_bound( const K& key ){
		Node* cur = root;
		KeyOfT kot;
		while (cur){
			if (kot(cur->data) < key)
				cur = cur->rChild;
			else if (kot(cur->data) > key)
				return cur;
			else
				cur= cur->rChild;
		}
		return end();
	}

	std::pair<iter, iter> equal_range( const K& key ){
		return make_pair(lower_bound(key), upper_bound(key));
	}

	const_iterator lower_bound(const K& key ) const{
		Node* cur = root;
		KeyOfT kot;
		while (cur){
			if (kot(cur->data) < key)
				cur = cur->rChild;
			else if (kot(cur->data) > key)
				return cur;
			else
				return cur;
		}
		return end();
	}

	const_iterator upper_bound( const K& key ) const{
		Node* cur = root;
		KeyOfT kot;
		while (cur){
			if (kot(cur->data) < key)
				cur = cur->rChild;
			else if (kot(cur->data) > key)
				return cur;
			else
				cur= cur->rChild;
		}
		return end();
	}

	std::pair<const_iterator, const_iterator> equal_range( const K& key ) const{
		return make_pair(lower_bound(key), upper_bound(key));
	}


	void swap(RBTree& other) {
    	//using std::swap;
    	std::swap(root, other.root);
	}

	bool IsBalance(){
		if (root && root->clr == 0){
			cout << "Root must be black" << endl;
			return false;
		}
		int blckNds = 0; //counting the black nodes
		Node* cur = root;
		while (cur){
			if (cur->clr == 1)
				++blckNds;
			cur = cur->lChild;
		}
		return Check(root, 0, blckNds);
	}

	int Height(){
		return Height(root);
	}

	
	int Height(Node* root){
		if (root == NULL)
			return 0;
		int leftH = Height(root->lChild);
		int rightH = Height(root->rChild);
		return leftH > rightH ? leftH + 1 : rightH + 1;
	}

    bool Check(Node* root, int blackNum, int blckNds) {    
		if (root == nullptr) {
			if (blckNds != blackNum) { //somewhere not so much blck nodes
				cout << "black nods difference error\n" << endl;
				return false;
			}
			return true;
		}

		if (root->clr == 1)//root -black
			++blackNum;

		if (root->clr == 0 && root->parent && root->parent->clr == 0){ //root - red and his parent red
			cout << "Red node has red parent error\n" << endl;
			return false;
		}

		return Check(root->lChild, blackNum, blckNds) && Check(root->rChild, blackNum, blckNds);
	}

	void RotateL(Node* parent) {
		Node* ChR = parent->rChild;
		Node* ChRL = ChR->lChild;
		parent->rChild = ChRL;
		if (ChRL)
			ChRL->parent = parent;
		Node* ppnode = parent->parent;
		ChR->lChild = parent;
		parent->parent = ChR;
		if (ppnode == nullptr){
			root = ChR;
			root->parent = nullptr;
		}
		else{
			if (ppnode->lChild == parent){
				ppnode->lChild = ChR;
			}
			else{
				ppnode->rChild = ChR;
			}
			ChR->parent = ppnode;
		}
	}

	void RotateR(Node* parent){
		Node* ChL = parent->lChild;
		Node* ChLR = ChL->rChild;
		parent->lChild = ChLR;
		if (ChLR)
			ChLR->parent = parent;
		Node* ppnode = parent->parent;
		ChL->rChild = parent;
		parent->parent = ChL;
		if (parent == root){
			root = ChL;
			root->parent = nullptr;
		}
		else{
			if (ppnode->lChild == parent)
				ppnode->lChild = ChL;
			else
				ppnode->rChild = ChL;
			ChL->parent = ppnode;
		}
	}

	//Destructor
	~RBTree(){
		Destroy(root);
		root = nullptr;
	}


};
