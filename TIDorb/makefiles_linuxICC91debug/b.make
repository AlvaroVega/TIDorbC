###############################################################################
# b.make
# makefile que contiene las reglas de compilación de libros .b
# Los libros .b generan una libreria con el mismo nombre del directorio a
# partir de los fuentes que contiene el directorio .b
###############################################################################

include $(DEPEN_FILE)

IDL_OBJS=$(shell ls _$(TARGET).O/*_r.o _$(TARGET).O/*_stc.o _$(TARGET).O/POA*.o 2>/dev/null)
IDL_SRC=$(shell ls *.idl *.idlp 2>/dev/null)

DO_IDL_SRC=$(shell ls *.do/*.idl *.do/*.idlp 2>/dev/null)
DO_IDL_OBJS=$(shell ls */_$(TARGET).O/*.o 2>/dev/null)

DO_C_SRC=$(shell ls */*.C 2>/dev/null)
DO_C_OBJS:=$(shell echo $(DO_C_SRC) | sed -e s/\\.C/\.o/g | sed s/\\//\\/_${TARGET}.O\\//g)

DO_PC_SRC=$(shell ls *.do/*.PC 2>/dev/null)
DO_PC_OBJS:=$(shell echo $(DO_PC_SRC) | sed -e s/\\.PC/\.o/g | sed s/\\//\\/_${TARGET}.O\\//g)

DO_pc_SRC=$(shell ls *.do/*.pc 2>/dev/null)
DO_pc_OBJS:=$(shell echo $(DO_pc_SRC) | sed -e s/\\.pc/\.o/g | sed s/\\//\\/_${TARGET}.O\\//g)

#DO_OBJS=$(shell find . -name "*.o" 2>/dev/null)
DO_OBJS=$(shell find . -name "*.o" | grep  _$(TARGET).O 2>/dev/null)

# Regla para la creacion de librerias
######################################

mk: $(DIR_MK) $(ARCHIVE) $(SL_ARCHIVE) install

$(ARCHIVE): $(OBJETOS) $(IDL_OBJS) $(DO_OBJS) $(DO_IDL_OBJS)
	$(PRE_AR)
	$(AR_CMD)
	$(POST_AR)

$(SL_ARCHIVE): $(OBJETOS) $(IDL_OBJS) $(DO_OBJS) $(DO_IDL_OBJS)
	$(PRE_SL)
	$(SL_CMD)

clean: $(DIR_CLEAN)
	@echo "---> `pwd` <---"
	rm -f $(OBJETOS)
	rm -f $(SL_ARCHIVE) $(SL_ARCHIVE_INS)
	rm -f $(ARCHIVE) $(ARCHIVE_INS)
	rm -f $(IDL_OBJS) _$(TARGET).O/*.C
	rm -f _$(TARGET).O/_$(TARGET).imk
	@$(KSH) -c '\
	if [ "x$(IDL_SRC)" != "x" ] ;\
	then \
	  for i in "$(OBJETOS)" ;\
	  do \
	    o=$${i#*/} ;\
	    $(ECHO) $(RM) -f $(LN_IDL)/$${o%.*}.idl ;\
	    $(RM) -f $(LN_IDL)/$${o%.*}.idl ;\
	    $(ECHO) $(RM) -f $(DES_HDRS)/idl/*$${o%.*}*.h ;\
	    $(RM) -f $(DES_HDRS)/idl/*$${o%.*}*.h ;\
	  done ;\
	fi ;\
	if [[ -f _$(TARGET).O/lista_clobber ]] ;\
	then \
	 cat _$(TARGET).O/lista_clobber | while read i ;\
	 do \
	  $(ECHO) $(RM) -f $$i ;\
	  $(RM) -f $$i ;\
	 done ;\
	fi ;\
	$(RM) -f _$(TARGET).O/lista_clobber ;\
	'

install: $(DIR_INSTALL) $(SL_ARCHIVE_INS) $(ARCHIVE_INS)

$(ARCHIVE_INS): $(ARCHIVE)
	@$(KSH) -c '\
	if [[ "$(STATIC)" = "SI" ]] ;\
	then \
	  if [[ $(ARCHIVE) -nt $(ARCHIVE_INS) ]] ;\
	  then \
	    $(MKDIR) -p $(DES_LIBS) ;\
	    $(ECHO) "\n---> Copiando $(ARCHIVE) a $(DES_LIBS)\n" ;\
	    $(ECHO) $(CP) -f $(ARCHIVE) $(ARCHIVE_INS) ;\
	    $(CP) -f $(ARCHIVE) $(ARCHIVE_INS) ;\
	  fi ;\
	fi'

$(SL_ARCHIVE_INS): $(SL_ARCHIVE)
	@$(KSH) -c '\
	if [[ $(SL_ARCHIVE) -nt $(SL_ARCHIVE_INS) ]] ;\
	then \
	  $(MKDIR) -p $(DES_LIBS) ;\
	  echo "\n---> Copiando $(SL_ARCHIVE) a $(DES_LIBS)\n" ;\
	  echo $(CP) -f $(SL_ARCHIVE) $(SL_ARCHIVE_INS) ;\
	  $(CP) -f $(SL_ARCHIVE) $(SL_ARCHIVE_INS) ;\
	fi'

depen: $(DIR_DEPEN) borra_fdepen $(DEPENDENCIAS)


