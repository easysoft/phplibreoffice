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

/** 
 *
 *  ANY TO ZVAL Functions 
 *
 **/
zval *create_zval_from_any(com::sun::star::uno::Any anyval TSRMLS_DC) {
    zval *retval;
    retval = (zval *) emalloc(sizeof (zval));
    TEST_PTR(retval, retval);
    TypeClass type;

    try {
        type = anyval.getValueTypeClass();

        switch (type) {

                //All those types can only be converted to Integer in PHP
            case TypeClass_BYTE:
            {
            }
            case TypeClass_SHORT:
            {
            }
            case TypeClass_UNSIGNED_SHORT:
            {
            }
            case TypeClass_UNSIGNED_LONG:
            {
            }
            case TypeClass_ENUM:
            {
            }
            case TypeClass_LONG:
            {
                sal_Int32 lval;
                anyval >>= lval;
                ZVAL_LONG(retval, lval);
                break;
            }
                //try to create the 64 bit int. If the O.S. supports it
            case TypeClass_HYPER:
            {
            }
            case TypeClass_UNSIGNED_HYPER:
            {
                sal_Int64 lval;
                anyval >>= lval;
                ZVAL_LONG(retval, lval);
                break;
            }

                //boolean
            case TypeClass_BOOLEAN:
            {
                sal_Bool bval;
                anyval >>= bval;
                ZVAL_BOOL(retval, bval);
                break;
            }

                //float
            case TypeClass_FLOAT: //it's a PHP double
            {
                float fval;
                anyval >>= fval;
                ZVAL_DOUBLE(retval, fval);
                break;
            }
                //double
            case TypeClass_DOUBLE:
            {
                double dval;
                anyval >>= dval;
                ZVAL_DOUBLE(retval, dval);
                break;
            }

                //strings
            case TypeClass_CHAR:
            {
            }
            case TypeClass_STRING:
            {
                OUString str;
                anyval >>= str;
                CP_ZVAL_STRING(retval, (char *) OUStringToOString(str, RTL_TEXTENCODING_ISO_8859_15).getStr(), 1);
                break;
            }

                //array
            case TypeClass_ARRAY:
            {
            } //just consider it to be a sequence
            case TypeClass_SEQUENCE:
            {
                array_init(retval);
                Sequence < ::com::sun::star::uno::Any > anyseq;
                anyval >>= anyseq;
                for (int index = 0; index < anyseq.getLength(); index++) {
                    add_index_zval(
                            retval, index,
                            create_zval_from_any(anyseq[index] TSRMLS_CC)
                            );
                }
                break;
            }

                //interface or service
            case TypeClass_SERVICE:
            {
            }
            case TypeClass_INTERFACE:
            {
                Reference < XInterface > x_iface, *x_iface_p;
                int type;
                int rsrc_id;
                anyval >>= x_iface;
                x_iface_p = new Reference< XInterface > (x_iface);
                TEST_PTR(x_iface_p, retval);

                object_init_ex(retval, ce_ptr);
                libreoffice_class_object *libreoffice_class_instance_p;
#if PHP_MAJOR_VERSION <7
                libreoffice_class_instance_p = (libreoffice_class_object *) zend_object_store_get_object(retval TSRMLS_CC);
#else
                libreoffice_class_instance_p = (libreoffice_class_object *) Z_OBJ_P(retval);
#endif

                //restore the XSingleServiceFactory Interface Reference
#if PHP_MAJOR_VERSION <7
                Reference <XSingleServiceFactory> *x_service_factory_p = (Reference <XSingleServiceFactory> *)zend_list_find(PUNO_G(remote_service_factory_rsrc_id), &type);
#else
                Reference <XSingleServiceFactory> *x_service_factory_p = (Reference <XSingleServiceFactory> *) libre_fetch_resource(PUNO_G(remote_service_factory_rsrc_id), "remote_service_factory_rsrc_id", &type);
#endif

                TEST_PTR(x_service_factory_p, retval);

                //create the XInvocation2 interface		
                Sequence<Any> arguments(1);
                arguments[0] <<= x_iface;

                Reference<XInvocation2> *x_invoc2_p =
                        new Reference<XInvocation2>
                        (
                        (*x_service_factory_p)->createInstanceWithArguments(arguments), UNO_QUERY
                        );
                TEST_PTR(x_invoc2_p, retval);

                //store the XInterface tmp_interface_ptr Reference
                libreoffice_class_instance_p->this_rsrc_id  = ZEND_REGISTER_RESOURCE(NULL, x_iface_p, uno_refer_rsrc_dtor);

                //Store the XInvocation2 Reference
                libreoffice_class_instance_p->x_invoc_rsrc_id = ZEND_REGISTER_RESOURCE(NULL, x_invoc2_p, uno_refer_rsrc_dtor);
                break;
            }

                //if void .. do nothing
            case TypeClass_VOID:
            {
                break;
            }

                //if struct
            case TypeClass_STRUCT:
            {

                char *str;
                int str_len;
                str = (char *) (OUStringToOString(anyval.getValueTypeName(), RTL_TEXTENCODING_ASCII_US).getStr());
                str_len = strlen(str);

                Reference <XIdlReflection> *x_idl_reflect_p;

                int type;
                int rsrc_id;

                //restore XIdlReflection resource
#if PHP_MAJOR_VERSION <7
                x_idl_reflect_p = (Reference <XIdlReflection> *) zend_list_find(PUNO_G(x_idl_reflec_rsrc_id), &type);
#else
                x_idl_reflect_p = (Reference <XIdlReflection> *) libre_fetch_resource(PUNO_G(x_idl_reflec_rsrc_id), "x_idl_reflec_rsrc_id", &type);
#endif

                TEST_PTR(x_idl_reflect_p, retval);

                Reference <XIdlClass> x_idl_class = (*x_idl_reflect_p)->forName(anyval.getValueTypeName());
                TEST_PTR(x_idl_class.is(), retval);

                Any any_obj;
                x_idl_class->createObject(any_obj);
                any_obj.setValue((anyval).getValue(), (anyval).getValueType());

                Any *any_obj_p = new Any(any_obj);
                TEST_PTR(any_obj_p, retval);

                //init object
                object_init_ex(retval, ce_ptr);
                libreoffice_class_object *libreoffice_class_instance_p;
#if PHP_MAJOR_VERSION <7
                libreoffice_class_instance_p = (libreoffice_class_object *) zend_object_store_get_object(retval TSRMLS_CC);
#else
                libreoffice_class_instance_p = (libreoffice_class_object *) Z_OBJ_P(retval);
#endif

                TEST_PTR(libreoffice_class_instance_p, retval);
                //type is Structs
                libreoffice_class_instance_p->type = TypeClass_STRUCT;

                //register and store the Any object
                libreoffice_class_instance_p->this_rsrc_id = ZEND_REGISTER_RESOURCE(
                        NULL, any_obj_p,
                        uno_any_rsrc_dtor);

                //register and store the XIdlClass Interface
                libreoffice_class_instance_p->x_idl_class_rsrc_id = ZEND_REGISTER_RESOURCE(
                        NULL, new Reference <XIdlClass> (x_idl_class),
                        uno_refer_rsrc_dtor);
                TEST_PTR(rsrc_id, retval);
                break;
            }
                //default: throw Error Message
            default:
            {
                zend_throw_exception(zend_exception_get_default(), "It was not possible to create a php var from the returned value.", 0 TSRMLS_CC);
                ZVAL_NULL(retval);
                break;
            }
        }

        return retval;
    } catch (Exception& e) {
        efree(retval);
        //throw PHP EXCEPTION
        zend_throw_exception(zend_exception_get_default(), (char *) OUStringToOString(e.Message, RTL_TEXTENCODING_ASCII_US).getStr(), 0 TSRMLS_CC);
    }
    ZVAL_NULL(retval);
    return retval;

}

