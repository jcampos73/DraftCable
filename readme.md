# DraftCable: herramientas y estados de las formas

## Inventario de herramientas

Este inventario describe las herramientas de interacción y colocación presentes en el código. Una función de la interfaz no equivale necesariamente a una clase `Tool`: parte del comportamiento sigue en `CDraftDrawView`, `CDraftDrawDoc` y las propias formas. La descripción procede de la lectura del código, no de una prueba funcional de cada herramienta.

### Clases de herramientas

| Clase | Función | Uso del stack y la selección | Integración actual |
|---|---|---|---|
| `CAbstractTool` | Base común: `MouseDown`, `MoveTo`, `MouseUp`, deselección y utilidades de creación. No es una herramienta elegible en la interfaz. | `DeselectAll` recorre `m_pObListSel`; `GetCurrentShape` obtiene el último objeto del documento; `_DoAddNewShapeToStack` comprueba `m_ObArray` e inserta mediante `AddObject`. | Base de las herramientas; `CPolyTool` deriva a través de `CPenTool`. Archivos `AbstractTool.h/.cpp`. |
| `CSelectionTool` | Selección por clic, selección con Ctrl, selección por rectángulo, movimiento y redimensionado mediante marcadores. | Busca candidatos en el documento; mueve y finaliza el gesto recorriendo `m_pObListSel`. El rectángulo auxiliar todavía se añade al documento y se retira mediante `DeleteObject(-1)`. | Delegación de pulsación, movimiento y suelta activada mediante `TEST_SELTOOL`; la vista retorna tras delegar. Archivos `SelectionTool.h/.cpp`. |
| `CPenTool` | Dibujo de formas y colocación normales. La creación por puntos se gestiona en `CPolyTool`; la rotación interactiva, en `CRotationTool`. | `MoveTo` recorre el documento y actualiza únicamente las formas con `m_Mode == _DRAFTDRAW_MODE_ACTIVE`. Identifica la forma actual mediante el último objeto y utiliza `m_pSh` para la creación. | Delegación activada mediante `TEST_PENTOOL`; excluye a `CPolyTool`, que tiene delegación propia. En la suelta, la vista continúa además por el procesamiento antiguo. Archivos `PenTool.h/.cpp`. |
| `CPolyTool` | Hija de `CPenTool`: primer arrastre y continuación por puntos de polilíneas, curvas, poliarcos y buses. | Conserva la identidad de la forma actual y la añade una sola vez al array activo. Reutiliza los eventos y la normalización de `CShapePolyline`; al terminar mantiene los puntos confirmados. | Delegación explícita de pulsación, movimiento, suelta, doble clic y Escape. La vista conserva el ajuste existente de límites de hoja. Archivos `PolyTool.h/.cpp`. |
| `CRotationTool` | Rotación interactiva de la selección existente, reutilizando `CShape::OnRotate`. | Recorre `m_pObListSel`; no busca por `IsSelected()`, no inserta objetos auxiliares ni depende del último objeto del stack. | `ID_BUTTON_ROTATE` crea la herramienta; la vista delega pulsación, movimiento y suelta. Archivos `RotationTool.h/.cpp`. |
| `CConnectorTool` | Inicio de conexiones, enlace de extremos y creación de sucesivos tramos de cable. | Mantiene una referencia explícita al tramo provisional, lo añade al documento y lo retira por identidad al cancelar. Busca extremos mediante `PtInRect` y enlaza con `LinkShapes`. | `TEST_CONNTOOL` activado: la vista delega pulsación, movimiento, suelta y Escape en el modo de cableado. Archivos `ConnectorTool.h/.cpp`. |

Las opciones `TEST_SELTOOL`, `TEST_PENTOOL` y `TEST_CONNTOOL` se encuentran al principio de `DraftDrawView.cpp`. Describen qué rutas de delegación están habilitadas en este código fuente.

### Herramientas y modos de la interfaz

