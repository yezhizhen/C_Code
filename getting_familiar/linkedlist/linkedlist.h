#ifndef _NODE_H_
#define _NODE_H_

//because compiler don't know what is node when we define next, we need to give struct a name
typedef struct _nodes{
	int value;
	struct _nodes *next;
} node;
	
#endif
