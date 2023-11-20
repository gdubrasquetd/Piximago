TEMPLATE = app
TARGET = Piximago

# Spécifiez le répertoire où les fichiers MOC seront générés
MOC_DIR = moc_files

# Spécifiez le répertoire où les fichiers .o seront générés
OBJECTS_DIR = obj_files

# Répertoire de construction spécifié
DESTDIR = ../build

# Votre projet Qt
QT += widgets

# Les fichiers sources de votre projet
SOURCES += main.cpp \
    viewer-app.cpp \
    slider-widget.cpp \
    buttons-layout-builder.cpp \
    image-manager.cpp \
    radio-button-widget.cpp \
    spin-box-widget.cpp

# Les fichiers d'en-tête de votre projet
HEADERS += viewer-app.h \
    slider-widget.h \
    buttons-layout-builder.h \
    image-manager.h \
    radio-button-widget.h \
    spin-box-widget.h


# Autres configurations de votre projet
