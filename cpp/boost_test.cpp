#include "assumption.h"

#include <vector>
#include <cmath>

#include <boost/spirit/home/support/detail/endian.hpp>
#include <boost/spirit/include/qi_binary.hpp>
#include <boost/asio/buffer.hpp>

namespace detail = boost::spirit::detail;
namespace endian = boost::spirit::endian;

void* store_big_endian_array(void* pointer, const float* values, size_t count)
{
  char* local = (char*)pointer;
  for (size_t i = 0; i < count; i++)
  {
    detail::store_big_endian<float, sizeof(float)>(local, values[i]);
    local += sizeof(float);
  }
  return (void*)local;
}

void* load_little_endian_array(const void* pointer, float* values, size_t count)
{
  char* local = (char*)pointer;
  for (size_t i = 0; i < count; i++)
  {
    *values =
      detail::load_little_endian<float, sizeof(float)>((const char*)local);
    local += sizeof(float);
    values++;
  }
  return (void*)local;
}

void* load_big_endian_array(const void* pointer, float* values, size_t count)
{
  char* local = (char*)pointer;
  for (size_t i = 0; i < count; i++)
  {
    *values =
      detail::load_big_endian<float, sizeof(float)>((const char*)local);
    local += sizeof(float);
    values++;
  }
  return (void*)local;
}

TEST(boost_assumption, spirit_endian)
{
  typedef std::unique_ptr<char[]> Buffer;
  typedef float Value;
  
  const size_t count = 3;
  const size_t size = count * sizeof(Value);
  
  char* spy;
  Buffer buffer = std::make_unique<char[]>(size);

  Value f1 = 0.12f, f2 = 3.45f, f3 = 6.78f, x = 0.0f;
  Value a[count] { f1, f2, f3 };

  detail::store_big_endian<Value, sizeof(Value)>((void*)buffer.get(), f1);
  spy = (char*)(&f1);
  for (size_t i = 0; i < sizeof(Value); i++)
  {
    EXPECT_EQ(buffer[sizeof(Value) - i - 1], spy[i]);
  }

  x = detail::load_little_endian<Value, sizeof(Value)>
    ((const void*)buffer.get());
  EXPECT_FALSE(::abs(f1 - x) < 0.0000001);
  spy = (char*)(&x);
  for (size_t i = 0; i < sizeof(Value); i++)
  {
    EXPECT_EQ(buffer[i], spy[i]);
  }

  x = detail::load_big_endian<Value, sizeof(Value)>((const void*)buffer.get());
  EXPECT_TRUE(::abs(f1 - x) < 0.0000001);
  spy = (char*)(&x);
  for (size_t i = 0; i < sizeof(Value); i++)
  {
    EXPECT_EQ(buffer[sizeof(Value) - i -1], spy[i]);
  }

  void* result;

  result = store_big_endian_array(buffer.get(), a, count);
  spy = (char*)result;
  EXPECT_EQ(size, (size_t)(spy - buffer.get()));
  for (size_t i = 0; i < count; i++)
  {
    spy = buffer.get() + i * sizeof(Value);
    EXPECT_FALSE(::abs(a[i] - *spy) < 0.0000001);
  }

  Value loaded[count];
  result = load_little_endian_array(buffer.get(), loaded, count);
  spy = (char*)result;
  EXPECT_EQ(size, (size_t)(spy - buffer.get()));
  for (size_t i = 0; i < count; i++)
  {
    spy = buffer.get() + i * sizeof(Value);
    EXPECT_FALSE(::abs(loaded[i] - *spy) < 0.0000001);
    EXPECT_FALSE(::abs(loaded[i] - a[i]) < 0.0000001);
  }

  result = load_big_endian_array(buffer.get(), loaded, count);
  spy = (char*)result;
  EXPECT_EQ(size, (size_t)(spy - buffer.get()));
  for (size_t i = 0; i < count; i++)
  {
    spy = buffer.get() + i * sizeof(Value);
    EXPECT_TRUE(::abs(loaded[i] - *spy) < 0.0000001);
    EXPECT_TRUE(::abs(loaded[i] - a[i]) < 0.0000001);
  }
}


TEST(boost_assumption, spirit_qi_binary)
{
  const size_t Size = 8;
  char test[8];
  ::memset(test, 0, Size);
  test[0] = '\x01';
  test[1] = '\x02';
  test[2] = '\x03';
  test[3] = '\x04';
  test[4] = '\x05';
  test[5] = '\x06';
  test[6] = '\x07';
  test[7] = '\x08';
  boost::asio::mutable_buffer buffer(test, Size);

  uint16_t word;
  bool parse_result;
  char const* beginning;
  char const* last;

  beginning = test;
  last = beginning + 2;
  parse_result = boost::spirit::qi::parse(
    beginning,
    last,
    boost::spirit::big_word,
    word);
  EXPECT_TRUE(parse_result);
  EXPECT_TRUE(beginning == last);
  EXPECT_EQ(0x0102, word);

  beginning = test;
  last = beginning + 2;
  parse_result = boost::spirit::qi::parse(
    beginning,
    last,
    boost::spirit::little_word,
    word);
  EXPECT_TRUE(parse_result);
  EXPECT_TRUE(beginning == last);
  EXPECT_EQ(0x0201, word);

  float z;
  float x = 1.2f;
  float y = 3.4f;
  ::memcpy(test, &x, sizeof(float));
  ::memcpy(test + sizeof(float), &y, sizeof(float));

  beginning = test;
  last = beginning + sizeof(float);
#ifdef _WIN32
  parse_result = boost::spirit::qi::parse(
    beginning,
    last,
    boost::spirit::little_bin_float,
    z);

#else
  parse_result = boost::spirit::qi::parse(
    beginning,
    last,
    boost::spirit::big_bin_float,
    z);
#endif
  EXPECT_TRUE(parse_result);
  EXPECT_TRUE(beginning == last);
  EXPECT_FLOAT_EQ(x, z);

  //boost::spirit::qi::little_bin_float_type floats[2];
  //beginning = test;
  //last = beginning + sizeof(float);
  //parse_result = boost::spirit::qi::parse(
  //  beginning,
  //  last,
  //  boost::spirit::qi::little_bin_float,
  //  floats);
  //boost::spirit::terminal

  //beginning = test;
  //last = beginning + 2 * sizeof(float);
  //parse_result = boost::spirit::qi::parse(
  //  beginning,
  //  last,
  //  boost::spirit::little_bin_float,
  //  floats);
}

