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



// this function is called when the resource which contains the
// reference to the instance of the object XController is destroyed

void libreoffice_refer_rsrc_dtor_hdlr(zend_rsrc_list_entry *rsrc TSRMLS_DC) {
    Reference < XInterface > *x_iface_p = (Reference < XInterface > *)rsrc->ptr;
    if (x_iface_p) {

        (*x_iface_p).clear();
        delete x_iface_p;
    }
}

//void php_destroy_socket(zend_resource *rsrc) /* {{{ */
//{
//        php_socket *php_sock = rsrc->ptr;
//
//        if (Z_ISUNDEF(php_sock->zstream)) {
//                if (!IS_INVALID_SOCKET(php_sock)) {
//                        close(php_sock->bsd_socket);
//                }
//        } else {
//                zval_ptr_dtor(&php_sock->zstream);
//        }
//        efree(php_sock);
//}

void libreoffice_any_rsrc_dtor_hdlr(zend_rsrc_list_entry *rsrc TSRMLS_DC) {
    Any *any_p = (Any *) rsrc->ptr;
    if (any_p) {
        (*any_p).clear();
        delete any_p;
    }
}

void x_compnt_fact_cli_rsrc_dtor_hdlr(zend_rsrc_list_entry *rsrc TSRMLS_DC) {
    Reference < XInterface > *x_iface = (Reference < XInterface > *)rsrc->ptr;
    if (x_iface) {
        Reference< XComponent >::query(*x_iface)->dispose();
        (*x_iface).clear();
        delete x_iface;
    }
}

void x_simple_reg_rsrc_dtor_hdlr(zend_rsrc_list_entry *rsrc TSRMLS_DC) {
    Reference < XSimpleRegistry > *x_simple_registry = (Reference < XSimpleRegistry > *)rsrc->ptr;
    if (x_simple_registry) {
        (*x_simple_registry)->close();
        (*x_simple_registry).clear();
        delete x_simple_registry;
    }

    /*
     * x_simple_registry->close();
                                    x_simple_registry.clear();
                                    x_component_ctx.clear();*/
}

static void forgdb() {
    printf("gdb\n");
};

#if PHP_MAJOR_VERSION < 7

