"""heap.py"""

import heapq
import itertools

try: input = raw_input
except NameError: pass

HEAP_REMOVED_SENTINEL = object()

class Heap(object):

  def __init__(self):
    self._heap = []
    self._counter = itertools.count()
    self._entry_finder = dict()
    self._size = 0

  def __len__(self):
    return self._size

  def __bool__(self):
    return bool(self._size)

  def __nonzero__(self):
    return self.__bool__()

  def push(self, task, priority=None):
    """Push or update a task with given priority.
    If priority is unspecified or None, use the task itself as the priority.
    """
    if task in self._entry_finder:
      self.remove(task)
    if priority is None:
      priority = task
    count = next(self._counter)
    entry = [priority, count, task]
    self._entry_finder[task] = entry
    heapq.heappush(self._heap, entry)
    self._size += 1

  def pop(self):
    """Pop task with smallest priority. Raises KeyError if empty."""
    while self._heap:
      priority, count, task = heapq.heappop(self._heap)
      if task is not HEAP_REMOVED_SENTINEL:
        del self._entry_finder[task]
        self._size -= 1
        return task
    raise KeyError('pop from an empty heap')

  def remove(self, task):
    """Remove a given task from the heap. Raises KeyError if empty."""
    entry = self._entry_finder.pop(task)
    self._size -= 1
    entry[-1] = HEAP_REMOVED_SENTINEL


def main():
  h = Heap()
  for i in range(10): h.push(i)
  assert len(h) == 10, len(h)
  for i in reversed(range(20, 30)): h.push(i)
  for i in range(10, 20): h.push(i)
  assert len(h) == 30, len(h)

  result = []
  while h:
    result.append(h.pop())

  assert result == list(range(30)), result

if __name__ == '__main__':
  main()
  print('pass')
