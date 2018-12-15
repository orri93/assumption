#ifndef _GOS_ASSUMPTION_H_
#define _GOS_ASSUMPTION_H_

#include <boost/asio.hpp>

#ifdef _WIN32
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#else
#include <stdio.h>
#endif

#include <memory>
#include <cassert>
#include <utility>
#include <iostream>
#include <map>
#include <set>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>

#include "interfaces.h"

#define _GOS_ASSUMPTION_FAST_ "fast"
#define _GOS_ASSUMPTION_THREAD_SAFE_ "threadsafe"

#define _GOS_ASSUMPTION_SET_CHECK_WITH_VARIABLE_
//#define _GOS_ASSUMPTION_SET_CHECK_FROM_DEFAULT_
#define _GOS_ASSUMPTION_COMPARE_WITH_FRIEND_

//#define _GOS_ASSUMPTION_COUT_

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

namespace state_machine_boost_msm
{

enum class Stage
{
  Undefined,
  Starting,
  NoData,
  NoNovosData,
  NoItgData,
  HoistConstrainstActivityUnavailable,
  ActivitiesAvailable,
  RequestingControl,
  RelinquishingControl,
  InControl,
  Exiting,
  Out
};

namespace events
{
struct Fundament
{
  Fundament(const char* file, const int& line, Stage& stage) :
    File(file), Line(line), State(stage)
  {}
  const char* File;
  int Line;
  Stage State;
};
struct Started : Fundament
{
  Started(const char* file, const int& line, Stage& stage) :
    Fundament(file, line, stage) { }
};
struct NovosDataAvailable {};
struct NovosDataUnavailable {};
struct ItgDataAvailable {};
struct ItgDataUnavailable {};
struct HoistConstrainstActivityBecomesAvailable {};
struct HoistConstrainstActivityBecomesUnavailable {};
struct ControlRequested {};
struct ControlRelinquished {};
struct ControlLost {};
struct Granted {};
struct Timeout {};
struct Quit {};
struct Exited {};
} // namespace events

namespace visitors
{

inline ::std::ostream &operator<<(::std::ostream &os, const Stage& state)
{
  switch (state)
  {
  case Stage::Undefined: os << "[undefined state]"; break;
  case Stage::Starting: os << "Starting"; break;
  default: os << "[Unsupported state]";
  }
  return os;
}

// an easy visitor
class SomeVisitor
{
public:
  SomeVisitor() : last_state_(Stage::Undefined) {}
  template <class T>
  void visit_state(T* astate, Stage& state)
  {
    std::cout << "visiting state:" << typeid(*astate).name()
      << " with data:" << state << std::endl;
    this->last_state_ = state;
  }
  const Stage& GetLastState() const
  {
    return this->last_state_;
  }
private:
  Stage last_state_;
};

// overwrite of the base state (not default)
struct my_visitable_state
{
  // signature of the accept function
  typedef ::boost::msm::back::args<void, SomeVisitor&, Stage&> accept_sig;

  // we also want polymorphic states
  virtual ~my_visitable_state() {}
  // default implementation for states who do not need to be visited
  void accept(SomeVisitor&, int) const {}
};

} // namespace visitors

namespace bnf = ::boost::msm::front;
namespace v = visitors;

class Engine_ : public bnf::state_machine_def<Engine_, v::my_visitable_state>
{
private:
  Stage last_stage_;
protected:
  void SetLastStage(const Stage& stage) { this->last_stage_ = stage; }
  const Stage GetLastStage() const { return this->last_stage_; }
public:
  template<class Event, class FSM>
  void on_entry(Event const&, FSM &)
  {
    ::std::cout << "Entering engine" << ::std::endl;
  }
  template<class Event, class FSM>
  void on_exit(Event const&, FSM &)
  {
    ::std::cout << "Entering engine" << ::std::endl;
  }

