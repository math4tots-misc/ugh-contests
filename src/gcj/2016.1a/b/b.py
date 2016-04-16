try: input = raw_input
except NameError: pass

def solve(N, lines):
  groups, row_number = group_lines_and_find_missing_row_number(N, lines)
  answer = []
  for i in range(N):
    if i == row_number:
      answer.append(groups[row_number][0][row_number])
    else:
      a = groups[i][0][row_number]
      b = groups[i][1][row_number]
      if groups[row_number][0][i] == a:
        answer.append(b)
      else:
        answer.append(a)
  return answer

def group_lines_and_find_missing_row_number(N, lines):
  lines = list(lines)
  groups = []
  row_number = None
  for position in range(N-1):
    lines.sort(key=lambda line: -line[position])
    if lines[-1][position] == lines[-2][position]:
      groups.append([lines.pop(), lines.pop()])
    else:
      row_number = position
      groups.append([lines.pop()])

  if row_number is None:
    row_number = N-1
  groups.append(lines)

  return groups, row_number


for i in range(1, 1+int(input())):
  N = int(input())
  lines = []
  for _ in range(2*N-1):
    lines.append(map(int, input().split()))
  print('Case #%d: %s' % (i, ' '.join(map(str, solve(N, lines)))))
