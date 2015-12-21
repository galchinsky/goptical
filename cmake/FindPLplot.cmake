##  FindPLplot.cmake:
##  Check for the presence of the PLplot headers and libraries
##  This file is biased to finding Fortran libraries
##  AstroFloyd, October 2010
##  
##  Copyright (c) 2010-2015 AstroFloyd - astrofloyd.org
##   
##  This file is part of the CMakeFiles package,
##  see: http://cmakefiles.sf.net/
##   
##  This is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published
##  by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
##  
##  This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
##  
##  You should have received a copy of the GNU General Public License along with this code.  If not, see 
##  <http://www.gnu.org/licenses/>.
##
##
##  This CMake module defines the following variables:
##    PLplot_FOUND        =  Libraries and headers found; TRUE/FALSE
##    PLplot_INCLUDE_DIR  =  Path to the PLplot header/module files
##    PLplot_LIBRARIES    =  Path to all parts of the PLplot libraries
##    PLplot_LIBRARY_DIRS =  Path to the directories containing the PLplot libraries



# Standard locations where to look for required components:
#include( CMakeLocations )

if( NOT PLplot_FIND_QUIETLY )
  message( STATUS "" )
  message( STATUS "Looking for PLplot..." )
endif( NOT PLplot_FIND_QUIETLY )


# Check for (FORTRAN)COMPILER-SPECIFIC header files:
find_path( PLplot_INCLUDE_DIR
  NAMES plplot.mod
  PATHS ${include_locations} ${lib_locations}
  PATH_SUFFIXES ${Fortran_COMPILER_NAME} plplot/${Fortran_COMPILER_NAME} fortran/modules/plplot/${Fortran_COMPILER_NAME}
)

# If not found, check for GENERAL Fortran header files:
if( NOT PLplot_INCLUDE_DIR )
  find_path( PLplot_INCLUDE_DIR
    NAMES plplot.mod
    PATHS ${include_locations} ${lib_locations}
    PATH_SUFFIXES plplot fortran/modules/plplot
    )
endif( NOT PLplot_INCLUDE_DIR )

# If not found, check for C header files:
if( NOT PLplot_INCLUDE_DIR )
  find_path( PLplot_INCLUDE_DIR
    NAMES plplot.h
    PATHS ${include_locations} ${lib_locations}
    PATH_SUFFIXES plplot fortran/modules/plplot
    )
endif( NOT PLplot_INCLUDE_DIR )




