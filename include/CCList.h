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
** CCList.h for project CContainers
**
** Made by mota
** email <mota@souitom.org>
**
** Started on  Sat May 14 03:08:37 2011 mota
** Last update Sun May 15 03:18:12 2011 mota
*/

#ifndef		CCLIST_H_
# define	CCLIST_H_

#include <stdlib.h>

#define		CCLIST_ENTRY(type)		\
struct type	*next;				\
struct type	*prev

#define		CCLIST_PROTO(name, type)	\
struct name					\
{						\
  struct type	*head;				\
  struct type	*tail;				\
  unsigned int	size;				\
}

#define		CCLIST_NAME(name)		\
struct name

#define		CCLIST_CREATE(name)		\
CCLIST_NAME(name)

#define		CCLIST_INIT(list)		\
do						\
  {						\
    (list)->head = NULL;			\
    (list)->tail = NULL;			\
    (list)->size = 0;				\
  } while (0)

#define		CCLIST_EMPTY(list)	((list)->head == NULL)
#define		CCLIST_HEAD(list)	(list)->head
#define		CCLIST_TAIL(list)	(list)->tail
#define		CCLIST_SIZE(list)	(list)->size
#define		CCLIST_NEXT(entry)	(entry)->next
#define		CCLIST_PREV(entry)	(entry)->prev

#define		CCLIST_DUP(list, newlist)	\
do						\
  {						\
    CCLIST_HEAD(newlist) = CCLIST_HEAD(list);	\
    CCLIST_TAIL(newlist) = CCLIST_TAIL(list);	\
    CCLIST_SIZE(newlist) = CCLIST_SIZE(list);	\
  } while (0)

#define		CCLIST_FIRST_ENTRY(list, entry)	\
do						\
  {						\
    CCLIST_HEAD(list) = (entry);		\
    CCLIST_TAIL(list) = (entry);		\
    (entry)->next = NULL;			\
    (entry)->prev = NULL;			\
    ++CCLIST_SIZE(list);			\
  } while (0)

/* Don't use with entry == list->head or entry == list->tail */
/* use CCLIST_POP_FRONT and CCLIST_POP_BACK instead */
#define		CCLIST_REMOVE(list, entry)	\
do						\
  {						\
    if ((entry)->prev != NULL)			\
      (entry)->prev->next = (entry)->next;	\
    if ((entry)->next != NULL)			\
      (entry)->next->prev = (entry)->prev;	\
    if (CCLIST_HEAD(list) == (entry))		\
      CCLIST_HEAD(list) = (entry)->next;	\
    if (CCLIST_TAIL(list) == (entry)){		\
      CCLIST_TAIL(list) = (entry)->prev;}	\
    --CCLIST_SIZE(list);			\
  } while (0)

#define		CCLIST_PUSH_FRONT(list, entry)	\
do						\
  {						\
    (entry)->next = CCLIST_HEAD(list);		\
    if ((entry)->next != NULL)			\
      (entry)->next->prev = (entry);		\
    (entry)->prev = NULL;			\
    CCLIST_HEAD(list) = (entry);		\
    if (CCLIST_TAIL(list) == NULL)		\
      CCLIST_TAIL(list) = (entry);		\
    ++CCLIST_SIZE(list);			\
  } while (0)

#define		CCLIST_POP_FRONT(list, var)	\
do						\
  {						\
    (var) = CCLIST_HEAD(list);			\
    CCLIST_REMOVE(list, var);			\
  } while (0)

#define		CCLIST_PUSH_BACK(list, entry)	\
do						\
  {						\
    (entry)->prev = CCLIST_TAIL(list);		\
    if ((entry)->prev != NULL)			\
      (entry)->prev->next = (entry);		\
    (entry)->next = NULL;			\
    CCLIST_TAIL(list) = (entry);		\
    if (CCLIST_HEAD(list) == NULL)		\
      CCLIST_HEAD(list) = (entry);		\
    ++CCLIST_SIZE(list);			\
  } while (0)

#define		CCLIST_POP_BACK(list, var)	\
do						\
  {						\
    (var) = CCLIST_TAIL(list);			\
    CCLIST_REMOVE(list, var);			\
  } while (0)

#define		CCLIST_INSERT(list, after, entry)	\
do							\
  {							\
    if ((after) != NULL)				\
      {							\
	(entry)->next = (after);			\
	(entry)->prev = (after)->prev;			\
	(after)->prev = (entry);			\
	if ((entry)->prev != NULL)			\
	  (entry)->prev->next = (entry);		\
	else						\
	  CCLIST_HEAD(list) = (entry);			\
	++CCLIST_SIZE(list);				\
      }							\
    else						\
      CCLIST_PUSH_FRONT(list, entry);			\
  } while (0)

#define		CCLIST_INSERT_AFTER(list, before, entry)	\
do								\
  {								\
    if ((before) != NULL)					\
      {								\
	(entry)->prev = (before);				\
	(entry)->next = (before)->next;				\
	(before)->next = (entry);				\
	if ((entry)->next != NULL)				\
	  (entry)->next->prev = (entry);			\
	else							\
	  CCLIST_TAIL(list) = (entry);				\
      }								\
    else							\
      CCLIST_PUSH_BACK(list, entry);				\
  } while (0)

