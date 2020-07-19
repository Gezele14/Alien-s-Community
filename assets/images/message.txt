#include <stdio.h>
#include <stdlib.h>
#include <structs.h>

int size = 0;

void print_list(node_t *head)
{
  node_t *current = head;

  while (current != NULL)
  {
    printf("%d\n", current->val);
    current = current->next;
  }
}

void add_end(node_t *head, int val)
{
  node_t *current = head;
  while (current->next != NULL)
  {
    current = current->next;
  }

  /* now we can add a new variable */
  current->next = (node_t *)malloc(sizeof(node_t));
  current->next->val = val;
  current->next->next = NULL;

  size++;
}

void add_start(node_t **head, int val)
{
  node_t *new_node;
  new_node = (node_t *)malloc(sizeof(node_t));

  new_node->val = val;
  new_node->next = *head;
  *head = new_node;

  size++;
}

int remove_start(node_t **head)
{
  int retval = -1;
  node_t *next_node = NULL;

  if (*head == NULL)
  {
    return -1;
  }

  next_node = (*head)->next;
  retval = (*head)->val;
  free(*head);
  *head = next_node;

  size--;

  return retval;
}

int remove_last(node_t *head)
{
  int retval = 0;
  /* if there is only one item in the list, remove it */
  if (head->next == NULL)
  {
    retval = head->val;
    free(head);
    return retval;
  }

  /* get to the second to last node in the list */
  node_t *current = head;
  while (current->next->next != NULL)
  {
    current = current->next;
  }

  /* now current points to the second to last item of the list, so let's remove current->next */
  retval = current->next->val;
  free(current->next);
  current->next = NULL;

  size--;

  return retval;
}

int remove_by_index(node_t **head, int n)
{
  int i = 0;
  int retval = -1;
  node_t *current = *head;
  node_t *temp_node = NULL;

  if (n >= size)
  {
    return -1;
  }

  if (n == 0)
  {
    return remove_start(head);
  }

  for (i = 0; i < n - 1; i++)
  {
    if (current->next == NULL)
    {
      return -1;
    }
    current = current->next;
  }

  temp_node = current->next;
  retval = temp_node->val;
  current->next = temp_node->next;
  free(temp_node);

  size--;

  return retval;
}

int get_size()
{
  return size;
}