int libreoffice_class_call_method(const char *method, INTERNAL_FUNCTION_PARAMETERS) {
#else

int libreoffice_class_call_method(zend_string *zmethod, zend_object *object, INTERNAL_FUNCTION_PARAMETERS) {
#endif

    int nargs;
    int type;
    com::sun::star::uno::Type any_type;
    InvocationInfo method_info;
#if PHP_MAJOR_VERSION <7
    zval ***args = NULL;
#else
    zval *args = NULL;
    const char *method = zmethod->val;
#endif
    //    php_set_error_handling(EH_THROW, zend_exception_get_default() TSRMLS_CC);
    //    php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
    libreoffice_class_object* this_instance_p;
    Reference < XInvocation2 > *x_invoc2_p;
    Reference < XTypeConverter > *x_type_conv_p;

    nargs = ZEND_NUM_ARGS();
    Sequence < com::sun::star::uno::Any > aParams(nargs);
    Sequence<short> aOutParamIndex(nargs);
    Sequence < com::sun::star::uno::Any > aOutParam(nargs);


    //         zend_object_handle handle = Z_OBJ_HANDLE_P(zobject);
    //
    //        EG(objects_store).object_buckets[handle].bucket.obj.object = object;
    //        
    //restore the XInvocation2 resource
#if PHP_MAJOR_VERSION <7
    this_instance_p = (libreoffice_class_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
#else
    zval *this_obj = getThis();
    this_instance_p = (libreoffice_class_object *) Z_OBJ_P(this_obj);
#endif

    TEST_PTR(this_instance_p, FAILURE);

#if PHP_MAJOR_VERSION <7
    x_invoc2_p = (Reference <XInvocation2> *) zend_list_find(this_instance_p->x_invoc_rsrc_id, &type);
#else
    x_invoc2_p = (Reference <XInvocation2> *) libre_fetch_resource(this_instance_p->x_invoc_rsrc_id, "x_invoc_rsrc_id", &type);
#endif

    if (type != uno_refer_rsrc_dtor) {
        zend_throw_exception(zend_exception_get_default(), "Invalid XInvocation2 reference.", 0 TSRMLS_CC);
        return FAILURE;
    }
    method_info = (*x_invoc2_p)->getInfoForName(OUString::createFromAscii(method), true);

    //     const char *method_name = OUStringToOString(method_info.aName, RTL_TEXTENCODING_ASCII_US).getStr();

    if (nargs) {
#if PHP_MAJOR_VERSION <7
        args = (zval ***) safe_emalloc(sizeof (zval *), nargs, 0);
        TEST_PTR(args, FAILURE);
#else
        args = (zval*) safe_emalloc(nargs, sizeof (zval), 0);
#endif
        zend_get_parameters_array_ex(nargs, args);
        if (strcmp(method, "setValue") == 0) {
            //            OUString teststr("你");
            //   xText->setString(teststr);
            //OUString teststr;
            //teststr= OUString::createFromAscii(wo);
            //   xText->setString(teststr);
            //xText->setString("マルチバイトテキスト helloe");
            //const char *aaaa = OUStringToOString(teststr,RTL_TEXTENCODING_UTF8).getStr();
            //printf("str %s,%p\n",OUStringToOString(teststr,RTL_TEXTENCODING_UTF8).getStr(),aaaa);
            //printf("str2 %s\n",OUStringToOString(xText->getString(),RTL_TEXTENCODING_UTF8).getStr());

            //            zval *tmp;
            //            MAKE_STD_ZVAL(tmp);
            ////            ZVAL_STRINGL(tmp, "你", strlen("你"), 1);
            //            
            //            OUString tmpString("你", strlen("你"), RTL_TEXTENCODING_ISO_8859_15, OSTRING_TO_OUSTRING_CVTFLAGS);
            ////            OUString tmpString("你");
            //            Z_STRVAL_P(tmp) = (char *)emalloc(20);
            //            char *buff = (char *) OUStringToOString(tmpString, RTL_TEXTENCODING_ISO_8859_15).getStr();
            //            int len = OUStringToOString(tmpString, RTL_TEXTENCODING_ISO_8859_15).getLength();
            //            memcpy(Z_STRVAL_P(tmp),buff,len);
            ////            Z_STRVAL_P(tmp) = (char *) OUStringToOString(tmpString, RTL_TEXTENCODING_UTF8).getStr();
            //            printf("teststr %s,%d\n",Z_STRVAL_P(tmp),OUStringToOString(tmpString, RTL_TEXTENCODING_ISO_8859_15).getLength());
            //            
            //            Z_STRLEN_P(tmp) = len;
            ////            *(Z_STRVAL_P(tmp)) = 0x4F60;
            ////            unsigned short teststr = 0x604F;
            //////            unsigned short teststr = 0x4F60;
            ////            Z_STRVAL_P(tmp)[0],'\\';
            ////            Z_STRVAL_P(tmp)[1],'u';
            ////            memcpy(Z_STRVAL_P(tmp)+2,&teststr,2);
            ////            Z_STRVAL_P(tmp)[4] = '\0';
#if PHP_MAJOR_VERSION <7
            zval *tmp = *args[0];
#else
            zval *tmp = &args[0];
#endif

            if (Z_TYPE_P(tmp) == IS_LONG) {
#if PHP_MAJOR_VERSION <7
                Z_TYPE_P(tmp) = IS_DOUBLE;
#else
                tmp->u1.v.type = IS_DOUBLE;
#endif
                //                  zend_print_zval_r(tmp,0);
                Z_DVAL_P(tmp) = (double) Z_LVAL_P(tmp);
            }
        }
        for (int i = 0; i < nargs; i++) {
            //            zend_print_zval_r(*args[i],0);
#if PHP_MAJOR_VERSION <7
            aParams[i] = create_any_from_zval(*args[i] TSRMLS_CC);
#else
            aParams[i] = create_any_from_zval(&args[i] TSRMLS_CC);
#endif
        }

    }

    //do XInvocation2->invoke
    try {
        Any anyval;
        //        printf("name %s\n",method);
        anyval = (*x_invoc2_p)->invoke(method_info.aName, aParams, aOutParamIndex, aOutParam);

        if (anyval.hasValue()) {
            zval *retval = create_zval_from_any(anyval TSRMLS_CC);
            memcpy(return_value, retval, sizeof (zval));
            efree(retval);
        } else {
            ZVAL_NULL(return_value);
        }
    } catch (Exception& e) {
        //throw PHP EXCEPTION
        zend_throw_exception(zend_exception_get_default(), (char *) OUStringToOString(e.Message, RTL_TEXTENCODING_ASCII_US).getStr(), 0 TSRMLS_CC);
    }

    //free arguments zval
    if (args) {
        efree(args);
    }

    //return
    return SUCCESS;
}



#if PHP_MAJOR_VERSION<7

/* libreoffice class destructor */
static void libreoffice_class_object_dtor(void *object TSRMLS_DC) {
    libreoffice_class_object *intern = (libreoffice_class_object *) object;
    zend_hash_destroy(intern->std.properties);
    FREE_HASHTABLE(intern->std.properties);
    efree(object);
}

/* class constructor - extended */

static zend_object_value libreoffice_class_object_new_ex(zend_class_entry *class_type, libreoffice_class_object **new_instance_pp TSRMLS_DC) {

    zend_object_value retval;
    zval *tmp;
    libreoffice_class_object *intern = (libreoffice_class_object *) emalloc(sizeof (libreoffice_class_object));
    memset(intern, 0, sizeof (libreoffice_class_object));
    intern->std.ce = class_type;
    *new_instance_pp = intern;

    ALLOC_HASHTABLE(intern->std.properties);
    zend_hash_init(intern->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);

#if PHP_VERSION_ID < 50399
    zend_hash_copy(intern->std.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof (zval *));
#else
    // Get zend object
    object_properties_init(&intern->std, class_type);
#endif
    retval.handle = zend_objects_store_put(intern, NULL, libreoffice_class_object_dtor, NULL TSRMLS_CC);

    retval.handlers = &libreoffice_class_handlers;

    return retval;

}

/* class constructor */
static zend_object_value libreoffice_class_object_new(zend_class_entry *class_type TSRMLS_DC) {
    libreoffice_class_object *new_instance_p;
    return libreoffice_class_object_new_ex(class_type, &new_instance_p TSRMLS_CC);
}
#else

static zend_object* libreoffice_class_object_new(zend_class_entry *ce TSRMLS_DC) {
    zend_object *object;
    object = zend_objects_new(ce);
    object->handlers = &libreoffice_class_handlers;
    object_properties_init(object, ce);

    object = (zend_object*) erealloc(object, sizeof (libreoffice_class_object));

    //    mysql_client *client = (mysql_client *) emalloc(sizeof (mysql_client));
    //    bzero(client, sizeof (mysql_client));

    //    zval _zobject;
    //    zval* zobject = &_zobject;
    //    ZVAL_OBJ(zobject, object);
    //    libre_set_object(zobject, client);

    return object;
}
#endif


//read the property content to the PHP user space.
#if PHP_MAJOR_VERSION<7

zval *libreoffice_class_read_property(zval *object, zval *member, int t TSRMLS_DC, const _zend_literal* abc) {
#else

zval *libreoffice_class_read_property(zval *object, zval *member, int t, void **cache_slot, zval *rv) {
#endif
    zval *return_value;

    int type;
    libreoffice_class_object *this_instance_p;
    Any *prop_obj_p;
    Any prop_value;
    Reference <XIdlClass> *x_idl_class;
#if PHP_MAJOR_VERSION<7
    MAKE_STD_ZVAL(return_value);
#else
    return_value = (zval*) emalloc(sizeof (zval));
#endif

    try {
#if PHP_MAJOR_VERSION <7
        this_instance_p = (libreoffice_class_object *) zend_objects_get_address(object TSRMLS_CC);
#else
        this_instance_p = (libreoffice_class_object *) Z_OBJ_P(object);
#endif

        TEST_PTR(this_instance_p, return_value);

        //restore XIdlClass
#if PHP_MAJOR_VERSION <7
        x_idl_class = (Reference <XIdlClass> *) zend_list_find(this_instance_p->x_idl_class_rsrc_id, &type);
#else
        x_idl_class = (Reference <XIdlClass> *) libre_fetch_resource(this_instance_p->x_idl_class_rsrc_id, "x_idl_class_rsrc_id", &type);
#endif

        TEST_PTR(x_idl_class, return_value);

        //restore Any that represents the property
#if PHP_MAJOR_VERSION <7
        prop_obj_p = (Any *) zend_list_find(this_instance_p->this_rsrc_id, &type);
#else
        prop_obj_p = (Any *) libre_fetch_resource(this_instance_p->this_rsrc_id, "this_rsrc_id", &type);
#endif


        //create the field interface
        Reference <XIdlField> x_idl_field = (*x_idl_class)->getField(OUString::createFromAscii(Z_STRVAL_P(member)));
        TEST_PTR(x_idl_field.is(), return_value);
        Reference <XIdlField2> x_idl_field2 = Reference <XIdlField2>(x_idl_field, UNO_QUERY);
        TEST_PTR(x_idl_field2.is(), return_value);
        prop_value = x_idl_field2->get(*prop_obj_p);

        x_idl_field.clear();
        x_idl_field2.clear();
        //create Zval from the Property Any Value
        zval *prop_zval = create_zval_from_any(prop_value TSRMLS_CC);
        memcpy(return_value, prop_zval, sizeof (zval));
        efree(prop_zval);
    } catch (Exception& e) {
        ZVAL_NULL(return_value);
        //throw PHP EXCEPTION
        zend_throw_exception(zend_exception_get_default(), (char *) OUStringToOString(e.Message, RTL_TEXTENCODING_ASCII_US).getStr(), 0 TSRMLS_CC);

    }
    return return_value;
}
#if PHP_MAJOR_VERSION <7

void libreoffice_class_write_property(zval *object, zval *member, zval *value TSRMLS_DC, const _zend_literal* abc) {
#else

void libreoffice_class_write_property(zval *object, zval *member, zval *value, void** cache) {
#endif
    zval tmp_member;
    int type;
    libreoffice_class_object *this_instance_p;
    Any *prop_obj_p;
    Any *prop_value_p;
    Reference <XIdlClass> *x_idl_class;
    Reference<XTypeConverter> *x_type_conv_p;
    Reference< XInvocation2 > *x_invoc2_p;
    com::sun::star::uno::Type any_type;

    try {

#if PHP_MAJOR_VERSION <7
        this_instance_p = (libreoffice_class_object *) zend_objects_get_address(object TSRMLS_CC);
#else
        this_instance_p = (libreoffice_class_object *) Z_OBJ_P(object);
#endif
        TEST_PTR(this_instance_p,);
        if (Z_TYPE_P(value) == IS_NULL) {
            return;
        }

        //restore XIdlClass
#if PHP_MAJOR_VERSION <7
        x_idl_class = (Reference <XIdlClass> *) zend_list_find(this_instance_p->x_idl_class_rsrc_id, &type);
#else
        x_idl_class = (Reference <XIdlClass> *) libre_fetch_resource(this_instance_p->x_idl_class_rsrc_id, "x_idl_class_rsrc_id", &type);
#endif
        TEST_PTR(x_idl_class,);
        //restore Any that represents the property
#if PHP_MAJOR_VERSION <7
        prop_obj_p = (Any *) zend_list_find(this_instance_p->this_rsrc_id, &type);
#else
        prop_obj_p = (Any *) libre_fetch_resource(this_instance_p->this_rsrc_id, "this_rsrc_id", &type);
#endif
        TEST_PTR(prop_obj_p,);

        //create the field interface
        Reference <XIdlField> x_idl_field = (*x_idl_class)->getField(OUString::createFromAscii(Z_STRVAL_P(member)));
        TEST_PTR(x_idl_field.is(),);

        Reference <XIdlField2> x_idl_field2 = Reference <XIdlField2>(x_idl_field, UNO_QUERY);
        TEST_PTR(x_idl_field2.is(),);


#if PHP_MAJOR_VERSION <7
        x_type_conv_p = (Reference<XTypeConverter> *)zend_list_find(PUNO_G(x_type_conv_rsrc_id), &type);
#else
        x_type_conv_p = (Reference<XTypeConverter> *) libre_fetch_resource(PUNO_G(x_type_conv_rsrc_id), "this_rsrc_id", &type);
#endif
        TEST_PTR(x_type_conv_p,);

        //get expected attribute type
        any_type = Type(x_idl_field2->getType()->getTypeClass(), x_idl_field2->getType()->getName());

        //create Any from the Value
        prop_value_p = new Any((*x_type_conv_p)->convertTo(create_any_from_zval(value TSRMLS_CC), any_type));

        x_idl_field2->set(*prop_obj_p, *prop_value_p);
        //register the var as a resource, so it will get destroyed when the script ends
        ZEND_REGISTER_RESOURCE(NULL, prop_value_p, uno_any_rsrc_dtor);
    } catch (Exception& e) {
        //throw PHP EXCEPTION
        zend_throw_exception(zend_exception_get_default(), (char *) OUStringToOString(e.Message, RTL_TEXTENCODING_ASCII_US).getStr(), 0 TSRMLS_CC);
    }
}
#if PHP_MAJOR_VERSION<7

static union _zend_function *libreoffice_class_get_method(zval **object, char *name, int len TSRMLS_DC, const struct _zend_literal* abc) {
#else

static union _zend_function *libreoffice_class_get_method(zend_object **object, zend_string *name, const zval *key) {
#endif
    zend_internal_function func;
    //    union _zend_function *func_desc_p;
    union _zend_function *func_desc_p = (union _zend_function *) ecalloc(1, sizeof (union _zend_function));
    func_desc_p->type = ZEND_OVERLOADED_FUNCTION;
    func_desc_p->common.num_args = 0;
    func_desc_p->common.arg_info = NULL;
    func_desc_p->common.fn_flags = 0;

#if PHP_MAJOR_VERSION<7
    func_desc_p->common.function_name = estrndup(name, len);
#else
    func_desc_p->common.function_name = name;
    zend_string_addref(name);
#endif

    return func_desc_p;

}




















//
//
//#if PHP_MAJOR_VERSION<7
//
//static union _zend_function *libreoffice_class_get_method(zval **object, char *name, int len TSRMLS_DC, const struct _zend_literal* abc) {
//#else
//
//static union _zend_function *libreoffice_class_get_method(zend_object **object, zend_string *name, const zval *key) {
//#endif
//    zend_internal_function func, *func_p = NULL;
//    union _zend_function *func_desc_p;
//    func.type = ZEND_OVERLOADED_FUNCTION;
//    func.num_args = 0;
//    func.arg_info = NULL;
//    func.fn_flags = 0;
//
//#if PHP_MAJOR_VERSION<7
//    func.function_name = estrndup(name, len);
//#else
//    func.function_name = name;
//    zend_string_addref(name);
//#endif
//
//    //f.handler = libreoffice_call_method);
//    func_p = &func;
//
//    if (func_p) {
//        /* duplicate this into a new chunk of emalloc'd memory,
//         * since the engine will efree it */
//        func_desc_p = (union _zend_function *) emalloc(sizeof (*func_p));
//        memcpy(func_desc_p, func_p, sizeof (*func_p));
//#if PHP_MINOR_VERSION>0
//#if PHP_MAJOR_VERSION>=7
//        func_desc_p->quick_arg_flags = 0;
//#endif
//#endif
//        return func_desc_p;
//    }
//
//}

