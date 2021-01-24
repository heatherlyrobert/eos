#================================[[ beg-code ]]================================#



#===(manditory)============================================#
# basename of executable, header, and all code files
NAME_BASE  = eos
# additional standard and outside libraries
LIB_STD    = -lm -lrt
# all heatherly libraries used, debug versions whenever available
LIB_MINE   = -lyDLST_debug -lySEC_debug -lyEXEC_debug -lySTR_debug -lyPARSE_debug
# directory for production code, no trailing slash
INST_DIR   = /sbin



#===(optional)=============================================#
# extra include directories required
INC_MINE   = 
# utilities generated, separate from main program
NAME_UTIL  = 
# libraries only for the utilities
LIB_UTIL   = 



#===(post-processing)======================================#
# create a rule for...
#install_prep       :
#remove_prep        :



#===(master template)======================================#
include /usr/local/sbin/make_program



#===(post-processing)======================================#
# create a rule for...
install_post       :
	ln --force --physical /sbin/eos        /sbin/nyx
	@sha1sum  /sbin/nyx
	ln --force --physical /sbin/eos        /sbin/hypnos
	@sha1sum  /sbin/hypnos
	ln --force --physical /sbin/eos        /sbin/hannibal
	@sha1sum  /sbin/hannibal
	ln --force --physical /sbin/eos        /sbin/@
	@sha1sum  /sbin/@
	ln --force --physical /sbin/eos_debug  /sbin/nyx_debug
	ln --force --physical /sbin/eos_debug  /sbin/hypnos_debug
	ln --force --physical /sbin/eos_debug  /sbin/hannibal_debug


#remove_post        :



#================================[[ end-code ]]================================#
