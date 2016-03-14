// ********* Just quick lib *********
fn readln() -> String {
  let mut s = String::new();
  assert!(std::io::stdin().read_line(&mut s).is_ok());
  s.trim().to_string()
}

fn conv<T>(s: &str) -> T where
    T: std::str::FromStr,
    <T as std::str::FromStr>::Err: std::fmt::Debug {
  s.parse().expect("parse")
}

fn split<T>(s: &str) -> Vec<T> where
    T: std::str::FromStr,
    <T as std::str::FromStr>::Err: std::fmt::Debug {
  s.split_whitespace().map(conv).collect()
}
// ********* end lib *********

fn main() {
  let t: usize = conv(&readln());
  for _ in 0..t {
    // read input
    let nm = split::<usize>(&readln());
    let n = nm[0];
    let m = nm[1];
    let mut maxdist: i64 = 1;
    let mut e: Vec<Vec<(usize, i64)>> = Vec::new();
    e.resize(n, Vec::new());
    for _ in 0..m {
      let xyv = split::<i64>(&readln());
      let x = xyv[0] as usize - 1;
      let y = xyv[1] as usize - 1;
      let v = xyv[2];
      e[x].push((y, v));
      e[y].push((x, v));
      maxdist += v;
    }
    let s = conv::<usize>(&readln()) - 1;
    // find shortest paths
    let mut d: Vec<i64> = Vec::new();
    d.resize(n, maxdist);
    let mut q: Vec<i64> = Vec::new();
    q.resize(n, maxdist);
    q[s] = 0;
    loop {
      let mut b = 0;
      let mut bd = maxdist;
      for x in 0..n {
        if d[x] == maxdist && q[x] < bd {
          b = x;
          bd = q[x];
        }
      }
      if bd == maxdist {
        break;
      }
      d[b] = bd;
      for &(ne, cost) in &e[b] {
        if bd + cost < q[ne] {
          q[ne] = bd + cost;
        }
      }
    }
    // print solution
    for x in 0..n {
      if x != s {
        print!("{} ", if d[x] == maxdist { -1 } else { d[x] });
      }
    }
    println!("");
  }
}
