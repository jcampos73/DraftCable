04/06/2005

Para trabajar con DrafCable.hpp es mejor hacer click con el botón derecho del ratón sobre los títulos
del árbol y seleccionar 'Move Up' o 'Move Down'.
Arrastrar y soltar no funciona bien.

Para generar un nuevo tópico abrir con MS Word un archivo del directorio .\html editar y guardar
con otro nombre.

Es mejor utilizar DreamWeaver y vincular con style.css porque MSWord incrusta la información
de estilos en el propio documento html.

12/06/2005

AYUDA INTERACTIVA

Refencias:

MFC2001ABR\\TN028: Context-Sensitive Help Support
MFC2001ABR\\Help: Support Tools (vccore.chm::/html/_core_help.3a_.support_tools.htm)

Descripción:

Utilizar la herramienta de la ayuda antigua '.hpj' para comprilar el fichero Resource.h a DrafCable.hm.

MFC2001ABR\\Q241209: HOWTO: Convert a New MFC Application to HTML Help Using WinHelp

Este fichero hay que procesarlo introducciendo '#define ' delante de cada línea.

También hay que cambiar la extensión '.hm' por '.h'.

Después introducir ALIAS en el proyecto de ayuda '.hpp', mapenado cada HIDD_... a un fichero de ayuda.



