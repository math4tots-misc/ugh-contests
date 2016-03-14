// java-singleton

import java.io.*;
import java.util.*;
import java.text.*;
import java.math.*;
import java.util.regex.*;
import java.lang.reflect.*;

class Singleton {
  public String str = null;
  private Singleton() {}
  private static Singleton instance = new Singleton();
  public static Singleton getSingleInstance() {
    return instance;
  }
}
