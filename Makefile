#================================[[ beg-code ]]================================#



#===(manditory)============================================#
# basename of executable, header, and all code files
NAME_BASE  = eos
# additional standard and outside libraries
LIB_STD    = -lm -lrt
# all heatherly libraries used, debug versions whenever available
LIB_MINE   = -lyDLST_debug -lySEC_debug -lyEXEC_debug -lyJOBS_debug -lySTR_debug -lyPARSE_debug
# all heatherly libraries used only in unit tests
LIB_MYUNIT = -lyENV_debug
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
include /usr/local/sbin/zeno_make



#===(post-processing)======================================#
# create a rule for...
install_post       :
	ln --force --physical /sbin/eos        /sbin/astraios
	@sha1sum  /sbin/astraios
	ln --force --physical /sbin/eos        /sbin/hypnos
	@sha1sum  /sbin/hypnos
	ln --force --physical /sbin/eos        /sbin/heracles
	@sha1sum  /sbin/heracles
	ln --force --physical /sbin/eos        /sbin/H
	@sha1sum  /sbin/H
	ln --force --physical /sbin/eos_debug  /sbin/astraios_debug
	ln --force --physical /sbin/eos_debug  /sbin/hypnos_debug
	ln --force --physical /sbin/eos_debug  /sbin/heracles_debug


#remove_post        :



#================================[[ end-code ]]================================#
