#ifndef _GOS_ASSUMPTION_BOOST_H_
#define _GOS_ASSUMPTION_BOOST_H_

#include <boost/asio.hpp>

#ifdef _WIN32
#include <SDKDDKVer.h>
#include <tchar.h>
#include <Windows.h>
#endif

#ifdef _GOS_ASSUMPTION_BOOST_STATE_MACHINE_
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
/* Moved to the CMake file */
#ifndef BOOST_MPL_LIMIT_VECTOR_SIZE
#define BOOST_MPL_LIMIT_VECTOR_SIZE 30
#endif
#endif

namespace gos
{
namespace assumption
{

#ifdef _GOS_ASSUMPTION_BOOST_STATE_MACHINE_
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

inline ::std::ostream& operator<<(::std::ostream& os, const Stage& state)
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
  void on_entry(Event const&, FSM&)
  {
    ::std::cout << "Entering engine" << ::std::endl;
  }
  template<class Event, class FSM>
  void on_exit(Event const&, FSM&)
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
    {
      std::cout << "leaving: Starting" << std::endl;
    }
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
    _row < Starting, Started, NoData >,
    //  +--------------+--------------+--------------+----------+----------+
    _row < NoData, NovosData, NoItgData >,
    _row < NoData, ItgData, NoNovosData >,
    _row < NoItgData, NovosDataAway, NoData >,
    _row < NoNovosData, ItgDataAway, NoData >,
    //  +--------------+--------------+--------------+----------+----------+
    _row < NoItgData, ItgData, HoistUnavail >,
    _row < HoistUnavail, ItgDataAway, NoItgData >,
    _row < NoNovosData, NovosData, HoistUnavail >,
    _row < HoistUnavail, NovosDataAway, NoNovosData >,
    _row < HoistUnavail, HoistReady, Available >,
    //  +--------------+--------------+--------------+----------+----------+
    _row < Available, HoistAway, HoistUnavail >,
    _row < Available, NovosDataAway, NoNovosData >,
    _row < Available, ItgDataAway, NoItgData  >,
    //  +--------------+--------------+--------------+----------+----------+
    a_row < Available, Requested, Requesting, &Engine_::RequestControl >,
    a_row < Requesting, Granted, InControl, &Engine_::InformRequestSuccess >,
    a_row < Requesting, Timeout, Available, &Engine_::NotifyRequestFailure >,
    a_row < InControl, Lost, Available, &Engine_::NotifyLost >,
    //  +--------------+--------------+--------------+----------+----------+
    a_row < InControl, Relinquished, Relinquishing, &Engine_::RelinquishControl >,
    a_row < Relinquishing, Lost, Available, &Engine_::InformRelinquishSuccess >,
    a_row < Relinquishing, Timeout, Available, &Engine_::NotifyRelinquishFailure >,
    //  +--------------+--------------+--------------+----------+----------+
    _row < InControl, HoistAway, HoistUnavail >,
    _row < InControl, NovosDataAway, NoNovosData >,
    _row < InControl, ItgDataAway, NoItgData  >,
    //  +--------------+--------------+--------------+----------+----------+
    _row < NoData, Quit, Exiting >,
    _row < NoItgData, Quit, Exiting >,
    _row < NoNovosData, Quit, Exiting >,
    _row < HoistUnavail, Quit, Exiting >,
    _row < Available, Quit, Exiting >,
    _row < Requesting, Quit, Exiting >,
    _row < InControl, Quit, Exiting >,
    _row < Relinquishing, Quit, Exiting >,
    //  +--------------+--------------+--------------+----------+----------+
    _row < Exiting, Exited, Out >
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

#endif

} /* namespace assumption */
} /* namespace gos */

#endif /* _GOS_ASSUMPTION_BOOST_H_ */

