/**
 *
 * Balanced Quad Tree (pa3)
 *
 * This file will be used for grading.
 *
 */

#include "QTree.h"

// Return the biggest power of 2 less than or equal to n
int biggestPow2(int n) {
  if( n < 1 ) return 0;
  int v = 1;
  while( v <= n ) v <<= 1;
  return v >> 1;
}

QTree::Node::Node(PNG & im, pair<int,int> ul, int sz, Node *par)
  :upLeft(ul),size(sz),parent(par),nw(NULL),ne(NULL),sw(NULL),se(NULL)
{
  var = varAndAvg(im,ul,size,avg);
}

QTree::~QTree(){
  clear();
}

QTree::QTree(const QTree & other) {
  copy(other);
}


QTree & QTree::operator=(const QTree & rhs){
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}


QTree::QTree(PNG & imIn, int leafB, RGBAPixel frameC, bool bal)
  : leafBound(leafB), balanced(bal), drawFrame(true), frameColor(frameC)
{
  im = imIn;
  root = new Node(imIn, make_pair(0,0), biggestPow2(imIn.width()), NULL);
  numLeaf = 1;
  q.push(root);
  while (numLeaf < leafBound) {
    Node* curr = q.top();
    q.pop();
    split(curr);
  }

  /* YOUR CODE HERE */
  
}


QTree::QTree(PNG & imIn, int leafB, bool bal)
  : leafBound(leafB), balanced(bal), drawFrame(false)
{
  im = imIn;
  root = new Node(imIn, make_pair(0,0), biggestPow2(imIn.width()), NULL);
  numLeaf = 1;
  q.push(root);
  while (numLeaf < leafBound) {
    Node* curr = q.top();
    q.pop();
    split(curr);
  }
  /* YOUR CODE HERE */
  
}


bool QTree::isLeaf( Node *t ) {
  
  /* YOUR CODE HERE */
  return ((t->nw == NULL) && (t->ne == NULL) && (t->sw == NULL) && (t->se == NULL));
  
}
  
void QTree::split( Node *t ) {
  /*
  if (t == NULL)
    return;
  if (numLeaf >= leafBound)
    return;
  */
  if (!isLeaf(t))
    return;
  /* YOUR CODE HERE */
  int half = t->size / 2;
  Node* northw = new Node(im, t->upLeft, half, t);
  Node* northe = new Node(im, make_pair(t->upLeft.first + half, t->upLeft.second), half, t);
  Node* southw = new Node(im, make_pair(t->upLeft.first, t->upLeft.second + half), half, t);
  Node* southe = new Node(im, make_pair(t->upLeft.first + half, t->upLeft.second + half), half, t);
  t->nw = northw;
  t->ne = northe;
  t->sw = southw;
  t->se = southe;
  q.push(northw);
  q.push(northe);
  q.push(southw);
  q.push(southe);
  numLeaf += 3;

  if (balanced && t != root){
    if (t == t->parent->nw){
      if (NNbr(t->parent) != NULL && isLeaf(NNbr(t->parent)))
	split(NNbr(t->parent));
      if (WNbr(t->parent) != NULL && isLeaf(WNbr(t->parent)))
        split(WNbr(t->parent));
    }

    else if (t == t->parent->ne){
      if (NNbr(t->parent) != NULL && isLeaf(NNbr(t->parent)))
        split(NNbr(t->parent));
      if (ENbr(t->parent) != NULL && isLeaf(ENbr(t->parent)))
        split(ENbr(t->parent));
    }

    else if (t == t->parent->sw){
      if (SNbr(t->parent) != NULL && isLeaf(SNbr(t->parent)))
        split(SNbr(t->parent));
      if (WNbr(t->parent) != NULL && isLeaf(WNbr(t->parent)))
        split(WNbr(t->parent));
    }

    else if (t == t->parent->se){
      if (SNbr(t->parent) != NULL && isLeaf(SNbr(t->parent)))
        split(SNbr(t->parent));
      if (ENbr(t->parent) != NULL && isLeaf(ENbr(t->parent)))
        split(ENbr(t->parent));
    }
    
  }

  // FOR BALANCED QTREES-------------------------------------------------
  // A split might cause one or two nbrs of the parent of t to split
  // to maintain balance.  Note that these two nbrs exist (unless they're
  // not in the image region) because the current set of leaves are
  // balanced.
  // if( t is a NW (or NE or SW or SE) child ) then we need to check that
  // the North and West (or North and East or South and West or
  // South and East) nbrs of t->parent have children. If they don't
  // we need to split them.
  
  
}


