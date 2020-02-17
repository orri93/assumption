#ifndef _GOS_ASSUMPTION_H_
#define _GOS_ASSUMPTION_H_

#include <cstdio>
#include <cassert>

#include <memory>
#include <utility>
#include <iostream>
#include <map>
#include <set>

#include <gos/assumption/interfaces.h>

#define _GOS_ASSUMPTION_FAST_ "fast"
#define _GOS_ASSUMPTION_THREAD_SAFE_ "threadsafe"

/* https://google.github.io/styleguide/cppguide.html#Variable_Names */

namespace gos
{
namespace assumption
{

//! A class to test the construction order
class Order
{
public:
  //! A Constructor that takes a reference to a string
  /*! The string is moved into the constructed object by the move function.
   *  The value of the referenced string is moved into the constructed object.
   *  That will leave the referenced string empty afterwards.
   */
  Order(std::string& value) : value_(std::move(value))
  {
    /* The value in constructor body will be empty string as the move in
     * the value assignment is executed before the content of
     * the constructor body
     */
    this->constructor_body_value_ = value;
  }
  //! Get the value of the internal string
  std::string value() { return this->value_; }
  //! Get the value of the referenced value after the construction move
  std::string constructor_body_value()
  {
    return this->constructor_body_value_;
  }
private:
  /* Underscore at end is ok */
  std::string value_;
  std::string constructor_body_value_;
};

//! A simple wrapper template
template<typename T> class Wrapper :
  public gos::interfaces::Wrapper<T>
{
public:
  //! The interface type
  typedef gos::interfaces::Wrapper<T> Interface;
  Wrapper() : value_(T()) /* This will zero int, double, float etc */
#if defined(_GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_)
    , is_set_(false)
#endif
  {}
  //! A Constructor that takes a constant reference to the wrapped object
  Wrapper(const T& value) : value_(value)
#if defined(_GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_)
    , is_set_(true)
#endif
  {}
  //! A copy Constructor that takes a constant reference to a wrapper interface
  Wrapper(const Interface& wrapper) : value_(wrapper.value_)
#if defined(_GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_)
    , is_set_(wrapper.is_set_)
#endif
  {}
  ~Wrapper()
  {
#ifdef _GOS_ASSUMPTION_COUT_
    std::cout << "Cleaning up a wrapper" << std::endl;
#endif
  }
  Wrapper& operator= (const Wrapper& wrapper)
  {
    if (this != &wrapper)
    {
      this->value_ = wrapper.value_;
#if defined(_GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_)
      this->is_set_ = wrapper.is_set_;
#endif
    }
    return *this;
  }
#ifdef _GOS_ASSUMPTION_COMPARE_WITH_FRIEND_
  friend bool operator==(const Wrapper& w1, const Wrapper& w2)
  {
#if defined(_GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_)
    return w1.value_ == w2.value_ && w1.is_set_ == w2.is_set_;
#else
    return w1.value_ == w2.value_;
#endif
  }
#else
  bool operator==(const Wrapper<T>& wrapper) const
  {
#if defined(_GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_)
    return this->value_ == wrapper.value_ && this->is_set_ == wrapper.is_set_;
#else
    return this->value_ == wrapper.value_;
#endif
  }
#endif
  //! Access to a constant reference to the wrapped object
  const T& value() const { return this->value_; }
  //! Checks if the wrapper is holding an object (is set or not)
  /*! Returns true if the wrapper is holding an object, otherwise false */
  const bool is_set() const
  {
#if defined(_GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_)
    return this->is_set_;
#endif
#if defined(_GOS_ASSUMPTION_SET_CHECK_FROM_DEFAULT_)
    return this->value_ != T();
#endif
  }
#ifdef _GOS_ASSUMPTION_COMPARE_WITH_FRIEND_
protected:
#else
private:
#endif
#if defined(_GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_)
  bool is_set_;
#endif
  /* Underscore at end is ok */
  T value_;
};

//! A simple holder class
template<typename T> class Holder :
  public gos::interfaces::CopyableHolder<T>,
  public gos::interfaces::ReferencableHolder<T>,
  public gos::interfaces::PointeredHolder<T>
{
public:
  //! A type for a unique pointer to the contained value
  typedef std::unique_ptr<T> Ptr;
  //! A Constructor that takes a reference to the contained value
  /*! The holder doesn't actually contained the value but rather
   *  a reference to the value */
  Holder(T& value) : value_(value) {}
  //! Access to a copy of the contained value
  T value() { return this->value_; }
  //! Access to a reference of the contained value
  T& reference() { return this->value_; }
  //! Access to a raw pointer to the contained value
  T* pointer() { return &this->value_; }
private:
  /* Underscore at end is ok */
  T& value_;
};

//! A array holding class
template<typename T> class ArrayHolder : public gos::interfaces::ArrayHolder<T>
{
public:
  //! The index type
  typedef typename gos::interfaces::ArrayHolder<T>::Index Index;
  //! The size type
  typedef typename gos::interfaces::ArrayHolder<T>::Size Size;
  ArrayHolder() : size_(0) {}
  //! A Constructor that creates an array on the heap with make unique
  ArrayHolder(const Size& size) : size_(size)
  {
    this->array_ = std::make_unique<T[]>(this->size_);
    assert(this->array_);
  }
  ~ArrayHolder()
  {
#ifdef _GOS_ASSUMPTION_COUT_
    std::cout << "Cleaning up an array holder" << std::endl;
#endif
  }
  //! Access a reference to an item by index
  T& get(const Index& index)
  {
    return this->array_[index];
  }
  //! Access a pointer to an item by index
  T* pointer(const Index& index)
  {
    return &this->array_[index];
  }
  //! Get the internal array size as constant reference
  /*! The internal state of the object stays unchanged by constant guard */
  const Size& size() const { return this->size_; }
private:
  typedef std::unique_ptr<T[]> Array;
  Array array_;
  Size size_;
};

//! A simple string holder class
class StringHolder : public gos::interfaces::StringHolder
{
public:
  StringHolder() {}
  //! A Constructor that takes a constant reference to text
  StringHolder(const std::string& text) : string_(text) {}
  //! A Constructor that takes a constant literal string
  StringHolder(const char*& string) : string_(string) {}
  //! Access to the constant literal string
  const char* string() const { return this->string_.c_str(); }
  //! Access to a copy of the string
  const std::string text() const { return this->string_; }
private:
  std::string string_;
};

//! A simple pointer holder class
template<typename T> class PointerHolder :
  public gos::interfaces::CopyableHolder<T>,
  public gos::interfaces::ReferencableHolder<T>,
  public gos::interfaces::PointeredHolder<T>
{
public:
  //! A type for a unique pointer
  typedef std::unique_ptr<T> Pointer;
  //! A Constructor that takes a reference to a unique pointer
  /*! The object contained in the pointer is moved into the constructed object
   *  by the move function.
   *  That will leave the referenced pointer un-set or undefined afterwards.
   */
  PointerHolder(Pointer& pointer) : pointer_(std::move(pointer)) {}
  //! Access to a copy of the contained object
  /*! This will only work if the contained object implements
   *  a copy constructor */
  T value() { return *this->pointer_; }
  //! Access to a reference of the contained object
  T& reference() { return *this->pointer_; }
  //! Access to a raw pointer of the contained object
  T* pointer() { return this->pointer_; }
private:
  Pointer pointer_;
};

//! A simple raw pointer holder class
template<typename T> class RawPointerHolder :
  public gos::interfaces::RawPointerHolder<T>
{
public:
  //! A type for a unique pointer
  typedef T* Pointer;
  //! The default constructor takes a pointer as input
  RawPointerHolder(const Pointer& pointer) :
    raw_(pointer)
  {}
  //! Access to a raw pointer of the contained object
  Pointer pointer()
  {
    return this->raw_;
  }
private:
  Pointer raw_;
};

//! The defaulting holder class
template<typename T>
class DefaultingHolder : public gos::interfaces::ReferencableHolder<T>
{
public:
  DefaultingHolder() : ReferencableHolder(T()) {}
};

//! A simple constant holder class
template<typename T> class ConstantHolder :
  public gos::interfaces::ConstantHolder<T>
{
public:
  ConstantHolder() {}
  //! A Constructor that creates a holder containing a value
  ConstantHolder(T& value) : value_(value) {}
  //! Returns a constant copy of the containing value
  /*! The internal state of the object stays unchanged by constant guard */
  const T value() const { return this->value_; }
  //! Returns a constant reference of the containing value
  /*! The internal state of the object stays unchanged by constant guard */
  const T& reference() const { return this->value_; }
  //! Returns a constant pointer to the containing value
  /*! The internal state of the object stays unchanged by constant guard */
  const T* pointer() const { return &this->value_; }
private:
  /* Underscore at end is ok */
  T& value_;
};

//! A nested structure holding class giving access to nested object by reference
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
  //! The name structure for literate string constants
  struct Name
  {
    //! The constant literate string A
    const char* const A = "A";
    //! The constant literate string ABC
    const char* const ABC = "ABC";
  };

