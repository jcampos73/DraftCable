# Comms

Biblioteca de comunicaciones de `db/dc.mdb`. Pieza: **R&S XU4200**.

Transceptor VHF de la familia Series4200 para control de tráfico aéreo. Frontal esquemático de altavoz, pantalla, teclado y mandos, basado en el [folleto oficial Series4200, páginas 6, 7 y 23](https://scdn.rohde-schwarz.com/ur/pws/dl_downloads/dl_common_library/dl_brochures_and_datasheets/pdf_1/Series4200_bro_en_5213-5700-12_v0800.pdf).

## Uso en DraftCable

Reiniciar DraftCable después de instalar estos datos: los tipos se cargan al inicio. Seleccionar `Comms` y volver a colocar la pieza `R&S XU4200` para utilizar los nuevos tipos. Las instancias de la radio ya guardadas en otros esquemas conservan sus datos anteriores; no se han migrado documentos del usuario.

Cada terminal de estilo **Unit pin** representa un conector completo, con las parejas J/P habituales. La referencia X del fabricante se conserva en la etiqueta; J3/P3 corresponde a X3. Los conectores están reorganizados en los bordes para facilitar el dibujo. El marco `CShapeFrmRect` de **500 × 200** atraviesa la división entre las mitades J/P de los trece conectores, siguiendo el criterio de piezas como `Tv_synth`. El contorno continuo exterior del equipo coincide también con ese marco y con la división J/P de las filas de conectores. Las etiquetas y el contenido del frontal quedan hacia el interior. Las mitades exteriores y los puntos de conexión sobresalen del marco y están cubiertos por el margen de repintado existente. La cabecera usa `ACTIVE` para mostrar el contorno discontinuo en la biblioteca.

Los puntos efectivos de conexión de los trece terminales están alineados con la rejilla de **10 × 10**. Debe ajustarse el punto de conexión, no la esquina del terminal: las cajas J/P tienen 30 unidades de ancho y pueden empezar a media celda. Se corrigieron nueve terminales desplazados 5 unidades, trasladando únicamente esos pines y sus etiquetas 5 unidades hacia la derecha.

USB y auriculares del frontal siguen siendo detalles gráficos. X12 continúa omitido por estar reservado en la revisión utilizada.

## Conectores específicos

Las referencias y asignaciones de contactos proceden del **manual del fabricante 6166.5368.02-01, capítulo 6.2, páginas PDF 197–211** ([copia pública del manual](https://www.deyuan0108.com/u/cms/www/202003/24170809ezmc.pdf)). Los nombres se han normalizado a ASCII para los archivos de DraftCable. El sexo se conserva en la propiedad J/P de cada terminal.

| Puertos | Archivo en Connector/ | Contactos modelados |
|---|---|---|
| X1 | RS_XU4200_AC_LNE.txt | L, N, E |
| X2 | RS_XU4200_DC_XLR3.txt | 1–3; 3 sin conectar |
| X3 | RF_N_50OHM.txt | Vivo y pantalla |
| X4, X8 | RF_BNC_50OHM.txt | Vivo y pantalla |
| X5 | RS_XU4200_TX_AUDIO_10P.txt | 1–10 |
| X6, X9 | RS_XU4200_LAN_RJ45.txt | Posiciones 2–9 |
| X7 | RS_XU4200_TX_EXT_DSUB15.txt | 1–15 |
| X10 | RS_XU4200_RX_AUDIO_10P.txt | 1–10; 1 y 10 reservados |
| X11 | RS_XU4200_RX_EXT_DSUB15.txt | 1–15; 10–13 reservados |
| X13, X14 | RS_XU4200_E1_10P.txt | 1–10; NC identificados |
| Extremo LAN estándar | ETH_RJ45_100BTX.txt | 1–8 |

**LAN:** el manual confirma que las tomas de diez posiciones aceptan RJ45 de ocho contactos. Las posiciones exteriores 1 y 10 son NC y se omiten del modelo de conexión activa: radio 2…9 corresponde a RJ45 1…8. Así se mantienen la numeración física de la radio y las ocho vías del cable.

Los tipos anteriores, incluidos RJ45_516, VGA, CANON_CD30 y RF_TV, no se han modificado. Los nuevos se añaden al final de `tbConnector`; los índices existentes se conservan.

## Plantillas de cable

Archivos en `Cables/`, también registrados en `tbCable`. Formato existente: contacto A; identificación/color; señal; contacto B. Una celda B vacía indica que esa posición no se conecta. NC y contactos reservados figuran como posiciones documentadas, no como conductores activos.

| Archivo | Función / extremos |
|---|---|
| RS_XU4200_AC_LNE.txt | X1 L/N/E a bornes L/N/PE |
| RS_XU4200_DC_XLR3_BREAKOUT.txt | X2 1/2 a DC+/DC−; contacto 3 abierto |
| RF_N_N_50OHM.txt | N a N, vivo y pantalla |
| RF_BNC_BNC_50OHM.txt | BNC a BNC, vivo y pantalla; incluye el enlace X4–X8 |
| RF_N_BNC_50OHM.txt | N a BNC, vivo y pantalla |
| RS_XU4200_LAN_RJ45_DIRECT.txt | Radio 2…9 a RJ45 1…8, directo |
| RS_XU4200_TX_AUDIO_1A1_10P.txt | Prolongación 1:1 del audio TX |
| RS_XU4200_RX_AUDIO_1A1_10P.txt | Prolongación de audio RX; reservados abiertos |
| RS_XU4200_TX_EXT_1A1_DSUB15.txt | Prolongación 1:1 de control TX |
| RS_XU4200_RX_EXT_1A1_DSUB15.txt | Prolongación de control RX; reservados abiertos |
| RS_XU4200_E1_1A1_10P.txt | Prolongación E1; NC abiertos |
| RS_XU4200_E1_CROSS_10P.txt | Variante cruzada E1 RX↔TX; NC abiertos |

Los cables `1A1` son plantillas de prolongación de una interfaz. No representan por sí solos un enlace funcional entre dos radios ni una asignación a un VCS concreto. El extremo de otro fabricante se debe definir a partir de ese equipo. La variante E1 cruzada es una correspondencia deducida de los pares RX/TX del manual: 2↔5, 3↔6 y sus pantallas 4↔7; no se presenta como una referencia comercial de cable de Rohde & Schwarz.

Los colores solo se indican en la plantilla AC y los identificadores de vivo/pantalla en coaxiales. No se han inventado colores de mazos de audio/control. La longitud, sección, protección, conectores comerciales y límites de potencia/pérdidas de un cable físico no se representan en este formato y dependen de la instalación. Las señales y reservas corresponden a la revisión citada, no a cualquier variante de Series4200.

## Validación y alcance

- Once archivos de conector y doce de cable: columnas, numeración, ausencia de contactos duplicados, correspondencia con extremos y posiciones NC/reservadas abiertas.
- Lectura mediante el fragmentador real de `CCableEdit` y comprobación de los conteos de compatibilidad existentes de DraftCable.
- Carga de la radio con `LoadUnit`, tipos de sus trece terminales, detección, enlaces recíprocos y desconexión al eliminar el cable.
- Rejilla: antes del ajuste, nueve clics redondeados a la rejilla no alcanzaban el pin previsto; después, los trece coinciden. Prueba nativa con `CConnectorTool`: trece conexiones completas entre dos radios mediante clics ajustados a la rejilla. Este ajuste solo modifica la geometría de nueve pines y sus nueve etiquetas; conserva el marco y todos los demás bytes de la pieza.
- Marco discontinuo pasando por la división J/P de todos los conectores; frontal y etiquetas interiores. Dibujo y puntos exteriores cubiertos por los márgenes `DCABLE_PADDINGX_DEFAULT` y `DCABLE_PADDINGY_DEFAULT` existentes.
- La asignación de tipos modificó solo sus trece campos y el texto del pie. El ajuste posterior del marco modifica exclusivamente los campos geométricos necesarios: tamaño de referencia, posiciones, cajas de etiqueta y contorno interior. Se conservan tipos, identificadores, señales, textos y fuentes, sin cambiar el motor de dibujo.
- Base de datos: adición de tipos sin renumerar los anteriores; modificación de la pieza de biblioteca sin cambiar otras piezas o bibliotecas. Los archivos compartidos anteriores conservan su contenido.

Pendiente probar interactivamente en la aplicación la elección de tipos, el conexionado, la documentación y el guardado/reapertura de un esquema real. Estas comprobaciones de software no sustituyen la validación del mazo contra el equipo y su configuración.
