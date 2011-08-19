/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef _ISLABS_BUILD_ARGUMENTS_PRIVATE_H_
#define _ISLABS_BUILD_ARGUMENTS_PRIVATE_H_

#include "islabs/build/Arguments.h"

#define DEF		113
#define D		145
#define F		147
#define G		148
#define M		154
#define N		155
#define R		159
#define S		160
#define T		161
#define V		163
#define WC		231
#define WL		240
#define NO		311
#define ALL		403
#define LOG		412
#define NEW		420
#define DEEP		504
#define HASH		510
#define NEAR		512
#define HELP		515
#define FAST		520
#define TREE		522
#define FULL		525
#define CLEAN		605
#define DEBUG		609
#define MARCH		613
#define FORCE		617
#define DEPTH		623
#define WORLD		642
#define STATIC		693
#define LEGACY		719
#define SHARED		721
#define TARGET		737
#define STATIC2		738
#define SOURCE		747	//?
#define RELEASE		827
#define DYNAMIC		831
#define VERSION		864
#define COPY_LIBS		1004
#define RUN_TESTS		1039
#define DEBUG_LIBC		1064
#define RECURSIVE		1074
#define MONOLITHIC		1168
#define BUILD_TESTS		1226
#define RUN_TESTS_ONLY		1534
#define WITH_PROFILING		1549
#define BUILD_TESTS_ONLY	1721
#define BUILD_AND_RUN_TESTS_ONLY	2459

unsigned int simple_hash( const char* string );

struct _Arguments {
	char* command;
	int whatever;

	unsigned int verbosity;
	char*	package_dir;
	int	invalid;
	int	near;
	int	no;
	int	all;
	int	log;
	int	new;
	int	deep;
	int depth;
	int	help;
	int	fast;
	int	tree;
	int	full;
	int	clean;
	int	debug;
	char*	march;
	int     force;
	int	legacy;
	int	buildShared;
	char*	target;
	int	buildStatic;
	int	release;
	int	dynamic;
	int	version;
	int	copyLibs;
	int	runTests;
	int	debugLibc;
	int	recursive;
	int	monolithic;
	int	buildTests;
	int	runTestsOnly;
	int	withProfiling;
	int	buildTestsOnly;
	
	IList*  defines;
};

#endif
