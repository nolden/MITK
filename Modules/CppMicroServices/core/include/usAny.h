/*=============================================================================

  Library: CppMicroServices


Copyright Kevlin Henney, 2000, 2001, 2002. All rights reserved.
Extracted from Boost 1.46.1 and adapted for CppMicroServices.

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

=========================================================================*/

#ifndef US_ANY_H
#define US_ANY_H

#include <algorithm>
#include <typeinfo>
#include <sstream>
#include <vector>
#include <list>
#include <set>
#include <map>

#include <usCoreConfig.h>

US_BEGIN_NAMESPACE

class Any;

US_Core_EXPORT std::string any_value_to_string(const Any& any);

US_Core_EXPORT std::string any_value_to_json(const Any& val);
US_Core_EXPORT std::string any_value_to_json(const std::string& val);
US_Core_EXPORT std::string any_value_to_json(bool val);

template<class T>
std::string any_value_to_string(const T& val)
{
  std::stringstream ss;
  ss << val;
  return ss.str();
}

template<class T>
std::string any_value_to_json(const T& val)
{
  return any_value_to_string(val);
}

/**
 * \internal
 */
template<typename Iterator>
std::string container_to_string(Iterator i1, Iterator i2)
{
  std::stringstream ss;
  ss << "[";
  const Iterator begin = i1;
  for ( ; i1 != i2; ++i1)
  {
    if (i1 == begin) ss << any_value_to_string(*i1);
    else ss << "," << any_value_to_string(*i1);
  }
  ss << "]";
  return ss.str();
}

/**
 * \internal
 */
template<typename Iterator>
std::string container_to_json(Iterator i1, Iterator i2)
{
  std::stringstream ss;
  ss << "[";
  const Iterator begin = i1;
  for ( ; i1 != i2; ++i1)
  {
    if (i1 == begin) ss << any_value_to_json(*i1);
    else ss << "," << any_value_to_json(*i1);
  }
  ss << "]";
  return ss.str();
}

template<class E>
std::string any_value_to_string(const std::vector<E>& vec)
{
  return container_to_string(vec.begin(), vec.end());
}

template<class E>
std::string any_value_to_json(const std::vector<E>& vec)
{
  return container_to_json(vec.begin(), vec.end());
}

template<class E>
std::string any_value_to_string(const std::list<E>& l)
{
  return container_to_string(l.begin(), l.end());
}

template<class E>
std::string any_value_to_json(const std::list<E>& l)
{
  return container_to_json(l.begin(), l.end());
}

template<class E>
std::string any_value_to_string(const std::set<E>& s)
{
  return container_to_string(s.begin(), s.end());
}

template<class E>
std::string any_value_to_json(const std::set<E>& s)
{
  return container_to_json(s.begin(), s.end());
}

template<class M>
std::string any_value_to_string(const std::map<M, Any>& m);

template<class K, class V>
std::string any_value_to_string(const std::map<K, V>& m);

template<class M>
std::string any_value_to_json(const std::map<M, Any>& m);

template<class K, class V>
std::string any_value_to_json(const std::map<K, V>& m);


/**
 * \ingroup MicroServicesUtils
 *
 * An Any class represents a general type and is capable of storing any type, supporting type-safe extraction
 * of the internally stored data.
 *
 * Code taken from the Boost 1.46.1 library. Original copyright by Kevlin Henney. Modified for CppMicroServices.
 */
class Any
{
public:

  /**
   * Creates an empty any type.
   */
  Any(): _content(nullptr)
  { }

  /**
   * Creates an Any which stores the init parameter inside.
   *
   * \param value The content of the Any
   *
   * Example:
   * \code
   * Any a(13);
   * Any a(string("12345"));
   * \endcode
   */
  template <typename ValueType>
  Any(const ValueType& value)
    : _content(new Holder<ValueType>(value))
  { }

  /**
   * Copy constructor, works with empty Anys and initialized Any values.
   *
   * \param other The Any to copy
   */
  Any(const Any& other)
    : _content(other._content ? other._content->Clone() : nullptr)
  { }

  ~Any()
  {
    delete _content;
  }

  /**
   * Swaps the content of the two Anys.
   *
   * \param rhs The Any to swap this Any with.
   */
  Any& Swap(Any& rhs)
  {
    std::swap(_content, rhs._content);
    return *this;
  }

  /**
   * Assignment operator for all types != Any.
   *
   * \param rhs The value which should be assigned to this Any.
   *
   * Example:
   * \code
   * Any a = 13;
   * Any a = string("12345");
   * \endcode
   */
  template <typename ValueType>
  Any& operator = (const ValueType& rhs)
  {
    Any(rhs).Swap(*this);
    return *this;
  }

