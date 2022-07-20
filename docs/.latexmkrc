$pdf_mode = 5;
$warnings_as_errors = 1;
$jobname = "relazione";
$aux_dir = "../build/docs/";
$emulate_aux = 1;
$pdf_previewer = "evince %O %S";
$silent = 1;
@default_files = ("main.tex");
set_tex_cmds("-shell-escape -interaction=nonstopmode %O %S");
add_cus_dep("svg", "pdf", "0", "svg2pdf");
add_cus_dep("drawio.svg", "pdf", "0", "drawio2pdf");
sub svg2pdf {
   system("sed 's/<text text-anchor=\"middle\" font-size=\"10px\" x=\"50%\" y=\"100%\">Viewer does not support full SVG 1.1<\\/text>//gm' $_[0].svg | cairosvg -o $_[0].pdf -");
}
sub drawio2pdf {
   system("sed 's/<text text-anchor=\"middle\" font-size=\"10px\" x=\"50%\" y=\"100%\">Viewer does not support full SVG 1.1<\\/text>//gm' $_[0].drawio.svg | cairosvg -o $_[0].pdf -");
}
