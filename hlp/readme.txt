04/06/2005

Para trabajar con DrafCable.hpp es mejor hacer click con el bot�n derecho del rat�n sobre los t�tulos
del �rbol y seleccionar 'Move Up' o 'Move Down'.
Arrastrar y soltar no funciona bien.

Para generar un nuevo t�pico abrir con MS Word un archivo del directorio .\html editar y guardar
con otro nombre.

Es mejor utilizar DreamWeaver y vincular con style.css porque MSWord incrusta la informaci�n
de estilos en el propio documento html.

12/06/2005

AYUDA INTERACTIVA

Refencias:

MFC2001ABR\\TN028: Context-Sensitive Help Support
MFC2001ABR\\Help: Support Tools (vccore.chm::/html/_core_help.3a_.support_tools.htm)

Descripci�n:

Utilizar la herramienta de la ayuda antigua '.hpj' para comprilar el fichero Resource.h a DrafCable.hm.

MFC2001ABR\\Q241209: HOWTO: Convert a New MFC Application to HTML Help Using WinHelp

Este fichero hay que procesarlo introducciendo '#define ' delante de cada l�nea.

Tambi�n hay que cambiar la extensi�n '.hm' por '.h'.

Despu�s introducir ALIAS en el proyecto de ayuda '.hpp', mapenado cada HIDD_... a un fichero de ayuda.



