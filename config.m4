PHP_ARG_ENABLE(libreoffice, whether to enable the libreoffice extension,
[  --enable-libreoffice	Enable the libreoffice extension])

if test "$PHP_libreoffice" != "no"; then





    if test -f "/opt/libreoffice6.0/program/types.rdb"; then       
            PHP_PUNO_URE_TYPES_RDB_PATH=/opt/libreoffice6.0/program/types.rdb
            PHP_SUBST(PHP_PUNO_URE_TYPES_RDB_PATH)
            PHP_PUNO_PROGRAM_OFFAPI_RDB_PATH=/opt/libreoffice6.0/program/types/offapi.rdb
            PHP_SUBST(PHP_PUNO_PROGRAM_OFFAPI_RDB_PATH)  
    else
            AC_MSG_ERROR([ File types.rdb could not be found. Check your libreoffice sdk install ])
    fi



    OO_SDK_HOME=/opt/libreoffice6.0/sdk
    PHP_SUBST(OO_SDK_HOME)

    PHP_LIBREOFFICE=/opt/libreoffice6.0/sdk/
    PHP_SUBST(PHP_LIBREOFFICE)


    PHP_REQUIRE_CXX()

    PRJ=$PHP_LIBREOFFICE
    PHP_SUBST(PRJ)

    OUT="\$(srcdir)/sdk/puno_automatic_generated"
    PHP_SUBST(OUT)

    PHP_ADD_LIBRARY(stdc++, 1, LIBREOFFICE_SHARED_LIBADD)


    EXTRA_CXXFLAGS="$EXTRA_CXXFLAGS \$(CC_FLAGS) \$(CC_INCLUDES) -I\$(OUT_COMP_INC) \$(CC_DEFINES)"
    PHP_SUBST(EXTRA_CXXFLAGS)	   

    EXTRA_LIBS="$EXTRA_LIBS \$(LINK_LIBS)"
    PHP_SUBST(EXTRA_LIBS)	   

    EXTRA_LDFLAGS="$EXTRA_LDFLAGS  \$(LINK_LIBS) \$(CPPUHELPERLIB) \$(CPPULIB) \$(SALHELPERLIB) \$(SALLIB) \$(STLPORTLIB)"
    PHP_SUBST(EXTRA_LDFLAGS)	   

        PHP_MODULES="cppumaker $PHP_MODULES"
        PHP_SUBST(PHP_MODULES)	

    AC_DEFINE(HAVE_LIBREOFFICELIB,1,[ ])

    PHP_SUBST(LIBREOFFICE_SHARED_LIBADD)


    PHP_ADD_MAKEFILE_FRAGMENT(Makefile.cppumaker)
    PHP_NEW_EXTENSION( libreoffice, libreoffice.cpp, $ext_shared,,,cxx)

fi