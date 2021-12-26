/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2006, 2008 Ferdinando Ametrano
 Copyright (C) 2012 Édouard Tallent

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#ifndef qlext_autolink_hpp
#define qlext_autolink_hpp

#include <boost/config.hpp>
#include <ql/version.hpp>

#ifdef _M_X64
#define QL_LIB_PLATFORM "-x64"
#else
#define QL_LIB_PLATFORM
#endif

/*** libraries to be linked ***/

// select thread opt:
#ifdef _MT
#define QL_LIB_THREAD_OPT "-mt"
#else
#define QL_LIB_THREAD_OPT
#endif

// select linkage opt:
#ifdef _DLL
#if defined(_DEBUG)
#define QL_LIB_RT_OPT "-gd"
#else
#define QL_LIB_RT_OPT
#endif
#else
#if defined(_DEBUG)
#define QL_LIB_RT_OPT "-sgd"
#else
#define QL_LIB_RT_OPT "-s"
#endif
#endif

#define QLEXT_LIB_NAME "QuantLibExt" QL_LIB_THREAD_OPT QL_LIB_RT_OPT QL_LIB_PLATFORM ".lib"

#pragma comment(lib, QLEXT_LIB_NAME)
#ifdef BOOST_LIB_DIAGNOSTIC
#pragma message("Will (need to) link to lib file: " QLEXT_LIB_NAME)
#endif

/* Also, these Boost libraries might be needed */
#if defined(QL_ENABLE_THREAD_SAFE_OBSERVER_PATTERN) || defined(QL_ENABLE_SINGLETON_THREAD_SAFE_INIT)
#define BOOST_LIB_NAME boost_system
#include <boost/config/auto_link.hpp>
#undef BOOST_LIB_NAME
#define BOOST_LIB_NAME boost_thread
#include <boost/config/auto_link.hpp>
#undef BOOST_LIB_NAME
#endif

#endif
