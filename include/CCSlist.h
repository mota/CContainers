/*
** Copyright(c), CContainers
** All rights reserved.
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the University of California, Berkeley nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
** EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** CCSlist.h for project CContainers
**
** Made by mota
** email <mota@souitom.org>
**
** Started on  Fri May 20 03:54:25 2011 mota
** Last update Fri May 20 05:13:30 2011 mota
*/

#ifndef		CCSLIST_H_
# define	CCSLIST_H_

#include <stdlib.h>

#define		CCSLIST_ENTRY(entry_type)	\
entry_type	*next;

#define		CCSLIST_PROTO(name, entry_type)	\
struct name					\
{						\
  entry_type	*head;				\
  size_t	size;				\
}

#define		CCSLIST_NAME(name)		\
struct name

#define		CCSLIST_CREATE(name)		\
CCSLIST_NAME(name)

#define		CCSLIST_EMPTY(list)	((list)->head == NULL)
#define		CCSLIST_HEAD(list)	(list)->head
#define		CCSLIST_SIZE(list)	(list)->size
#define		CCSLIST_NEXT(entry)	(entry)->next

#define		CCSLIST_FOREACH(list, tmp)	\
for ((tmp) = CCSLIST_HEAD(list);		\
     (tmp) != NULL;				\
     (tmp) = CCSLIST_NEXT(tmp))

#define		CCSLIST_FOREACH_SAFE(list, tmp, junk)	\
for ((tmp) = CCSLIST_HEAD(list);			\
     (tmp) != NULL && ((junk) = CCSLIST_NEXT(tmp));	\
     (tmp) = (junk))

#define		CCSLIST_INIT(list)		\
do						\
  {						\
    CCSLIST_HEAD(list) = NULL;			\
    CCSLIST_SIZE(list) = 0;			\
  } while (0)

#define		CCSLIST_REF(list, newlist)	\
do						\
  {						\
    CCSLIST_HEAD(newlist) = CCSLIST_HEAD(list);	\
    CCSLIST_SIZE(newlist) = CCSLIST_SIZE(list);	\
  } while (0)

#define		CCSLIST_REMOVE_HEAD(list, tmp)		\
do							\
  {							\
    (tmp) = CCSLIST_HEAD(list)				\
    CCSLIST_HEAD(list) = CCSLIST_HEAD(list)->next;	\
    --CCSLIST_SIZE(list);				\
  } while (0)

#define		CCSLIST_REMOVE(list, entry, entry_type)	\
do							\
  {							\
    entry_type	*_tmp;					\
							\
    if ((entry) == CCSLIST_HEAD(list))			\
      CCSLIST_REMOVE_HEAD(list, _tmp);			\
    else						\
      {							\
	CCSLIST_FOREACH(list, _tmp)			\
	  if (CCSLIST_NEXT(_tmp) == (entry))		\
	    break;					\
	CCSLIST_NEXT(_tmp) = CCSLIST_NEXT(entry);	\
	--CCSLIST_SIZE(list);				\
      }							\
  } while (0)

#define		CCSLIST_INSERT_AFTER(list, before, entry)	\
do								\
  {								\
    CCSLIST_NEXT(entry) = CCSLIST_NEXT(before);			\
    CCSLIST_NEXT(before) = (entry);				\
    ++CCSLIST_SIZE(list);					\
  } while (0)

#define		CCSLIST_INSERT(list, entry)			\
do								\
  {								\
    CCSLIST_INSERT_AFTER(list, CCSLIST_HEAD(list), entry);	\
  } while (0)

#define		CCSLIST_REVERSE(list, list_type, entry_type)	\
do								\
  {								\
    entry_type			*_tmp;				\
    CCSLIST_CREATE(list_type)	_tmp_list;			\
								\
    CCSLIST_INIT(_tmp_list);					\
    while (!CCSLIST_EMPTY(list))				\
      {								\
	CCSLIST_POP_HEAD(list, _tmp);				\
	CCSLIST_INSERT(list, _tmp);				\
      }								\
    CCSLIST_REF(&_tmp_list, list);				\
  } while (0)

/* void	(*copy_func)(const entry_type * const ref, entry_type *cpy); */
/* void (*delete_func)(entry_type *entry); */
#define		CCSLIST_COPY(list, newlist, entry_type, copy_func, delete_func)	\
do									\
  {									\
    entry_type	*_tmp;							\
    entry_type	*_entry;						\
									\
    CCSLIST_INIT(newlist);						\
    CCSLIST_FOREACH(list, _tmp)						\
      {									\
	if ((_entry = malloc(sizeof *_entry)) == NULL)			\
	  {								\
	    CCSLIST_CLEAR(newlist, delete_func);			\
	    break;							\
	  }								\
	copy_func(_tmp, _entry);					\
	CCSLIST_INSERT(newlist, _entry);				\
      }									\
    CCSLIST_REVERSE(newlist);						\
  } while (0)

/* int (*cmp_func)(entry_type *left, entry_type *right) */
#define		CCSLIST_FIND(list, tmp, ref, cmp_func)	\
do							\
  {							\
    CCSLIST_FOREACH(list, tmp)				\
      {							\
	if (cmp_func(ref, tmp) == 0)			\
	  break;					\
      }							\
  } while (0)

/* int	(*cmp_func)(val_type left, val_type right) */
#define		CCSLIST_FIND_FIELD(list, tmp, val, field, cmp_func)	\
do									\
  {									\
    CCSLIST_FOREACH(list, tmp)						\
      {									\
	if (cmp_func(val, (tmp)->field) == 0)				\
	  break;							\
      }									\
  } while (0)

/* In C: void	(*free_func)(entry_type *entry) */
/* In C++: void	(*free_func)(void *entry) */
#define		CCSLIST_DESTROY_HEAD(list, free_func)	\
do							\
  {							\
    void	*_tmp;					\
							\
    CCSLIST_POP_HEAD(list, _tmp);			\
    free_func(_tmp);					\
  } while (0)

#define		CCSLIST_CLEAR(list, free_func)	\
do						\
  {						\
    while (!CCSLIST_EMPTY(list))		\
      {						\
	CCSLIST_DESTROY_HEAD(list, free_func);	\
      }						\
  } while (0)

#define		CCSLIST_FREE(list)		\
CCSLIST_CLEAR(list, free)

#endif		/* !CCSLIST_H */