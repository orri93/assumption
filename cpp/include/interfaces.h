#ifndef _GOS_ASSUMPTION_INTERFACES_H_
#define _GOS_ASSUMPTION_INTERFACES_H_

#include <string>
#include <memory>

namespace gos
{
namespace interfaces
{
/* https://google.github.io/styleguide/cppguide.html#Function_Names */
/* Accessors and mutators (get and set functions) may be named like variables.
 * These often correspond to actual member variables, but this is not required.
 * For example, int count() and void set_count(int count).
 */

//! A simple wrapper template interface
template<typename T> class Wrapper
{
public:
  virtual ~Wrapper() {}
  //! Access to a constant reference to the wrapped object
  virtual const T& value() const = 0;
  //! Should check if the wrapper is holding an object (is set or not)
  virtual const bool is_set() const = 0;
};

//! A simple holder interface
template<typename T> class Holder
{
public:
  virtual ~Holder() {}
  //! Access to a reference of the contained value
  virtual T& reference() = 0;
  //! Access to a raw pointer to the contained value
  virtual T* pointer() = 0;
};

//! A array holding interface
template<typename T> class ArrayHolder
{
public:
  //! The index type
  typedef unsigned int Index;
  //! The size type
  typedef Index Size;
  virtual ~ArrayHolder() {}
  //! Access a reference to an item by index
  virtual T& get(const Index& index) = 0;
  //! Access a pointer to an item by index
  virtual T* pointer(const Index& index) = 0;
  //! Get the internal array size as constant reference
  /*! The internal state of the object stays unchanged by constant guard */
  virtual const Size& size() const = 0;
};

//! A simple string holder interface
class StringHolder
{
public:
  virtual ~StringHolder() {}
  //! Access to the constant literal string
  virtual const char* string() const = 0;
  //! Access to a copy of the string
  virtual const std::string text() const = 0;
};

//! A simple pointer holder interface
template<typename T> class PointerHolder
{
public:
  virtual ~PointerHolder() {}
  //! Access to a copy of the contained object
  virtual T value() = 0;
  //! Access to a reference of the contained object
  virtual T& reference() = 0;
  //! Access to a raw pointer of the contained object
  virtual T* pointer() = 0;
};

template<typename T> class RawPointerHolder
{
public:
  virtual ~RawPointerHolder() {}
  //! Access to a raw pointer of the contained object
  virtual T* pointer() = 0;
};

//! A simple constant holder interface
template<typename T> class ConstantHolder
{
public:
  virtual ~ConstantHolder() {}
  //! Should return a constant copy of the containing value
  virtual const T value() const = 0;
  //! Should return a constant reference of the containing value
  /*! The internal state of the object stays unchanged by constant guard */
  virtual const T& reference() const = 0;
  //! Should return a constant pointer to the containing value
  /*! The internal state of the object stays unchanged by constant guard */
  virtual const T* pointer() const = 0;
};

//! The Assumption interface
template<typename T, typename W, typename H> class Assumption
{
public:
  //! The id type
  typedef std::string Id;
  //! The index type
  typedef unsigned int Index;
  //! The array type
  typedef std::unique_ptr<T[]> Array;
  //! The array wrapper type
  typedef std::unique_ptr<W[]> WrapperArray;
  //! A type for unique pointer to a holder
  typedef std::unique_ptr<H> HolderPtr;
  virtual ~Assumption() {}
  //! A method that should create the unique items for the unique assumption
  virtual void unique(Array& a, WrapperArray& wrapper, HolderPtr& holder) = 0;
  //! Should check if the id is contained in the object
  virtual bool has(const Id& id) = 0;
  //! Should return a reference to a value from an array by id and index
  virtual T& value(const Id& id, const Index& index) = 0;
  //! Should return a reference to a wrapper from an array by id and index
  virtual W& wrapper(const Id& id, const Index& index) = 0;
  //! Should return a reference to a holder by id
  virtual H& holder(const Id& id) = 0;
};

} /* namespace interfaces */
} /* namespace gos */

#endif /* _GOS_ASSUMPTION_INTERFACES_H_ */

