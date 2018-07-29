<?php
include __DIR__ . "/boots.php";


$excel = new libreofficeExcel();
$sheets = $excel->open("/tmp/example03.xlsx");
//获得第一个页签 
$sheet = $sheets->getByIndex(0);
//see api `https://wiki.openoffice.org/wiki/Calc/API/Programming`
$cell1 = $sheet->getCellByPosition(0, 0);
$cell1->setValue(1);
$cell2 = $sheet->getCellByPosition(1, 0);
$cell2->setValue(2.1);
$cell3 = $sheet->getCellByPosition(2, 0);
$cell3->setString("test");
$excel->save();


$excel->exportPDF("/tmp/example03.pdf");
$excel->exportHtml("/tmp/example03.html");

?>