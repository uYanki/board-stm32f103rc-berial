//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_META_H
#define EGL_META_H
//----------------------------------------------------------------------------


//============================================================================
// interface
//============================================================================
// external
#include "egl_core.h"
EGL_NAMESPACE_BEGIN

// new
// type relation traits
template<typename T, typename U> struct is_type_equal;   // test for equality of types (const-volatile insensitive)
template<typename T, typename U> struct is_type_cvequal; // test for equality of types (const-volatile sensitive)
// type mutators
template<typename> struct add_const;
template<typename> struct add_volatile;
template<typename> struct add_cv;
//----------------------------------------------------------------------------




//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================


//============================================================================
// is_type_equal
//============================================================================
template<typename T, typename U>
struct is_type_equal
{
  enum {res=is_type_cvequal<typename add_cv<T>::res, typename add_cv<U>::res>::res};
};
//----------------------------------------------------------------------------


//============================================================================
// is_type_cvequal
//============================================================================
template<typename T, typename U>
struct is_type_cvequal
{
  enum {res=false};
};
//----

template<typename T>
struct is_type_cvequal<T, T>
{
  enum {res=true};
};
//----------------------------------------------------------------------------


//============================================================================
// add_const
//============================================================================
template<typename T>
struct add_const
{
  typedef const T res;
};
//----

template<typename T>
struct add_const<T&>
{
  typedef T &res;
};
//----------------------------------------------------------------------------


//============================================================================
// add_volatile
//============================================================================
template<typename T>
struct add_volatile
{
  typedef volatile T res;
};
//----

template<typename T>
struct add_volatile<T&>
{
  typedef T &res;
};
//----------------------------------------------------------------------------


//============================================================================
// add_cv
//============================================================================
template<typename T>
struct add_cv
{
  typedef const volatile T res;
};
//----

template<typename T>
struct add_cv<T&>
{
  typedef T &res;
};
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