  /**
   * Assignment operator for Any.
   *
   * \param rhs The Any which should be assigned to this Any.
   */
  Any& operator = (const Any& rhs)
  {
    Any(rhs).Swap(*this);
    return *this;
  }

  /**
   * returns true if the Any is empty
   */
  bool Empty() const
  {
    return !_content;
  }

  /**
   * Returns a string representation for the content.
   *
   * Custom types should either provide a <code>std::ostream& operator<<(std::ostream& os, const CustomType& ct)</code>
   * function or specialize the any_value_to_string template function for meaningful output.
   */
  std::string ToString() const
  {
    return _content->ToString();
  }

  /**
   * Returns a JSON representation for the content.
   *
   * Custom types should specialize the any_value_to_json template function for meaningful output.
   */
  std::string ToJSON() const
  {
    return Empty() ? "null" : _content->ToJSON();
  }

  /**
   * Returns the type information of the stored content.
   * If the Any is empty typeid(void) is returned.
   * It is suggested to always query an Any for its type info before trying to extract
   * data via an any_cast/ref_any_cast.
   */
  const std::type_info& Type() const
  {
    return _content ? _content->Type() : typeid(void);
  }

private:

  class Placeholder
  {
  public:
    virtual ~Placeholder()
    { }

    virtual std::string ToString() const = 0;
    virtual std::string ToJSON() const = 0;

    virtual const std::type_info& Type() const = 0;
    virtual Placeholder* Clone() const = 0;
  };

  template <typename ValueType>
  class Holder: public Placeholder
  {
  public:
    Holder(const ValueType& value)
      : _held(value)
    { }

    virtual std::string ToString() const override
    {
      return any_value_to_string(_held);
    }

    virtual std::string ToJSON() const override
    {
      return any_value_to_json(_held);
    }

    virtual const std::type_info& Type() const override
    {
      return typeid(ValueType);
    }

    virtual Placeholder* Clone() const override
    {
      return new Holder(_held);
    }

    ValueType _held;

  private: // intentionally left unimplemented
    Holder& operator=(const Holder &);
  };

private:
    template <typename ValueType>
    friend ValueType* any_cast(Any*);

    template <typename ValueType>
    friend ValueType* unsafe_any_cast(Any*);

    Placeholder* _content;
};

class BadAnyCastException : public std::bad_cast
{
public:

  BadAnyCastException(const std::string& msg = "")
    : std::bad_cast(), _msg(msg)
  {}

  ~BadAnyCastException() throw() {}

  virtual const char * what() const throw() override
  {
    if (_msg.empty())
      return "US_PREPEND_NAMESPACE(BadAnyCastException): "
             "failed conversion using US_PREPEND_NAMESPACE(any_cast)";
    else
      return _msg.c_str();
  }

private:

  std::string _msg;
};

/**
 * any_cast operator used to extract the ValueType from an Any*. Will return a pointer
 * to the stored value.
 *
 * Example Usage:
 * \code
 * MyType* pTmp = any_cast<MyType*>(pAny)
 * \endcode
 * Will return NULL if the cast fails, i.e. types don't match.
 */
template <typename ValueType>
ValueType* any_cast(Any* operand)
{
  return operand && operand->Type() == typeid(ValueType)
      ? &static_cast<Any::Holder<ValueType>*>(operand->_content)->_held
      : nullptr;
}

/**
 * any_cast operator used to extract a const ValueType pointer from an const Any*. Will return a const pointer
 * to the stored value.
 *
 * Example Usage:
 * \code
 * const MyType* pTmp = any_cast<MyType*>(pAny)
 * \endcode
 * Will return NULL if the cast fails, i.e. types don't match.
 */
template <typename ValueType>
const ValueType* any_cast(const Any* operand)
{
  return any_cast<ValueType>(const_cast<Any*>(operand));
}

/**
 * any_cast operator used to extract a copy of the ValueType from an const Any&.
 *
 * Example Usage:
 * \code
 * MyType tmp = any_cast<MyType>(anAny)
 * \endcode
 * Will throw a BadCastException if the cast fails.
 * Dont use an any_cast in combination with references, i.e. MyType& tmp = ... or const MyType& = ...
 * Some compilers will accept this code although a copy is returned. Use the ref_any_cast in
 * these cases.
 */
template <typename ValueType>
ValueType any_cast(const Any& operand)
{
  ValueType* result = any_cast<ValueType>(const_cast<Any*>(&operand));
  if (!result) throw BadAnyCastException("Failed to convert between const Any types");
  return *result;
}

