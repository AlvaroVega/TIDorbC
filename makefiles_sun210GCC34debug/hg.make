# Makefile de libro de copia de headers exportados (.hg)

mk: $(DIR_MK) install

install: $(DIR_INSTALL) $(HG_MK)

clean: $(DIR_CLEAN) $(HG_CLEAN)

hg_mk:
	@$(KSH)  -c '\
	listaF=`$(FIND) . -name "*.[Hh]"`;\
	for f in $$listaF ;\
	do \
	  g=$(DES_HDRS)/$$f ;\
	  if [[ $$f -nt $$g ]] ;\
	  then \
	    $(MKDIR) -p $(DES_HDRS) ;\
	    $(ECHO) "$(CP_COMANDO) $$f $$g" ;\
	    $(RM) -f $$g ;\
	    $(CP_COMANDO) -f $$f $$g ;\
	  fi ;\
	done ;\
	'
	
hg_clean:
	@$(KSH)	 -c '\
	listaF=`$(FIND) . -name "*.[Hh]"`;\
	for f in $$listaF ;\
	do \
	  g=$(DES_HDRS)/$$f ;\
	  $(ECHO) $(RM) -f $$g ;\
	  $(RM) -f $$g ;\
	done ;\
	'