| Herramienta o función | Comandos principales | Implementación observada |
|---|---|---|
| Selección simple y con Ctrl | `ID_BUTTON_SELECT` | Crea un `CSelectionTool`; mantiene las referencias seleccionadas en la lista de la vista. |
| Selección por rectángulo | Iniciada al pulsar en una zona sin forma seleccionable | Parte de `CSelectionTool`; utiliza un `CShapeRect` auxiliar dentro del stack del documento. |
| Movimiento de la selección | Arrastre de las formas seleccionadas | `CSelectionTool` recorre `m_pObListSel` y delega el movimiento en las formas. |
| Redimensionado de formas existentes | Arrastre de los marcadores de selección | Rama de `CSelectionTool` y procesamiento geométrico de las formas. |
| Dibujo de rectángulo, elipse, línea, arco y Board | `ID_BUTTON_RECT`, `ID_BUTTON_ELLIPSE`, `ID_BUTTON_LINE`, `ID_BUTTON_ARC`, `ID_BUTTON_BOARD` | Estos comandos crean un `CPenTool` y preparan la forma correspondiente, incluido `CShapeBoard`. |
| Colocación de componentes y chasis de rack | `ID_PLACE_PART`, `ID_PLACE_CHASSIS` | Diálogo de bibliotecas; la ruta `SetShapeToPlace` crea un `CPenTool`, carga un `CShapeUnit` y lo añade al documento antes de terminar la colocación. |
| Cableado por tramos | `ID_PLACE_WIRE` | Prepara `CShapeWire` e instancia `CConnectorTool`; con `TEST_CONNTOOL` activado, los eventos de cableado se delegan en esa clase. |
| Polilínea, curva y poliarco | `ID_BUTTON_POLYLINE`, `ID_BUTTON_CURVE`, `ID_BUTTON_POLYARC` | Crea `CPolyTool`, prepara `CShapePolyline`, `CShapeCurve` o `CShapePolyArc` y activa `_TOOLTYPEPOLY_DRAFTCABLE`. |
| Bus o mazo | `ID_PLACE_BUS` | Crea `CPolyTool`, prepara un `CShapeBus` y activa el modo de dibujo por puntos. |
| Polígonos y figuras generadas | Familia `IDS_MENU_POLYGON_*` | Triángulo, pentágono, hexágono, octógono, polígono de N lados, estrella, engranaje, cruz de N brazos, paralelogramo, trapecio, rombo, cruz y figuras T, L y recta. Se generan como unidades; no tienen una clase `Tool` individual. |
| Pines y conectores de unidad | `ID_PLACE_PIN` | Configura y añade un `CShapePin` para su colocación. |
| Puertos izquierdo y derecho | `ID_PLACE_PORT`, `ID_PLACE_PORTRIGHT` | Carga las unidades de biblioteca `Standard.port_left.ddw` y `Standard.port_right.ddw`. |
| Marca de no conexión | `ID_PLACE_NOCON` | Carga la unidad `Standard.noconnect.ddw`. |
| Entrada y unión de bus | `ID_PLACE_BUSENTRY`, `ID_PLACE_BUSJUNC` | Carga las unidades `Standard.junction.ddw` y `Standard.busjunc.ddw`. |
| Texto y etiqueta o alias | `ID_PLACE_TEXT`, `ID_PLACE_NETALIAS` | Prepara un `CShapeLabel` con sus propiedades de texto y orientación. |
| Imagen | `ID_PLACE_PICTURE` | Crea un `CShapePicture`, permite elegir el archivo y lo añade al documento para colocarlo. |
| Regla de medición | `ID_BUTTON_RULE` | Prepara un `CShapeRule`; no existe una clase `RuleTool` separada. |
| Rotación interactiva | `ID_BUTTON_ROTATE` | Crea un `CRotationTool` y sustituye `m_tooSel`; trabaja sobre la selección existente mediante pulsación, arrastre y suelta. |
| Pan o desplazamiento de la vista | `ID_BUTTON_PAN` | `_DoPanMove` modifica el desplazamiento de la vista; no necesita una forma auxiliar. |
| Zoom con cursor, acercar o alejar | `ID_ZOOM_CURSORPLUS`, `ID_ZOOM_CURSORMINUS` | Selecciona el modo de zoom; el procesamiento correspondiente permanece en la vista. |
| Zoom por área | `ID_ZOOM_AREA` | Prepara un `CShapeZoomRect`; el procesamiento antiguo de la vista calcula el zoom y retira el auxiliar mediante `DeleteObject(-1)`. |
| Zoom directo y encuadre completo | `ID_ZOOM_IN`, `ID_ZOOM_OUT`, `ID_ZOOM_ALL` | Comandos de la vista, sin una clase `Tool` independiente. |
| Ajuste a rejilla | `ID_SNAP_TOGRID`, `ID_SNAP_TOGRID2` | Opciones de apoyo al dibujo y la interacción gestionadas en la vista. |