  // The list of FSM states
  struct Starting : public bnf::state<v::my_visitable_state>
  {
    // every (optional) entry/exit methods get the event passed.
    template <class Event, class FSM>
    void on_entry(Event const& e, FSM& fsm)
    {
      fsm.SetLastStage(Stage::Starting);
      events::Fundament& fundament = (events::Fundament&)e;
      fundament.State = Stage::Starting;
      std::cout << "entering: Starting" << std::endl;
    }
    template <class Event, class FSM>
    void on_exit(Event const&, FSM&)
    { std::cout << "leaving: Starting" << std::endl; }
    void accept(visitors::SomeVisitor& vis, Stage& state) const
    {
      state = Stage::Starting;
      vis.visit_state(this, state);
    }
  };
  struct NoData : public bnf::state<visitors::my_visitable_state>
  {
    // every (optional) entry/exit methods get the event passed.
    template <class Event, class FSM>
    void on_entry(Event const&, FSM&)
    {
      std::cout << "entering: No data" << std::endl;
    }
    template <class Event, class FSM>
    void on_exit(Event const&, FSM&)
    {
      std::cout << "leaving: No data" << std::endl;
    }
    void accept(visitors::SomeVisitor& vis, Stage& state) const
    {
      state = Stage::NoData;
      vis.visit_state(this, state);
    }
  };
  struct NoNovosData : public ::boost::msm::front::state<>
  {
    // every (optional) entry/exit methods get the event passed.
    template <class Event, class FSM>
    void on_entry(Event const&, FSM&)
    {
      std::cout << "entering: No NOVOS data" << std::endl;
    }
    template <class Event, class FSM>
    void on_exit(Event const&, FSM&)
    {
      std::cout << "leaving: No NOVOS data" << std::endl;
    }
  };
  struct NoItgData : public ::boost::msm::front::state<>
  {
    // every (optional) entry/exit methods get the event passed.
    template <class Event, class FSM>
    void on_entry(Event const&, FSM&)
    {
      std::cout << "entering: No ITG data" << std::endl;
    }
    template <class Event, class FSM>
    void on_exit(Event const&, FSM&)
    {
      std::cout << "leaving: No ITG data" << std::endl;
    }
  };
  struct HoistConstrainstActivityUnavailable : public ::boost::msm::front::state<>
  {
    // every (optional) entry/exit methods get the event passed.
    template <class Event, class FSM>
    void on_entry(Event const&, FSM&)
    {
      std::cout << "entering: Hoist Constrainst Activity Unavailable" << std::endl;
    }
    template <class Event, class FSM>
    void on_exit(Event const&, FSM&)
    {
      std::cout << "leaving: Hoist Constrainst Activity Unavailable" << std::endl;
    }
  };
  struct ActivitiesAvailable : public ::boost::msm::front::state<>
  {
    // every (optional) entry/exit methods get the event passed.
    template <class Event, class FSM>
    void on_entry(Event const&, FSM&)
    {
      std::cout << "entering: Activities Available" << std::endl;
    }
    template <class Event, class FSM>
    void on_exit(Event const&, FSM&)
    {
      std::cout << "leaving: Activities Available" << std::endl;
    }
  };
  struct RequestingControl : public ::boost::msm::front::state<>
  {
    // every (optional) entry/exit methods get the event passed.
    template <class Event, class FSM>
    void on_entry(Event const&, FSM&)
    {
      std::cout << "entering: Requesting Control" << std::endl;
    }
    template <class Event, class FSM>
    void on_exit(Event const&, FSM&)
    {
      std::cout << "leaving: Requesting Control" << std::endl;
    }
  };
  struct RelinquishingControl : public ::boost::msm::front::state<>
  {
    // every (optional) entry/exit methods get the event passed.
    template <class Event, class FSM>
    void on_entry(Event const&, FSM&)
    {
      std::cout << "entering: Relinquishing Control" << std::endl;
    }
    template <class Event, class FSM>
    void on_exit(Event const&, FSM&)
    {
      std::cout << "leaving: Relinquishing Control" << std::endl;
    }
  };
  struct InControl : public ::boost::msm::front::state<>
  {
    // every (optional) entry/exit methods get the event passed.
    template <class Event, class FSM>
    void on_entry(Event const&, FSM&)
    {
      std::cout << "entering: In Control" << std::endl;
    }
    template <class Event, class FSM>
    void on_exit(Event const&, FSM&)
    {
      std::cout << "leaving: In Control" << std::endl;
    }
  };
  struct Exiting : public ::boost::msm::front::state<>
  {
    // every (optional) entry/exit methods get the event passed.
    template <class Event, class FSM>
    void on_entry(Event const&, FSM&)
    {
      std::cout << "entering: Exiting" << std::endl;
    }
    template <class Event, class FSM>
    void on_exit(Event const&, FSM&)
    {
      std::cout << "leaving: Exiting" << std::endl;
    }
  };
  struct Out : public ::boost::msm::front::state<>
  {
    // every (optional) entry/exit methods get the event passed.
    template <class Event, class FSM>
    void on_entry(Event const&, FSM&)
    {
      std::cout << "entering: Out" << std::endl;
    }
    template <class Event, class FSM>
    void on_exit(Event const&, FSM&)
    {
      std::cout << "leaving: Out" << std::endl;
    }
  };

  // the initial state of the player SM. Must be defined
  typedef Starting initial_state;

  // Transition actions
  void RequestControl(events::ControlRequested const&)
  {

  }
  void RelinquishControl(events::ControlRelinquished const&)
  {

  }
  void InformRequestSuccess(events::Granted const&)
  {
  }
  void NotifyRequestFailure(events::Timeout const&)
  {
  }
  void NotifyLost(events::ControlLost const&)
  {
  }
  void InformRelinquishSuccess(events::ControlLost const&)
  {
  }    
  void NotifyRelinquishFailure(events::Timeout const&)
  {
  }    
  
