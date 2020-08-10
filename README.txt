------------------------
RezWaki's Mouse Expander
------------------------

Purpose of this app is to make routine work easier.
You can script your mouse up with this one!
Script writing here is easy as hell, example of simple script is in "testscript.spt"

Before starting this app make sure you have setted up all right,
all configurable vars is stored in "mouse_expander.dat" file, open it with notepad and edit.

Config file supports code-style comments, this means that any line started with "//" will be ignored during parsing. But remember - scripting does not support the comments.

Little scripting help:

------------------------------------------------------------------------------------
COMMAND	     | VALUE TYPE | DESCRIPTION
------------------------------------------------------------------------------------
MoveMouseTo  | X,Y	  | Moves a mouse to specified location
MouseClick   | 1/2	  | Clicks specified mouse button - 1 = left, 2 = right
Wait	     | Integer	  | Wait before execute next line, 1000 = 1 second
Loop	     | -	  | Add this at the end of the script if you want it to loop

Example of using all of these commands is in "testscript.spt" file!

To terminate the script press F6 key, to close this app press F12.

Don't try to edit the config file when app is running, close it firstly and disable "Read-only" file attribute.
