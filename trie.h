#pragma once

#include <cstdint>
#include <unordered_map>

#include <iostream>

namespace tools {

namespace detail {

struct pair_hash {
  template <typename Pair>
  constexpr std::size_t operator()(const Pair& p) const {
    return std::hash<typename Pair::first_type>{}(p.first) ^
           std::hash<typename Pair::second_type>{}(p.second);
  }
};

}  // namespace detail

struct empty {};

template <typename Key, typename Value>
// TODO: Regular<Key> && Hashable<Key> && Regular_ish<Value>
class trie {
 public:
  using key_type = Key;
  using value_type = Value;

 private:
  using node_id = std::ptrdiff_t;

  struct node {
    node() = default;
    node(node_id id) : id(id) {}

    node_id id = 0;
    value_type value{};
  };

  node_id root_id() const { return 1; }
  node_id next_id() { return last_id_++; }

  node_id last_id_ = root_id();
  key_type terminal_;

  // a slot map would be ideal actually.
  std::unordered_map<std::pair<node_id, key_type>, node, detail::pair_hash> edges_;

 public:
  trie(key_type terminal = key_type{}) : terminal_(terminal) {}

  // Much like map.insert - returns the insertion result and wether or not we
  // inserted anything at all.
  //
  // instead of an iterator, returns reference because I don't have iterators.
  // I don't accept value because we still have to be able to default construct
  // it.
  template <typename R>
  // Range of key_type
  std::pair<value_type&, bool> insert(const R& r) {
    node_id cur_id = root_id();

    for (const auto& e : r) {
      auto& _node = edges_[{cur_id, e}];
      if (!_node.id)
        _node.id = next_id();

      cur_id = _node.id;
    }

    auto& _node = edges_[{cur_id, terminal_}];
    if (_node.id) {
        return {_node.value, false};
    }
    _node.id = next_id();
    return {_node.value, true};
  }
};

}  // namespace tools
