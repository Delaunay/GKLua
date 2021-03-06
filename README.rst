GKLua
=====

.. warning::

   THIS DOES NOT COMPILE, DO NOT TRY


Lua Integration for Unreal Engine based on the Script Plugin by Epic.
Updated to 5.0 with a few improvements

Requirements
~~~~~~~~~~~~

* UE5.0+

Development Install
~~~~~~~~~~~~~~~~~~~

Install the python package to run this example as a gym environment

.. code-block:: bash

   git submodule add https://github.com/Delaunay/GKLua Plugins/GKLua


Usage
~~~~~

#. Create a new lua file inside the your content folder

#. When the editor is going to import the script it will ask you which
   class this script is inheriting from.

#. When the content of the script is modified the script will be reimported by the editor

.. code-block:: lua

   function BeginPlay()
      print("BeginPlay in lua" .. this.Get_ActorTransform())
   end

   function Tick(delta)
      print("Tick in lua")
   end

   function Destoy()
      print("End play in lua")
   end

   function MyCustomFunctionCallableInBlueprint()
      print("Callable")
   end


Overview
--------

.. note::

   You need to launch the editor once for the plugin to generate the Lua wrapeprs

* GKLuaEditor

  * "Compiles" the blueprint

    * Generate the blueprint class from a blueprint a sets up the calls
    * Code is loaded in `FLuaContext::Initialize` when `luaL_loadstring(LuaState, TCHAR_TO_ANSI(*Code)` is called
    * We can use `lua_dump` to retrive the compiled bytecode
    * bytecode can be loaded using `lua_loadstring` NOT `luaL_loadstring` which is currently used

  * Specialize the Blueprint Editor

* GKLuaGenerator

  * Generates lua wrappers
  * Hooks into the UHT (Unreal Header Tools) to generate the wrappers

* GKLua

  * Manage the lua VM
  * Register new libraries at runtime
  * Call Lua Functions
  * All objects that interacts with LUA get a lua_state

    * Lua can be hooked to Tick, Destoy, BeginPlay

  * You can use this in lua to access the current object being implemented by the script

  * Objects that use lua create a ScriptBlueprint which holds the lua script
    so lua is next to the blueprint system. Used to be able to specify a source file.
    seems deprecated which might mean the code is bundled in the .asset instead of being
    readable text.

Expose your own classes
-----------------------

.. todo::

   ....

Snippet
-------

Call a UFUNCTION

.. code-block::

   const FGeneratedWrappedFunction& InFuncDef;
   UFunction* Func = InFuncDef.Func;

   FStructOnScope FuncParams(Func);

   // Calls the ufunction with its parameters allocated inside the struct memory
   InObj->ProcessEvent((UFunction*)InFunc, FuncParams.GetStructMemory());

   // return value is inside FuncParams
   ReturnValue = (const FProperty* Prop, FuncParams.GetStructMemory())