Los comandos de colocación que no sustituyen `m_tooSel` mantienen la herramienta que estuviera instalada. Por eso la tabla distingue la preparación de cada forma de la ruta que procesa posteriormente sus eventos; no se presupone una clase independiente por cada botón.

### Funciones relacionadas que no son herramientas de ratón

- **SIC y edición de piezas:** `ID_TOOL_SIC` y `ID_EDIT_PART` abren o cambian el contexto de edición; utilizan las herramientas anteriores sobre la colección de piezas.
- **Inserción de hojas desde archivo:** `ID_PLACE_SHEETS` llama a `OnInsertDocument`; es una operación de importación.
- **Estados de ratón:** `MouseState` y `MouseDragState` son clases auxiliares del estado de interacción, no herramientas adicionales.
- **`CButtonStates`:** es un control de botón de la interfaz, no una herramienta CAD.

## Estados de las formas

Las propiedades `m_Mode` y `m_TypeSelect` se combinan para representar el estado de interacción de una forma.

```cpp
#define _DRAFTDRAW_MODE_ACTIVE 0
#define _DRAFTDRAW_MODE_IDLE 1

#define _DRAFTDRAW_SEL_SIZING_RECT   0
#define _DRAFTDRAW_SEL_MOVING_RECT   2
#define _DRAFTDRAW_SEL_RESIZING_RECT 3
```

### Combinaciones habituales

| `m_Mode` | `m_TypeSelect` | Significado habitual |
|---|---|---|
| `1` (`MODE_IDLE`) | `SIZING_RECT` (`0`) | Forma en reposo, sin selección |
| `1` (`MODE_IDLE`) | `MOVING_RECT` (`2`) | Seleccionada, con marcadores, sin arrastre |
| `0` (`MODE_ACTIVE`) | `SIZING_RECT` (`0`) | En creación |
| `0` (`MODE_ACTIVE`) | `MOVING_RECT` (`2`) | En movimiento |
| `0` (`MODE_ACTIVE`) | `RESIZING_RECT` (`3`) | En redimensionado |

Los nombres abreviados de la tabla corresponden a las constantes `_DRAFTDRAW_MODE_*` y `_DRAFTDRAW_SEL_*` anteriores.

Estas son las combinaciones de uso habitual observadas en el código, no una enumeración exhaustiva de todas las rutas ni una validación del comportamiento en ejecución.

### Cuándo cambia `m_Mode`

- **A `0` (`_DRAFTDRAW_MODE_ACTIVE`):** al construir una forma preparada para dibujarse, al iniciar un movimiento o redimensionado y durante el arrastre desde las herramientas. `OnMouseMove` utiliza `m_TypeSelect` para distinguir la operación; el dibujo también consulta el estado para representarla.
- **A `1` (`_DRAFTDRAW_MODE_IDLE`):** al finalizar creación, movimiento o redimensionado en las ramas correspondientes de `OnLButtonUp`; también en `Select()`, `Unselect()` y distintos procesos de carga o finalización.

**`m_Mode == 1` no significa que la forma esté deseleccionada.** Puede seguir seleccionada, con `m_TypeSelect == _DRAFTDRAW_SEL_MOVING_RECT`, sin estar siendo arrastrada.