#define		CCLIST_FOREACH(list, tmp)				\
for ((tmp) = CCLIST_HEAD(list); (tmp) != NULL; (tmp) = (tmp)->next)

#define		CCLIST_FOREACH_SAFE(list, tmp, junk)			\
for ((tmp) = CCLIST_HEAD(list); (tmp) != NULL && ((junk) = (tmp)->next); (tmp) = (junk))

#define		CCLIST_RFOREACH(list, tmp)				\
for ((tmp) = CCLIST_TAIL(list); (tmp) != NULL; (tmp) = (tmp)->prev)

#define		CCLIST_RFOREACH_SAFE(list, tmp, junk)			\
for ((tmp) = CCLIST_TAIL(list); (tmp) != NULL && ((junk) = (tmp)->prev); (tmp) = (junk))

/* int (*cmp_func)(entry_type *left, entry_type ^right) */
#define		CCLIST_FIND(list, tmp, ref, cmp_func)	\
do							\
  {							\
    CCLIST_FOREACH(list, tmp)				\
      {							\
	if (cmp_func(ref, tmp))				\
	  break;					\
      }							\
  } while (0)

#define		CCLIST_FIND_SAFE(list, tmp, junk, ref, cmp_func)	\
do									\
  {									\
    CCLIST_FOREACH_SAFE(list, tmp, junk)				\
      {									\
	if (cmp_func(ref, tmp))						\
	  break;							\
      }									\
  } while (0)

#define		CCLIST_RFIND(list, tmp, ref, cmp_func)	\
do							\
  {							\
    CCLIST_RFOREACH(list, tmp)				\
      {							\
	if (cmp_func(ref, tmp))				\
	  break;					\
      }							\
  } while (0)

#define		CCLIST_RFIND_SAFE(list, tmp, junk, ref, cmp_func)	\
do									\
  {									\
    CCLIST_RFOREACH_SAFE(list, tmp, junk)				\
      {									\
	if (cmp_func(ref, tmp))						\
	  break;							\
      }									\
  } while (0)

/* int	(*cmp_func)(val_type left, val_type right) */
#define		CCLIST_FIND_FIELD(list, tmp, val, field, cmp_func)	\
do									\
  {									\
    CCLIST_FOREACH(list, tmp)						\
      {									\
	if (cmp_func(val, (tmp)->field))				\
	  break;							\
      }									\
  } while (0)

#define		CCLIST_FIND_FIELD_SAFE(list, tmp, junk, val, field, cmp_func) \
do									\
  {									\
    CCLIST_FOREACH_SAFE(list, tmp, junk)				\
      {									\
	if (cmp_func(val, (tmp)->field))				\
	  break;							\
      }									\
  } while (0)

#define		CCLIST_RFIND_FIELD(list, tmp, val, field, cmp_func)	\
do									\
  {									\
    CCLIST_RFOREACH(list, tmp)						\
      {									\
	if (cmp_func(val, (tmp)->field))				\
	  break;							\
      }									\
  } while (0)

#define		CCLIST_RFIND_FIELD_SAFE(list, tmp, junk, val, field, cmp_func) \
do									\
  {									\
    CCLIST_RFOREACH_SAFE(list, tmp, junk)				\
      {									\
	if (cmp_func(val, (tmp)->field))				\
	  break;							\
      }									\
  } while (0)

#define		CCLIST_REVERSE(list, list_type, entry_type)	\
do								\
  {								\
    struct entry_type		*_tmp;				\
    CCLIST_CREATE(list_type)	_tmp_list;			\
    CCLIST_INIT(&_tmp_list);					\
    while (!CCLIST_EMPTY(list))					\
      {								\
	CCLIST_POP_FRONT(list, _tmp);				\
	CCLIST_PUSH_FRONT(&(_tmp_list), _tmp);			\
      }								\
    CCLIST_DUP(&(_tmp_list), list);				\
  } while (0)

#define		CCLIST_SWAP(list, left, right, type)	\
do							\
  {							\
    struct type *_tmp = CCLIST_NEXT(right);		\
    if ((_tmp) == (left))				\
      (_tmp) = CCLIST_PREV(_tmp);			\
    CCLIST_REMOVE(list, right);				\
    CCLIST_INSERT(list, left, right);			\
    CCLIST_REMOVE(list, left);				\
    if (_tmp == NULL)					\
      CCLIST_PUSH_BACK(list, left);			\
    else						\
      CCLIST_INSERT(list, _tmp, left);			\
  } while (0)

#define		CCLIST_CLEAR(list, tmp, free_func)	\
do							\
  {							\
    while (!CCLIST_EMPTY(list))				\
      {							\
	CCLIST_POP_FRONT(list, tmp);			\
	free_func(tmp);					\
      }							\
  } while (0)

#define		CCLIST_FREE(list, tmp)		\
do						\
  {						\
    CCLIST_CLEAR(list, tmp, free);		\
  } while (0)

#endif		/* !CCLIST_H_ */