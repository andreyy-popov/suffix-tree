//============================================================================
// Name        : suffix_tree.cpp
// Date        : 17.12.2016
// Author      : Andrey Popov
// Copyright   : All rights reserved
//============================================================================

#include "suffix_tree.h"

// Премещение к следующему элементу.
suffix_tree::iterator suffix_tree::iterator::operator++(int) {
  iterator it(tree);
  return (tree = tree->sibling, it);
}

// Конструктор.
suffix_tree::suffix_tree(const std::string& s, std::string::size_type p1, std::string::size_type p2, suffix_tree* parent, suffix_tree* sibling, suffix_tree* child) :
  s(s), p1(p1), p2(p2), parent(parent), sibling(sibling), child(child), suffix_link(nullptr)
{}

// Деструктор.
suffix_tree::~suffix_tree() {
  if (child) {
    delete child;
  }
  if (sibling) {
    delete sibling;
  }
}

// Выбор ветви по совпадению первого символа.
suffix_tree* suffix_tree::select_edge(std::string::value_type ch) {
  for (suffix_tree& edge : *this) {
    if (edge.s[edge.p1] == ch) {
      return &edge;
    }
  }
  return nullptr;
}

// Разделение ветви.
suffix_tree* suffix_tree::split_edge(std::string::size_type d) {
  suffix_tree* tree = new suffix_tree(s, p1, p1 + d, parent, nullptr, this);
  return (parent->child = parent->child->replace_edge(tree), p1 += d, parent = tree);
}

// Замена ветви.
suffix_tree* suffix_tree::replace_edge(suffix_tree* edge) {
  if (p1 == edge->p1) {
    return (edge->sibling = sibling, sibling = nullptr, edge);
  }
  return (sibling = sibling->replace_edge(edge), this);
}

// Восходящий обход.
suffix_tree* suffix_tree::walk_up(std::string::size_type i, suffix_tree& w, std::string::size_type start, std::string::size_type end) {
  if (parent->is_root()) {
    return parent->walk_down(i, w, start + 1, end);
  }
  if (suffix_link) {
    return suffix_link->walk_down(i, w, start, end);
  }
  return parent->walk_up(i, w, start - parent->edge_length(i), end);
}

// Нисходящий обход.
suffix_tree* suffix_tree::walk_down(std::string::size_type i, suffix_tree& w, std::string::size_type start, std::string::size_type end) {
  if (start != end) {
    suffix_tree* edge = select_edge(s[start]);
    if (edge) {
      std::string::size_type d1 = end - start;
      std::string::size_type d2 = edge->edge_length(i);
      if (d1 > d2) {
        return edge->walk_down(i, w, start + d2, end);
      } else {
        std::string::size_type d = d1 - 1;
        if (s[start + d] != s[edge->edge_start() + d]) {
          return edge->split_edge(d)->walk_down(i, w, start + d, end);
        }
      }
    } else {
      if (!w.is_root()) {
        w.suffix_link = this;
      }
      return (child = new suffix_tree(s, start, std::string::npos, this, child, nullptr));
    }
  }
  return nullptr;
}

// Расширение суффиксного дерева.
suffix_tree& suffix_tree::extend_tree(std::string::size_type i) {
  suffix_tree* tree = single_extension(i);
  if (tree) {
    return tree->extend_tree(i);
  }
  return *this;
}

// Завершение построения суффиксного дерева.
void suffix_tree::finalize_tree() {
  if (p2 == std::string::npos) {
    p2 = s.length();
  } else {
    for (suffix_tree& edge : *this) {
      edge.finalize_tree();
    }
  }
}

// Построение суффиксного дерева.
void suffix_tree::build_tree(std::string::size_type i) {
  if (i == s.length()) {
    root().finalize_tree();
  } else {
    extend_tree(i).build_tree(i + 1);
  }
}

// Создание начального дерева.
std::shared_ptr<suffix_tree> suffix_tree::initial_tree(const std::string& s) {
  std::shared_ptr<suffix_tree> root(new suffix_tree(s, 0, 0, nullptr, nullptr, nullptr));
  root->child = new suffix_tree(s, 0, std::string::npos, root.get(), nullptr, nullptr);
  return root;
}

// Создание суффиксного дерева.
std::shared_ptr<suffix_tree> suffix_tree::make(const std::string& s) {
  std::shared_ptr<suffix_tree> root(initial_tree(s));
  root->child->build_tree(1);
  return root;
}
