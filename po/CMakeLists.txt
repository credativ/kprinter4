find_package(Gettext REQUIRED)

file(GLOB catalogs *.po)

gettext_create_translations("kprinter4.pot" ALL ${catalogs})
