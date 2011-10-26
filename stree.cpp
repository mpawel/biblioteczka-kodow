#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <string>

using namespace std;

const int MAX_LENGTH = 1000;
char T[ MAX_LENGTH ];
int N; // długość napisu -1

class Suffix {
public :
  int origin_node;
  int first_char_index;
  int last_char_index;
  Suffix( int node, int start, int stop )
    : origin_node( node ),
      first_char_index( start ),
      last_char_index( stop ){};
  int Explicit(){ return first_char_index > last_char_index; }
  int Implicit(){ return last_char_index >= first_char_index; }
  void Canonize();
};

class Node {
public :
  int suffix_node;
  Node() : suffix_node(-1) {}
  static int Count;
};

int Node::Count = 1;
Node Nodes[ MAX_LENGTH * 2 ];

struct Edge {
  int first_char_index;
  int last_char_index;
  int end_node;
  int start_node;
  void Insert();  
  void Remove();
  static int Hash( int node, int c );
  static Edge Find( int node, int c );

  int SplitEdge( Suffix &s );

  Edge() : start_node(-1) {}
  Edge( int init_first, int init_last, int parent_node ):
    first_char_index(init_first), last_char_index(init_last),
    start_node(parent_node) { end_node = Node::Count++; }
};

const int HASH_TABLE_SIZE = 2179;
Edge Edges[ HASH_TABLE_SIZE ];

int Edge::Hash( int node, int c) { return ( ( node << 8 ) + c ) % HASH_TABLE_SIZE; }

Edge Edge::Find( int node, int c ) {
  int i = Hash( node, c );
  for ( ; ; ) {
    if ( Edges[ i ].start_node == node && c == T[ Edges[ i ].first_char_index ] ) return Edges[ i ];
    if ( Edges[ i ].start_node == -1 ) return Edges[ i ];
    i = ++i % HASH_TABLE_SIZE;
  }
}

void Edge::Insert() {
  int i = Hash( start_node, T[ first_char_index ] );
  while ( Edges[ i ].start_node != -1 )
    i = ++i % HASH_TABLE_SIZE;
  Edges[ i ] = *this;
}

void Edge::Remove() {
  int i = Hash( start_node, T[ first_char_index ] );
  while ( Edges[ i ].start_node != start_node ||
	  Edges[ i ].first_char_index != first_char_index )
    i = ++i % HASH_TABLE_SIZE;
  for ( ; ; ) {
    Edges[ i ].start_node = -1;
    int j = i;
    for ( ; ; ) {
      i = ++i % HASH_TABLE_SIZE;
      if ( Edges[ i ].start_node == -1 ) return;
      int r = Hash( Edges[ i ].start_node, T[ Edges[ i ].first_char_index ] );
      if ( i >= r && r > j ) continue;
      if ( r > j && j > i ) continue;
      if ( j > i && i >= r ) continue;
      break;
    }
    Edges[ j ] = Edges[ i ];
  }
}

int Edge::SplitEdge( Suffix &s )
{
  Remove();
  Edge *new_edge =
    new Edge( first_char_index,
	      first_char_index + s.last_char_index - s.first_char_index,
	      s.origin_node );
  new_edge->Insert();
  Nodes[ new_edge->end_node ].suffix_node = s.origin_node;
  first_char_index += s.last_char_index - s.first_char_index + 1;
  start_node = new_edge->end_node;
  Insert();
  return new_edge->end_node;
}

void Suffix::Canonize()
{
  if ( !Explicit() ) {
    Edge edge = Edge::Find( origin_node, T[ first_char_index ] );
    int edge_span = edge.last_char_index - edge.first_char_index;
    while ( edge_span <= ( last_char_index - first_char_index ) ) {
      first_char_index = first_char_index + edge_span + 1;
      origin_node = edge.end_node;
      if ( first_char_index <= last_char_index ) {
	edge = Edge::Find( edge.end_node, T[ first_char_index ] );
	edge_span = edge.last_char_index - edge.first_char_index;
      };
    }
  }
}

void AddPrefix( Suffix &active, int last_char_index )
{
  int parent_node;
  int last_parent_node = -1;

  for ( ; ; ) {
    Edge edge;
    parent_node = active.origin_node;
    //
    // Step 1 is to try and find a matching edge for the given node.
    // If a matching edge exists, we are done adding edges, so we break
    // out of this big loop.
    //
    if ( active.Explicit() ) {
      edge = Edge::Find( active.origin_node, T[ last_char_index ] );
      if ( edge.start_node != -1 )
	break;
    } else { //implicit node, a little more complicated
      edge = Edge::Find( active.origin_node, T[ active.first_char_index ] );
      int span = active.last_char_index - active.first_char_index;
      if ( T[ edge.first_char_index + span + 1 ] == T[ last_char_index ] )
	break;
      parent_node = edge.SplitEdge( active );
    }
    //
    // We didn't find a matching edge, so we create a new one, add
    // it to the tree at the parent node position, and insert it
    // into the hash table.  When we create a new node, it also
    // means we need to create a suffix link to the new node from
    // the last node we visited.
    //
    Edge *new_edge = new Edge( last_char_index, N, parent_node );
    new_edge->Insert();
    if ( last_parent_node > 0 )
      Nodes[ last_parent_node ].suffix_node = parent_node;
    last_parent_node = parent_node;
    //
    // This final step is where we move to the next smaller suffix
    //
    if ( active.origin_node == 0 )
      active.first_char_index++;
    else
      active.origin_node = Nodes[ active.origin_node ].suffix_node;
    active.Canonize();
  }
  if ( last_parent_node > 0 )
    Nodes[ last_parent_node ].suffix_node = parent_node;
  active.last_char_index++;  //Now the endpoint is the next active point
  active.Canonize();
};

int main()
{
  cin.getline( T, MAX_LENGTH - 1 );
  N = strlen( T ) - 1;

  Suffix active( 0, 0, -1 );  // The initial active prefix
  for ( int i = 0 ; i <= N ; i++ )
    AddPrefix( active, i );
};
