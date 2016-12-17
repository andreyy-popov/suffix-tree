//============================================================================
// Name        : suffix_tree.h
// Date        : 17.12.2016
// Author      : Andrey Popov
// Copyright   : All rights reserved
//============================================================================

#ifndef _SUFFIX_TREE_H_
#define _SUFFIX_TREE_H_

#include <string>
#include <memory>

class suffix_tree {
  const std::string& s;
  std::string::size_type p1;
  std::string::size_type p2;
  suffix_tree* parent;
  suffix_tree* sibling;
  suffix_tree* child;
  suffix_tree* suffix_link;
  // Конечная позиция.
  std::string::size_type edge_end(std::string::size_type i) const
    { return (p2 == std::string::npos) ? (i + 1) : p2; }
  // Длина ветви.
  std::string::size_type edge_length(std::string::size_type i) const
    { return edge_end(i) - p1; }
  // Проверка на принадлежность корню дерева.
  bool is_root() const
    { return !parent; }
  // Запрос корня дерева.
  suffix_tree& root()
    { return is_root() ? *this : parent->root(); }
  // Выбор ветви по совпадению первого символа.
  suffix_tree* select_edge(std::string::value_type ch);
  // Разделение ветви.
  suffix_tree* split_edge(std::string::size_type d);
  // Замена ветви.
  suffix_tree* replace_edge(suffix_tree* edge);
  // Восходящий обход.
  suffix_tree* walk_up(std::string::size_type i, suffix_tree& w, std::string::size_type start, std::string::size_type end);
  // Нисходящий обход.
  suffix_tree* walk_down(std::string::size_type i, suffix_tree& w, std::string::size_type start, std::string::size_type end);
  // Шаг расширения суффиксного дерева.
  suffix_tree* single_extension(std::string::size_type i)
    { return walk_up(i, *this, edge_start(), edge_end(i)); }
  // Расширение суффиксного дерева.
  suffix_tree& extend_tree(std::string::size_type i);
  // Завершение построения суффиксного дерева.
  void finalize_tree();
  // Построение суффиксного дерева.
  void build_tree(std::string::size_type i);
  // Создание начального дерева.
  static std::shared_ptr<suffix_tree> initial_tree(const std::string& s);
  // Конструктор.
  suffix_tree(const std::string& s, std::string::size_type p1, std::string::size_type p2, suffix_tree* parent, suffix_tree* sibling, suffix_tree* child);
public:
  class iterator {
    suffix_tree* tree;
  public:
    // Конструктор.
    iterator(suffix_tree* tree = nullptr) : tree(tree)
      {}
    // Премещение к следующему элементу списка.
    iterator operator++(int);
    iterator& operator++()
      { return (tree = tree->sibling, *this); }
    // Доступ к элементу списка.
    suffix_tree& operator*()
      { return *tree; }
    suffix_tree* operator->()
      { return tree; }
    // Проверка на равентство.
    bool operator==(const iterator& it) const
      { return (tree == it.tree); }
    // Проверка на неравентство.
    bool operator!=(const iterator& it) const
      { return (tree != it.tree); }
  };
  // Деструктор.
  ~suffix_tree();
  // Текст всей строки.
  const std::string& str() const
    { return s; }
  // Начальная позиция ветви.
  std::string::size_type edge_start() const
    { return p1; }
  // Конечная позиция ветви.
  std::string::size_type edge_end() const
    { return p2; }
  // Длина ветви.
  std::string::size_type edge_length() const
    { return p2 - p1; }
  // Текст ветви.
  std::string edge_str() const
    { return s.substr(p1, edge_length()); }
  // Итератор начала списка.
  iterator begin() const
    { return iterator(child); }
  // Итератор конца списка.
  iterator end() const
    { return iterator(); }
  // Создание суффиксного дерева.
  static std::shared_ptr<suffix_tree> make(const std::string& s);
};

#endif // _SUFFIX_TREE_H_
