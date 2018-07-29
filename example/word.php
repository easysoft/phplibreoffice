<?php

include __DIR__."/boots.php";



$doc = new libreofficeWord();
$text = $doc->open("/tmp/example01.docx");
//text api see `https://wiki.openoffice.org/wiki/Documentation/DevGuide/Text/Text_Documents`
$text->setString("contents string1");
$cursor = $text->createTextCursor();
$cursor->gotoEnd(false);
$text->insertString($cursor, "\ncontents string2", false);
$doc->save();


$doc->exportPDF("/tmp/example01.pdf");
$doc->exportHtml("/tmp/example01.html");