  // Shorten some types
  // Events
  typedef events::Started Started;
  typedef events::NovosDataAvailable NovosData;
  typedef events::NovosDataUnavailable NovosDataAway;
  typedef events::ItgDataAvailable ItgData;
  typedef events::ItgDataUnavailable ItgDataAway;
  typedef events::HoistConstrainstActivityBecomesAvailable HoistReady;
  typedef events::HoistConstrainstActivityBecomesUnavailable HoistAway;
  typedef events::ControlRequested Requested;
  typedef events::ControlRelinquished Relinquished;
  typedef events::ControlLost Lost;
  typedef events::Granted Granted;
  typedef events::Timeout Timeout;
  typedef events::Quit Quit;
  typedef events::Exited Exited;
  
  // States
  typedef ActivitiesAvailable Available;
  typedef HoistConstrainstActivityUnavailable HoistUnavail;
  typedef ActivitiesAvailable Avail;
  typedef RequestingControl Requesting;
  typedef RelinquishingControl Relinquishing;
  
  // Transition table for player
  struct transition_table : ::boost::mpl::vector<
    //    Start          Event          Next           Action     (Guard)
    //  +--------------+--------------+--------------+----------+----------+
   _row < Starting     , Started      , NoData >,
    //  +--------------+--------------+--------------+----------+----------+
   _row < NoData       , NovosData    , NoItgData >,
   _row < NoData       , ItgData      , NoNovosData >,
   _row < NoItgData    , NovosDataAway, NoData >,
   _row < NoNovosData  , ItgDataAway  , NoData >,
    //  +--------------+--------------+--------------+----------+----------+
   _row < NoItgData    , ItgData      , HoistUnavail >,
   _row < HoistUnavail , ItgDataAway  , NoItgData >,
   _row < NoNovosData  , NovosData    , HoistUnavail >,
   _row < HoistUnavail , NovosDataAway, NoNovosData >,
   _row < HoistUnavail , HoistReady   , Available >,
    //  +--------------+--------------+--------------+----------+----------+
   _row < Available    , HoistAway    , HoistUnavail >,
   _row < Available    , NovosDataAway, NoNovosData >,
   _row < Available    , ItgDataAway  , NoItgData  >,
    //  +--------------+--------------+--------------+----------+----------+
  a_row < Available    , Requested    , Requesting   , &Engine_::RequestControl >,
  a_row < Requesting   , Granted      , InControl    , &Engine_::InformRequestSuccess >,
  a_row < Requesting   , Timeout      , Available    , &Engine_::NotifyRequestFailure >,
  a_row < InControl    , Lost         , Available    , &Engine_::NotifyLost >,
    //  +--------------+--------------+--------------+----------+----------+
  a_row < InControl    , Relinquished , Relinquishing, &Engine_::RelinquishControl >,
  a_row < Relinquishing, Lost         , Available    , &Engine_::InformRelinquishSuccess >,
  a_row < Relinquishing, Timeout      , Available    , &Engine_::NotifyRelinquishFailure >,
    //  +--------------+--------------+--------------+----------+----------+
   _row < InControl    , HoistAway    , HoistUnavail >,
   _row < InControl    , NovosDataAway, NoNovosData >,
   _row < InControl    , ItgDataAway  , NoItgData  >,
    //  +--------------+--------------+--------------+----------+----------+
   _row < NoData       , Quit         , Exiting >,
   _row < NoItgData    , Quit         , Exiting >,
   _row < NoNovosData  , Quit         , Exiting >,
   _row < HoistUnavail , Quit         , Exiting >,
   _row < Available    , Quit         , Exiting >,
   _row < Requesting   , Quit         , Exiting >,
   _row < InControl    , Quit         , Exiting >,
   _row < Relinquishing, Quit         , Exiting >,
    //  +--------------+--------------+--------------+----------+----------+
   _row < Exiting      , Exited       , Out >
  > {};

  // Replaces the default no-transition response.
  template <class FSM, class Event>
  void no_transition(Event const& e, FSM&, int state)
  {
    std::cout << "no transition from state " << state
      << " on event " << typeid(e).name() << std::endl;
  }

  // Pick a back-end
  typedef ::boost::msm::back::state_machine<Engine_> Engine;
};

class StateEngine : public ::boost::msm::back::state_machine<Engine_>
{
public:
  const Stage GetStage() const { return GetLastStage(); }
};

} // namespace state_machine_boost_msm

} /* namespace assumption */
} /* namespace gos */

#endif /* _GOS_ASSUMPTION_H_ */

