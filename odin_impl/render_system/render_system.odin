/*
Abstract:
Any subsystem which wants to draw something has to fill out a RenderCommand
then pass it to the RenderCommandRecipient
This recipient uses the information in the queue of RenderCommands to use a backend
to draw stuff on the screen
*/

RenderCommandRecipient :: struct {
	queue: 
}

RenderKind :: enum {
	LINE, 
}

RenderCommand :: struct {

}