#ifndef _GOS_ASSUMPTION_H_
#define _GOS_ASSUMPTION_H_

#ifdef _WIN32
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#else
#include <stdio.h>
#endif

#include <cassert>
#include <utility>
#include <iostream>
#include <map>
#include <set>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "interfaces.h"

#define _GOS_ASSUMPTION_FAST_ "fast"
#define _GOS_ASSUMPTION_THREAD_SAFE_ "threadsafe"

#define _GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_
//#define _GOS_ASSUMPTION_SET_CHECK_FROM_DEFAULT_

/* https://google.github.io/styleguide/cppguide.html#Variable_Names */

namespace gos
{
namespace assumption
{

class Order
{
public:
  Order(std::string& value) : value_(std::move(value))
  {
    /* The value in constructor body will be empty string as the move in
     * the value assignment is executed before the content of
     * the constructor body
     */
    this->constructor_body_value_ = value;
  }
  std::string value() { return this->value_; }
  std::string constructor_body_value()
  {
    return this->constructor_body_value_;
  }
private:
  /* Underscore at end is ok */
  std::string value_;
  std::string constructor_body_value_;
};

template<typename T> class Wrapper :
  public gos::interfaces::Wrapper<T>
{
public:
  typedef gos::interfaces::Wrapper<T> Interface;
  Wrapper() : value_(T()) /* This will zero int, double, float etc */
#if defined(_GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_)
    , is_set_(false)
#endif
  {}
  Wrapper(const T& value) : value_(value)
#if defined(_GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_)
    , is_set_(true)
#endif
  {}
  Wrapper(const Interface& wrapper) : value_(wrapper.value_)
#if defined(_GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_)
    , is_set_(wrapper.is_set_)
#endif
  {}
  ~Wrapper()
  {
    std::cout << "Cleaning up a wrapper" << std::endl;
  }
  const T& value() const { return this->value_; }
  const bool is_set() const
  {
#if defined(_GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_)
    return this->is_set_;
#endif
#if defined(_GOS_ASSUMPTION_SET_CHECK_FROM_DEFAULT_)
    return this->value_ != default(T);
#endif
  }
private:
#if defined(_GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_)
  bool is_set_;
#endif
  /* Underscore at end is ok */
  T value_;
};

template<typename T> class Holder :
  public gos::interfaces::Holder<T>
{
public:
  typedef std::unique_ptr<T> Ptr;
  Holder(T& value) : value_(value) {}
  T value() { return this->value_; }
  T& reference() { return this->value_; }
  T* pointer() { return &this->value_; }
private:
  /* Underscore at end is ok */
  T& value_;
};

template<typename T> class ArrayHolder : public gos::interfaces::ArrayHolder<T>
{
public:
  ArrayHolder() : size_(0) {}
  ArrayHolder(const Size& size) : size_(size)
  {
    this->array_ = std::make_unique<T[]>(this->size_);
    assert(this->array_);
  }
  ~ArrayHolder()
  {
    std::cout << "Cleaning up an array holder" << std::endl;
  }
  T& get(const Index& index)
  {
    return this->array_[index];
  }
  T* pointer(const Index& index)
  {
    return &this->array_[index];
  }
  const Size& size() const { return this->size_; }
private:
  typedef std::unique_ptr<T[]> Array;
  Array array_;
  Size size_;
};

class StringHolder : public gos::interfaces::StringHolder
{
public:
  StringHolder() {}
  StringHolder(const std::string& text) : string_(text) {}
  StringHolder(const char*& string) : string_(string) {}
  const char* string() const { return this->string_.c_str(); }
  const std::string text() const { return this->string_; }
private:
  std::string string_;
};

template<typename T> class PointerHolder :
  public gos::interfaces::PointerHolder<T>
{
public:
  typedef std::unique_ptr<T> Pointer;
  PointerHolder(Pointer& pointer) : pointer_(std::move(pointer)) {}
  T value() { return *this->pointer_; }
  T& reference() { return *this->pointer_; }
  T* pointer() { return this->pointer_; }
private:
  Pointer pointer_;
};

template<typename T> class ConstantHolder :
  public gos::interfaces::ConstantHolder<T>
{
public:
  ConstantHolder() {}
  ConstantHolder(T& value) : value_(value) {}
  const T value() const { return this->value_; }
  const T& reference() const { return this->value_; }
  const T* pointer() const { return &this->value_; }
private:
  /* Underscore at end is ok */
  T& value_;
};

template<class T> class NestedReferenceHolder
{
private:
  /* https://google.github.io/styleguide/cppguide.html#Structs_vs._Classes */
  /* Struct is used only for passive objects that carry data */
  struct Collection
  {
    T A;
    T ABC;
  };

public:
  struct Name
  {
    const char* const A = "A";
    const char* const ABC = "ABC";
  };

  static Name names() { return Name{}; }
  Collection& collection() { return this->collection_; }

private:
  Collection collection_;
};

template<class T, class W, class H>
class Assumption : public gos::interfaces::Assumption<T, W, H>
{
public:
  
  typedef Index Size;
  const Size ArraySize = 8;
  const Id UniqueId = "id";

  void unique(Array& a, WrapperArray& wrapper, HolderPtr& holder)
  {
    a = std::make_unique<T[]>(this->ArraySize);
    assert((bool)a);
    wrapper = std::make_unique<W[]>(this->ArraySize);
    assert((bool)wrapper);
    holder = std::make_unique<H>(this->ArraySize);
    assert((bool)holder);

    /* Zero set the array the other arrays are self zeroing */
    for (Index i = 0; i < this->ArraySize; i++)
    {
      a[i] = T(); /* This will zero int, double, float etc */
    }

    std::string id = UniqueId;

    const size_t ExpectedSize = 1;

    this->id_set_.insert(id);
    assert(this->id_set_.size() == ExpectedSize);
    this->array_map_[id] = std::move(a);
    assert(!(bool)a);
    assert(this->array_map_.size() == ExpectedSize);
    this->wrapper_array_map_[id] = std::move(wrapper);
    assert(!(bool)wrapper);
    assert(this->wrapper_array_map_.size() == ExpectedSize);
    this->holder_map_[id] = std::move(holder);
    assert(!(bool)holder);
    assert(this->holder_map_.size() == ExpectedSize);
  }

  bool has(const Id& id)
  {
    return this->id_set_.find(id) != this->id_set_.end();
  }
  T& value(const Id& id, const Index& index)
  {
    return this->array_map_[id][index];
  }
  W& wrapper(const Id& id, const Index& index)
  {
    return this->wrapper_array_map_[id][index];
  }
  H& holder(const Id& id) { return *this->holder_map_[id]; }

private:
  typedef std::map<Id, Array> ArrayMap;
  typedef std::map<Id, WrapperArray> WrapperArrayMap;
  typedef std::map<Id, HolderPtr> HolderMap;
  typedef std::set<Id> IdSet;

  ArrayMap array_map_;
  WrapperArrayMap wrapper_array_map_;
  HolderMap holder_map_;
  IdSet id_set_;
};

} /* namespace assumption */
} /* namespace gos */

#endif /* _GOS_ASSUMPTION_H_ */

