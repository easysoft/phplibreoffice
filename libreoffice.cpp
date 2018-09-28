/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2010 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Rotsen Marcello <rotsen.marcello@wstech2.net>                |
  +----------------------------------------------------------------------+
 */

#include <php_libreoffice.h>
#include "libreoffice_class.cpp"
#include "libreoffice_converters.cpp"
#include "libreoffice_functions.cpp"


/* {{{ libreoffice_functions[]
 *
 * Every user visible function must have an entry in libreoffice_functions[].
 */

zend_function_entry libreoffice_functions[] = {
    ZEND_FE(get_remote_xcomponent, NULL)
    ZEND_FE(create_struct, NULL)
    { NULL, NULL, NULL}
};

/* {{{ libreoffice_module_entry
 */
zend_module_entry libreoffice_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "libreoffice",
    libreoffice_functions,
    PHP_MINIT(libreoffice),
    PHP_MSHUTDOWN(libreoffice),
    PHP_RINIT(libreoffice),
    PHP_RSHUTDOWN(libreoffice),
    PHP_MINFO(libreoffice),
#if ZEND_MODULE_API_NO >= 20010901
    "0.9",
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_LIBREOFFICE

BEGIN_EXTERN_C()
ZEND_GET_MODULE(libreoffice)
END_EXTERN_C()
#endif

/* {{{ php_libreoffice_init_globals
 */
/* Uncomment this function if you have INI entries*/
static void php_libreoffice_init_globals(zend_libreoffice_globals *libreoffice_globals) {
    libreoffice_globals->x_idl_reflec_rsrc_id = 0;
    libreoffice_globals->remote_service_factory_rsrc_id = 0;
    libreoffice_globals->x_type_conv_rsrc_id = 0;
}

