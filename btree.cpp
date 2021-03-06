#include <iostream>

/**
 * @brief clase btree 
 * 
 * @tparam T 
 * @tparam 2 : numero de keys aceptados (si =2 entonces 3 childs)
 */
template <class T, int BTREE_ORDER = 2> class btree {
  enum state {
    BT_OVERFLOW,
    BT_UNDERFLOW,
    NORMAL,
  };
  struct node {
    int count;
    T data[BTREE_ORDER + 1];
    node *children[BTREE_ORDER + 2];

    node() {
      count = 0;
      for (int i = 0; i < BTREE_ORDER + 2; i++) {
        children[i] = NULL;
      }
    }
    void insert_in_node(int pos, const T &value) {
      int j = count;
      while (j > pos) {
        data[j] = data[j - 1];
        children[j + 1] = children[j];
        j--;
      }
      data[j] = value;
      children[j + 1] = children[j];

      count++;
    }

    bool is_overflow() { return count > BTREE_ORDER; }
  };
  node *root;

public:
  btree() { root = new node; }

  /**
   * @brief funciones insert
   * 
   * @param value 
   */

  void insert(const T &value) {
    int state = insert(root, value);
    if (state == BT_OVERFLOW) {
      split_root(root, value);
    }
  }

  int insert(node *ptr, const T &value) {

    int pos = 0;
    while (pos < ptr->count && ptr->data[pos] < value) {
      pos++;
    }
    if (ptr->children[pos] != NULL) {
      // long page_id = children[pos];

      int state = insert(ptr->children[pos], value);
      if (state == BT_OVERFLOW) {
        split(ptr, pos);
      }
    } else {
      ptr->insert_in_node(pos, value);
    }
    return ptr->is_overflow() == true ? BT_OVERFLOW : NORMAL;
  }

/**
 * @brief Caso Split nodo normal
 * 
 * @param ptr 
 * @param pos 
 */
  void split(node *ptr, int pos) {
    node *node_in_overflow = ptr->children[pos];
    node *child1 = node_in_overflow;
    child1->count = 0;
    node *child2 = new node();

    int iter = 0;
    int i;
    for (i = 0; iter < BTREE_ORDER / 2; i++) {
      child1->children[i] = node_in_overflow->children[iter];
      child1->data[i] = node_in_overflow->data[iter];
      child1->count++;

      iter++;
    }
    child1->children[i] = node_in_overflow->children[iter];

    ptr->insert_in_node(pos, node_in_overflow->data[iter]);

    iter++; // the middle element

    for (i = 0; iter < BTREE_ORDER + 1; i++) {
      child2->children[i] = node_in_overflow->children[iter];
      child2->data[i] = node_in_overflow->data[iter];
      child2->count++;

      iter++;
    }
    child2->children[i] = node_in_overflow->children[iter];

    ptr->children[pos] = child1;
    ptr->children[pos + 1] = child2;
  }

/**
 * @brief Caso split del root
 * 
 * @param ptr 
 * @param value 
 */

  void split_root(node *ptr, const T &value) {
    node *node_in_overflow = ptr;
    node *child1 = new node();
    node *child2 = new node();

    int pos = 0;
    int iter = 0;
    int i;
    for (i = 0; iter < BTREE_ORDER / 2; i++) {
      child1->children[i] = node_in_overflow->children[iter];
      child1->data[i] = node_in_overflow->data[iter];
      child1->count++;

      iter++;
    }
    child1->children[i] = node_in_overflow->children[iter];
    iter++; // the middle element
    for (i = 0; iter < BTREE_ORDER + 1; i++) {
      child2->children[i] = node_in_overflow->children[iter];
      child2->data[i] = node_in_overflow->data[iter];
      child2->count++;

      iter++;
    }
    child2->children[i] = node_in_overflow->children[iter];

    ptr->children[pos] = child1;
    ptr->data[0] = node_in_overflow->data[BTREE_ORDER / 2];
    ptr->children[pos + 1] = child2;
    ptr->count = 1;
  }

/**
 * @brief Funcion para buscar una key
 * 
 * @param value 
 * @return true 
 * @return false 
 */
  bool find(const T &value) { 
    auto result = find(value,this->root);
    return((result)? true:false);
   }

  bool find(const T &object,node *other){
    int pos = 0;
    if (other->children[0] != 0) {
      while (pos < other->count && other->data[pos] <= object) {
        pos++;
      }
      return find(object,other->children[pos]);
    }else {
      while (pos < other->count && other->data[pos] < object) {
        pos++;
      }
      if(other->data[pos]==object){
        return true;
      }
      return false;
  }
  }

/**
 * @brief Funcion para imprimir el arbol por pasos
 * 
 */
  void print() {
    print(root, 0);
    std::cout << "________________________\n";
  }
  void print(node *ptr, int level) {
    if (ptr) {
      int i;
      for (i = ptr->count - 1; i >= 0; i--) {
        print(ptr->children[i + 1], level + 1);

        for (int k = 0; k < level; k++) {
          std::cout << "    ";
        }
        std::cout << ptr->data[i] << "\n";
      }
      print(ptr->children[i + 1], level + 1);
    }
  }
};


int main(){

  btree<int> bt;
    std::string values = "zxcnrat"; //122,120,99,110,114,97,116
    for(auto c : values) {
       bt.insert((int)c);
       bt.print();
    }
    bt.print();

    (bt.find(100))?std::cout<<"\nel numero que busco esta en el arbol":std::cout<<"\nel  numero que busco no esta en el arbol";
}
