# Makefile de libro de copia (cp)

# Variables:
#       CP_DESTINO  -- Directorio donde se copia
#       CP_CON_EXT  -- Conservar la extension al copiar (SI|NO)
#       CP_PERMISOS -- Permisos a poner en la copia (por defecto: ???)
#       CP_NCP      -- Patron de ficheros a OMITIR en la copia
#       CP_COMANDO  -- Comando de copia (TIENE QUE SER 'com file > file2')
#       CP_MK       -- Si se asigna con NO en el lmk, se omite el TARGET
#       CP_CLOBBER  -- idem CP_MK
#       CP_MYPREFIX -- Prefijo que se asigna a los ficheros que se copian
#       CP_MYEXT    -- id. extension o sufijo
# NOTA: el comando find no encuentra los ficheros que no tengan una
#       extension ("*.*").

mk: $(DIR_MK) $(CP_MK)

install: $(DIR_INSTALL)

clean: $(DIR_CLEAN) $(CP_CLEAN)

cp_mk:
	@$(KSH)  -c '\
	if [[ $(CP_CON_EXT) = "SI" ]] ;\
	then \
	  ext=; else ext=".*";\
	fi ;\
	listaF=`$(FIND) * -type f | grep -v [Mm]ake | grep -v "\.cp/" | grep -v "CVS/"`;\
	listaF="$$listaF `$(FIND) * -type l | grep -v [Mm]ake | grep -v "\.cp/"`";\
	listaD=`$(FIND) * -type d | grep -v "\.cp" | grep -v "CVS/"`;\
	for f in $$listaF ;\
	do \
	  TF=$${f#./} ;\
	  g=$(CP_DESTINO)/$(CP_MYPREFIX)$${TF%$$ext}$(CP_MYEXT) ;\
	  if [[ $$TF -nt $$g ]] ;\
	  then \
	    dir=$${g%/*} ;\
	    $(MKDIR) -p $$dir ;\
	    $(ECHO) "$(CP_COMANDO) $$TF $$g" ;\
	    $(CP_COMANDO) -f $$TF $$g ;\
	    $(ECHO) "$(CHMOD) $(CP_PERMISOS) $$g" ;\
	    $(CHMOD) $(CP_PERMISOS) $$g; $(ECHO) ;\
	  fi ;\
	done ;\
	for f in $$listaD ;\
	do \
	  g=$(CP_DESTINO)/$(CP_MYPREFIX)$$f ;\
	  if [[ $$f -nt $$g ]] ;\
	  then \
	    $(ECHO) $(MKDIR) -p $$g ;\
	    $(MKDIR) -p $$g ;\
	  fi ;\
	done ;\
	'
	
cp_clean:
	@$(KSH)  -c ' \
	if [[ $(CP_CON_EXT) = "SI" ]] ;\
	then \
	  ext=; else ext=".*";\
	fi ;\
	listaF=`$(FIND) . -type f | grep -v [Mm]ake | grep -v "\.cp/" | grep -v "CVS/"`;\
	listaF="$$listaF `$(FIND) . -type l | grep -v [Mm]ake | grep -v "\.cp/"`";\
	listaD=`$(FIND) * -type d | grep -v "\.cp" | grep -v "CVS/"`;\
	for f in $$listaF ;\
	do \
	  TF=$${f#./} ;\
	  g=$(CP_DESTINO)/$(CP_MYPREFIX)$${TF%$$ext}$(CP_MYEXT) ;\
	  if [[ -a $$g ]] ;\
	  then \
	    $(ECHO) "$(RM) -f $$g" ;\
	    $(RM) -f $$g ;\
	  fi ;\
	done ;\
	for f in $$listaD ;\
	do \
	  g=$(CP_DESTINO)/$(CP_MYPREFIX)$$f ;\
	  if [[ -a $$g ]] ;\
	  then \
	    $(ECHO) $(RMDIR) $$g ;\
	    $(RMDIR) $$g ;\
	  fi ;\
	done ;\
	'


