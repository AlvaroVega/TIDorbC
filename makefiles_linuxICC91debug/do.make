###############################################################################
# do.make
# makefile que contiene las reglas de compilación de libros .do
# Los libros .do solo realizan el proceso de compilacion y llevan
# los objetos al nivel anterior para su posterior enlazado.
###############################################################################

include $(DEPEN_FILE)

mk: $(DIR_MK) $(OBJETOS)

clean: $(DIR_CLEAN)
	@$(KSH) -c '\
	echo "---> `pwd` <---" ;\
	rm -Rf _$(TARGET).O ;\
	'

depen: $(DIR_DEPEN) borra_fdepen $(DEPENDENCIAS)