`MODE_ACTIVE` (`0`) expresa interacción activa y `MODE_IDLE` (`1`) indica que la forma está fuera de la operación activa, aunque puede seguir seleccionada. La selección persistente también se representa mediante `m_TypeSelect`, y `CSelectionTool` mantiene las referencias a las formas seleccionadas en `m_pObListSel`.

## Usos de `IsSelected()`

Se han localizado 11 llamadas no comentadas en cinco archivos, además de la declaración en `Shape.h`, la definición en `Shape.cpp` y una llamada comentada en `ConnectorTool.cpp`. Las filas con dos llamadas se indican expresamente. El inventario describe las rutas presentes en el código; no presupone que todas se ejecuten en cada interacción.

| Lugar | Para qué lo utiliza | Llamadas |
|---|---|---|
| `CSelectionTool::MouseDown` (`SelectionTool.cpp`) | Distinguir si la forma pulsada ya estaba seleccionada y gestionar la selección y Ctrl. | 1 |
| `CSelectionTool::_AddToSelectedArray` (`SelectionTool.cpp`) | Decidir si añade la forma a `m_pObListSel`, incluida la selección por rectángulo y el auxiliar. | 1 |
| `CDraftDrawView::OnLButtonUp` (`DraftDrawView.cpp`) | Enviar la suelta a las formas consideradas seleccionadas en el recorrido antiguo. | 1 |
| `CDraftDrawView::OnMouseMove` (`DraftDrawView.cpp`) | Filtrar las formas que actualiza mediante movimiento o dimensionado en el recorrido antiguo; la rotación utiliza `CRotationTool`. | 1 |
| `CDraftDrawView::_AddToSelectedArray` (`DraftDrawView.cpp`) | Incorporar formas a la lista de selección. | 1 |
| `CDraftDrawDoc::MoveShapes` y `ScaleShapes` (`DraftDrawDoc.cpp`) | Elegir sobre qué formas actúan los comandos colectivos de movimiento y escala. | 2 |
| Ambas variantes de `CShape::Move` (`Shape.cpp`) | Recolocar los marcadores de selección después de cambiar la geometría. | 2 |
| Listado de objetos de `OMParser` (`OMParser.cpp`) | Mostrar `Selected=...`, con ramas para unidades y otras formas. | 2 |
| **Total** | | **11** |

### Significado actual

La implementación de `CShape::IsSelected()` devuelve verdadero si se cumple cualquiera de estas condiciones:

```cpp
m_Mode == _DRAFTDRAW_MODE_ACTIVE
m_TypeSelect == _DRAFTDRAW_SEL_MOVING_RECT
m_TypeSelect == _DRAFTDRAW_SEL_RESIZING_RECT
```

La implementación combina esas comparaciones mediante `|`. Este cuadro documenta su comportamiento sin modificarlo.

Actualmente se mezclan dos preguntas: si la forma pertenece a la selección y si participa en una operación. Por ejemplo, una forma recién construida con `m_Mode == _DRAFTDRAW_MODE_ACTIVE` ya devuelve verdadero, aunque no haya sido seleccionada por el usuario.

Los usos tienen finalidades distintas: `CSelectionTool` gestiona la selección, la vista y el documento conservan recorridos colectivos, y `CShape::Move` actualiza sus marcadores. `CPenTool` ya no utiliza `IsSelected()`: actualiza únicamente formas en `ACTIVE`. Cambiar el significado de `IsSelected()` requiere revisar por separado las rutas que todavía lo utilizan.

## Herramienta de rotación

`CRotationTool`, declarada en `RotationTool.h` e implementada en `RotationTool.cpp`, deriva de `CAbstractTool` y gestiona la rotación interactiva de la selección existente. El comando `ID_BUTTON_ROTATE` la instala en `m_tooSel`; `CDraftDrawView` le entrega los eventos. La rotación ya no depende de una rama de `CPenTool::MoveTo`.

### Responsabilidades y métodos

