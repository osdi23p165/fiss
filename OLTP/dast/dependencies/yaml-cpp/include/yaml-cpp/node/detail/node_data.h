#ifndef VALUE_DETAIL_NODE_DATA_H_62B23520_7C8E_11DE_8A39_0800200C9A66
#define VALUE_DETAIL_NODE_DATA_H_62B23520_7C8E_11DE_8A39_0800200C9A66

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "yaml-cpp/dll.h"
#include "yaml-cpp/node/detail/node_iterator.h"
#include "yaml-cpp/node/iterator.h"
#include "yaml-cpp/node/ptr.h"
#include "yaml-cpp/node/type.h"

namespace YAML {
namespace detail {
class node;
}  // namespace detail
}  // namespace YAML

namespace YAML {
namespace detail {
class YAML_CPP_API node_data {
 public:
  node_data();
  node_data(const node_data&) = delete;
  node_data& operator=(const node_data&) = delete;

  void mark_defined();
  void set_mark(const Mark& mark);
  void set_type(NodeType::value type);
  void set_tag(const std::string& tag);
  void set_null();
  void set_scalar(const std::string& scalar);
  void set_style(EmitterStyle::value style);

  bool is_defined() const { return m_isDefined; }
  const Mark& mark() const { return m_mark; }
  NodeType::value type() const {
    return m_isDefined ? m_type : NodeType::Undefined;
  }
  const std::string& scalar() const { return m_scalar; }
  const std::string& tag() const { return m_tag; }
  EmitterStyle::value style() const { return m_style; }

  // size/iterator
  std::size_t size() const;

  const_node_iterator begin() const;
  node_iterator begin();

  const_node_iterator end() const;
  node_iterator end();

  // sequence
  void push_back(node& node, shared_memory_holder pMemory);
  void insert(node& key, node& value, shared_memory_holder pMemory);

  // indexing
  template <typename Key>
  node* get(const Key& key, shared_memory_holder pMemory) const;
  template <typename Key>
  node& get(const Key& key, shared_memory_holder pMemory);
  template <typename Key>
  bool remove(const Key& key, shared_memory_holder pMemory);

  node* get(node& key, shared_memory_holder pMemory) const;
  node& get(node& key, shared_memory_holder pMemory);
  bool remove(node& key, shared_memory_holder pMemory);

  // map
  template <typename Key, typename Value>
  void force_insert(const Key& key, const Value& value,
                    shared_memory_holder pMemory);

 public:
  static std::string empty_scalar;

 private:
  void compute_seq_size() const;
  void compute_map_size() const;

  void reset_sequence();
  void reset_map();

  void insert_map_pair(node& key, node& value);
  void convert_to_map(shared_memory_holder pMemory);
  void convert_sequence_to_map(shared_memory_holder pMemory);

  template <typename T>
  static node& convert_to_node(const T& rhs, shared_memory_holder pMemory);

 private:
  bool m_isDefined;
  Mark m_mark;
  NodeType::value m_type;
  std::string m_tag;
  EmitterStyle::value m_style;

  // scalar
  std::string m_scalar;

  // sequence
  typedef std::vector<node*> node_seq;
  node_seq m_sequence;

  mutable std::size_t m_seqSize;

  // map
  typedef std::vector<std::pair<node*, node*>> node_map;
  node_map m_map;

  typedef std::pair<node*, node*> kv_pair;
  typedef std::list<kv_pair> kv_pairs;
  mutable kv_pairs m_undefinedPairs;
};
}
}

#endif  // VALUE_DETAIL_NODE_DATA_H_62B23520_7C8E_11DE_8A39_0800200C9A66
urns a pointer so that it can be NULL (if there is no such
    // key).
    return static_cast<const node_ref&>(*m_pRef).get(key, pMemory);
  }
  template <typename Key>
  node& get(const Key& key, shared_memory_holder pMemory) {
    node& value = m_pRef->get(key, pMemory);
    value.add_dependency(*this);
    return value;
  }
  template <typename Key>
  bool remove(const Key& key, shared_memory_holder pMemory) {
    return m_pRef->remove(key, pMemory);
  }

  node* get(node& key, shared_memory_holder pMemory) const {
    // NOTE: this returns a non-const node so that the top-level Node can wrap
    // it, and returns a pointer so that it can be NULL (if there is no such
    // key).
    return static_cast<const node_ref&>(*m_pRef).get(key, pMemory);
  }
  node& get(node& key, shared_memory_holder pMemory) {
    node& value = m_pRef->get(key, pMemory);
    key.add_dependency(*this);
    value.add_dependency(*this);
    return value;
  }
  bool remove(node& key, shared_memory_holder pMemory) {
    return m_pRef->remove(key, pMemory);
  }

  // map
  template <typename Key, typename Value>
  void force_insert(const Key& key, const Value& value,
                    shared_memory_holder pMemory) {
    m_pRef->force_insert(key, value, pMemory);
  }

 private:
  shared_node_ref m_pRef;
  typedef std::set<node*> nodes;
  nodes m_dependencies;
};
}
}

#endif  // NODE_DETAIL_NODE_H_62B23520_7C8E_11DE_8A39_0800200C9A66