/* NNbr(t)
 * return the same-sized quad tree node that is north of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::NNbr(Node *t) {
  if (t == root)
    return NULL;

  if (t == t->parent->nw){
    if (NNbr(t->parent) != NULL)
      return NNbr(t->parent)->sw;
  }
  
  else if (t == t->parent->ne){
    if (NNbr(t->parent) != NULL)
      return NNbr(t->parent)->se;
  }
  
  else if (t == t->parent->sw)
    return t->parent->nw;
  
  else if (t == t->parent->se)
    return t->parent->ne;

  return NULL;

  /* YOUR CODE HERE */
  
}

/* SNbr(t)
 * return the same-sized quad tree node that is south of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::SNbr(Node *t) {
  if (t == root)
    return NULL;

  if (t == t->parent->nw)
    return t->parent->sw;

  else if (t == t->parent->ne)
    return t->parent->se;

  else if (t == t->parent->sw){
    if (SNbr(t->parent) != NULL)
      return SNbr(t->parent)->nw;
  }
  
  else if (t == t->parent->se){
    if (SNbr(t->parent) != NULL)
      return SNbr(t->parent)->ne;
  }
    
  return NULL;
  /* YOUR CODE HERE */
  
}

/* ENbr(t)
 * return the same-sized quad tree node that is east of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::ENbr(Node *t) {
  if (t == root)
    return NULL;

  if (t == t->parent->nw)
    return t->parent->ne;

  else if (t == t->parent->ne){
    if (ENbr(t->parent) != NULL)
      return ENbr(t->parent)->nw;
  }

  else if (t == t->parent->sw)
    return t->parent->se;

  else if (t == t->parent->se){
    if (ENbr(t->parent) != NULL)
      return ENbr(t->parent)->sw;
  }

  return NULL;
  /* YOUR CODE HERE */
  
}

/* WNbr(t)
 * return the same-sized quad tree node that is west of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::WNbr(Node *t) {
  if (t == root)
    return NULL;

  if (t == t->parent->nw){
    if (WNbr(t->parent) != NULL)
      return WNbr(t->parent)->ne;
  }

  else if (t == t->parent->ne)
    return t->parent->nw;

  else if (t == t->parent->sw){
    if (WNbr(t->parent) != NULL)
      return WNbr(t->parent)->se;
  }

  else if (t == t->parent->se)
    return t->parent->sw;

  return NULL;
  /* YOUR CODE HERE */
  
}

bool QTree::write(string const & fileName){

  /* YOUR CODE HERE */
  PNG img(im.width(), im.height());
  writeLeaf(img, root);
  

  // include the following line to write the image to file.
  return(img.writeToFile(fileName));
}

void QTree::writeLeaf(PNG& img, Node* curr){
  if (curr == NULL)
    return;
  if(isLeaf(curr)){
    for(int i = 0; i < curr->size; i++){
      for(int j = 0; j < curr->size; j++){
	RGBAPixel* currPix = img.getPixel(curr->upLeft.first + i, curr->upLeft.second + j);
	if ((i == 0 || j == 0 || i == (curr->size - 1) || j == (curr->size - 1)) && drawFrame){
	  currPix->r = frameColor.r;
	  currPix->g = frameColor.g;
	  currPix->b = frameColor.b;
	  currPix->a = frameColor.a;
	}
	else {
	  currPix->r = curr->avg.r;
	  currPix->g = curr->avg.g;
	  currPix->b = curr->avg.b;
	  currPix->a = curr->avg.a;
	}

      }
    } 
  }
  else{
    writeLeaf(img, curr->nw);
    writeLeaf(img, curr->ne);
    writeLeaf(img, curr->sw);
    writeLeaf(img, curr->se);
  }

  
}

void QTree::clear() {

  /* YOUR CODE HERE */
  clearNode(root);
  
}

void QTree::clearNode(Node* curr) {
  if (curr == NULL)
    return;
  clearNode(curr->nw);
  clearNode(curr->ne);
  clearNode(curr->sw);
  clearNode(curr->se);
  delete curr;
}


void QTree::copy(const QTree & orig) {
  //clear();
  im = orig.im;
  leafBound = orig.leafBound;
  balanced = orig.balanced;
  drawFrame = orig.drawFrame;
  frameColor = orig.frameColor;
  
  root = copyNode(NULL, orig.root);
  /* YOUR CODE HERE */
  
}

QTree::Node * QTree::copyNode(Node* parent, const Node* copyCurr){
  if (copyCurr == NULL)
    return NULL;
  Node* tempNode = new Node(im, copyCurr->upLeft, copyCurr->size, parent);
  tempNode->nw = copyNode(tempNode, copyCurr->nw);
  tempNode->ne = copyNode(tempNode, copyCurr->ne);
  tempNode->sw = copyNode(tempNode, copyCurr->sw);
  tempNode->se = copyNode(tempNode, copyCurr->se);
  return tempNode;
}
