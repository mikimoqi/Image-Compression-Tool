/**
 *
 * Balanced Quad Tree (pa3)
 *
 */

#ifndef _QTREE_H_
#define _QTREE_H_

#include <utility>
#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "var.h"
#include <queue>
using namespace std;
using namespace cs221util;

/**
 * Quad Tree: This is a structure used in decomposing an image
 * into squares of similarly colored pixels.
 *
 * You should not remove anything from this class definition, but
 * you will find it helpful to add your own private helper functions to it.
 *
 * This file will be used for grading.
 */

class QTree {
 private:

  /**
   * The Node class is private to the tree class via the principle of
   * encapsulation---the end user does not need to know our node-based
   * implementation details.
   * given for PA3
   */
  class Node {
  public:
    // Node constructors
    Node(PNG & imIn, pair<int,int> ul, int size, Node *par);

    pair<int,int> upLeft;	// upper-left coords of Node's subimage
    int size;			// side length of Node's subimage in pixels
    Node * parent; // parent
    RGBAPixel avg; // Average color of Node's subimage
    double var;	   // sum of variance of Node's subimage
    Node * nw;	   // ptr to nw subtree
    Node * ne;	   // ptr to ne subtree
    Node * sw;	   // ptr to sw subtree
    Node * se;	   // ptr to se subtree
  };


 public:

  /* =============== start of given functions ====================*/

  /**
   * QTree destructor.
   * Destroys all of the memory associated with the
   * current QTree. This function should ensure that
   * memory does not leak on destruction of a QTree.
   * 
   * @see QTree.cpp
   */
  ~QTree();

  /**
   * Copy constructor for a QTree.
   * Since QTree allocate dynamic memory (i.e., they use "new", we
   * must define the Big Three). This uses your implementation
   * of the copy funtion.
   * @see QTree.cpp
   *
   * @param other = the QTree we are copying.
   */
  QTree(const QTree & other);

  /**
   * Overloaded assignment operator for QTree. 
   * Part of the Big Three that we must define because the class
   * allocates dynamic memory. This uses your implementation
   * of the copy and clear funtions.
   *
   * @param rhs = the right hand side of the assignment statement.
   */
  QTree & operator=(const QTree & rhs);

  /* =============== end of given functions ====================*/

  /* =============== public PA3 FUNCTIONS =========================*/

  /**
   * Constructor that builds a QTree out of the given PNG.
   * 
   * The QTree represents the subimage from (0,0) to (2^k,2^k) for the
   * largest possible integer k.
   * Every node corresponds to a square of pixels in the original PNG,
   * represented by an (x,y) pair for the upper left corner of the
   * square and a size for the number of pixels on the square's side.
   *
   * The node's four children correspond to a partition
   * of the node's square into four equal-size quadrants.
   *
   *  +----+----+
   *  | nw | ne |
   *  |    |    |
   *  +----+----+
   *  | sw | se |
   *  |    |    |
   *  +----+----+
   *
   * The QTree constructor takes an image and an approximate bound
   * on the number of leaves in the tree. As long as the number of
   * leaves in the QTree is less than the leafBound, the constructor
   * splits the leaf with the highest variance (see "split" below).
   * The four children of the original leaf become new leaves of the
   * QTree and the original leaf becomes an internal node (no longer
   * a leaf).
   *
   * If the QTree is supposed to be balanced, the creation of these
   * new leaves may force other nodes to split, which in turn may
   * force other splits...
   *
   * The QTree constructor can take a frameColor (see "write" below)
   * and a flag that tells the constructor whether or not to produce
   * a balanced quad tree.
   */
  QTree(PNG & imIn, int leafBound, RGBAPixel frameColor, bool bal=false);
  QTree(PNG & imIn, int leafBound, bool bal=false);

  /**
   * Write renders a PNG image by coloring one square for each leaf
   * (leaf.size X leaf.size pixels with origin leaf.upLeft) of the
   * QTree using the leaf's average color for the entire square.
   * If the QTree has a frameColor, the 1-pixel border in each leaf
   * square is colored with the frameColor. 
   * It then writes the image to file fileName and 
   * returns any error returned by the write to file.
   * return(im.writeToFile(fileName))
   * You may want a recursive helper function for this one.
   */
  bool write(string const & fileName);

  /* =============== end of public PA3 FUNCTIONS =========================*/

 private:
  /*
   * Private member variables.
   *
   * You must use these as specified in the spec and may not rename them.
   * You may add more if you need them.
   */
  Node* root;		// ptr to the root of the QTree
  int numLeaf;		// number of leaves in quad tree
  PNG im;		// image
  int leafBound;	// approx. upper bound on number of leaves in QTree
  bool balanced=false;	// true=create balanced QTree
  bool drawFrame=false;	// true=draw 1 pixel frame around every leaf square
  RGBAPixel frameColor;	// color of frame if drawn
  

  struct cmp {
    bool operator() (Node* left, Node* right) { return  left->var < right->var;}
  };
  priority_queue<Node*, vector<Node*>, cmp> q;


  /* =================== private PA3 functions ============== */

  /**
   * Private helper function for the constructor. Splits the given
   * node t and, in the balanced version, may recursively split
   * neighbors of the parent of t to preserve balance.
   * @param t = ptr to a node to be split.
   */
  void split(Node * t);

  /**
   * Private helper function that returns true if t points to a leaf
   * of the QTree.
   * @param t = ptr to a node.
   */
  bool isLeaf( Node *t );

  /**
   * Private helper function used to implement balanced QTrees.
   * Return ptr to the quad tree node that is the same size as t and
   * is N,S,E,or W of node t.
   * Return NULL if that node doesn't exist in the QTree.
   * @param t = ptr to a node
   */
  Node *NNbr(Node * t);
  Node *SNbr(Node * t);
  Node *ENbr(Node * t);
  Node *WNbr(Node * t);

  /**
   * Destroys all dynamically allocated memory associated with the
   * current QTree class. Complete for PA3.
   * You may want a recursive helper function for this one.
   */
  void clear();

  /**
   * Copies the "other" QTree into the current QTree.
   * Does not free any memory. Called by copy constructor and op=.
   * You may want a recursive helper function for this one.
   * @param other The QTree to be copied.
   */
  void copy(const QTree & other);

  Node * copyNode(Node* parent, const Node* copyNode);

  void writeLeaf(PNG & img, Node* curr);

  void clearNode(Node* curr);

  /* =================== end of private PA3 functions ============== */
  
};
  
#endif

