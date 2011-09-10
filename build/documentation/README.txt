                    Build Version 0.9.1 (2011) README
              Copyright (c) 2004-2011, Daniel Robert Bradley

                       One tool to build them all,
                        one tool to compile them.
                        One tool to link them all
                      and in the darkness grind them.

Last updated: 10 September 2011

ABOUT
	Build is a replacement for make. It was conceived out  of  the
	slow refinement of the makefiles that I had developed to build
	the libraries that form OpenXDS. Eventually the makefiles were
	refined to the point where I could use the exact same makefile
	for all libraries.  Unfortunately I also began to  forget  how
	the makefiles actually worked.

	I decided that I may as well  code  what  the  makefiles  were
	doing in C and be done with make. Hence build.

KEY CONCEPT

	Build uses information stored in the directory structure of  a
	project to correctly  build  libraries  or  executables.  Each
	project directory name is either prefixed with "lib"  or  not,
	indicating whether or not it is a library.

USING BUILD

	To build a build compliant project one need only change into
	the project directory and type "build":

		cd ~/myproject
		build

	or to compile all dependency projects as well:

		cd ~/myproject
		build --all

PROJECT DIRECTORY STRUCTURE

	Below is shown the directory structure of a  library  project.

	libcoffee-0.1.1/include/CoffeeAPI.h
	libcoffee-0.1.1/source/include/{Grounds.h,Milk.h}
	libcoffee-0.1.1/source/cplusplus/{Grounds.cpp,Milk.cpp}

	Header  files that would be included in a program  making  use
	of the library are  put  in  the  project  directory's  public
	include directory,  while header files private to the  library
	are put in the source directory's include directory.
	
	Below shows the  layout  of  the  project  directory  after  a
	build.  Note  that  directories  beside  the  hypons (--) are
	created during the build process.
	
	libcoffee-0.1.1/include/CoffeeAPI.h
--	libcoffee-0.1.1/lib/libcoffee.a
--	libcoffee-0.1.1/lib/{libcoffee.so.0,libcoffee.so}
--	libcoffee-0.1.1/obj/{Grounds.o,Milk.o}
	libcoffee-0.1.1/source/include/{Grounds.h,Milk.h}
	libcoffee-0.1.1/source/cplusplus/{Grounds.cpp,Milk.cpp}

	The names of  binaries are taken from the project's  directory
	name. Library names also use the version of the package.

		{LIB}{NAME}-{VERSION}

	As can be  seen  the  product of library  projects are  object
	files for each  source file in ./obj, and  three  libraries in
	./lib - two versioned shared libraries and one archive.

	Non-library  projects produce one binary that is placed in the
	./bin directory.

PLATFORM SPECIFIC SOURCE FILES

	Source files located in subdirectories of the source directory
	are compiled if the directory name matches a  section  of  the
	PLATFORM environment variable, which is  delimited by a  colon
	':' separator,  i.e.  "posix:linux-gnu:X11".  Note  that  this
	variable  must be exported.  This  allows projects to  support
	multiple  operating  systems/environments without the need for
	#ifdefs within code.

DEALING WITH LIBRARY DEPENDENCIES

	There are two  types of  library  dependencies that need to be
	catered  for  by  a  build  system.  The  first are additional
	libraries by yourself or third parties.  The second are system
	libraries  that  only  require a "-l" argument to be passed to
	the linker.

	Additional  libraries  dependencies  are  handled  by  build's
	ability  to  recursively  descend  into  and  build dependency
	project  directories  that are located  within the parent. For
	example  if our coffee library depended on a sugar library (or
	maybe   that   should  be a  caffeine library)  the  directory
	structure would be as follows:
	
