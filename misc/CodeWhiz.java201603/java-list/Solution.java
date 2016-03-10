import java.io.*;
import java.util.*;

public class Solution {
  public static void main(String[] args) {
    final Scanner sc = new Scanner(System.in);
    final int N = sc.nextInt();
    final List<Long> list = new ArrayList<Long>();
    for (int i = 0; i < N; i++) {
      list.add(sc.nextLong());
    }
    final int Q = sc.nextInt();
    for (int i = 0; i < Q; i++) {
      final String op = sc.next();
      if (op.equals("Insert")) {
        final int idx = sc.nextInt();
        final long val = sc.nextLong();
        list.add(idx, val);
      } else if (op.equals("Delete")) {
        final int idx = sc.nextInt();
        list.remove(idx);
      }
    }
    System.out.print(list.get(0));
    for (int i = 1; i < list.size(); i++) {
      System.out.print(" ");
      System.out.print(list.get(i));
    }
    System.out.println();
  }
}