# Check for the libraries:
if( PLplot_INCLUDE_DIR )
  
  # Check for the library files:
  find_library( PLplot_LIBRARY
    NAMES plplot_${Fortran_COMPILER_NAME} plplotd_${Fortran_COMPILER_NAME}
    PATHS ${lib_locations}
    PATH_SUFFIXES plplot/${Fortran_COMPILER_NAME} ${Fortran_COMPILER_NAME} plplot
    )
  
  if( NOT PLplot_LIBRARY )
    find_library( PLplot_LIBRARY
      NAMES plplot plplotd
      PATHS ${lib_locations}
      PATH_SUFFIXES plplot
      )
  endif( NOT PLplot_LIBRARY )
  
  
  if( PLplot_LIBRARY )
    set( PLplot_LIBRARY_DIR "" )
    get_filename_component( PLplot_LIBRARY_DIRS ${PLplot_LIBRARY} PATH )
    # Set uncached variables as per standard.
    set( PLplot_FOUND TRUE )
    set( PLplot_LIBRARIES ${PLplot_LIBRARY} )
    
  else( PLplot_LIBRARY )
    
    set( PLplot_FOUND FALSE )
    message( STATUS "!! FindPLplot: Could not find PLplot libraries" )
  endif( PLplot_LIBRARY )
  
  
  
  
  # Find cxx bindings:
  find_library( PLplot_cxx_LIBRARY
    NAMES plplotcxx plplotcxxd
    PATHS ${lib_locations}
  )
  if( PLplot_cxx_LIBRARY )
    set( PLplot_LIBRARIES ${PLplot_LIBRARIES} ${PLplot_cxx_LIBRARY} )
  endif( PLplot_cxx_LIBRARY )
  
  
  # Find F77 bindings:
  find_library( PLplot_f77_LIBRARY
    NAMES plplotf77 plplotf77d
    PATHS ${lib_locations}
  )
  if( PLplot_f77_LIBRARY )
    set( PLplot_LIBRARIES ${PLplot_LIBRARIES} ${PLplot_f77_LIBRARY} )
  endif( PLplot_f77_LIBRARY )
  
  
  # Find F90 bindings:
  find_library( PLplot_f90_LIBRARY
    NAMES plplotf90 plplotf90d
    PATHS ${lib_locations}
  )
  if( PLplot_f90_LIBRARY )
    set( PLplot_LIBRARIES ${PLplot_LIBRARIES} ${PLplot_f90_LIBRARY} )
  endif( PLplot_f90_LIBRARY )
  
  
  # Find F95 bindings:
  # Check for COMPILER-SPECIFIC libraries:
  find_library( PLplot_f95_LIBRARY
    NAMES plplotf95_${Fortran_COMPILER_NAME} plplotf95d_${Fortran_COMPILER_NAME}
    PATHS ${lib_locations}
    )
  # If not found, check for GENERAL libraries:
  if( NOT PLplot_f95_LIBRARY )
    find_library( PLplot_f95_LIBRARY
      NAMES plplotf95 plplotf95d
      PATHS ${lib_locations}
      )
  endif( NOT PLplot_f95_LIBRARY )
  if( PLplot_f95_LIBRARY )
    set( PLplot_LIBRARIES ${PLplot_LIBRARIES} ${PLplot_f95_LIBRARY} )
    get_filename_component( PLplot_LIBRARY_DIRS ${PLplot_LIBRARY} PATH )
  endif( PLplot_f95_LIBRARY )
  
  # Find F95c bindings:
  # Check for COMPILER-SPECIFIC libraries:
  find_library( PLplot_f95c_LIBRARY
    NAMES plplotf95c_${Fortran_COMPILER_NAME} plplotf95cd_${Fortran_COMPILER_NAME}
    PATHS ${lib_locations}
    )
  # If not found, check for GENERAL libraries:
  if( NOT PLplot_f95c_LIBRARY )
    find_library( PLplot_f95c_LIBRARY
      NAMES plplotf95c plplotf95cd
      PATHS ${lib_locations}
      )
  endif( NOT PLplot_f95c_LIBRARY )
  if( PLplot_f95c_LIBRARY )
    set( PLplot_LIBRARIES ${PLplot_LIBRARIES} ${PLplot_f95c_LIBRARY} )
  endif( PLplot_f95c_LIBRARY )
  
  
  # Find wxwidgets bindings:
  find_library( PLplot_wxwidgets_LIBRARY
    NAMES plplotwxwidgets plplotwxwidgetsd
    PATHS ${lib_locations}
  )
  if( PLplot_wxwidgets_LIBRARY )
    set( PLplot_LIBRARIES ${PLplot_LIBRARIES} ${PLplot_wxwidgets_LIBRARY} )
  endif( PLplot_wxwidgets_LIBRARY )
  
  
else( PLplot_INCLUDE_DIR )
  
  set( PLplot_FOUND FALSE )
  message( STATUS "!! FindPLplot: Could not find PLplot headers" )
  
endif( PLplot_INCLUDE_DIR )


	    
if( PLplot_FOUND )
  
  if( NOT PLplot_FIND_QUIETLY )
    message( STATUS "Found components for PLplot:" )
    message( STATUS "* PLplot_INCLUDE_DIR  = ${PLplot_INCLUDE_DIR}" )
    message( STATUS "* PLplot_LIBRARIES    = ${PLplot_LIBRARIES}" )
  endif( NOT PLplot_FIND_QUIETLY )
  
else( PLplot_FOUND )
  
  if( PLplot_FIND_REQUIRED )
    message( FATAL_ERROR "!! FindPLplot: Could not find PLplot headers or libraries" )
  else( PLplot_FIND_REQUIRED )
    message( STATUS "!! FindPLplot: Could not find PLplot headers or libraries" )
  endif( PLplot_FIND_REQUIRED )
  
endif( PLplot_FOUND )
