/*
   This file is part of gdub.
   (C) 2006 Steve Hoffmann 

   gdub is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   gdub is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with gdub; see the file COPYING.  If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.	

*/

/**
 * @file queuedef.c
 * @author Steve Hoffmann
 * @brief definitions to set up and manage a queue
 */

/*
 * $Log$
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/*#include "memman.h"
#include "memmac.h"*/
#include "memory.h"
#include "queuedef.h"


BOOL queueisempty(Queue *q) {
  return (q->noofelements == 0 ? True:False);
}

void emptyqueue(void *spacetab, Queue *q, int queuesize) {

  if (queuesize <= 0) {
    printf("Attempt to initialize a queue of size %d", queuesize);
    exit(-1);
  } 

  q->queuespace = ALLOCMEMORY(spacetab, NULL, Queueelem, queuesize);
  q->queuesize = queuesize;
  q->noofelements = 0;
  q->enqueueindex = 0;
  q->dequeueindex = 0;
}

void enqueue(void* spacetab, Queue *q, Queueelem elem) {

  if(q->noofelements == q->queuesize) {
    resizequeue(spacetab, q);
  }

  q->queuespace[q->enqueueindex]=elem;
  q->noofelements++;

  /*implements circular datastructure*/
  if (q->enqueueindex == q->queuesize-1) {
    q->enqueueindex = 0;
  } else {
    q->enqueueindex++;
  }

}

Queueelem dequeue(Queue *q) {

  Queueelem elem;		

  if(queueisempty(q)) {
    printf("Attempt to dequeue element from empty queue\n");
    exit(-1);
  }	

  elem = q->queuespace[q->dequeueindex];
  q->noofelements--;

  /*implements circular data structure*/
  if(q->dequeueindex == q->queuesize-1) {
    q->dequeueindex = 0;
  } else {
    q->dequeueindex++;
  }

  return elem;
}

void resizequeue(void* spacetab, Queue *q) {

  Queueelem *src;
  Queueelem *dest;
  void *ptr;

  /*resize queue to double*/
  q->queuespace = ALLOCMEMORY(spacetab, q->queuespace, Queueelem, (q->queuesize*2));

  if (q->dequeueindex >= q->enqueueindex) {

    /*ptr arithmetics to move queue elements*/
    src  = &q->queuespace[q->dequeueindex];
    dest = &q->queuespace[q->queuesize+q->dequeueindex]; 	

    ptr = memmove(dest, src,((q->queuesize)-q->dequeueindex)*sizeof(Queueelem));
    assert(ptr == dest);

    q->dequeueindex = (q->dequeueindex+q->queuesize);
  }

  q->queuesize = q->queuesize*2;
}

void showqueue(Queue *q) {
  int i;
  Queueelem elem;

  printf("[");	
  for(i=0; i < q->queuesize; i++) {
    elem = q->queuespace[i];
    if (i != q->enqueueindex && i != q->dequeueindex)
      printf("%d", elem);
    if (i == q->enqueueindex)
      printf("%d*", elem);
    if(i == q->dequeueindex)
      printf("%d^", elem);
    if(i+1 != q->queuesize)
      printf(",");
  }	
  printf("]\n");

}

void wrapqueue(void *spacetab, Queue *q) {

  FREEMEMORY(spacetab, q->queuespace);

  q->enqueueindex = 0;
  q->dequeueindex = 0;
  q->queuesize = 0;
  q->noofelements = 0;
}
