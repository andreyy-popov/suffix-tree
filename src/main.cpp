//============================================================================
// Name        : main.cpp
// Date        : 17.12.2016
// Author      : Andrey Popov
// Copyright   : All rights reserved
//============================================================================

#include "suffix_tree.h"
#include <iostream>

void print_suffix_tree(suffix_tree& tree, int indent = 0) {
  std::cout << tree.edge_str();
  suffix_tree::iterator begin = tree.begin();
  suffix_tree::iterator end = tree.end();
  if (begin == end) {
    std::cout << std::endl;
  } else {
    indent += tree.edge_length();
    for (suffix_tree::iterator it = begin; it != end; ++it) {
      if (it != begin) {
        for (int i = 0; i < indent; ++i) {
          std::cout << ' ';
        }
      }
      print_suffix_tree(*it, indent);
    }
  }
}

void print_suffix_tree_edges(suffix_tree& tree) {
  std::cout << tree.edge_str() << std::endl;
  for (suffix_tree& subtree : tree) {
    print_suffix_tree_edges(subtree);
  }
}

int main() {
  const std::string s("ATAAATG$");
  std::shared_ptr<suffix_tree> tree = suffix_tree::make(s);
  print_suffix_tree(*tree);
  std::cout << std::endl;
  print_suffix_tree_edges(*tree);
  return 0;
}

/*
  01234567
  ATAAATG$
  TAAATG$
  AAATG$
  AATG$
  ATG$
  TG$
  G$
  $

  AAATG$
    TG$
   TAAATG$
    G$
  TAAATG$
   G$
  G$
  $

  AAATG$
  AAATG$
  ATG$
  A
  A
  G$
  G$
  G$
  TG$
  T
  T
  $
*/