/** 
 *
 * ZVAL TO ANY Functions 
 *
 **/
com::sun::star::uno::Any create_any_from_zval(zval *zval_param TSRMLS_DC) {
    int type;
    zval *zval_any;
    Any const_any;
    Reference<XInterface> *ref_p;
    Reference<XTypeConverter> *x_type_conv_p;
    com::sun::star::uno::Type any_type;
    zval_any = zval_param;
    int arr_size = 0;
    try {
        switch (Z_TYPE_P(zval_any)) {
            case IS_STRING:
            {
                const_any = makeAny(OUString(Z_STRVAL_P(zval_any), Z_STRLEN_P(zval_any), RTL_TEXTENCODING_ISO_8859_15, OSTRING_TO_OUSTRING_CVTFLAGS));
                break;
            }

            case IS_DOUBLE:
            {
                //                            printf("______________-\n");
                const_any = makeAny(Z_DVAL_P(zval_any));
                break;
            }

            case IS_LONG:
            {
                const_any = makeAny(Z_LVAL_P(zval_any));
                //                                const_any=makeAny(zval_any->value.dval);
                break;
            }
#if PHP_MAJOR_VERSION<7
            case IS_BOOL:
            {
                const_any = makeAny((sal_Bool) Z_LVAL_P(zval_any));
                break;
            }
#else
            case IS_TRUE:
            {
                const_any = makeAny((sal_Bool) 1);
                break;
            }
            case IS_FALSE:
            {
                const_any = makeAny((sal_Bool) 0);
                break;
            }
#endif
            case IS_OBJECT:
            {
                libreoffice_class_object* this_instance_p;
#if PHP_MAJOR_VERSION <7
                this_instance_p = (libreoffice_class_object *) zend_object_store_get_object(zval_any TSRMLS_CC);
#else
                this_instance_p = (libreoffice_class_object *) Z_OBJ_P(zval_any);
#endif

#if PHP_MAJOR_VERSION <7
                ref_p = (Reference <XInterface> *)zend_list_find(this_instance_p->this_rsrc_id, &type);
#else
                ref_p = (Reference <XInterface> *)libre_fetch_resource(this_instance_p->this_rsrc_id, "this_rsrc_id", &type);
#endif

                if (type == uno_refer_rsrc_dtor) {
                    const_any = makeAny(*ref_p);

                } else if (type == uno_any_rsrc_dtor) {

                    Type t = ((Any *) ref_p)->getValueType();
                    const_any.setValue(((Any *) ref_p)->getValue(), t);

                } else {
                    zend_throw_exception(zend_exception_get_default(), "It was not possible to create the com::sun::star::uno::Any from this object.", 0 TSRMLS_CC);
                }
                break;
            }

            case IS_ARRAY:
            {

                arr_size = zend_hash_num_elements(HASH_OF(zval_any));
                Sequence < com::sun::star::uno::Any > seq(arr_size);
                for (int index = 0; index < arr_size; index++) {
#if PHP_MAJOR_VERSION <7
                    zval **tmp;
                    zend_hash_index_find(HASH_OF(zval_any), index, (void **) &tmp);
                    seq[index] = create_any_from_zval(*tmp TSRMLS_CC);
#else
                    zval *tmp = zend_hash_index_find(HASH_OF(zval_any), index);
                    seq[index] = create_any_from_zval(tmp TSRMLS_CC);
#endif

                }

                const_any = makeAny(seq);
                break;
            }

            default:
            {
                zend_throw_exception(zend_exception_get_default(), "It was not possible to create the com::star::uno::Any from this object.", 0 TSRMLS_CC);
                break;
            }
        }

    } catch (Exception& e) {
        //throw PHP EXCEPTION
        zend_throw_exception(zend_exception_get_default(), (char *) OUStringToOString(e.Message, RTL_TEXTENCODING_ASCII_US).getStr(), 0 TSRMLS_CC);

    }
    return const_any;
}