| Método | Responsabilidad |
|---|---|
| `MouseDown` | Finaliza cualquier gesto anterior, comprueba `m_pObListSel`, guarda el punto inicial y `m_RectLast`, y pone las formas en `ACTIVE`. No selecciona nuevas formas. |
| `MoveTo` | Durante un arrastre y con el botón izquierdo pulsado, recorre `m_pObListSel`, llama a `CShape::OnRotate` y acumula la región de repintado anterior y posterior al giro. |
| `MouseUp` | Finaliza el gesto mediante `FinishDrag`. |
| `FinishDrag` | Devuelve las formas a `IDLE`, mantiene la selección y marca el documento como modificado si ha cambiado algún ángulo. También lo utiliza el destructor. |
| `AddUpdateRect` | Obtiene el rectángulo envolvente mediante `GetSizingRect`, añade el margen de repintado y lo acumula. Actualmente no lo recorta a los límites de la hoja. |

La herramienta utiliza `m_bDragging` y `m_bChanged` para controlar su gesto. Trabaja directamente con la lista de selección prestada por la vista; no busca formas mediante `IsSelected()`, no añade objetos auxiliares al stack ni utiliza su última posición para identificar el objeto de trabajo.

### Uso y estados

1. Seleccionar una o varias formas con `CSelectionTool`.
2. Activar el botón de rotación (`ID_BUTTON_ROTATE`).
3. Pulsar y arrastrar horizontalmente: cada actualización con desplazamiento a la derecha aplica +5 grados; a la izquierda, -5 grados. El movimiento exclusivamente vertical no modifica el ángulo.
4. Soltar para finalizar. Las formas vuelven a `IDLE` y permanecen en la selección.

| Momento | Estado de las formas |
|---|---|
| Seleccionadas antes del gesto | `IDLE`; continúan en la lista de selección. |
| Durante el gesto de rotación | `ACTIVE`; `OnRotate` actualiza el ángulo y la matriz. |
| Después de soltar o finalizar el gesto | `IDLE`; conservan el ángulo alcanzado y la selección. |

El pivote utilizado para dibujar cada forma es su propio centro; no se introduce un pivote común para el grupo. Sin selección la herramienta no realiza cambios.

La vista captura el ratón durante el gesto para recibir la suelta fuera del área cliente. La pérdida de captura finaliza el gesto. Escape también lo finaliza, conserva el ángulo alcanzado y vuelve a selección. Al sustituir la herramienta, su destructor cierra un gesto pendiente.

### Separación entre interacción y dibujo

El recorrido de actualización es `CRotationTool::MoveTo` → `CShape::OnRotate` → `CShape::Rotate`. Este último actualiza `m_fAngle` y los coeficientes `m_eM11`, `m_eM12`, `m_eM21` y `m_eM22`. La herramienta no transforma los puntos mediante `DoRotate` ni recalcula los conexionados.

El dibujo del giro corresponde a `CDraftDrawView::DoWorldTransform`, que aplica la matriz al contexto gráfico recibido, alrededor del centro de la forma. No dibuja por sí mismo: la forma se pinta después mediante `OnDraw`.

Para comprobar el dibujo en el buffer se ha añadido una aplicación local de la transformación en las dos llamadas operativas a `OnDraw(pDC1)`:

| Ruta de `CDraftDrawView::OnDraw` | Uso |
|---|---|
| Primer recorrido, formas en `IDLE` | Repintado parcial de las formas que intersectan la región actualizada. Incluye formas seleccionadas que ya no se están arrastrando. |
| Segundo recorrido, formas en `ACTIVE` | Dibujo durante creación, movimiento, redimensionado o rotación. |

En ambas rutas se guarda el estado de `pDC1` con `SaveDC`, se activa `GM_ADVANCED`, se llama a `DoWorldTransform(pSh, pDC1->m_hDC)`, se dibuja y se restaura el contexto con `RestoreDC`. La transformación utiliza las coordenadas locales que la vista ya prepara para el buffer. Esta prueba solo cambia el dibujo; no modifica la geometría ni las relaciones de conexión.

El repintado completo de las formas en `IDLE` mantiene su ruta directa a `pDC`. Los giros fijos del menú de las formas son comandos independientes de esta herramienta.

