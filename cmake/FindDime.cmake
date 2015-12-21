#
# Try to find Dime library and include path.
#
# Dime_FOUND - System found Dime
# Dime_INCLUDE_PATH - include directory of Dime
# Dime_LIBRARY - Dime library
# 

# find include dir
FIND_PATH( Dime_INCLUDE_PATH dime/Input.h
	/usr/include
	/usr/local/include
	/opt/local/include
	$ENV{Dime_DIR}/include)
	
# find lib
FIND_LIBRARY( Dime_LIBRARY
	NAMES Dime dime
	PATHS
	/usr/lib64
	/usr/lib
	/usr/local/lib64
	/usr/local/lib
	/sw/lib
	/opt/local/lib
	$ENV{Dime_DIR}/lib)

# check if anything was found
if (Dime_INCLUDE_PATH)
	SET( Dime_FOUND 1 CACHE STRING "Set to 1 if Dime is found, 0 otherwise")
else (Dime_INCLUDE_PATH)
	SET( Dime_FOUND 0 CACHE STRING "Set to 1 if Dime is found, 0 otherwise")
endif (Dime_INCLUDE_PATH)

mark_as_advanced( Dime_FOUND )
