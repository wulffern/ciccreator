Cell
====

Cell is the fundamental object of all objects in the layout (Ok, some
object, like Port, inherit only Rect, but most inherit Cell). Cell can
be found in ``cic-core/src/core/cell.cpp``

Properties
^^^^^^^^^^^^^


In the object definition file the Cell can contain certain properties

* name [mandatory] : Name of the cell, must be uniq
* class [optional] : will use ``LayoutCell`` if it’s not specified. In ``cic-core/src/core/design.cpp`` constructor there is a list of allowedcells
* inherit [optional] : Cell from which this cell will inherit all instructions
* leech [optional] : Cell from which this cell will copy all instructions, but will not follow the inheritance hierarchy. In other words it only cares about the first parent, no grandparents.
* comment [optional] : can occur anywhere, and is ignored by the compiler



Action
^^^^^^^^^^^^^


Each cell will trigger actions in a specific sequence. The sequence is
as follows:

* Create object if the class exists
* Set name
* Run ``afterNew`` of all parents, starting with the oldest
* Run ``afterNew`` of the current cell
* Set all properties, and run all custom functions on the object
* Run ``beforePlace`` of all parents, starting with the oldest
* Run ``beforePlace`` of the current cell
* Call place() on the object
* Run ``afterPlace`` of all parents, starting with the oldest
* Run ``afterPlace`` of the current cell
* Run ``beforeRoute`` of all parents, starting with the oldest
* Run ``beforeRoute`` of the current cell
* Call route()
* Run ``afterRoute`` of all parents, starting with the oldest
* Run ``afterRoute`` of the current cell
* Call addAllPorts() on the object, to place remaining Ports.
* Run ``beforePaint`` of all parents, starting with the oldest
* Run ``beforePaint`` of the current cell
* Call paint() on the object
* Run ``afterPaint`` of all parents, starting with the oldest
* Run ``afterPaint`` of the current cell
* Add the object as a child of Design
* Add to the static list of cells
