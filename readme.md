# DraftCable: herramientas y estados de las formas

## Inventario de herramientas

Este inventario describe las herramientas de interacción y colocación presentes en el código. Una función de la interfaz no equivale necesariamente a una clase `Tool`: parte del comportamiento sigue en `CDraftDrawView`, `CDraftDrawDoc` y las propias formas. La descripción procede de la lectura del código, no de una prueba funcional de cada herramienta.

### Clases de herramientas

| Clase | Función | Uso del stack y la selección | Integración actual |
|---|---|---|---|
| `CAbstractTool` | Base común: `MouseDown`, `MoveTo`, `MouseUp`, deselección y utilidades de creación. No es una herramienta elegible en la interfaz. | `DeselectAll` recorre `m_pObListSel`; `GetCurrentShape` obtiene el último objeto del documento; `_DoAddNewShapeToStack` comprueba `m_ObArray` e inserta mediante `AddObject`. | Base de las tres herramientas siguientes. Archivos `AbstractTool.h/.cpp`. |
| `CSelectionTool` | Selección por clic, selección con Ctrl, selección por rectángulo, movimiento y redimensionado mediante marcadores. | Busca candidatos en el documento; mueve y finaliza el gesto recorriendo `m_pObListSel`. El rectángulo auxiliar todavía se añade al documento y se retira mediante `DeleteObject(-1)`. | Delegación de pulsación, movimiento y suelta activada mediante `TEST_SELTOOL`; la vista retorna tras delegar. Archivos `SelectionTool.h/.cpp`. |
| `CPenTool` | Dibujo de formas y colocación de componentes. También conserva ramas de manipulación, incluida rotación según `m_iToolSel`, y estados para dibujo por puntos. | `MoveTo` recorre el documento y filtra por `IsSelected()`; el resultado de `m_pObListSel->Find` no se utiliza. Identifica la forma actual mediante el último objeto y utiliza `m_pSh` para la creación. | Delegación activada mediante `TEST_PENTOOL`. En la suelta, la vista continúa además por el procesamiento antiguo. Archivos `PenTool.h/.cpp`. |
| `CConnectorTool` | Inicio de conexiones, enlace de extremos y creación de sucesivos tramos de cable. | Busca destinos en el documento y añade nuevos `CShapeWire`; utiliza `m_pSh` y el último objeto como referencias de trabajo. | La clase se compila y se instancia al elegir cable, pero `TEST_CONNTOOL` está comentado: la vista conserva el procesamiento de los eventos de conexión. Archivos `ConnectorTool.h/.cpp`. |

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
| Cableado por tramos | `ID_PLACE_WIRE` | Prepara `CShapeWire` e instancia `CConnectorTool`; con `TEST_CONNTOOL` desactivado, los eventos siguen la lógica de conexión y continuación de cadena de la vista. |
| Polilínea, curva y poliarco | `ID_BUTTON_POLYLINE`, `ID_BUTTON_CURVE`, `ID_BUTTON_POLYARC` | Prepara `CShapePolyline`, `CShapeCurve` o `CShapePolyArc` y activa `_TOOLTYPEPOLY_DRAFTCABLE`. Estos comandos no crean una clase de herramienta específica. |
| Bus o mazo | `ID_PLACE_BUS` | Prepara un `CShapeBus` y activa el modo de dibujo por puntos. |
| Polígonos y figuras generadas | Familia `IDS_MENU_POLYGON_*` | Triángulo, pentágono, hexágono, octógono, polígono de N lados, estrella, engranaje, cruz de N brazos, paralelogramo, trapecio, rombo, cruz y figuras T, L y recta. Se generan como unidades; no tienen una clase `Tool` individual. |
| Pines y conectores de unidad | `ID_PLACE_PIN` | Configura y añade un `CShapePin` para su colocación. |
| Puertos izquierdo y derecho | `ID_PLACE_PORT`, `ID_PLACE_PORTRIGHT` | Carga las unidades de biblioteca `Standard.port_left.ddw` y `Standard.port_right.ddw`. |
| Marca de no conexión | `ID_PLACE_NOCON` | Carga la unidad `Standard.noconnect.ddw`. |
| Entrada y unión de bus | `ID_PLACE_BUSENTRY`, `ID_PLACE_BUSJUNC` | Carga las unidades `Standard.junction.ddw` y `Standard.busjunc.ddw`. |
| Texto y etiqueta o alias | `ID_PLACE_TEXT`, `ID_PLACE_NETALIAS` | Prepara un `CShapeLabel` con sus propiedades de texto y orientación. |
| Imagen | `ID_PLACE_PICTURE` | Crea un `CShapePicture`, permite elegir el archivo y lo añade al documento para colocarlo. |
| Regla de medición | `ID_BUTTON_RULE` | Prepara un `CShapeRule`; no existe una clase `RuleTool` separada. |
| Rotación interactiva | `ID_BUTTON_ROTATE` | Cambia `m_iToolSel` sin sustituir `m_tooSel`. Hay ramas de rotación en la vista y en `CPenTool`; la ruta depende de la herramienta que permanezca instalada. |
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

## Referencias en el código

- `Shape.h`: definición de las constantes.
- `Shape.cpp`: constructor de `CShape`, `OnLButtonDown`, `OnMouseMove`, `OnLButtonUp`, `Select`, `Unselect` e `IsSelected`.
- `SelectionTool.cpp` y `PenTool.cpp`: actualización de las formas durante la interacción.
