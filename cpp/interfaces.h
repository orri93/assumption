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

template<typename T> class Wrapper
{
public:
  virtual ~Wrapper() {}
  virtual const T& value() const = 0;
  virtual const bool is_set() const = 0;
};

template<typename T> class Holder
{
public:
  virtual ~Holder() {}
  virtual T& reference() = 0;
  virtual T* pointer() = 0;
};

template<typename T> class ArrayHolder
{
public:
  typedef unsigned int Index;
  typedef Index Size;
  virtual ~ArrayHolder() {}
  virtual T& get(const Index& index) = 0;
  virtual T* pointer(const Index& index) = 0;
  virtual const Size& size() const = 0;
};

class StringHolder
{
public:
  virtual ~StringHolder() {}
  virtual const char* string() const = 0;
  virtual const std::string text() const = 0;
};

template<typename T> class PointerHolder
{
public:
  virtual ~PointerHolder() {}
  virtual T value() = 0;
  virtual T& reference() = 0;
  virtual T* pointer() = 0;
};

template<typename T> class ConstantHolder
{
public:
  virtual ~ConstantHolder() {}
  virtual const T value() const = 0;
  virtual const T& reference() const = 0;
  virtual const T* pointer() const = 0;
};

template<class T, class W, class H> class Assumption
{
public:
  typedef std::string Id;
  typedef unsigned int Index;
  typedef std::unique_ptr<T[]> Array;
  typedef std::unique_ptr<W[]> WrapperArray;
  typedef std::unique_ptr<H> HolderPtr;
  virtual ~Assumption() {}
  virtual void unique(Array& a, WrapperArray& wrapper, HolderPtr& holder) = 0;
  virtual bool has(const Id& id) = 0;
  virtual T& value(const Id& id, const Index& index) = 0;
  virtual W& wrapper(const Id& id, const Index& index) = 0;
  virtual H& holder(const Id& id) = 0;
};

} /* namespace interfaces */
} /* namespace gos */

#endif /* _GOS_ASSUMPTION_INTERFACES_H_ */
