import java.io.*;
import java.util.*;
import java.text.*;
import java.math.*;
import java.util.regex.*;

class Student implements Comparable<Student> {
   private int token;
   private String fname;
   private double cgpa;
   public Student(int id, String fname, double cgpa) {
      super();
      this.token = id;
      this.fname = fname;
      this.cgpa = cgpa;
   }
   public int getToken() {
      return token;
   }
   public String getFname() {
      return fname;
   }
   public double getCgpa() {
      return cgpa;
   }
   public int compareTo(Student other) {
     // Students with higher gpa served first.
     if (cgpa != other.cgpa) {
       if (cgpa > other.cgpa) {
         return -1;
       } else {
         return 1;
       }
     }

     // Students based on first name.
     if (!fname.equals(other.fname)) {
       return fname.compareTo(other.fname);
     }

     // By token
     return token - other.token;
   }
}

public class Solution {

    public static void main(String[] args) {
      Scanner in = new Scanner(System.in);
      int totalEvents = Integer.parseInt(in.nextLine());
      PriorityQueue<Student> queue = new PriorityQueue<Student>();
      while(totalEvents>0){
         String event = in.next();
         
         if (event.equals("SERVED")) {
           queue.poll();
         } else if (event.equals("ENTER")) {
           String name = in.next();
           double gpa = in.nextDouble();
           int token = in.nextInt();
           queue.add(new Student(token, name, gpa));
         }

         totalEvents--;
      }
      if (queue.size() == 0) {
        System.out.println("EMPTY");
      } else {
        Student student = queue.poll();
        while (student != null) {
          System.out.println(student.getFname());
          student = queue.poll();
        }
      }
    }
}