  //! A static access to the name structure of literate string contants
  static Name names() { return Name{}; }
  //! A reference to an internal instance of the invisible collection
  /*! This function returns a reference to an instance of a private nested
   * structure named Collection. The instance is created on the stack.
   */
  Collection& collection() { return this->collection_; }

private:
  Collection collection_;
};

//! The Assumption class
template<typename T, typename W, typename H>
class Assumption : public gos::interfaces::Assumption<T, W, H>
{
public:
  //! The interface type
  typedef gos::interfaces::Assumption<T, W, H> Interface;
  //! The id type
  typedef typename Interface::Id Id;
  //! The index type
  typedef typename Interface::Index Index;
  //! The array type
  typedef typename Interface::Array Array;
  //! The array wrapper type
  typedef typename Interface::WrapperArray WrapperArray;
  //! The holder unique pointer type
  typedef typename Interface::HolderPtr HolderPtr;
  //! The size type
  typedef Index Size;

  //! The constant array size
  const Size ArraySize = 8;
  //! The constant unique id
  const Id UniqueId = "id";
  //! A method to create the unique items for the unique assumption
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
  //! Check if the id is contained in the object
  /*! Returns true if the id is is contained in the object, otherwise false */
  bool has(const Id& id)
  {
    return this->id_set_.find(id) != this->id_set_.end();
  }
  //! Returns a reference to a value from an array by id and index
  T& value(const Id& id, const Index& index)
  {
    return this->array_map_[id][index];
  }
  //! Returns a reference to a wrapper from an array by id and index
  W& wrapper(const Id& id, const Index& index)
  {
    return this->wrapper_array_map_[id][index];
  }
  //! Returns a reference to a holder by id
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

