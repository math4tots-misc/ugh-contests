import collections
import os
import sys
rdir = os.path.dirname(os.path.realpath(__file__))

standard_includes = set()
header_queue = collections.deque()
processed = dict()
deps = dict()
processing = set()

main_cutoff = set([
    'using namespace std;',
    "def main():",
])

def process_header(hdr):
  with open(os.path.join(os.path.join(rdir, 'lib'), hdr)) as f:
    data = f.read()
  return process(data, hdr)

def process(data, tag, strip_main=True):
  s = ''
  deps[tag] = set()
  for line in data.splitlines():
    if line.startswith('#include <'):
      assert line.endswith('>'), line
      standard_includes.add(line[len('#include <'):-len('>')])
    elif line.startswith('#x '):
      hdr = line[len('#x '):]
      deps[tag].add(hdr)
    elif strip_main and line in main_cutoff:
      break
    else:
      s += line + '\n'
  processed[tag] = s
  return s

def process_header_recursively(hdr):
  if hdr not in processing:
    processing.add(hdr)
    process_header(hdr)
    for dep in deps[hdr]:
      process_header_recursively(dep)

def process_recursively(data):
  process(data, '<main>', strip_main=False)
  for dep in deps['<main>']:
    process_header_recursively(dep)

def combine():
  used = set()
  s = ''
  if standard_includes:
    s = '// standard headers\n'
    for inc in sorted(standard_includes):
      s += '#include <%s>\n' % inc
  for dep in linearize_deps():
    if dep not in used:
      used.add(dep)
      s += processed[dep]
  return s

def linearize_deps():
  marked = set()
  order = []
  while len(order) < len(deps):
    for tag in (tag for tag in deps if tag not in marked):
      dd = set(d for d in deps[tag] if d not in marked)
      if not dd:
        marked.add(tag)
        order.append(tag)
        break
    else:
      raise Exception("Circular dep including: %r (%s)" % (
          [tag for tag in deps if tag not in marked],
          deps))
  return order

def main(data):
  process_recursively(data)
  return combine()

if __name__ == '__main__':
  print(main(sys.stdin.read()))

