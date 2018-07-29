<?php

class libreofficeExcel {


    private $path = NULL;
    private $excelCompoment = NULL;
    private $libreoffice = NULL;

    //__construct connect
    public function __construct() {
        $this->libreoffice = new libreoffice();
    }

    //create if not exists
    public function open($path) {
        if (is_file($path)) {//exists
            $load_url = "file://" . $path;
            $this->path = $load_url;
        } else {//create
            $load_url = "private:factory/scalc";
            $this->path = "file://" . $path;
        }

        $load_props = array();
        $this->excelCompoment = call_user_func_array(array($this->libreoffice, "loadComponentFromURL"), array($load_url, "_blank", 0, $load_props));
        return $this->excelCompoment->getSheets();
    }


    private function checkOpen() {
        if (empty($this->path)) {
            throw new Exception("not open Excel");
        }
    }

    public function save() {
        $this->checkOpen();
        $store_props = array();
        $store_props[0] = create_struct("com.sun.star.beans.PropertyValue");
        $store_props[0]->Name = "FilterName";
        $store_props[0]->Value = "MS Excel 97";
        $this->excelCompoment->storeToURl($this->path, $store_props);
    }

    public function exportPDF($target) {
        $target = "file://" . $target;
        $this->checkOpen();
        $store_props = array();
        $store_props[0] = create_struct("com.sun.star.beans.PropertyValue");
        $store_props[0]->Name = "FilterName";
        $store_props[0]->Value = "writer_pdf_Export";
        $this->excelCompoment->storeToURl($target, $store_props);
    }

    public function exportHTML($target) {
        $target = "file://" . $target;
        $this->checkOpen();
        $store_props = array();
        $store_props[0] = create_struct("com.sun.star.beans.PropertyValue");
        $store_props[0]->Name = "FilterName";
        $store_props[0]->Value = "HTML (StarCalc)";//excel转html
        $this->excelCompoment->storeToURl($target, $store_props);
    }

}

class libreofficeWord {

    private $path = NULL;
    private $docCompoment = NULL;
    private $libreoffice = NULL;

    //__construct connect
    public function __construct() {
        $this->libreoffice = new libreoffice();
    }

    //create if not exists
    public function open($path) {
        if (is_file($path)) {//exists
            $load_url = "file://" . $path;
            $this->path = $load_url;
        } else {//create
            $load_url = "private:factory/swriter";
            $this->path = "file://" . $path;
        }

        $load_props = array();
        $this->docCompoment = call_user_func_array(array($this->libreoffice, "loadComponentFromURL"), array($load_url, "_blank", 0, $load_props));
        return $this->docCompoment->getText();
    }


    private function checkOpen() {
        if (empty($this->path)) {
            throw new Exception("not open Doc");
        }
    }

    public function save() {
        $this->checkOpen();
        $store_props = array();
        $store_props[0] = create_struct("com.sun.star.beans.PropertyValue");
        $store_props[0]->Name = "FilterName";
        $store_props[0]->Value = "MS Word 97";
        $this->docCompoment->storeToURl($this->path, $store_props);
    }

    public function exportPDF($target) {
        $target = "file://" . $target;
        $this->checkOpen();
        $store_props = array();
        $store_props[0] = create_struct("com.sun.star.beans.PropertyValue");
        $store_props[0]->Name = "FilterName";
        $store_props[0]->Value = "writer_pdf_Export";
        $this->docCompoment->storeToURl($target, $store_props);
    }

    public function exportHTML($target) {
        $target = "file://" . $target;
        $this->checkOpen();
        $store_props = array();
        $store_props[0] = create_struct("com.sun.star.beans.PropertyValue");
        $store_props[0]->Name = "FilterName";
        $store_props[0]->Value = "HTML (StarWriter)"; //word转html
        $this->docCompoment->storeToURl($target, $store_props);
    }

}
?>
