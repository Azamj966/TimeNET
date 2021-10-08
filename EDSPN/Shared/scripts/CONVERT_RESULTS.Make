###############################################################################
# Program:		CONVERT_RESULTS.Make
#
# Parameters:		The model name, the TNETHOME variable, and the models-
#			directory
#
# Purpose:		Compile the result measures code fragment and link it with
#			the existing fragment from convert_results.
#
# Project Director:	Reinhard German
#
# Programmer:		Ulrich Hertlein
#
# History:		Release 1: April 14, 1994
###############################################################################

CC	= cc
EXT_LD_FLAGS =

CFLAGS	= -O

CONVERT_OBJ	= ${TNETHOME}/Shared/bin/convert_results.o
NETIO_OBJ	= ${TNETHOME}/Shared/bin/load_net.o \
			${TNETHOME}/Shared/bin/load_funcs.o
LIB	= $(EXT_LD_FLAGS) -lm


$(MODEL).result:	$(MODEL)_ERC.c $(CONVERT_OBJ)
	$(CC) $(CFLAGS) -o $@ $(MODEL)_ERC.c \
		$(CONVERT_OBJ) $(NETIO_OBJ) $(LIB)


#
# clean-up
#
clean:	
	rm -f $(MODEL).result $(MODEL).c
