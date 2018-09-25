#include "assumption.h"

#include <memory>

#define _GOS_ASSUMPTION_TEST_EXPECTED_BOOL_SIZE 1
#define _GOS_ASSUMPTION_TEST_EXPECTED_FLOAT_SIZE 4
#define _GOS_ASSUMPTION_TEST_EXPECTED_DOUBLE_SIZE 8

TEST(assumption, size)
{
  size_t boolsize = sizeof(bool);
  EXPECT_EQ(_GOS_ASSUMPTION_TEST_EXPECTED_BOOL_SIZE, boolsize);
  size_t floatsize = sizeof(float);
  EXPECT_EQ(_GOS_ASSUMPTION_TEST_EXPECTED_FLOAT_SIZE, floatsize);
  size_t doublesize = sizeof(double);
  EXPECT_EQ(_GOS_ASSUMPTION_TEST_EXPECTED_DOUBLE_SIZE, doublesize);
}

TEST(assumption, construction)
{
  typedef gos::assumption::Order Order;
  const char* const Text = "Text";
  const std::string TextString(Text);
  const std::string Empty;
  std::string text(Text);
  Order order(text);
  EXPECT_EQ(Empty, text); // Because of the move in the constructor
  EXPECT_EQ(TextString, order.value());
  EXPECT_EQ(Empty, order.constructor_body_value());
}

TEST(assumption, defaulting)
{
  typedef gos::assumption::Wrapper<double> Wrapper;
  Wrapper wrapper;
  EXPECT_FALSE(wrapper.is_set());
  EXPECT_DOUBLE_EQ(0.0, double()); // double() should be 0.0
  EXPECT_DOUBLE_EQ(double(), wrapper.value());
}

// Moving object from one object to another
TEST(assumption, moving)
{
  typedef gos::assumption::Wrapper<double> Wrapper;
  typedef gos::interfaces::Holder<Wrapper> Interface;
  typedef gos::assumption::Holder<Wrapper> Holder;
  typedef std::unique_ptr<Interface> HolderPtr;

  const double cv(93.418);

  const char* const Text = "Text";
  const std::string TextString(Text);
  const std::string Empty;
  std::string a(Text);
  EXPECT_EQ(TextString, a);

  std::string b(std::move(a));
  EXPECT_EQ(TextString, b);
  EXPECT_EQ(Empty, a);

  Wrapper wrapper(cv);
  HolderPtr ha = std::make_unique<Holder>(wrapper);
  EXPECT_TRUE((bool)ha);
  EXPECT_EQ(cv, ha->reference().value());
  HolderPtr hb = std::move(ha);
  EXPECT_FALSE((bool)ha);
  EXPECT_TRUE((bool)hb);
  EXPECT_EQ(cv, hb->reference().value());

  HolderPtr hc;
  EXPECT_FALSE((bool)hc);
  // No problems here, but hd will also be undefined
  HolderPtr hd = std::move(hc);
  EXPECT_FALSE((bool)hc);
  EXPECT_FALSE((bool)hd);
}

// Handing over or entrusting one object to another
TEST(assumption, entrust)
{
  typedef gos::interfaces::Wrapper<std::string> WrapperInterface;
  typedef std::unique_ptr<WrapperInterface> WrapperPtr;
  typedef gos::assumption::Wrapper<std::string> Wrapper;
  typedef gos::interfaces::Holder<WrapperInterface> HolderInterface;
  typedef gos::assumption::Holder<WrapperInterface> Holder;
  typedef std::unique_ptr<HolderInterface> HolderPtr;

  const char* const Text = "Text";
  WrapperPtr a, b;
  HolderPtr h;

  a = std::make_unique<Wrapper>(std::string(Text));
  EXPECT_TRUE((bool)a);
  EXPECT_FALSE((bool)b);
  b = std::move(a);
  EXPECT_FALSE((bool)a);
  EXPECT_TRUE((bool)b);
}

TEST(assumption, reference)
{
  typedef gos::assumption::Wrapper<double> Wrapper;
  typedef gos::assumption::NestedReferenceHolder<Wrapper> Holder;
  EXPECT_EQ(std::string("A"), std::string(Holder::Name().A));
  EXPECT_EQ(std::string("ABC"), std::string(Holder::Name().ABC));
  Holder holder;
  EXPECT_FALSE(holder.collection().A.is_set());
  EXPECT_FALSE(holder.collection().ABC.is_set());
  holder.collection().A = Wrapper();
  holder.collection().ABC = Wrapper(10.10);
  Wrapper& a = holder.collection().A;
  Wrapper& abc = holder.collection().ABC;
  EXPECT_FALSE(a.is_set());
  EXPECT_TRUE(abc.is_set());
  EXPECT_DOUBLE_EQ(10.1, abc.value());
}

TEST(assumption, unique)
{
  typedef float Value;
  typedef gos::interfaces::StringHolder StringHolderInterface;
  typedef StringHolderInterface* StringHolderInterfacePtr;
  typedef std::unique_ptr<StringHolderInterface> StringUniquePtr;
  typedef gos::assumption::StringHolder StringHolder;
  typedef gos::assumption::Wrapper<Value> Wrapper;
  typedef gos::assumption::ArrayHolder<Wrapper> Holder;
  typedef gos::assumption::Assumption<Value, Wrapper, Holder> Assumption;
  typedef Assumption::Array Array;
  typedef Assumption::WrapperArray WrapperArray;
  typedef Assumption::HolderPtr HolderPtr;

  const char* const Text = "Text";
  StringUniquePtr first, second, fourth;
  StringHolderInterfacePtr third;
  first = std::make_unique<StringHolder>(Text);
  EXPECT_TRUE(first);
  EXPECT_FALSE(second);
  EXPECT_EQ(std::string(Text), std::string(first->string()));
  second = std::move(first);
  EXPECT_FALSE(first);
  EXPECT_TRUE(second);
  EXPECT_EQ(std::string(Text), second->text());
  third = second.release();
  EXPECT_FALSE(second);
  EXPECT_NE(nullptr, third);
  EXPECT_EQ(std::string(Text), third->text());
  fourth = StringUniquePtr(third);
  third = nullptr;
  EXPECT_TRUE(fourth);
  EXPECT_EQ(std::string(Text), fourth->text());
  fourth.reset();
  EXPECT_FALSE(fourth);
  EXPECT_EQ(nullptr, third);

  Array a;
  WrapperArray wrapper_array;
  HolderPtr holder;
  Assumption assumption;

  assumption.unique(a, wrapper_array, holder);
  // Expect the pointers to be unset because it has been moved into theres maps
  EXPECT_FALSE((bool)holder);
  EXPECT_FALSE((bool)wrapper_array);
  EXPECT_FALSE((bool)a);

  // Test access to items
  EXPECT_TRUE(assumption.has(assumption.UniqueId));

  // Access to value (with an id and index)
  Value& v = assumption.value(assumption.UniqueId, 2);
  EXPECT_EQ(float(), v);

  // Access to a wrapper (with an id and index)
  Wrapper& w = assumption.wrapper(assumption.UniqueId, 4);
  EXPECT_FALSE(w.is_set());
  EXPECT_EQ(float(), w.value());

  // Access to array holder
  Holder& h = assumption.holder(assumption.UniqueId);
  EXPECT_EQ(assumption.ArraySize, h.size());
  w = h.get(0);
  EXPECT_FALSE(w.is_set());
  EXPECT_EQ(0.0f, w.value());
  EXPECT_EQ(float(), w.value());  // Should be equal to 0.0f
}