zend_function_entry libreoffice_class_functions[] = {
    PHP_ME(libreoffice, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
//    PHP_ME(libreoffice, openDoc, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
//    PHP_ME(libreoffice, save, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
//    PHP_ME(libreoffice, exportPdf, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
//    PHP_ME(libreoffice, exportHtml, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

/* Remove if there's nothing to do at module start */

/* {{{ PHP_MINIT_FUNCTION
 */

PHP_MINIT_FUNCTION(libreoffice) {
    ZEND_INIT_MODULE_GLOBALS(libreoffice, php_libreoffice_init_globals, NULL);

    zend_class_entry libreoffice_ce;
    INIT_CLASS_ENTRY(libreoffice_ce, "libreoffice", libreoffice_class_functions);
    ce_ptr = zend_register_internal_class(&libreoffice_ce TSRMLS_CC);

    ce_ptr->create_object = libreoffice_class_object_new;
    memcpy(&libreoffice_class_handlers, zend_get_std_object_handlers(), sizeof (zend_object_handlers));
    libreoffice_class_handlers.clone_obj = NULL;

    libreoffice_class_handlers.get_method = libreoffice_class_get_method;
    libreoffice_class_handlers.call_method = libreoffice_class_call_method;
    libreoffice_class_handlers.write_property = libreoffice_class_write_property;
    libreoffice_class_handlers.read_property = libreoffice_class_read_property;
    libreoffice_class_handlers.get_property_ptr_ptr = NULL;

    /* OpenOffice Classes and Interface init functions */
    uno_refer_rsrc_dtor = zend_register_list_destructors_ex(libreoffice_refer_rsrc_dtor_hdlr, NULL, "Reference_resource_destructor", module_number);
    uno_any_rsrc_dtor = zend_register_list_destructors_ex(libreoffice_any_rsrc_dtor_hdlr, NULL, "Any_resource_destructor", module_number);
    x_compnt_fact_cli_rsrc_dtor = zend_register_list_destructors_ex(x_compnt_fact_cli_rsrc_dtor_hdlr, NULL, "XMultiComponentFactoryClient_resource_destructor", module_number);
    x_simple_reg_rsrc_dtor = zend_register_list_destructors_ex(x_simple_reg_rsrc_dtor_hdlr, NULL, "XSimpleRegistry_resource_destructor", module_number);

    //register constants
#include "libreoffice_constants.c"
    return SUCCESS;
}

//PHP_METHOD(libreoffice, save) {
//    char *path;
//    size_t path_len = 0;
//    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &path_len) == FAILURE) {
//    }
//    try {
//
//    } catch (Exception& e) {
//        //throw PHP EXCEPTION
//        zend_throw_exception(zend_exception_get_default(), (char *) OUStringToOString(e.Message, RTL_TEXTENCODING_ASCII_US).getStr(), 0 TSRMLS_CC);
//    }
//}

//PHP_METHOD(libreoffice, openDoc) {
//    char *path;
//    size_t path_len = 0;
//    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &path_len) == SUCCESS) {
//        zend_update_property_string(ce_ptr, getThis(), ZEND_STRL("path"), path TSRMLS_CC);
//    }
//    //    try {
//    //        
//    //    }catch (Exception& e) {
//    //        //throw PHP EXCEPTION
//    //        zend_throw_exception(zend_exception_get_default(), (char *) OUStringToOString(e.Message, RTL_TEXTENCODING_ASCII_US).getStr(), 0 TSRMLS_CC);
//    //    }
//}

#define DEFAULT_CONN "uno:socket,host=localhost,port=3536;urp;StarOffice.ServiceManager"

PHP_METHOD(libreoffice, __construct) {//do noting now
    int rsrc_id;
    char *str;
    size_t str_len = 0;
    PUNO_DEBUG("get_remote_xcomponent - %d<BR>", time(NULL));
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &str, &str_len) == FAILURE) {
    }
    try {
        if (str_len == 0) {
            str = DEFAULT_CONN;
            str_len = strlen(DEFAULT_CONN);
        }
        OUString sConnectionString(str, str_len, RTL_TEXTENCODING_ISO_8859_15, OSTRING_TO_OUSTRING_CVTFLAGS);

        Reference< XComponentContext > component_ctx(::cppu::defaultBootstrap_InitialComponentContext());

        PUNO_DEBUG("::cppu::bootstrap_InitialComponentContext( x_simple_registry ) - %d<BR>\n", time(NULL));

        TEST_PTR(component_ctx.is(),);

        /* Gets the service manager instance to be used (or null). This method has
         been added for convenience, because the service manager is a often used
         object.
         */
        Reference< XMultiComponentFactory > client_component_factory(
                component_ctx->getServiceManager(), UNO_QUERY);
        TEST_PTR(client_component_factory.is(),);
        //register the client factory as a resource, so the dispose method can be called at the end of the script execution
        ZEND_REGISTER_RESOURCE(NULL, new Reference<XMultiComponentFactory>(client_component_factory), x_compnt_fact_cli_rsrc_dtor);
        
        PUNO_DEBUG("x_compnt_fact_cli - %d<BR>", time(NULL));


        Reference< XUnoUrlResolver > resolver(
                client_component_factory->createInstanceWithContext(
                OUString::createFromAscii("com.sun.star.bridge.UnoUrlResolver"),
                component_ctx),
                UNO_QUERY);
        TEST_PTR(resolver.is(),);
        PUNO_DEBUG("Reference< XUnoUrlResolver > resolver( x_iface, UNO_QUERY )  - %d<BR>", time(NULL));

        // Resolves the component context from the remote office server , on the uno URL given by argv[0].
        Reference< XInterface > remote_instance = Reference< XInterface >(resolver->resolve(sConnectionString), UNO_QUERY);
        TEST_PTR(remote_instance.is(),);

        PUNO_DEBUG("x_iface = Reference< XInterface >( resolver->resolve( sConnectionString ), UNO_QUERY  - %d<BR>", time(NULL));
        // gets the server component context as property of the office component factory

        Reference< XPropertySet > x_prop_set(remote_instance, UNO_QUERY);
        TEST_PTR(x_prop_set.is(),);
        x_prop_set->getPropertyValue(OUString::createFromAscii("DefaultContext")) >>= component_ctx;

        // gets the service manager from the office
        Reference< XMultiComponentFactory > server_component_factory(
                component_ctx->getServiceManager());
        TEST_PTR(server_component_factory.is(),);

        PUNO_DEBUG("x_compnt_fact_server - %d<BR>", time(NULL));

        /* Creates an instance of a component which supports the services specified by the factory. Important: using the office component context.
         */
        //		Reference < XComponentLoader > component_loader(
        //				server_component_factory->createInstanceWithContext(
        //						OUString( str_instance,str_instance_len,RTL_TEXTENCODING_ISO_8859_15,OSTRING_TO_OUSTRING_CVTFLAGS ),
        //						component_ctx ) , UNO_QUERY );
        Reference < XComponentLoader > component_loader(
                server_component_factory->createInstanceWithContext(
                OUString("com.sun.star.frame.Desktop", strlen("com.sun.star.frame.Desktop"), RTL_TEXTENCODING_ISO_8859_15, OSTRING_TO_OUSTRING_CVTFLAGS),
                component_ctx), UNO_QUERY);

        TEST_PTR(component_loader.is(),);

        //create an XComponentLoader interface
        Reference< XComponentLoader > *component_loader_p =
                new Reference< XComponentLoader > (component_loader);

        TEST_PTR(component_loader_p,);

        PUNO_DEBUG("x_compnt_loader - %d<BR>", time(NULL));

        //create a XMultiServiceFactory
        Reference <XMultiServiceFactory>
                x_service_factory(component_ctx->getServiceManager(), UNO_QUERY);
        TEST_PTR(x_service_factory.is(),);

        //create an XSingleService Factory Interface
        Reference<XSingleServiceFactory> *remote_service_factory_p =
                new Reference<XSingleServiceFactory>
                (
                x_service_factory->createInstance(
                OUString(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.script.Invocation"))), UNO_QUERY
                );
        TEST_PTR(remote_service_factory_p,);

        //Store the XSingleService Factory Reference
        PUNO_G(remote_service_factory_rsrc_id) = ZEND_REGISTER_RESOURCE(NULL, remote_service_factory_p, uno_refer_rsrc_dtor);
        TEST_PTR(PUNO_G(remote_service_factory_rsrc_id),);

        PUNO_DEBUG("x_service_factory_p - %d<BR>", time(NULL));

        //create an XTypeConverter Interface
        Reference<XTypeConverter> *x_type_conv_p =
                new Reference<XTypeConverter>
                (
                x_service_factory->createInstance(
                OUString(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.script.Converter"))), UNO_QUERY
                );
        TEST_PTR(x_type_conv_p,);

        //create the XInvocation2 interface		
        Sequence<Any> arguments(1);
        arguments[0] <<= component_loader;

        Reference<XInvocation2> *x_invoc2_p =
                new Reference<XInvocation2>
                (
                (*remote_service_factory_p)->createInstanceWithArguments(arguments), UNO_QUERY
                );

        TEST_PTR(x_invoc2_p,);

        //
        //init the php class instance which will be returned by this call
        //
        //        object_init_ex(return_value, ce_ptr);
        //        libreoffice_class_object *this_instance_p;
        //        this_instance_p = (libreoffice_class_object *) zend_object_store_get_object(return_value TSRMLS_CC);

        libreoffice_class_object *this_instance_p;
#if PHP_MAJOR_VERSION <7
        this_instance_p = (libreoffice_class_object *) zend_object_store_get_object(getThis() TSRMLS_CC);  
#else
        zval *this_obj = getThis();
        this_instance_p = (libreoffice_class_object *)Z_OBJ_P(this_obj);
#endif
        TEST_PTR(this_instance_p,);

        //store the XComponentLoader Reference
        this_instance_p->this_rsrc_id = ZEND_REGISTER_RESOURCE(NULL, component_loader_p, uno_refer_rsrc_dtor);
        rsrc_id;
        TEST_PTR(this_instance_p->this_rsrc_id,);


        //Store the XTypeConverter Reference
        PUNO_G(x_type_conv_rsrc_id) = ZEND_REGISTER_RESOURCE(NULL, x_type_conv_p, uno_refer_rsrc_dtor);
        TEST_PTR(PUNO_G(x_type_conv_rsrc_id),);

        //Store the XInvocation2 Reference
        this_instance_p->x_invoc_rsrc_id = ZEND_REGISTER_RESOURCE(NULL, x_invoc2_p, uno_refer_rsrc_dtor);
        TEST_PTR(this_instance_p->x_invoc_rsrc_id,);

        //this object type is INTERFACE
        this_instance_p->type = TypeClass_INTERFACE;

        //init the "Structs Factory"
        Reference <XIdlReflection> x_idl_reflect(x_service_factory->createInstance(
                OUString(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.reflection.CoreReflection"))), UNO_QUERY);
        TEST_PTR(x_idl_reflect.is(),);
        PUNO_DEBUG("x_idl_reflect - %d<BR>", time(NULL));
        Reference <XIdlReflection> *x_idl_reflect_p = new Reference <XIdlReflection> (x_idl_reflect);
        TEST_PTR(x_idl_reflect_p,);

        PUNO_G(x_idl_reflec_rsrc_id) = ZEND_REGISTER_RESOURCE
                (
                NULL,
                x_idl_reflect_p,
                uno_refer_rsrc_dtor
                );

        TEST_PTR(PUNO_G(x_idl_reflec_rsrc_id),);



    } catch (Exception& e) {
        //throw PHP EXCEPTION
        zend_throw_exception(zend_exception_get_default(), (char *) OUStringToOString(e.Message, RTL_TEXTENCODING_ASCII_US).getStr(), 0 TSRMLS_CC);
    }
}


/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(libreoffice) {

    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}
/* }}} */


/* Remove if there's nothing to do at request start */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(libreoffice) {
    PUNO_G(debug_mode) = 0;
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(libreoffice) {
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(libreoffice) {
    php_info_print_table_start();
    php_info_print_table_row(2, "libreoffice support", "enabled");
    php_info_print_table_end();

    /* Remove comments if you have entries in php.ini
     DISPLAY_INI_ENTRIES();
     */
}
/* }}} */

