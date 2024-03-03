#include "rbtree.cpp"
#include <algorithm>
#include <string>
#include <utility>
#include <iostream>

template <class T>
unsigned int crc32(const T& val){
    unsigned int c=0, crc=0, i, j;
	unsigned int crc_table[256];
    size_t size;
    if (true){
    }
	for (i=0; i<256; i++){
        crc = i<<24;
        for (j = 0; j<8; j++)
            crc = (crc & 0x80000000) ? (crc << 1)^ 0x04C11DB7 : crc << 1;
        crc_table[i] = crc;
    }
    crc =0UL;
    for (;c<size; c++)
        crc = crc_table[(crc >>24) ^val[c]] ^ (crc << 8);
	while (size){
		crc = crc_table[(crc>>24) ^ (size & 0xFF)] ^ (crc << 8);
		size >>=8;
	}
	return (~crc);
}
//?string
template <>
unsigned int crc32(const std::string& val){
    unsigned int c=0, crc=0, i, j;
	unsigned int crc_table[256];
    size_t size;
    if (true){
        size = val.size();
    }
	for (i=0; i<256; i++){
        crc = i<<24;
        for (j = 0; j<8; j++)
            crc = (crc & 0x80000000) ? (crc << 1)^ 0x04C11DB7 : crc << 1;
        crc_table[i] = crc;
    }
    crc =0UL;
    for (;c<size; c++)
        crc = crc_table[(crc >>24) ^val[c]] ^ (crc << 8);
	while (size){
		crc = crc_table[(crc>>24) ^ (size & 0xFF)] ^ (crc << 8);
		size >>=8;
	}
	return (~crc);
}
//!vector
//!list



template<class K, class V>
class Map{
	size_t mapsz = 0;
	struct MapKeyOfT{
		unsigned int operator()(const pair<const K, V>& kv){
			//std:: cout << "this is kv: " << kv.first << " " << crc32(kv.first) << "\n";
			return crc32(kv.first);
		}
	};
	RBTree<K, pair<const K, V>, MapKeyOfT> rbtree;

public:
	typedef typename RBTree<K, pair<const K, V>, MapKeyOfT>::iter        iterator;
	typedef typename RBTree<K, pair<const K, V>, MapKeyOfT>::reviter  reviterator;
	typedef typename RBTree<K, pair<const K, V>, MapKeyOfT>::const_iterator citer; 


	//Default
	Map()
		:mapsz(0){}


	Map& operator=( const Map& other ){
		if (this == &other) {
			return *this;
		}
		clear();
		for (const auto& pair : other.rbtree) {
			insert(pair);
		}
		mapsz = other.mapsz;
		return *this;
	}


	

	//Element access:
	V& operator[](const K& key){
		pair<iterator, bool> ret = rbtree.Insert(make_pair(key, V()));
		return ret.first->second;
	}

	V& at(const K& key) {
		iterator it =  rbtree.begin();
		while((it->first)!=key)
			++it;
		if (!(it!=rbtree.end()))
			throw std::out_of_range("Key not found in map");
		return it->second; 
	}

	//Iterators:
	iterator begin(){
		return rbtree.begin();
	}

	iterator end(){
		return rbtree.end();
	}

	citer begin() const{
		return rbtree.begin();
	}

	citer end() const{
		return rbtree.end();
	}



	reviterator rbegin(){
		return rbtree.rbegin();
	}

	reviterator rend(){
		return rbtree.rend();
	}
	

	//Capacity:
	size_t size() const{
		return mapsz;
	}

	bool empty() const{
		if (mapsz==0) 
			return 1;
		return 0;
	}

	size_t max_size() const{
		return mapsz;
	}

	//Modifiers: 

	pair<iterator, bool> insert(const pair<const K, V>& kv){
		pair<iterator, bool> ans = rbtree.Insert(kv);
		if(ans.second)
			mapsz +=1;
		return rbtree.Insert(kv);
	}

	iterator erase(iterator pos){
		std::cout << mapsz << "mapsz\n";
		mapsz-=1;
		return rbtree.Erase(pos);
	}

	void clear(){
		iterator pos = rbtree.begin();
		while (pos!=rbtree.end()){
			std::cout << "puff\n";
			std::cout << "erase elem " << pos->first << " " << "\n"; 
			pos = rbtree.Erase(pos);
			std::cout << "lifed over erase first\n";
			mapsz-=1;
			std::cout << "mapsz overlived\n";
		}
		std::cout << "while end\n";
		mapsz=0;		
	}
		
	void swap( Map& other ){
		std::swap(mapsz, other.mapsz);
		rbtree.swap(other.rbtree);
	}


	//Lookup 




	//Non-member functions ==,!=,<,<=,>,>=,
	bool operator==(const Map& another) {
		if (size() != another.size()) {
			return false;
		}
		citer ant_it = (another.begin());
		citer it = begin();

		while (ant_it != (another.end())) {
			if (ant_it->first != it->first || ant_it->second != it->second) {
				return false;
			}
			++it;
			++ant_it;
		}
		return true;
	}

	bool operator!=(const Map& another) {
		if (size() != another.size()) {
			return true;
		}
		citer ant_it = (another.begin());
		citer it = begin();

		while (ant_it != (another.end())) {
			if (ant_it->first != it->first || ant_it->second != it->second) {
				return true;
			}
			++it;
			++ant_it;
		}
		return false;
	}

	





	~Map(){}
	
};




int main()
{	
	
    
	Map<std::string, std::string> dict, dict2;
	dict.insert(make_pair("sort", ""));
	dict.insert(make_pair("string", "ַ"));
	dict.insert(make_pair("count", ""));
	dict.insert(make_pair("string", "(ַ)")); 
	Map<std::string, std::string>::iterator ite = dict.begin();

	while (ite != dict.end()){
		cout << ite->first << ":" << ite->second << " \n" << endl;
		++ite;
	}
    

	//std::cout << dict.size();
	//dict.clear();
	std::cout << dict.size();

	dict2.insert(make_pair("1", "cats"));
	dict2.insert(make_pair("20", "dogs"));
	dict2.insert(make_pair("550", "frogs"));
	dict2.insert(make_pair("meme", "foxes")); 
	//dict.swap(dict2);
	if (dict==dict2){
		std::cout << "AAA\n" ; 
	}
	dict = dict2;
	if (dict==dict2){
		std::cout << "AAA\n" ; 
	}
	Map<std::string, std::string>::iterator it = dict.begin();
	it = dict.erase(it);
	std::cout << dict.at("20") <<" at \n";
	
	std::cout << "outputhing new map\n";
	while (it != dict.end()){
		cout << it->first << ":" << it->second << endl;
		++it;
	}
	std:: cout << endl;

	std::cout << dict["1"] << " " << dict["20"];
	return 0;
};
