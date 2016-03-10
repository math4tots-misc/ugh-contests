import java.io.*;
import java.util.*;

public class Solution {
  public static void main(String[] args) {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT. Your class should be named Solution. */
    final Scanner sc = new Scanner(System.in);
    final int N = sc.nextInt();
    final int M = sc.nextInt();
    final BitSet[] B = new BitSet[3];
    B[1] = new BitSet();
    B[2] = new BitSet();
    for (int i = 0; i < M; i++) {
      final String cmd = sc.next();
      final int a = sc.nextInt();
      final int b = sc.nextInt();
      if (cmd.equals("AND")) {
        B[a].and(B[b]);
      } else if (cmd.equals("OR")) {
        B[a].or(B[b]);
      } else if (cmd.equals("XOR")) {
        B[a].xor(B[b]);
      } else if (cmd.equals("FLIP")) {
        B[a].flip(b);
      } else if (cmd.equals("SET")) {
        B[a].set(b);
      } else {
        throw new RuntimeException(cmd);
      }
      System.out.print(B[1].cardinality());
      System.out.print(" ");
      System.out.println(B[2].cardinality());
    }
  }
}