/**
 * any_cast operator used to extract a copy of the ValueType from an Any&.
 *
 * Example Usage:
 * \code
 * MyType tmp = any_cast<MyType>(anAny)
 * \endcode
 * Will throw a BadCastException if the cast fails.
 * Dont use an any_cast in combination with references, i.e. MyType& tmp = ... or const MyType& tmp = ...
 * Some compilers will accept this code although a copy is returned. Use the ref_any_cast in
 * these cases.
 */
template <typename ValueType>
ValueType any_cast(Any& operand)
{
  ValueType* result = any_cast<ValueType>(&operand);
  if (!result) throw BadAnyCastException("Failed to convert between Any types");
  return *result;
}

/**
 * ref_any_cast operator used to return a const reference to the internal data.
 *
 * Example Usage:
 * \code
 * const MyType& tmp = ref_any_cast<MyType>(anAny);
 * \endcode
 */
template <typename ValueType>
const ValueType& ref_any_cast(const Any & operand)
{
  ValueType* result = any_cast<ValueType>(const_cast<Any*>(&operand));
  if (!result) throw BadAnyCastException("RefAnyCast: Failed to convert between const Any types");
  return *result;
}

/**
 * ref_any_cast operator used to return a reference to the internal data.
 *
 * Example Usage:
 * \code
 * MyType& tmp = ref_any_cast<MyType>(anAny);
 * \endcode
 */
template <typename ValueType>
ValueType& ref_any_cast(Any& operand)
{
  ValueType* result = any_cast<ValueType>(&operand);
  if (!result) throw BadAnyCastException("RefAnyCast: Failed to convert between Any types");
  return *result;
}

/**
 * \internal
 *
 * The "unsafe" versions of any_cast are not part of the
 * public interface and may be removed at any time. They are
 * required where we know what type is stored in the any and can't
 * use typeid() comparison, e.g., when our types may travel across
 * different shared libraries.
 */
template <typename ValueType>
ValueType* unsafe_any_cast(Any* operand)
{
  return &static_cast<Any::Holder<ValueType>*>(operand->_content)->_held;
}

/**
 * \internal
 *
 * The "unsafe" versions of any_cast are not part of the
 * public interface and may be removed at any time. They are
 * required where we know what type is stored in the any and can't
 * use typeid() comparison, e.g., when our types may travel across
 * different shared libraries.
 */
template <typename ValueType>
const ValueType* unsafe_any_cast(const Any* operand)
{
  return any_cast<ValueType>(const_cast<Any*>(operand));
}


template<class K>
std::string any_value_to_string(const std::map<K, Any>& m)
{
  std::stringstream ss;
  ss << "{";
  typedef typename std::map<K, Any>::const_iterator Iterator;
  auto i1 = m.begin();
  const Iterator begin = i1;
  const Iterator end = m.end();
  for ( ; i1 != end; ++i1)
  {
    if (i1 == begin) ss << i1->first << " : " << i1->second.ToString();
    else ss << ", " << i1->first << " : " << i1->second.ToString();
  }
  ss << "}";
  return ss.str();
}

template<class K, class V>
std::string any_value_to_string(const std::map<K, V>& m)
{
  std::stringstream ss;
  ss << "{";
  typedef typename std::map<K, V>::const_iterator Iterator;
  auto i1 = m.begin();
  const Iterator begin = i1;
  const Iterator end = m.end();
  for ( ; i1 != end; ++i1)
  {
    if (i1 == begin) ss << i1->first << " : " << i1->second;
    else ss << ", " << i1->first << " : " << i1->second;
  }
  ss << "}";
  return ss.str();
}

template<class K>
std::string any_value_to_json(const std::map<K, Any>& m)
{
  std::stringstream ss;
  ss << "{";
  typedef typename std::map<K, Any>::const_iterator Iterator;
  auto i1 = m.begin();
  const Iterator begin = i1;
  const Iterator end = m.end();
  for ( ; i1 != end; ++i1)
  {
    if (i1 == begin) ss << "\"" << i1->first << "\" : " << i1->second.ToJSON();
    else ss << ", " << "\"" << i1->first << "\" : " << i1->second.ToJSON();
  }
  ss << "}";
  return ss.str();
}

template<class K, class V>
std::string any_value_to_json(const std::map<K, V>& m)
{
  std::stringstream ss;
  ss << "{";
  typedef typename std::map<K, V>::const_iterator Iterator;
  auto i1 = m.begin();
  const Iterator begin = i1;
  const Iterator end = m.end();
  for ( ; i1 != end; ++i1)
  {
    if (i1 == begin) ss << "\"" << i1->first << "\" : " << i1->second;
    else ss << ", " << "\"" << i1->first << "\" : " << i1->second;
  }
  ss << "}";
  return ss.str();
}

US_END_NAMESPACE

#endif // US_ANY_H
