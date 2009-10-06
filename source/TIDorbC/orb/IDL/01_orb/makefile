#----------------------------------------------------------------------
# Cambiar el orden de compilacion
#----------------------------------------------------------------------


#----------------------------------------------------------------------
# Incluir las reglas de compilacion
#---------------------------------------------------------------------

TIPO_NIVEL=do

include $(DIRMAKE)/make.rules 

MYIDL_FLAGS=-expanded
MY_DELETE_FILES=source/CORBA/ORB.C source/CORBA/TypeCode.C

include $(DIRMAKE)/$(TIPO_NIVEL).make