--	libcoffee-0.1.1/dep/libsugar-0.1.2/include/SugarAPI.h
--	libcoffee-0.1.1/dep/libsugar-0.1.2/source/cplusplus/Sugar.cpp
	libcoffee-0.1.1/include/CoffeeAPI.h
  	libcoffee-0.1.1/source/include/{Grounds.h,Milk.h}
  	libcoffee-0.1.1/source/cplusplus/{Grounds.cpp,Milk.cpp}

	When  run if a  recursive  mode is selected (--all, --level n)
	build will descend into each  dependency and compile it before
	compiling those libraries/programs above.
	
	Build   will   add  the  public  include  directories  of  all
	dependency  packages, and of their dependencies, to the header
	search  path  of the project being built.

	Build  will  also  include  the  lib directories of dependency
	directories  to the  library  search path of the project being
	built.  And  will  add  appropriate  "-l"  arguments  for each
	library in those directories.
	
BUILDING THIRD PARTY LIBRARIES	

BUILDING WITH SYSTEM LIBRARIES	

	Build determines system libraries by checking the ./source/lib
	directory.  Within this  directory are either library files or
	link files  [see using link files]  to  the  library.  If  the
	library  is  an  actual  library,  or  the  link  is valid.  A
	"-l<lib>" argument is passed to the linker.
	
	Thus if a  project  relied  on  linking to the X Window System
	library (libX11.so) the  following text file would be added to
	the project.
	
	libcoffee-0.1.1/source/lib/libX11.so.link

	The contents of the file would be the location of the library.

	/usr/X11R6/lib/libX11.so

	Note   that  when  building  a  monolithic  executable  system
	libraries are still dynamically linked.

USING LINK FILES (handling multi-package dependencies)

	Often  it will be  inconvenient for libraries to be physically
	contained  within a parent  due  to it  being a  dependency of
	multiple projects, or perhaps maintained by another person.

	To solve this problem dependency libraries may also use a link
	file to  indicate the  location of a  dependency library.  For
	example our coffee library example could have been arranged as
	below:

--	libcoffee-0.1.1/dep/libsugar.link
	libcoffee-0.1.1/include/CoffeeAPI.h
	libcoffee-0.1.1/source/include/{Grounds.h,Milk.h}
	libcoffee-0.1.1/source/cplusplus/{Grounds.cpp,Milk.cpp}
	libsugar-0.1.2/include/Sugar.h
	libsugar-0.1.2/source/cplusplus/Sugar.cpp

	Where the contents of libsugar.link is:

		$BUILD_PATH/libsugar-0.1.2

	$BUILD_PATH  is an  environment  variable  --  but it is  also
	pre-populated with a projects  ancestry directories to several
	levels.

	It  should  be  noted  that  under  POSIX-style  systems  that
	symbolic links could be used,  however these are not available
	on windows.  Windows  style  shortcuts (Cygwin symlinks)  were
	tried, but these caused problems with CVS.

	Link files  provide a similar mechanism but also provides some
	nice  features.  A  link  file  may  contain  multiple  paths.
	Starting  at the top of the  file,  each path is tried until a
	valid  path is reached.  Thus link files may be used to easily
	indicate fine grained library version  dependencies. Note also
	that the name of the link file need not match the name  of the
	target library.

	Changes  to version  dependencies may  thereby be  recorded in
	whatever revision control system is used.

	Example:

--	libcoffee-0.1/dep/libsugar.link
	libcoffee-0.1/include/CoffeeAPI.h
	libcoffee-0.1/source/include/{Grounds.h,Milk.h}
	libcoffee-0.1/source/cplusplus/{Grounds.cpp,Milk.cpp}
	libsugar-0.1/include/Sugar.h
	libsugar-0.1/source/cplusplus/Sugar.cpp

	[libsugar.link]
	$BUILD_PATH/libsugar-0.1.2
	$BUILD_PATH/libsugar-0.1
	$BUILD_PATH/libsugar-0
	$BUILD_PATH/libsugar
	../../libsugar-0.1.2
	../../libsugar-0.1
	../../libsugar-0
	../../libsugar

	This is  useful for when your  development CVS checkout uses a
	larger grained  versioning that the one used for releases.  If
	you  keep  your  link  files  up-to-date a release may contain
	fine  grained package version dependency information and still
	be appropriately built by build.

SUPPORTED LANGUAGES

	Build currently  supports C and C++, with initial  support for
	Java.

SUPPORTED OPERATING SYSTEMS

	Linux, Mac OS X, Windows
