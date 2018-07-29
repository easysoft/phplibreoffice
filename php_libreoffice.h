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



#ifndef PHP_PUNO_H
#define PHP_PUNO_H

/*GENERAL HEADER */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
}

#include <stdio.h>
#include <osl/file.hxx>
#include <osl/process.h>
#include <string.h>
#include <com/sun/star/registry/XSimpleRegistry.hpp>
#include <cppuhelper/bootstrap.hxx>
#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/frame/XComponentLoader.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/lang/XSingleServiceFactory.hpp>
#include <com/sun/star/lang/XSingleComponentFactory.hpp>
#include <com/sun/star/script/XTypeConverter.hpp>
#include <com/sun/star/script/XInvocation2.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/reflection/XIdlReflection.hpp>
#include <com/sun/star/reflection/XIdlField2.hpp>

extern zend_module_entry libreoffice_module_entry;
#define phpext_libreoffice_ptr &libreoffice_module_entry

#ifdef PHP_WIN32
#define PHP_PUNO_API __declspec(dllexport)
#else
#define PHP_PUNO_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

using namespace com::sun::star::uno;
using namespace com::sun::star::lang;
using namespace com::sun::star::beans;
using namespace com::sun::star::bridge;
using namespace com::sun::star::frame;
using namespace com::sun::star::registry;
using namespace com::sun::star::script;
using namespace com::sun::star::reflection;
using namespace com::sun::star::script;
using namespace com::sun::star::uno;
using namespace rtl;
using namespace std;

PHP_MINIT_FUNCTION( libreoffice);
PHP_MSHUTDOWN_FUNCTION( libreoffice);
PHP_RINIT_FUNCTION( libreoffice);
PHP_RSHUTDOWN_FUNCTION( libreoffice);
PHP_MINFO_FUNCTION( libreoffice);

ZEND_FUNCTION( get_remote_xcomponent);
ZEND_FUNCTION( create_struct);

//resource destrutor handlers
void libreoffice_refer_rsrc_dtor_hdlr (zend_rsrc_list_entry *rsrc TSRMLS_DC);
void libreoffice_any_rsrc_dtor_hdlr (zend_rsrc_list_entry *rsrc TSRMLS_DC);
void x_compnt_fact_cli_rsrc_dtor_hdlr (zend_rsrc_list_entry *rsrc TSRMLS_DC);
void x_simple_reg_rsrc_dtor_hdlr (zend_rsrc_list_entry *rsrc TSRMLS_DC);

//conversion any to zval
zval *create_zval_from_any(com::sun::star::uno::Any TSRMLS_DC);
com::sun::star::uno::Any create_any_from_zval(zval ** TSRMLS_DC);

//user space functions
void
		create_struct_ex(zval *, com::sun::star::uno::Any *, char *,
				int TSRMLS_DC);

//object handling methods
zval *libreoffice_class_read_property(zval *object, zval *member, int t TSRMLS_DC);
void libreoffice_class_write_property(zval *object, zval *member, zval *value TSRMLS_DC);
int libreoffice_class_call_method(char *method, INTERNAL_FUNCTION_PARAMETERS);
static zend_object_value libreoffice_class_object_new(zend_class_entry *class_type TSRMLS_DC);
static union _zend_function *libreoffice_class_get_method(zval **object, char *name, int len TSRMLS_DC);

#define TEST_PTR( ptr , ret_type ) if(!ptr) { zend_throw_exception(zend_exception_get_default(),"Invalid Resource or Name!",0 TSRMLS_CC); return  ret_type; }

//#define PUNO_DEBUG( message, ...) if(PUNO_G(debug_mode)) { php_printf( message, __VA_ARGS__ ); }
#define PUNO_DEBUG( message, ...) 


ZEND_BEGIN_MODULE_GLOBALS( libreoffice)
int x_idl_reflec_rsrc_id;
int remote_service_factory_rsrc_id;
int x_type_conv_rsrc_id;
zend_bool debug_mode;
ZEND_END_MODULE_GLOBALS( libreoffice)

ZEND_DECLARE_MODULE_GLOBALS( libreoffice)

/* In every utility function you add that needs to use variables 
 in php_libreoffice_globals, call TSRMLS_FETCH(); after declaring other
 variables used by that function, or better yet, pass in TSRMLS_CC
 after the last function argument and declare your utility function
 with TSRMLS_DC after the last declared argument.  Always refer to
 the globals in your function as PUNO_G(variable).  You are
 encouraged to rename these macros something shorter, see
 examples in any other php module directory.
 */

#ifdef ZTS
#define PUNO_G(v) TSRMG(libreoffice_globals_id, zend_libreoffice_globals *, v)
#else
#define PUNO_G(v) (libreoffice_globals.v)
#endif

int uno_refer_rsrc_dtor;
int uno_any_rsrc_dtor;
int x_compnt_fact_cli_rsrc_dtor;
int x_simple_reg_rsrc_dtor;



/* ponteiro para a classe */
static zend_class_entry *ce_ptr;

/* handlers da classe (construtor, destrutor, etc) */
static zend_object_handlers libreoffice_class_handlers;

/* struct para controle da classe */
typedef struct _libreoffice_class_object {
	zend_object std;
	int this_rsrc_id;
	int x_idl_class_rsrc_id;
	int x_invoc_rsrc_id;
	TypeClass type;
} libreoffice_class_object;

PHP_METHOD(libreoffice, __construct);
//PHP_METHOD(libreoffice, openDoc);
//PHP_METHOD(libreoffice, save);
//PHP_METHOD(libreoffice, exportPdf);
//PHP_METHOD(libreoffice, exportHtml);

#endif	/* PHP_PUNO_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