### Alcance de la verificación

Se ha comprobado la compilación Debug Win32 y el comportamiento de la herramienta con sustitutos de los límites MFC: selección vacía, varias formas, pasos angulares, movimiento vertical, botón suelto, arrastres repetidos y cierre por destrucción.

En las pruebas visuales anteriores al cambio local de `pDC1` se observaron formas sin giro visible y alteraciones de la cuadrícula. La prueba de transformación local compila, pero su resultado visual posterior no se ha confirmado aquí; tampoco se ha validado una rotación correcta para todas las clases de formas, rellenos, imágenes o elementos conectados.

No se ha añadido deshacer de rotación ni cambiado la serialización. La separación de la herramienta y el soporte gráfico del giro son aspectos distintos y deben comprobarse por separado.

## Referencias en el código

- `Shape.h`: definición de las constantes.
- `Shape.cpp`: constructor de `CShape`, `OnLButtonDown`, `OnMouseMove`, `OnLButtonUp`, `Select`, `Unselect` e `IsSelected`.
- `SelectionTool.cpp` y `PenTool.cpp`: actualización de las formas durante la interacción.
- `RotationTool.h/.cpp`: herramienta de rotación y ciclo del gesto; `DraftDrawView.cpp`: entrega de eventos y transformación de los contextos de dibujo.

## Selección inicial del tipo de cable

`CConnectorTool` puede asignar un cable predeterminado al iniciar desde un pin de componente. Las asociaciones están en `ConnectorCableDefaults.ini`, dentro del directorio base configurado de DraftCable. La sección `[DefaultCable]` utiliza nombres de archivo de conector, con extensión `.txt`, y nombres de archivo de cable de `Cables/`; no utiliza índices numéricos de la base.

El INI incluido activa siete asociaciones del XU4200: alimentación AC/DC, LAN, audio TX/RX y control extendido TX/RX. RF N/BNC y E1 permanecen manuales por sus distintas opciones de extremo o cruce. El tipo anterior `ETH_RJ45_100BTX.txt` tampoco recibe por defecto el cable de la radio: su asignación 2–9 → 1–8 tiene un sentido definido.

Para la pieza `Comms.R&S RS5400` se añaden dos asociaciones: `ETH_RJ45_8P8C.txt` → `ETH_RJ45_STRAIGHT_T568B.txt` e `IEC_C14_LNPE.txt` → `AC_IEC_C13_LNPE.txt`. Los tres puertos Ethernet usan el latiguillo directo de ocho contactos; las dos alimentaciones usan L/N/PE. MGMT y AC 2 figuran como opcionales. Véase `db/Comms.md` para alcance, fuentes y validación de la pieza.

- Solo se aplica al pin de origen y cuando el tramo todavía no tiene tipo. Una elección explícita conserva prioridad.
- Una continuación o un inicio desde un extremo de cable existente hereda su `m_strTypeIdent`, incluido el caso vacío. Todos los nuevos tramos conservan así la selección de su tramo anterior.
- Conectar el destino no sustituye el tipo ni invierte la asignación de contactos. Dibujar desde espacio libre mantiene el comportamiento manual.
- Un INI ausente, una asociación vacía, un tipo de pin inválido o un archivo de cable inexistente dejan el comportamiento anterior. Los nombres de cable se toman con la escritura real del archivo para coincidir con el catálogo existente.
- La selección es un valor inicial modificable mediante las propiedades actuales del cable. La regla no valida el conexionado eléctrico completo entre equipos.

Se reutiliza `CShape::m_strTypeIdent`, que ya se guarda en el documento; no se cambia el formato de las formas. La nueva regla se invoca en `CConnectorTool::__DoLinkPin` únicamente para el extremo inicial, conservando el enlace y la limpieza que ya hacía ese método.

Verificación: pruebas nativas de los ciclos de la herramienta, siete asociaciones, continuidad por varios tramos, elección manual, inicio desde cable existente, destino, cancelación, borrado externo, alternativas del INI y serialización. Pendiente la comprobación interactiva del usuario sobre un esquema real.
