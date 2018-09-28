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


#include <time.h>
#include <php_libreoffice.h>

ZEND_FUNCTION(create_struct) {
    char *str;
    size_t str_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len) == FAILURE) {
        return;
    }
    create_struct_ex(return_value, NULL, str, str_len TSRMLS_CC);
}

void create_struct_ex(zval *retval, Any *anyval, char *str, int str_len TSRMLS_DC) {

    Reference <XIdlReflection> *x_idl_reflec_p;
    int type;
    int rsrc_id;
    try {

        //restore XIdlReflection resource

#if PHP_MAJOR_VERSION <7
        x_idl_reflec_p = (Reference <XIdlReflection> *) zend_list_find(PUNO_G(x_idl_reflec_rsrc_id), &type);
#else
        x_idl_reflec_p = (Reference <XIdlReflection> *)libre_fetch_resource(PUNO_G(x_idl_reflec_rsrc_id), "x_idl_reflec_rsrc_id", &type);
#endif

        TEST_PTR(x_idl_reflec_p,);
        Reference <XIdlClass> xIdlClass = (*x_idl_reflec_p)->forName(OUString(str, str_len, RTL_TEXTENCODING_ISO_8859_15, OSTRING_TO_OUSTRING_CVTFLAGS));
        TEST_PTR(xIdlClass.is(),);
        //Reference <XIdlField2> field (xidlfield, UNO_QUERY);
        Any any_obj;
        xIdlClass->createObject(any_obj);

        if (anyval != NULL) {
            any_obj.setValue((*anyval).getValue(), (*anyval).getValueType());
        }

        Any *any_obj_p = new Any(any_obj);
        TEST_PTR(any_obj_p,);

        //init object
        object_init_ex(retval, ce_ptr);
#if PHP_MAJOR_VERSION <7
        libreoffice_class_object *new_struct_p = (libreoffice_class_object *) zend_object_store_get_object(retval TSRMLS_CC);
#else
        libreoffice_class_object *new_struct_p = (libreoffice_class_object*) Z_OBJ_P(retval);
#endif
        TEST_PTR(new_struct_p,);
        //type is Structs
        new_struct_p->type = TypeClass_STRUCT;

        //register and store the Any object
        new_struct_p->this_rsrc_id = ZEND_REGISTER_RESOURCE(
                NULL, any_obj_p,
                uno_any_rsrc_dtor);

        //register and store the XIdlClass Interface
        Reference <XIdlClass> *x_idl_class_p = new Reference <XIdlClass> (xIdlClass);
        TEST_PTR(x_idl_class_p,);
        new_struct_p->x_idl_class_rsrc_id = ZEND_REGISTER_RESOURCE(
                NULL, x_idl_class_p,
                uno_refer_rsrc_dtor);

    } catch (Exception& e) {
        //throw PHP EXCEPTION
        zend_throw_exception(zend_exception_get_default(), (char *) OUStringToOString(e.Message, RTL_TEXTENCODING_ASCII_US).getStr(), 0 TSRMLS_CC);
    }
}

ZEND_FUNCTION(get_remote_xcomponent) {
    int rsrc_id;
    char *str;
    char *rdb_component_path;
    char *str_instance;
    size_t str_instance_len;
    size_t str_len;
    PUNO_DEBUG("get_remote_xcomponent - %d<BR>", time(NULL));
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &str, &str_len, &str_instance, &str_instance_len) == FAILURE) {
        return;
    }
    try {

        //		OUString sConnectionString(str,str_len,RTL_TEXTENCODING_ISO_8859_15,OSTRING_TO_OUSTRING_CVTFLAGS);

        OUString sConnectionString("uno:socket,host=localhost,port=3536;urp;StarOffice.ServiceManager");
        //OUString sConnectionString("uno:socket,host=localhost,port=2083;urp;StarOffice.ServiceManager");
        //str = "uno:socket,host=localhost,port=8100;urp;StarOffice.ServiceManager"


        //		// Creates a simple registry service types.rdb
        //		Reference< XSimpleRegistry > registry( ::cppu::createSimpleRegistry() );
        //		TEST_PTR(registry.is(),);
        //		// Connects the registry to a persistent data source represented by an URL.
        //		rdb_component_path=(char *) safe_emalloc(sizeof(char),strlen(PHP_EXTENSION_DIR)+256,0);
        //		TEST_PTR(rdb_component_path,);
        //
        //		sprintf(rdb_component_path,"file://%s/types.rdb",PHP_EXTENSION_DIR);
        //		PUNO_DEBUG( "rdb_component_path - %d<BR>", time(NULL));
        //		registry->open( OUString::createFromAscii(rdb_component_path)
        //				,sal_True, sal_False );
        //		TEST_PTR(registry->isValid(),);
        //
        //		//register as resource for deletion on script's ending
        //		ZEND_REGISTER_RESOURCE (NULL,
        //				new Reference< XSimpleRegistry >(registry),
        //				x_simple_reg_rsrc_dtor);
        //
        //		PUNO_DEBUG("x_simple_registry->isValid(): %d<BR>", time(NULL));
        //		/* Bootstraps an initial component context with service manager upon a given
        //		 registry. This includes insertion of initial services:
        //		 - (registry) service manager, shared lib loader,
        //		 - simple registry, nested registry,
        //		 - implementation registration
        //		 - registry typedescription provider, typedescription manager (also
        //		 installs it into cppu core)
        //		 */
        //
        //		Reference< XComponentContext > component_ctx(
        //				::cppu::bootstrap_InitialComponentContext( registry ) );

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
        object_init_ex(return_value, ce_ptr);
        libreoffice_class_object *this_instance_p;
#if PHP_MAJOR_VERSION <7
        this_instance_p = (libreoffice_class_object *) zend_object_store_get_object(return_value TSRMLS_CC);
#else
        this_instance_p = (libreoffice_class_object *) Z_OBJ_P(return_value);
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
